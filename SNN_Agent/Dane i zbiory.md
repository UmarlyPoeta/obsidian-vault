---
typ: snn-zbiorcza
tags: [snn, dane, dataset]
zaktualizowano: 2026-10-05
---
# Dane i zbiory

> [!skrot] W 30 sekund
> - **Zbiór główny:** wersjonowany manifest `dataset/versions/v2.0.0/manifest.csv`. Zawiera 10 853 nagrań, 4878 grup źródłowych i 20,8 h audio z trzech źródeł: ESC-50, DataSEC i VOICe. W gicie są tylko manifesty (~7 MB), audio leży poza repo (wyjątek: wycinki VOICe).
> - **Kontrakt danych:** `docs/DATASET_CONTRACT.md` + `snn_pipeline/dataset_contract.py`. Najważniejsza zasada: podział po **grupie = nagraniu źródłowym**, nigdy po pliku. Split jest zapisany w kolumnie manifestu.
> - **Walidator:** `snn_pipeline/validate_dataset.py`. Kontrole krytyczne K1–K10, ostrzeżenia O1–O5.
> - **Artefakty spike'owe** (CSV 7 kanałów na klip) powstają z manifestu przez `encoder_twin.py build-manifest`. Obowiązuje `spikes_v2`. `spikes_manifest7` jest **unieważniony** (przeciek + `clock_tick`).
> - **Etykiety:** `label` positive/negative + `kind` positive / stationary / loud_event / speech / animal, czyli taksonomia do raportu FA/h.

## Źródła

| źródło | plików (v2.0.0) | grup | h | licencja | gdzie lokalnie |
|---|---:|---:|---:|---|---|
| DataSEC (Zenodo) | 3226 | 3207 | 15,27 | CC BY 4.0 | `dataset/datasec/PT_DATASET_250314/`, 4,6 GB, poza gitem |
| ESC-50 | 2000 | 1528 | 2,78 | CC BY-NC 3.0 (niekomercyjna) | `data/ESC-50-master/audio/` (publiczne) |
| VOICe | 5627 | 207 | 2,75 | Other (Attribution) | `architecture_14_neurons_patryk_09_07/voice_extracted/` (w repo: 8888 plików, 1,5 GB; glass 4444, hard_negative 4444) |

Źródło: `dataset/versions/v2.0.0/stats.md`, `docs/DATASET_CONTRACT.md` (sekcja „Co musisz mieć lokalnie”).

- ESC-50: szkło to klasa **39 = glass_breaking**. Klasa 38 to `clock_tick` i właśnie ten błąd skaził `spikes_manifest7` (nazwy plików `glass_*_..-38.csv` w `spikes_manifest7/train`). W `snn_pipeline/config.py` poprawka 38→39 jest w `origin/master` względem `origin/main`.
- VOICe to wielogodzinne syntetyczne miksy polifoniczne (glassbreak/gunshot/babycry). Wycina je `voice_extract.py`: glass jako pozytyw, gunshot/babycry jako trudne negatywy.
- `notebooks/dataset` uznano za archiwalny i nieużywany (`DATASET_CONTRACT.md`, „Skąd bierzemy tło stacjonarne”).

## Wersje zbioru

| wersja | data | commit buildera | nagrań | grup | h | pozytywne | manifest sha256 |
|---|---|---|---:|---:|---:|---:|---|
| v1.0.0 | 2026-08-26 | `164e0762` | 14 114 | 4878 | 23,1 | 3422 | `96f32902…` |
| v2.0.0 | 2026-08-27 | `2a5a5e2f` | 10 853 | 4878 | 20,8 | 4593 | `b3dcc110…` |

Źródło: `dataset/versions/*/dataset.json`, `stats.md`. Katalogi `dataset/versions/` dodano do gita 2026-08-31 (commit `fa00b018`).

**Podział v2.0.0** (`stats.md`):

| split | nagrania | grupy | h | pozytywne | grupy pozytywne |
|---|---:|---:|---:|---:|---:|
| train | 7120 | 3337 | 14,17 | 2846 (40,0%) | 156 |
| val | 1866 | 770 | 3,22 | 863 (46,2%) | 100 |
| test | 1867 | 771 | 3,41 | 884 (47,3%) | 96 |

