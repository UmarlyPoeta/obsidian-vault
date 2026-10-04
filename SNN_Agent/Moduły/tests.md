---
typ: snn-modul
sciezka: tests/
tags: [snn, modul, testy, ci]
zaktualizowano: 2026-10-05
---
# tests

> [!skrot] W 30 sekund
> - **Co to:** testy pytest warstwy integracyjnej. `tests/w0/` obejmuje edge, backend i kontrakty (37 plików), `tests/runtime/` symulator [[snn_runtime]] (8 plików + fixture). Do tego szablon raportu charakteryzacji neuronu. Poza `tests/` leżą testy ML: `snn_pipeline/tests/`, `ga_neuron_search/test_genome.py`, `test_forward.py`.
> - **Wejście → wyjście:** kod + fixture'y (`contracts/fixtures`, `tests/runtime/fixtures/lui8-v2-manifest.json`, syntetyczne strumienie serial) → zielono albo czerwono w PR gate.
> - **Gdzie działa:** lokalnie w `.venv-w0`, w CI na `ubuntu-latest` z Pythonem 3.12 (`.github/workflows/pr.yml`).

## Struktura (drzewo z opisem plików)
```
tests/
├── neuron_characterization_20260322.md   # niewypełniony szablon: V_th, drift co 15 min, CV < 1% po resecie zasilania
├── runtime/                              # 127 testów
│   ├── conftest.py                       # fixture'y lui8, mutate (przelicza encoder_hash), channels
│   ├── fixtures/lui8-v2-manifest.json    # prawdziwa sieć 7-4-3-1 (8 płytek, 24 połączenia)
│   ├── test_manifest.py (43)             # kody odrzuceń P1, w tym test_our_best_model_cannot_yet_claim_champion
│   ├── test_reference_tool.py (7)        # konwerter hw_*.json → ModelManifest
│   ├── test_integrator.py (8)            # integrator ramkowy kontra referencja warstwowa
│   ├── test_decoder.py (17)              # KOfWDecoder; 6 testów parytetu z snn_pipeline.stream_eval.count_alarms
│   ├── test_streaming.py (16)            # ten sam strumień pocięty na różne paczki = te same decyzje; luki, warmup
│   ├── test_backend_integration.py (3)   # LuiRuntime za prawdziwym IngestService; string from_environment
│   ├── test_telemetry.py (21)            # NeuronFrame: statusy, gap raz, przerzedzanie, ≤ 50 neuronów
│   └── test_topology_draft.py (12)       # TopologyDraft: porty 1–3, jeden przewód na port, DRAFT_NOT_A_MODEL
└── w0/                                   # 927 testów
    ├── fakes.py, backend_env.py          # atrapy (FakeRuntime itd.) i środowisko backendu
    ├── edge: test_serial_protocol (39), test_batching (14), test_bridge (30), test_sources (22),
    │         test_outbox (14), test_commands (68), test_alarm (20), test_camera (35), test_sinks (15),
    │         test_config (44), test_probe (5), test_api (59), test_import_boundary (2)
    ├── backend: test_production_api (48), test_ingest (34), test_sessions (27), test_worker (35),
    │         test_vision (67), test_policy (19), test_notify (26), test_auth (69), test_backend_config (55),
    │         test_keyvault (23), test_storage (31), test_images (26), test_imaging (25), test_publisher (7),
    │         test_device_commands (18), test_azurite_integration (2), test_scenarios (1)
    ├── kontrakty/mock: test_contracts (24), test_mock_api (18)
    └── test_e2e.py (5)                   # cały łańcuch w jednym procesie: bajty serial → alarm ack
```
(Liczby w nawiasach pochodzą z `pytest --collect-only` na `feat/patryk-telemetry-p3`.)

