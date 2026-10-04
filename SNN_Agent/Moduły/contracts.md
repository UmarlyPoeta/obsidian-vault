---
typ: snn-modul
sciezka: contracts/
tags: [snn, modul, kontrakty]
zaktualizowano: 2026-10-05
---
# contracts

> [!skrot] W 30 sekund
> - **Co to:** „SNN wire contracts v1.0”, czyli wykonywalne kontrakty komunikacji między enkoderem (Uno), edge (RPi), backendem, symulacją SNN i dashboardem. 18 schematów JSON Schema 2020-12 w `v1/`, reguły semantyczne w `validation.py`, fixtures, klient demo.
> - **Wejście → wyjście:** payload JSON → `validate(name, payload, manifest=…)` → OK albo `ContractError` (kody 409/422).
> - **Gdzie działa:** wszędzie, gdzie płyną dane (W0: lokalny mock API `rpi_agents.cloud.app.mock_api`).
> - **Związek z ML:** `EncoderProfile` (mapa kanałów + `encoder_hash`), `ModelManifest` (model, topologia, provenance, status championa), `SpikeBatch`, `SNNDecision`, `NeuronFrame`, `TopologyDraft`. To jest **kontrakt danych runtime**, a nie kontrakt zbioru treningowego. Ten drugi opisują [[Dane i zbiory]] i `DATASET_CONTRACT.md`.

## Struktura
```
contracts/
├── README.md         źródło prawdy W0: typy i właściciele, kanały/hashe, czas i sesje, idempotencja, mock
├── __init__.py
├── validation.py     ContractError, content_hash (sha256 JCS), validate() + reguły semantyczne per typ
├── demo.py           klient demo mocka (python -m contracts.demo)
├── v1/               AlarmCommand, BatchAck, CaptureCommand, CommandAck, DeviceStatus, EncoderProfile, Error, Event,
│                     ModelManifest, NeuronFrame, SNNDecision, SessionControl, SessionCreate, SessionState,
│                     SpikeBatch, StreamGap, TopologyDraft, VisionResult  (.schema.json)
└── fixtures/         przykłady (spike, silence, gap, trigger, duplicate, model-manifest, encoder-profile, topology-draft, …) + index.json + demo-model.txt
```

## Jak działa
- `validate()` (`validation.py:65`):
  - wymaga `schema_version == "1.0"`, inaczej 409 `VERSION_MISMATCH`;
  - waliduje schemat i nie powtarza w komunikacie podanych wartości, bo mogłyby zawierać sekrety;
  - potem sprawdza reguły semantyczne, np. `EncoderProfile`: unikalne nazwy kanałów i indeksy ciągłe od 0 (`:74-80`).
- Hashe (README):
  - `encoder_hash = sha256(JCS(EncoderProfile))`;
  - `model_hash = sha256(JCS(ModelManifest))`;
  - hash plików = SHA-256 surowych bajtów;
  - prefiks `sha256:`.
- `channel_map`: nazwy cechy **nie wolno** wnioskować z pozycji, nazwy CSV ani liczby wejść. Wariant `base`: peak, peak_cnt, cv, zcr, flux, hf_lo, hf_hi. W `swap` pozycje 1–2 to hjorth_mobility i autocorr_lag1.
- `ModelManifest` (fixture `model-manifest.json`): `encoder_profile`, `artifacts[]` z sha256, `runtime` (dt_us, potential_unit), `decoder` (window_us, threshold, cooldown_us), `topology` (neurony z tau_mem_us, tau_syn_us, v_leak, v_threshold, v_reset, refractory_us; połączenia z portem 1–3, znakiem i wagą), `provenance` (training_run_id, dataset_manifest_hash, checkpoint_hash, evaluation_hash, seed, calibration_id).
- „Champion wymaga provenance treningu/danych/oceny i nie może zawierać profilu demo … sam napis `evaluated_champion` nie dowodzi jakości” (README).
- `TopologyDraft` (P3, commit ef42c20e): szkic topologii płytek (boards z x/y, connections z `target_port` i excitatory/inhibitory) dla dashboardu. Fixture `draft-lui8`.

## Jak uruchomić
```sh
python3.12 -m venv .venv-w0
.venv-w0/bin/python -m pip install --require-hashes -r requirements-w0.lock
.venv-w0/bin/python -m pytest tests/w0 -q
.venv-w0/bin/python -m rpi_agents.cloud.app.mock_api      # 127.0.0.1:8000
.venv-w0/bin/python -m contracts.demo
```

## Konfiguracja i parametry
- Body ≤ 64 KiB.
- Batch spików trwa ≤ 1 s; `batch_seq` od 0.
- Capture ≤ 3 klatki po 1 MiB; alarm ≤ 30 s; TTL komendy ≤ 30 s.
- `NeuronFrame`: 0–50 neuronów.
- Mock ma limity 16 sesji, 256 batchy na sesję i 1024 mutacje idempotentne.

## Wyniki
Brak. README zastrzega: „Osiągniętych metryk demo nie wolno używać w artykule”.

## Powiązania
[[rpi_agents]] (mock API, edge) · [[snn_runtime]] (SNNDecision, NeuronFrame) · [[software]] (dashboard) · [[encoder]] (EncoderProfile) · [[models]] · [[tests]] (`tests/w0`)

## Na gałęziach
- Na `origin/main` katalogu nie ma.
- W0: 637989ee (#52, 2026-09-24, Wiktor Goszczyński). DeviceStatus: 02521fde (#57).
- **Bieżąca `feat/patryk-telemetry-p3`** względem `origin/master` dodaje `TopologyDraft.schema.json`, `fixtures/topology-draft.json` i +18 linii w `validation.py` (ef42c20e, 2026-09-27, UmarlyPoeta, „P3: telemetria NeuronFrame i kontrakt szkicu topologii”).
- `origin/feat/encoder-features` i `origin/feat/master-pipeline` zmieniają `fixtures/encoder-profile.json` (zamrożony profil K1).

## Znane problemy / TODO
- W0 nie implementuje: auth, bazy, prawdziwego vision, inferencji, metryk badawczych, GPIO i deploymentu (README, „NIEzaimplementowane w W0”).
- `ModelManifest` fixture ma status `demo`. **Niepewne:** czy istnieje już manifest prawdziwego modelu.

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.
