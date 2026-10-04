---
typ: snn-modul
sciezka: models/
tags: [snn, modul, modele]
zaktualizowano: 2026-10-05
---
# models

> [!skrot] W 30 sekund
> - **Co to:** katalog na checkpointy i konfiguracje sprzętowe, celowo śledzony w gicie mimo reguł `**/models/` i `**/*.pt` (wyjątek `!/models/` w `.gitignore`).
> - **Na bieżącej gałęzi zawiera tylko `README.md`.** Jedyne prawdziwe checkpointy (`v2_*.pt`) są na gałęzi `origin/fah-metric-kn`.
> - **Wejście → wyjście:** `snn_hw_pipeline.py train` → `.pt` (wagi + epoka + faza), `.json` (nastawy płytek + metryki + `model_provenance`), `train_log_*.csv`.
> - **Gdzie działa:** PC (trening); JSON idzie na płytki Lu.i i do runtime.

## Konwencja (z `models/README.md`)
```
models/hw_<wersja_zbioru>_s<seed>.pt      checkpoint (wagi + epoka + faza)
models/hw_<wersja_zbioru>_s<seed>.json    konfiguracja sprzętowa + metryki
models/train_log_<wersja>_s<seed>.csv     przebieg treningu per epoka
```
- `v2` oznacza artefakt spike'owy zbudowany z `dataset/versions/v2.0.0`.
- Checkpoint sieci kilkunastoneuronowej waży kilkadziesiąt kB.
- Nie trzymać tu artefaktów spike'owych.
- Motywacja: do sierpnia 2026 w repo nie było żadnego `.pt` („żadnego wyniku nie dało się ponownie ocenić”). Rozrzut między seedami 0,53–0,59 val-F1 jest większy niż różnice, o które toczył się spór.

## Inwentaryzacja modeli we wszystkich miejscach repo

| plik | gdzie (ref) | format | z jakiego runu | metryki | uwagi |
|---|---|---|---|---|---|
| `models/v2_recallfa_s0.pt` + `hw_v2_recallfa_s0.json` + `train_v2_recallfa_s0.csv` + `train_v2_s0.log` | `origin/fah-metric-kn` | torch state_dict + JSON | Wiersz 1, 2026-09-08, `spikes_v2`, seed 0, pw 3,0, `spk_w` 0, 120 ep | test k=1: recall 86,3%, 219 FA/h | **najlepszy recall na czystym zbiorze** |
| `models/v2_spk05_pw15_s0.{pt,csv,out}` + `hw_v2_spk05_pw15_s0.json` | jw. | jw. | Wiersz 2, 2026-09-14 | 62,0% / 113 FA/h | |
| `models/v2_spk03_pw10_s0.{pt,csv,out}` + `hw_v2_spk03_pw10_s0.json` | jw. | jw. | Wiersz 3, 2026-09-14 | 67,3% / 139 FA/h | |
| `models/WYNIKI.md` | jw. | md | dziennik wierszy 1–3 | | ostatni commit e09456a8 (2026-09-14, Karolina Nocek) |
| `architecture_14_neurons_patryk_09_07/hw_config.json` | bieżąca | JSON nastaw | v2 6 kanałów, run A, seed 1 (2026-07-14) | test F1 0,487 | wycofane; checkpoint `sweep_pw10_s1.pt` nie istnieje |
| `architecture_14_neurons_patryk_09_07/hw7_config.json` | bieżąca | JSON | v3 7 kanałów, seed 2 | test F1 0,609 | wycofane; `hw7_s2.pt` nie istnieje |
| `…/hw7_s{1,2,3}.json`, `hw15_s{1,2,3}.json`, `sweep_*_s*.json` (+ `.csv` logi) | bieżąca | JSON + CSV | sweepy lipcowe | | wycofane (`spikes_manifest(7)`) |
| `ga_neuron_search/wyniki_*_hw_config_N{5,7,8,9}.json` | bieżąca | JSON nastaw + `topology` | GA 2026-08-24 | N8 fine: clip-F1 0,658 | wycofane; `.pt` wymienione w `TRAINING_REPORT.md` nie istnieją |
| `contracts/fixtures/demo-model.txt` | bieżąca | znacznik tekstowy | — | — | demo, bez wag |

Brak `models/WYNIKI_v2.md`, do którego odsyłają `DECYZJE_SESJI.md` i `ga_neuron_search/WYNIKI.md`.

## Format `.json` (eksport `export_config`)
Klucze:
- `dt_s`, `v_th`, `channels`;
- `boards{nazwa: {tau_syn_ms, tau_mem_ms, v_leak, led_bar_pct, scale_k, synapses[{port, from, sign, pot_pct, w_sim, pulses_to_fire_100Hz}]}}`;
- `model_provenance` (seed, pos_weight, hat_frac, epochs, quantized, checkpoint, train_data z `channels.json`, val_data, test_data, git_commit, trained_utc);
- `val_metrics`, `val_events`, `robustness`, `robustness_ensemble`, `test_*`, `val_stream`/`test_stream` (recall @ 1 i 6 FA/h).

GA dodaje `topology`, `pos_weight`, `tuned_k`, `tune_k_table`. Opis: `snn_hw_pipeline.py:557-622`, `:808-847`.

## Na gałęziach
- `models/README.md` jest wszędzie od commitu adb3ada7 (2026-08-31, UmarlyPoeta, „track checkpoints”).
- Pliki modeli są tylko na `origin/fah-metric-kn` (nie wmergowane do `master`).
- Kontrakt `ModelManifest` (status `evaluated_champion`, hashe, provenance) jest w [[contracts]].

## Znane problemy / TODO
- Brak championa w `master`. Najlepsze checkpointy leżą na niezmergowanej gałęzi.
- Wiersz `.json` nie zgadza się z konwencją nazw README (`hw_v2_recallfa_s0` vs `hw_<wersja>_s<seed>`). Drobiazg.

## Powiązania
[[Wyniki i metryki]] · [[Architektury sieci]] · [[ga_neuron_search]] · [[contracts]] · [[snn_runtime]]

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.