## Jak działa (co pokrywają)
- **Protokół i edge:** gramatyka, CRC i zakresy linii `$B/$F`, luki `tx_drop` kontra `lost_or_corrupt`, ramki scalone, regresja seq, batching na siatce hopów, trwałość i idempotencja outboxa, jednokrotna obsługa komend, TTL, limity alarmu, konfiguracja TOML.
- **Granica importów** (`test_import_boundary.py`): `python -S -c "import rpi_agents.agent.ports, rpi_agents.agent.gpio, rpi_agents.runtime.ports"` musi działać bez site-packages, a strażnik GPIO odrzuca `RPiGPIOFactory` z listy zabronionych.
- **E2E** (`test_e2e.py:1-8`): prawdziwe są bridge, API z tokenem, ingest, worker i polityka. Udawane są urządzenie serial (odtwarzane bajty), kamera, GPIO, runtime SNN (atrapa liczenia spike'ów), vision i skrzynka pocztowa. Docstring zastrzega: „nothing here says anything about the real hardware, the real SNN or a real model”.
- **Runtime:** równoważność dekodera z regułą FA/h, niezależność wyniku od wielkości paczki, telemetria nie zmienia decyzji, szkic topologii nigdy nie jest uruchamialny.
- **Testy z warunkiem:** Azurite (`SNN_TEST_AZURITE=1`), Pillow (kamera, imaging, scenariusze), `snn_pipeline` + numpy (parytet dekodera).

### Testy poza `tests/` (zakres innych notatek)
`snn_pipeline/tests/{conftest,test_spike_artifact,test_voice_labels}.py` → [[snn_pipeline]]. `ga_neuron_search/test_genome.py` (importuje `genome` lokalnie) → [[ga_neuron_search]]. `test_forward.py` w korzeniu (torch + `snn_pipeline.snn_model.GlassBreakSNN`) → [[snn_pipeline]]. Żaden z nich nie jest w CI. W `.venv` nie ma pytesta ani torcha, więc ich nie uruchamiałem.

## Jak uruchomić
```sh
# dokładnie to, co robi CI (lint + testy):
.venv-w0/bin/python -m ruff check --config ruff-w0.toml contracts rpi_agents tests/w0 snn_runtime tests/runtime
.venv-w0/bin/python -m pytest tests/w0 tests/runtime -q
# bez zapisu do repo:
PYTHONDONTWRITEBYTECODE=1 .venv-w0/bin/python -m pytest tests/w0 tests/runtime -q -p no:cacheprovider
```

## Konfiguracja i parametry — CI (PR gate)
Plik `.github/workflows/pr.yml`, workflow „W0 integration gate”:
- **Wyzwalacze:** `pull_request` i `push` na `master`, `workflow_dispatch`. Uprawnienia `contents: read`. `concurrency` anuluje poprzedni przebieg dla tego samego PR.
- **Job `contracts`** („contracts-and-mock”, timeout 10 min): `actions/checkout` przypięty po SHA (v4), `persist-credentials: false`, **sparse checkout** tylko `/.gitmodules /contracts/ /rpi_agents/ /tests/w0/ /snn_runtime/ /tests/runtime/ /requirements-w0.lock /ruff-w0.toml`, bez historycznych datasetów. Python 3.12. `pip install --require-hashes -r requirements-w0.lock`. Lint ruff na `contracts rpi_agents tests/w0`, a `snn_runtime` i `tests/runtime` tylko jeśli istnieją. Potem `pytest tests/w0 [tests/runtime] -q`.
- **Job `pr-gate`** (`needs: contracts`, `if: always()`): `test "$RESULT" = success`. Wymagany status check nazywa się `pr-gate`.
- Ruff (`ruff-w0.toml`): `target-version py312`, `line-length 110`, reguły `E4, E7, E9, F, I`.
- Środowisko W0 (`requirements-w0.in`): fastapi, uvicorn, jsonschema, rfc8785, httpx, pytest, ruff. Lock ma 29 przypiętych pakietów z hashami. Plik ostrzega: „Never install this full stack on the Pi or into GA venv”.
- **Poza CI zostają:** `snn_pipeline`, `ga_neuron_search`, `software/`, testy Pillow i Azurite (w CI są pomijane, bo lock W0 nie ma Pillow), parytet dekodera (numpy jest poza sparse checkoutem i poza lockiem).

## Wyniki
Uruchomione 2026-10-05 na `feat/patryk-telemetry-p3` (ef42c20e + nieśledzone pliki), Python 3.12.13:

| Komenda | Wynik |
|---|---|
| `.venv-w0` `pytest tests/w0 tests/runtime -q -p no:cacheprovider` | **1021 passed, 33 skipped**, 1 warning (Starlette o httpx), 34,8 s |
| `.venv-w0` `pytest tests/runtime` | 121 passed, 6 skipped (parytet dekodera: brak numpy i `snn_pipeline`) |
| jw. z `PYTHONPATH=.venv/lib/python3.12/site-packages` (numpy 2.5.3) `tests/runtime/test_decoder.py` | **17 passed**, czyli dekoder strumieniowy = `count_alarms` |
| `ruff check --no-cache --config ruff-w0.toml` (ścieżki CI) | All checks passed |

Pominięte w `tests/w0`: 3 testy Azurite (brak `SNN_TEST_AZURITE=1`), reszta to testy wymagające Pillow (kamera, imaging, scenariusze), część z nich sparametryzowana. Na stderr pojawia się komunikat `failed to switch led off: OSError`, to log z testu alarmu ze wstrzykniętym błędem. **niepewne:** dokładny rozkład 33 pominięć na pliki, bo raport `-rs` grupuje pozycje.

## Powiązania
[[snn_runtime]], [[rpi_agents]], [[contracts]], [[Sprzęt i wdrożenie]].

## Na gałęziach
- `chore/pr-gate-runtime-paths` (f46c7244, Wiktor): dodaje `snn_runtime` i `tests/runtime` do sparse checkoutu, lintu i pytest. Scalone jako #59 (2c25ccbc). Pętla „jeśli katalog istnieje” pozwala scalić workflow przed modułem i po nim.
- `feat/wiktor-edge-cloud-05` (= #60): większość `tests/w0` backendu (`test_auth`, `test_worker`, `test_vision`, `test_e2e` itd.).
- `feat/patryk-state-stream-decoder` / `feat/patryk-telemetry-p3`: `test_integrator`, `test_decoder`, `test_streaming`, `test_backend_integration` / `test_telemetry`, `test_topology_draft`.
- `feat/karolina-dashboard`: `tests/dashboard/test_dashboard.py` (178 linii). **Uwaga:** pr.yml nie obejmuje `tests/dashboard`.
- `feat/rpi` i `feat/dashboard`: stare `rpi_agents/tests/` (inny agent).

## Znane problemy / TODO
- W CI nie ma testów ML, nie ma Pillow i nie ma parytetu z regułą FA/h. Ten ostatni chroni przed rozjazdem decyzji na Pi i metryki wyboru modelu, a działa tylko lokalnie.
- `tests/neuron_characterization_20260322.md` to niewypełniony szablon.

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.