**`kind` w v2.0.0:** positive 4593 · stationary 853 (3,55 h) · loud_event 2871 (10,16 h) · speech 1788 · animal 748.

**Zmiana v1 → v2** (issue #34, gałąź `feat/voice-label-fix-dataset-contract-v2`, commit `6398209f`, 2026-08-27, UmarlyPoeta):
- etykieta VOICe pochodzi teraz z katalogu ekstrakcji, a nie z reguły maksymalnego pokrycia. Odzyskano 1412 klipów szkła z klasy negatywnej i usunięto 241 nie-szkieł z pozytywów;
- strażnik polifonii jest asymetryczny: odrzucany jest skażony negatyw, zachowywany skażony pozytyw;
- podział VOICe wzięto z oficjalnych list (69/69/69 miksów);
- dodano kontrole K8/K9/O5.

## Kontrakt danych (wersje na gałęziach)

| gałąź | commit | co wnosi |
|---|---|---|
| `origin/feat/dataset-contract-v1` | `2a5a5e2f` (2026-08-26, patryk_kozlowski) | `DATASET_CONTRACT.md`, `dataset_contract.py`, `validate_dataset.py` z K1–K7 i O1–O4, `build_dataset_version.py`, wersja v1.0.0 |
| `origin/feat/voice-label-fix-dataset-contract-v2` | `6398209f` (2026-08-27) | poprawka etykiet VOICe, K8, K9, O5, `tests/test_voice_labels.py`, wersja v2.0.0 |
| (później, w `master`) | `439d28e3` (2026-08-28) | przebudowa artefaktu z v2.0.0, strażniki przecieku i kolejności strumienia w builderze |
| (w `master`) | `7861316d` (2026-08-30, Karolina Nocek, #37) | **K10** (koncentracja grup), próg akceptacji i FA/h jako metryka selekcji |
| `origin/feat/continuous-dataset` → `origin/feat/master-pipeline` | `a009acec` (2026-09-23, kuleczkomen) | `dataset/continuous/`: generator ciągłych strumieni testowych z adnotacjami, zamrożone manifesty seed 42/43/44 |

**Schemat manifestu** (`dataset_contract.py:42-58`): `id, filepath, sha256, bytes, label, kind, source, subclass, group_id, split, duration_s, sample_rate, channels, subtype, license`.

**Reguły grup** (`DATASET_CONTRACT.md`):
- `esc50_<fold>_<clipId>` (ujęcia A/B to jedno nagranie);
- `voice_synthetic_NNN` (miks);
- `datasec_<plik>` (1 plik = 1 grupa);
- Freesound `<źródło>_fs<idUploadu>`.

Podział robi `GroupShuffleSplit` 70/15/15 osobno w każdym źródle, seed 42.

**Wersjonowanie** semantyczne. MAJOR = zmiana składu lub podziału, wtedy wyniki są nieporównywalne. Wersji nie wolno nadpisywać (builder odmawia).

**Audio:** bez transkodowania i normalizacji. Zakres: sr ≥ 16 kHz, ≤ 2 kanały, 0,15–600 s (`dataset_contract.py`). Wszystkie pliki v2.0.0 to 44,1 kHz, mono, PCM_16.

### Walidator `validate_dataset.py`

| kod | typ | co sprawdza |
|---|---|---|
| K1 | błąd | schemat, dozwolone wartości, unikalne `id` |
| K2 | błąd | brakujące pliki |
| K3 | błąd | **przeciek**: grupa w więcej niż jednym splicie |
| K4 | błąd | sha256 różne od manifestu |
| K5 | błąd | identyczna treść w różnych splitach |
| K6 | błąd | audio nieczytelne lub poza zakresem |
| K7 | błąd | za mało grup pozytywnych w teście (min. 12) |
| K8 | błąd | podklasa VOICe niezgodna z katalogiem ekstrakcji |
| K9 | błąd | artefakt pochodny ma inną etykietę niż manifest |
| K10 | błąd | koncentracja: 90% pozytywów testu w < 12 grupach (`validate_dataset.py:62`, `:169-195`); w `DATASET_CONTRACT.md` brak opisu K10 |
| O1–O5 | ostrzeżenia | duplikaty wewnątrz splitu; udział pozytywów poza 1–60%; brak `kind` w splicie; licencja NC; artefakt bez `files.csv` |

Na artefakcie `spikes_v2` walidator przeszedł, „K10 OK: 90% pozytywów w 63/96 grup” (`models/WYNIKI.md` @ `origin/fah-metric-kn`).

## Artefakty spike'owe

| artefakt | z czego | kanały | stan | gdzie |
|---|---|---|---|---|
| `spikes_manifest` | stary `dataset/combined/manifest.csv` (13,5 k plików, już nie istnieje) | 6 | wycofany | lokalnie, 172 MB, 13 495 plików |
| `spikes_manifest7` | jw. | 7 | **wycofany (issue #36)**: 194/194 miksów VOICe testu jest też w treningu, ESC-50 „szkło” = `clock_tick` | lokalnie, 184 MB; train 10 819 / val 1338 / test 1338 plików |
| `spikes_v2` | v2.0.0 | 7 | **obowiązujący**: 10 853/10 853 plików, 0/69 wspólnych miksów | nie ma lokalnie (nieśledzony); ścieżka `architecture_14_neurons_patryk_09_07/spikes_v2/{train,val,test}`. Liczby w `freeze_manifest.json`: train 7116, val 1869, test 1870 plików |
| `spikes_ext` | `voice_extracted` (tylko VOICe) | 14 (7 HW + 7 z banku cech) | poligon selekcji cech, nie produkcja | `ga_neuron_search/spikes_ext` (nieśledzony) |

- Format CSV: `frame,s0..s6[,label]`, jeden plik na klip. Obok leżą `files.csv` (`filepath,label,kind,source,group_id,csv`) i `channels.json` z blokiem `provenance` (wersja zbioru, sha256 enkodera i banku cech).
- Obowiązki buildera (`DATASET_CONTRACT.md`): przerwać przy przecieku grup, przeplatać klasy w strumieniu, rozgrzewać floor tylko na `kind == stationary`, zapisać `files.csv`.
- Regresja: `snn_pipeline/tests/test_spike_artifact.py`.
- Odtworzenie: `python architecture_14_neurons_patryk_09_07/encoder_twin.py build-manifest --manifest dataset/versions/v2.0.0/manifest.csv --root . --out architecture_14_neurons_patryk_09_07/spikes_v2 --warmup-seconds 30`.

> [!blad] Wycofane źródła
> - `dataset/combined/manifest.csv` → `manifest.DEPRECATED.csv`: 5226 wierszy, bez VOICe, etykiety sprzed #34.
> - `DECYZJE_SESJI.md` §C1 opisuje plik 13,5 k pod tą samą ścieżką. Już nie istnieje, więc `spikes_manifest7` jest nieodtwarzalny (`DATASET_CONTRACT.md`, ostatnia sekcja).

## Okna treningowe
- `SpikeClips` tnie klipy na okna T=200 ramek (2 s), stride 50 (`snn_hw_pipeline.py:277`), z cache `_cache_T200_s50.npz`.
- Bez `--val-data` dzieli 80/20 po plikach (`split_by_file`), co **nie** usuwa przecieku po nagraniu źródłowym. Dlatego trzeba podawać katalogi splitów z manifestu.

## Znane ograniczenia (z `stats.md` v2.0.0)
- Pozytywy są zdominowane przez VOICe z 207 miksów. Niezależnych nagrań szkła jest o rząd wielkości mniej niż plików.
- Udział pozytywów: train ~40%, val/test ~46–47%.
- Tło stacjonarne to zastępnik (DataSEC + ESC-50), a nie nagranie docelowego pomieszczenia.
- ESC-50 jest niekomercyjne. Podzbiór do produktu wymaga filtra `license != 'CC BY-NC 3.0'`.
- **Niepewne:** status nagrań własnego tła i ciągłego strumienia testowego. `dataset/continuous` istnieje tylko na gałęziach `feat/continuous-dataset` i `feat/master-pipeline`.

## Powiązania
[[dataset]] · [[snn_pipeline]] · [[encoder]] · [[Architektury sieci]] · [[Wyniki i metryki]] · [[Model SNN — od dźwięku do decyzji]] · [[tests]] · [[Branche — szczegóły]]

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.
