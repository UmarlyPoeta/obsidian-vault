---
typ: snn-modul
sciezka: dataset/
tags: [snn, modul, dane]
zaktualizowano: 2026-10-05
---
# dataset

> [!skrot] W 30 sekund
> - **Co to:** katalog wersji głównego zbioru SNN. Śledzone w gicie są tylko `dataset/versions/v1.0.0/` i `v2.0.0/`, po trzy pliki: `manifest.csv`, `dataset.json`, `stats.md`.
> - **Wejście → wyjście:** produkt `snn_pipeline/build_dataset_version.py`. Czytają go `validate_dataset.py`, `encoder_twin.py build-manifest` i testy.
> - **Gdzie działa:** dane na PC. Audio (DataSEC, ESC-50) **nie** leży w repo.
> - Pełny opis zbiorów, kontraktu i walidatorów: [[Dane i zbiory]].

## Struktura (bieżąca gałąź)
```
dataset/                    6,7 MB
└── versions/
    ├── v1.0.0/  manifest.csv (14 114 rekordów), dataset.json, stats.md     3,8 MB
    └── v2.0.0/  manifest.csv (10 853 rekordów), dataset.json, stats.md     2,9 MB   ← obowiązująca
```
Lokalnie oczekiwane są też (poza gitem, wg `DATASET_CONTRACT.md`):
- `dataset/datasec/PT_DATASET_250314/` (3226 plików, 4,6 GB);
- `dataset/clean/` (VOICe, miksy i listy `source/*.txt` z oficjalnym podziałem).

**Niepewne:** czy te katalogi istnieją na tej maszynie. Nie sprawdzano, wymagałoby to przeglądu dużych danych.

## Jak działa
- Jeden wiersz `manifest.csv` = jedno nagranie (kolumny: [[Dane i zbiory]]). Split jest zapisany jako kolumna, nie odtwarzany z ziarna.
- `dataset.json` zawiera `version`, `created_utc`, `git_commit` buildera, parametry splitu, liczebności, źródła i `manifest_sha256`:
  - v2.0.0: `b3dcc1101a6522ec…`, commit `2a5a5e2f`, 2026-08-27T17:46Z;
  - v1.0.0: `96f32902918bcd7b…`, commit `164e0762`, 2026-08-26T18:29Z.

## Jak uruchomić
```bash
python snn_pipeline/build_dataset_version.py --version v2.1.0      # nowa wersja; istniejącej nie nadpisze
python snn_pipeline/validate_dataset.py --version v2.0.0 [--quick]
# kontrola, ile plików manifestu masz lokalnie — skrypt w docs/DATASET_CONTRACT.md
```

## Wyniki
- v2.0.0: 20,8 h, 4593 pozytywów; grupy pozytywne train/val/test 156 / 100 / 96.
- v1.0.0 (błędne etykiety VOICe, issue #34): 23,1 h, 3422 pozytywów; grupy pozytywne 231 / 62 / 59.

Źródło: `stats.md`.

## Powiązania
[[Dane i zbiory]] · [[snn_pipeline]] · [[Architektury sieci]] (`voice_extracted/`, `spikes_*`) · [[encoder]] · [[tests]]

## Na gałęziach
- `dataset/versions/` jest śledzony od commitu `fa00b018` (2026-08-31, UmarlyPoeta). Na `origin/feat/dataset-contract-v1` i `…-v2` wersje istniały tylko lokalnie u autora (brak w `git ls-tree`).
- **`origin/feat/continuous-dataset`** (kuleczkomen, 2026-09-06..23) dodaje `dataset/continuous/`:
  - `README.md`;
  - `eval/` (annotations, audio_io, cli, manifest, stream_builder, validate);
  - `tests/` (3 pliki).
  - To generator ciągłych strumieni testowych: tło z ESC-50 z `kind`, minimalne przerwy, rozłączność eval/train, `group_id` zgodny z `dataset_contract.py`.
- **`origin/feat/master-pipeline`** importuje `dataset/continuous` (26575919) i zamraża manifesty `out/continuous_eval_seed{42,43,44}.manifest.json` (f5a38158, 2026-09-26). Audio jest deterministyczne, regenerowane tą samą komendą.
- `origin/feat/dataset-remove-atrifacts` (b8a4ac1e, 2026-09-01): usunięcie artefaktów (zawarte w bieżącej gałęzi).

## Znane problemy / TODO
- Brak własnego nagrania tła docelowego pomieszczenia. „FA w ciszy” jest mierzone na zastępniku (`stats.md`, „Znane ograniczenia”).
- Ciągły zbiór testowy (wymagany przez `CURRENT_ASSUMPTIONS.md`) nie jest jeszcze w `master` ani na bieżącej gałęzi.

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.
