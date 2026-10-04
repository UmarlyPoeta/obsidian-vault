---
typ: snn-modul
sciezka: rpi_agents/
tags: [snn, modul]
zaktualizowano: 2026-10-05
---
# rpi_agents

> [!skrot] W 30 sekund
> - **Co to:** cała warstwa integracyjna poza samą siecią. `agent/` to most edge na Raspberry Pi (Uno serial → `SpikeBatch` → trwały outbox → backend, komendy zdjęcia i alarmu, heartbeat `DeviceStatus`). `cloud/` to backend FastAPI w rolach API i worker (sesje, ingest z runtime'em SNN, zdarzenia, vision gpt-5-mini, polityka alarmu, e-mail) z wdrożeniem na Azure. `deploy/` to systemd i przykładowy TOML.
> - **Wejście → wyjście:** linie `$B/$F` z Arduino Uno (115200 8N1) → HTTPS `/v1/...` → decyzja SNN → komenda `capture` → JPEG → vision → polityka → komenda `alarm` (GPIO LED/buzzer) + e-mail.
> - **Gdzie działa:** `agent/` na RPi 5 (docelowo także Pi Zero), tylko biblioteka standardowa plus Pillow dla kamery. `cloud/` lokalnie (`both`) albo w Azure Container Apps (`-api` i `-worker`).

## Struktura (drzewo z opisem plików)
```
rpi_agents/
├── README.md, MIGRATION.md      # W0: pochodzenie z feat/dashboard @ bb1dbe02, co przeniesiono, a czego nie
├── agent/                       # EDGE (Pi). Tylko stdlib, poza kamerą (Pillow) i GPIO (gpiozero, lazy)
│   ├── serial_protocol.py       # protokół linii Uno→Pi v1: parser, CRC-8, StreamTracker (luki)
│   ├── batching.py              # BatchAssembler: zdarzenia → SpikeBatch na siatce hopów, 250 ms
│   ├── bridge.py                # usługa: 3 wątki (serial / worker / komendy), BootContext na boot Uno
│   ├── sources.py               # SerialPortSource (termios raw 8N1), ReplaySource, event_stream (stall/tick)
│   ├── outbox.py                # trwały outbox SQLite (WAL), idempotencja po request_id, KV, komendy
│   ├── api.py                   # ApiClient + UrllibTransport (https poza loopback, bez redirectów), Backoff
│   ├── commands.py              # CommandHandler: capture/alarm, TTL ≤ 30 s, raz na command_id
│   ├── alarm.py                 # GpioAlarm: LED GPIO17, buzzer GPIO27 (BCM), lokalny limit czasu
│   ├── camera.py                # UvcCamera (v4l2, YUYV→JPEG, wybór po numerze seryjnym USB), ReplayCamera
│   ├── sinks.py                 # LocalDirSink (demo), BackendImageSink (POST /v1/events/{id}/image)
│   ├── config.py                # parser TOML → zamrożone dataclassy, nieznane klucze = błąd
│   ├── probe.py                 # CLI: kilka sekund nasłuchu portu → podsumowanie JSON
│   ├── synthetic.py             # syntetyczne strumienie v1 do testów (NIE wyjście enkodera)
│   ├── gpio.py, ports.py        # strażnik backendu GPIO, interfejsy Camera/Serial/AlarmAdapter
│   └── hwtest/                  # mega_standin.ino (Arduino Mega udający Uno) + README
├── runtime/ports.py             # Protocol SNNRuntime: load/reset/step/snapshot/checkpoint/restore
├── cloud/
│   ├── README.md                # projekt backendu, uruchomienie, Azure, co zweryfikowano
│   ├── app/                     # api.py, ingest.py, sessions.py, worker.py, vision.py, policy.py, notify.py,
│   │                            # auth.py, storage(_azure).py, keyvault.py, backend_config.py, demo_runtime.py,
│   │                            # mock_api.py + mock_store.py (lokalny mock W0 na fixture'ach), admin.py, server.py
│   └── infra/                   # Dockerfile, deploy.sh (az CLI), main.bicep, scenarios/synthetic_intruder.py
└── deploy/                      # edge.example.toml, snn-edge.service (system), snn-edge.user.service, README
```

## Jak działa

### Protokół Uno → Pi (serial line protocol v1)
Wprowadzony przez Wiktora na `feat/wiktor-edge-cloud` (3b957e33), w `master` jako #54 (432358f9). Obecny plik różni się od wersji z gałęzi jedną linią. Opis w `agent/serial_protocol.py:1-12`:

```
$B,<ver>,<build_id>,<fs_hz>,<hop>,<n_ch>,<pulse_us>,<chset>*<CRC>     linia boot
$F,<seq>,<t_us>,<n>,<mask>,<flags>,<txdrop>*<CRC>                     linia ramki (jedna na hop ≈ 10 ms)
```
| Pole | Format i zakres (kod) |
|---|---|
| `ver` | dziesiętnie, musi być `1` (`PROTOCOL_VERSION`, `:20`, `:108`) |
| `build_id` | 8 znaków hex wielkimi literami (`:31`), np. `5717A0D0` |
| `fs_hz` | 1000–100000 (`:110`), w praktyce 19231 |
| `hop` | 1–4096 próbek, w praktyce 192 |
| `n_ch` | 1–8 kanałów, w praktyce 7 |
| `pulse_us` | 1–1 000 000 (synthetic używa 6000) |
| `chset` | `[a-z0-9_]{1,12}`, np. `swap` (mapa kanałów base/swap, MIGRATION.md) |
| `seq` | u32, numer hopu |
| `t_us` | u32 `micros()` Uno. Pi rozwija go do 64 bitów (`FrameEvent.source_us`) |
| `n` | 1–65535 próbek objętych ramką. `n/hop > 1` oznacza ramkę scaloną (`MERGED`) |
| `mask` | 2 cyfry hex, bit c = spike kanału c. Bity ≥ `n_ch` są odrzucane |
| `flags` | 1–2 cyfry hex, bit 0 = `priming` (rozgrzewka progów enkodera) |
| `txdrop` | u16, licznik linii porzuconych przez Uno |
| `CRC` | CRC-8, poly 0x07, init 0, bez odbicia i bez XOR końcowego, liczone po bajtach między `$` i `*` (`crc8`, `:48-54`) |

Reguły: linia kończy się LF lub CRLF, maks. 48 bajtów (`MAX_LINE_BYTES`), bufor składania 64 bajty (dłuższa linia jest odrzucana do LF, `LineOverflow`). Linie zaczynające się od `#` to komentarze firmware'u. `StreamTracker` (`:189-265`) emituje zdarzenia `BootEvent` (każdy `$B` to nowy `boot_id`), `FrameEvent`, `GapEvent(first_seq, missing_hops, txdrop_delta, cause="tx_drop"|"lost_or_corrupt")` przed ramką, która ujawnia lukę, oraz `RejectedEvent(code)` z kodami `TOO_LONG, NOT_PROTOCOL, BAD_FORMAT, BAD_CRC, BAD_RANGE, NO_BOOT, SEQ_DUPLICATE, SEQ_REGRESSION`. Anomalie: `TIME_SKEW` (> 2000 µs odchyłki od siatki), `MERGED`, `TXDROP`, `N_SEQ_MISMATCH`. Pi → Uno: znak `I` = „wydrukuj ponownie linię boot” (maks. raz na sekundę, deploy/README). Mega stand-in obsługuje dodatkowo `G`/`T`/`L` do wstrzykiwania błędów (hwtest/README).

> [!wazne] Firmware Uno z protokołem v1 jeszcze nie istnieje
> Protokół v1 emituje dziś tylko `hwtest/mega_standin/mega_standin.ino` (na żadnej gałęzi nie ma innego `.ino` z `crc8`/`$F`). Firmware enkodera `encoder_v2.ino` wypisuje CSV `frame,s0..s6`, z którego korzysta `praca_wtorek_2026_08_04/narzedzia/capture_serial.py`. Zob. [[encoder]], [[praca_wtorek_2026_08_04]].

### Batching → `SpikeBatch` (`agent/batching.py`)
`BatchAssembler(boot_id, channels, fs_hz, hop, batch_us=250_000)` (`:55`) układa hopy na siatce `origin + round(k·hop/fs)` zakotwiczonej raz w `micros()` Uno. Paczka to całkowita liczba hopów (maks. 1 s, `MAX_BATCH_US`). Spike trafia na **początek** hopu, a jego `dt_us` jest offsetem od początku paczki. Hopy z primingiem i ciche hopy to zaobserwowana cisza. Wcześniejsze hopy ramki scalonej liczą się do `dropped_events`. Luka zamyka paczkę i nic nie jest interpolowane (`:1-15`). `to_spike_batch` (`:164-181`) produkuje:

```json
{"schema_version":"1.0","request_id":"<session_id>.b<batch_seq>","device_id":"…","session_id":"…","epoch":1,
 "boot_id":"…","batch_seq":0,"encoder_hash":"sha256:…","source_start_us":…,"source_end_us":…,
 "spikes":[{"dt_us":0,"channel":"hf_lo"}],"quality":{"dropped_events":0,"adc_clipped":false}}
```
`adc_clipped` jest zawsze `false`, bo firmware nie zgłasza przycięć. Schemat dopuszcza do 4096 spike'ów na paczkę (`contracts/v1/SpikeBatch.schema.json`).

### Most (`agent/bridge.py`)
`python -m rpi_agents.agent.bridge --config edge.toml`. Pracują trzy wątki: główny tylko czyta serial (bufor jądra mieści ok. 0,35 s danych Uno), worker tworzy sesje, opróżnia outbox po kolei i wysyła heartbeat, a trzeci obsługuje komendy (`bridge.py:1-15`). Każdy boot Uno oznacza nową sesję (`BootContext`), a nowa sesja powstaje dopiero po dostarczeniu stopu poprzedniej (jedna aktywna sesja na urządzenie). Kody `SESSION_LOST/STOPPED/MISMATCH` każą otworzyć nową sesję (`:65`). `DeviceStatus` (`:561-602`): `state ∈ {starting, running, stalled, reconnecting, error, stopping}` plus liczniki serial, outbox i kamery.

### Edge ↔ chmura (HTTP, `agent/api.py:193-218`, `cloud/app/api.py:198-309`)
| Metoda i ścieżka | Kto | Treść |
|---|---|---|
| `POST /v1/sessions` (opcjonalny scenariusz demo) | Pi | `SessionCreate` → `session_id`, `epoch` |
| `POST /v1/sessions/{id}/batches` | Pi | `SpikeBatch` → `BatchAck` |
| `POST /v1/sessions/{id}/stop` | Pi | zatrzymanie |
| `GET /v1/devices/{id}/commands` | Pi (poll co 1 s) | `CaptureCommand` / `AlarmCommand` |
| `POST /v1/commands/{id}/ack` | Pi | `CommandAck` (completed / failed / expired) |
| `POST /v1/events/{id}/image` | Pi | surowy JPEG + sha256 + `captured_at` |
| `POST /v1/devices/{id}/status` | Pi (heartbeat co 5 s) | `DeviceStatus` |
| `GET /v1/sessions/{id}`, `/v1/devices/{id}/status`, `/v1/events[/{id}]`, `/v1/events/{id}/images/{i}` | operator / dashboard | odczyt |
| `/auth/login`, `/auth/logout`, `/auth/session`, `/healthz` | operator | cookie `__Host-`, CSRF |
| `GET /v1/sessions/{id}/telemetry` (SSE `event: snapshot`) | dashboard | **tylko w mocku** (`mock_api.py:214`) |

Autoryzacja urządzenia: bearer `device_id~secret`, a w bazie trzymany jest tylko hash (cloud/README). Ingest (`cloud/app/ingest.py:1-15`) działa tak: walidacja, potem rekord `processing`, potem `runtime.step`, a przy triggerze w jednej transakcji powstają zdarzenie, komenda capture i outbox. Gdy proces padnie w trakcie, sesja dostaje `SESSION_LOST` i urządzenie otwiera nową epokę. Runtime jest ładowany z `SNN_RUNTIME=module:factory` (`backend_config.py:117-121, :209`). `demo` to atrapa liczenia spike'ów (próg 3, `demo_runtime.py`), prawdziwy runtime to `snn_runtime.runtime:from_environment` (zob. [[snn_runtime]]).

### Bezpieczeństwo komend (`commands.py`, `alarm.py`)
Komenda żyje maks. 30 s (`MAX_TTL_S`), wygaśnięcie liczy lokalny zegar monotoniczny, każdy `command_id` jest obsługiwany najwyżej raz i trwale. Alarm: wyjścia są wymuszone na OFF przy tworzeniu, timer wyłącza je po `min(duration, max_ms)`, czas ciągłej pracy ograniczają `max_continuous_ms` i `atexit`. Domyślna polityka w chmurze to `manual-review-only-v1`, która nigdy nie alarmuje. `armed-glass-and-person-v1` trzeba włączyć jawnie (cloud/README).

## Jak uruchomić
```sh
# edge (Pi), na pierwszy plan; błąd konfiguracji = exit 2
python -m rpi_agents.agent.bridge --config /etc/snn-edge/edge.toml
python -m rpi_agents.agent.probe --port /dev/serial/by-id/<mega> --seconds 10 [--send G --send-at 4]
# backend lokalnie (cloud/README)
python -m venv .venv && .venv/bin/pip install --require-hashes -r requirements-backend.lock
python -m rpi_agents.cloud.app.admin hash-password
python -m rpi_agents.cloud.app.admin issue-device-token <device_id>
python -m rpi_agents.cloud.app.server both --check-config
# obraz i Azure
docker build -f rpi_agents/cloud/infra/Dockerfile -t snn-backend .
rpi_agents/cloud/infra/deploy.sh      # az CLI + main.bicep; najpierw `az deployment group what-if`
```
Minimalne demo bez Azure: `SNN_STORAGE=memory SNN_ALLOW_MEMORY=1 SNN_RUNTIME=demo SNN_ALLOW_DEMO_RUNTIME=1 SNN_DEMO_VISION=glass_person SNN_ALLOW_DEMO_VISION=1 SNN_POLICY=armed-glass-and-person-v1 SNN_MANIFEST_PATH=contracts/fixtures/model-manifest.json SNN_OPERATOR_USERNAME=operator SNN_OPERATOR_PASSWORD_HASH=‹ukryte› SNN_INSECURE_DEV=1 SNN_ALLOW_LIVE=1`. Instalacja jako usługa: [[Sprzęt i wdrożenie]].

## Konfiguracja i parametry
`deploy/edge.example.toml` ma sekcje `[device]` (`id`, `input_kind ∈ {uno, stand_in, replay}`), `[backend]` (`url`, `credential_file` chmod 600, `ca_file`, `demo_scenario ∈ {silence, spike, trigger, vision_unavailable}`, `timeout_s`), `[session]` (`mode ∈ {demo, replay, live}`, `model_hash`, `encoder_hash`), `[serial]` (`path` = `/dev/serial/by-id/...`, `baud 115200`, `channels = ["peak","peak_cnt","cv","zcr","flux","hf_lo","hf_hi"]`, `expected_build_id`, `stall_s 2.0`), `[camera]` (`serial` USB, 1280×720, 15 fps), `[images]` (`local_dir`, `keep`), `[alarm]` (`enabled=false`, `led_pin 17`, `buzzer_pin 27`, `max_ms 10000`, `max_continuous_ms 30000`), `[state]` (`dir /var/lib/snn-edge`), `[limits]` (`max_pending 5000`, `pre_session_batches 400`, `batch_ms 250`, `heartbeat_s 5`, `command_poll_s 1`, `drain_s 5`). Zmienne backendu są opisane na górze `cloud/app/backend_config.py` (`SNN_STORAGE`, `SNN_AZURE_ACCOUNT`, `SNN_RUNTIME`, `SNN_VISION_*`, `SNN_ALLOWED_HOSTS`, `SNN_ALERT_RECIPIENTS`, `SNN_SMTP_*`).

## Wyniki
Według cloud/README sekcja „Verified”: cały łańcuch przeszedł na Pi 5 z Mega stand-in i kamerą D415 przeciw backendowi (laptop + Azurite) oraz na Azure (students, francecentral, gpt-5-mini): trzy zdarzenia skończyły się `alarm_confirmed`, alarm zastosowano z **atrapą GPIO**, a vision był skryptowy albo syntetyczny. Obraz kontenera ma 297 MB i działa bez roota. **Niezweryfikowane:** prawdziwy LED i buzzer, prawdziwy runtime SNN, Uno z mikrofonem, KEDA, trafność modelu.

## Powiązania
- [[snn_runtime]] jest podpinany do `ingest.py` przez `SNN_RUNTIME`. Kontrakty JSON: [[contracts]]. Firmware enkodera: [[encoder]].
- Testy `tests/w0` i pr-gate: [[tests]]. Architektura fizyczna i protokoły: [[Sprzęt i wdrożenie]].

## Na gałęziach
| Gałąź | Co ma w `rpi_agents/` |
|---|---|
| `feat/rpi` (9c329a9f, 2026-07-14, vityk-dev) | stary agent: maszyna stanów `WAKE → CAPTURE → PREFILTER → [VISION] → DECIDE`, Gemini vision, picamera2, `deploy/install.sh`, `snn-agent.service`; w docs planowany 2-bajtowy pakiet SPI (magic `0b101`, bity N1–N3) |
| `feat/dashboard` (bb1dbe02, 2026-08-13) | jak wyżej plus `cloud_sync.py`, `sync_queue.py` i stary backend `cloud/app/{main,routes_api,routes_dashboard}.py`. Źródło migracji W0 (MIGRATION.md) |
| `feat/wiktor-edge-cloud` (3b957e33) | 1 commit: `serial_protocol.py` + test (= #54 w master) |
| `feat/wiktor-edge-cloud-05` (d74a7008) | 10 commitów, 71 plików, +11153: cały `cloud/app`, `alarm.py`, infra Azure, `requirements-backend.*`, testy (= #60 b7cc0a69 w master) |
| `origin/master` | #52 W0, #54 protokół, #55 kamera UVC, #56 batching, #57 bridge/outbox/deploy, #60 backend + Azure |
| `feat/karolina-dashboard` (11a084b5, 2026-09-26) | +22 pliki: `cloud/app/routes_dashboard.py`, `templates/index.html`, `static/js/*` (network, raster, inspector, runtime), `DASHBOARD.md`, `tests/dashboard`. Czyta `/v1/devices/{id}/status`, `/v1/events` |

## Znane problemy / TODO
- `deploy/README.md` mówi, że nie ma adaptera alarmu i alarm dostaje `UNSUPPORTED_COMMAND`. To nieaktualne od #60, bo istnieje `agent/alarm.py`, a `edge.example.toml` ma sekcję `[alarm]`.
- Produkcyjny `cloud/app/api.py` nie ma trasy SSE telemetrii, którą `snn_runtime/TELEMETRY.md` §7 przypisuje backendowi.
- Mock W0: tylko loopback, 256 paczek na sesję (ok. 64 s), 16 sesji (deploy/README).
- Decyzje właściciela (cloud/README): okablowanie buzzera, retencja zdjęć, dystrybucja tokenów urządzeń.

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.
