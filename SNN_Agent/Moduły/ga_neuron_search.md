---
typ: snn-modul
sciezka: ga_neuron_search/
tags: [snn, modul, ga]
zaktualizowano: 2026-10-05
---
# ga_neuron_search

> [!skrot] W 30 sekund
> - **Co to:** algorytm genetyczny szukający **topologii** (masek łączności) sieci SNN dla płytek Lu.i przy stałej liczbie neuronów N. Osobny run dla każdego N odpowiada na pytanie „ile płytek się opłaca”. Wagi, τ i V_leak dalej uczy backprop (`LuiLayer` z `snn_hw_pipeline.py`).
> - **Wejście → wyjście:** katalogi spike-CSV (train/val[/test]) → `wyniki_*.json/csv` (najlepsza topologia per N), checkpoint zwycięzcy `.pt` i `*_hw_config_N*.json` (nastawy płytek).
> - **Gdzie działa:** PC/Mac, CPU (MPS zmierzony ok. 3× wolniej), równolegle przez `--workers`.
> - **Kluczowy wynik:** N=8, topologia `7-4-3-1`, clip-F1 0,658 / FA 0,158 (2026-08-24). Liczony na `spikes_manifest7`, więc **wycofany** (issue #36).

## Struktura
```
ga_neuron_search/   (520 KB)
├── genome.py            reprezentacja, walidacja, repair, mutacje, krzyżowanie (bez torcha)
├── genome_new.py        1 linia „# nieużywane”
├── ga.py                silnik GA: GAConfig, turniej + elityzm, cache po Genome.key(), successive halving, ocena hurtem
├── net.py               GenomeNet na LuiLayer, genome_loss, AP, genome_eval_events (clip-F1/FA/AP)
├── fitness.py           synth_fitness, RealFitness (proxy-trening), ParallelFitness (pula procesów)
├── run_search.py        CLI: sweep po N, parsymonia, --train-winner, --tune-k
├── winner.py            train_full (HAT→QAT, 5 seedów), tune_k, export_genome_config
├── validate_hw_config.py round-trip: sieć odtworzona z hw_config_*.json → te same metryki
├── export_winner.py, calibrate.py, plot_history.py, build_ext_dataset.py (spikes_ext, 14 kanałów)
├── test_genome.py       testy niezmienników (bez torcha)
├── run.bat, run.ps1, run_real_sweep.bat
├── README.md, WYNIKI.md (dziennik, wiersze manifest7 oznaczone jako wycofane), TRAINING_REPORT.md, WYJASNIENIE_dla_zespolu.md
├── wyniki_{synth,demo,test,real,best,ext_hw7,ext_14}.{json,csv}, wyniki_demo.png
├── wyniki_real_hw_config_N8.json, wyniki_real_tuned_hw_config_N8.json, wyniki_fine_tuned_hw_config_N8.json (★ „champion”)
├── wyniki_best_tuned_hw_config_N5.json, wyniki_best_runner_N{7,9}_hw_config.json
└── logi: wyniki_real_run.log, ab_run.log, build_ext*.log, calibrate-results.txt, run-search-test.txt, _sizetest.txt
```

## Jak działa
- **Genom:** `Genome.layers` = lista warstw. Neuron to lista 1–3 indeksów wejść w warstwie poprzedniej. Warstwa 0 to cechy enkodera (nie liczy się do N), ostatnia warstwa to 1 neuron decyzyjny (README, „Reprezentacja genomu”).
- **Niezmienniki** (README, tabela): fan-in ≤ 3, fan-out ≤ 3, DAG, $n_{k+1} \le 3 n_k$, N stałe w runie, D ma fan-in = min(3, n_ost). Pokrycie cech wymuszane było do 2026-08-24; potem zastąpiła je selekcja cech `mut_drop_feature` z karą `--feature-penalty`.
- **Mutacje** (`genome.py:486-614`): `mut_rewire`, `mut_fanin`, `mut_move_neuron`, `mut_split_layer`, `mut_merge_layers`, `mut_drop_feature`. Do tego `crossover` (`:615`) i `repair` (`:287`) po każdej operacji.
- **GAConfig** (`ga.py:27-41`): pop 24, gens 15, elite 3, turniej 3, crossover_p 0,6, mutation_rate 1,2, max 4 warstwy ukryte, patience 6, screen_mult 1 (README: domyślnie 3 w `run_search.py`).
- **Fitness `RealFitness`** (`fitness.py:77`):
  - krótki proxy-trening `GenomeNet` (faza HAT, kilka epok);
  - ocena zdarzeniowa na val metryką `ap`, `clip_f1` albo `recall_fa` (`run_search.py:68`, recall @ `--stream-budget` 6 FA/h przez `stream_eval`);
  - minus kara za średni fan-out i liczbę cech;
  - cache po topologii.
  - Każdy worker resetuje seedy, więc wyniki są identyczne przy dowolnym `--workers` (README).
- **Zwycięzca:**
  - parsymonia wybiera najmniejsze N w granicach `parsimony_eps` od najlepszego;
  - `winner.train_full` robi pełne HAT→QAT (60 ep, domyślnie 5 seedów, `winner.py:31-34`);
  - `tune_k` sprawdza reguły dekodera k=1..6;
  - `export_genome_config` eksportuje nastawy;
  - `validate_hw_config.py` odtwarza sieć z JSON i porównuje metryki (tolerancja 0,02).

## Jak uruchomić
```bash
cd ga_neuron_search
python test_genome.py                                   # mechanika GA, bez torcha
python run_search.py --neurons 4 6 8 10 --mode synth --out wyniki_synth
python run_search.py --neurons 4 6 8 10 --mode real \
   --data <spikes>/train --val-data <spikes>/val --arch-dir ../architecture_14_neurons_patryk_09_07 \
   --k 1 --metric clip_f1|ap|recall_fa --epochs 12 --pop 24 --gens 15 --workers 18 --out wyniki_real
python run_search.py ... --pos-weight 1.5 --train-winner --tune-k 1 2 3 4 5 6
```
README ostrzega: zawsze podawaj `--val-data`. Bez niego `split_by_file` nie usuwa przecieku po nagraniu źródłowym.

**Uruchomione 2026-10-05:** `.venv/bin/python -B test_genome.py` daje „WSZYSTKIE TESTY PRZESZŁY” (random_genome N=2..11, 500 mutacji/N, 200 krzyżowań/N, zbieżność run_ga synth).

## Konfiguracja i parametry
- `--k` (domyślnie 2 w `run_search.py:67`; po zmianie 2026-08-24 dziennik `WYNIKI.md` zaleca k=1), `--metric` (domyślnie `clip_f1`), `--stream-budget 6`, `--fitness-seeds 3`, `--screen-mult 3`, `--parsimony-eps`, `--feature-penalty`, `--channels-head 7`, `--device cpu|cuda`.
- Budżet proxy-treningu: 12 epok, wybrany z `calibrate-results.txt`. Przy 2–8 epokach wariancja AP ±0,31, przy 12 epokach ±0,018 (`WYNIKI.md`).

## Wyniki
Zob. tabela B w [[Wyniki i metryki]]. W skrócie:

| data | co | wynik | źródło |
|---|---|---|---|
| 2026-08-24 | sweep N=4/6/8/10 (proxy AP, k=2) | N=4 0,4163 `7-3-1`; N=6 0,4158 `7-3-2-1`; **N=8 0,4483 `7-4-3-1`**; N=10 0,4328 `7-9-1` | `TRAINING_REPORT.md` §2, `wyniki_real.json` |
| 2026-08-24 | zwycięzca N=8, pw 3,0, k=2 | clip-F1 0,605, rec 0,831, prec 0,476, FA 0,229, AP 0,473; 156 parametrów | `TRAINING_REPORT.md` §3 |
| 2026-08-24 | pw 1,4, k=3 („champion”) | clip-F1 **0,658**, rec 0,800, prec 0,559, FA 0,158, AP 0,510 | `wyniki_fine_tuned_hw_config_N8.json`, commit 2edb9087 (Wiktor Goszczyński) |
| 2026-08-24 | A/B cech `spikes_ext` k=1 | 7 HW 0,611 (`7-5-2-1`) vs 14 kanałów 0,665 (`14-2-2-3-1`, wpiął crest i spectral_flatness) | `WYNIKI.md` |

Wszystkie powyższe liczono na `spikes_manifest7` (`wyniki_real_run.log:1-2`) albo na poligonie `spikes_ext`. **Nie spełniają kontraktu danych.** FA w tabeli to `clip_fa_rate` (odsetek klipów), a nie FA/h.

## Powiązania
- Używa `LuiLayer`/`SpikeClips` z `architecture_14_neurons_patryk_09_07/snn_hw_pipeline.py` (`--arch-dir`) i `snn_pipeline/stream_eval*` dla `recall_fa`.
- Konsumowany przez `master_pipeline/ga_runner.py` (gałąź `origin/feat/master-pipeline`).
- Zob. [[Architektury sieci]], [[snn_pipeline]], [[Model SNN — od dźwięku do decyzji]], [[Wyniki i metryki]].

## Na gałęziach
- Na `origin/main` katalogu nie ma.
- Wprowadzenie: commit 5b3a3b95 (2026-08-10, knocek, „karolina testuje esos”). Zmiana k=1 i bank cech: 164e0762 (2026-08-24). Kampania Wiktora: 4938d180 → 7a47a283 → 2edb9087 (2026-08-24).
- `origin/feat/training_N8` (e36bc467, 2026-08-24) i `origin/feat/testing-ideas` mają własne `TRAINING_REPORT.md`.
- `origin/feat/master-pipeline` (2026-09-24..26, MarcelDoro): checkpoint/resume w `run_ga` (55e12637), `ga.py` +190 linii, eksport hardware korzysta z checkpointu `winner.train_full` (762acd50), ranking championów między runami (`master_pipeline/champion.py`, 93808d40). `config.json` tam: dane `spikes_v2`, N ∈ {4, 6, 8}, pop 30, gens 20, metryka `clip_f1`.

## Znane problemy / TODO
- **Brak checkpointów:** `TRAINING_REPORT.md` wymienia `wyniki_real_winner_N8.pt`, `wyniki_fine_tuned_winner_N8.pt` itd. Żadnego `.pt` nie ma w katalogu ani na żadnej gałęzi (`git ls-tree`). Są tylko JSON-y z nastawami.
- Stara etykieta: `run_search` drukował „clip-F1”, a była to AP (`WYNIKI.md`, „Znany błąd”).
- Rekomendacje do zrobienia: front Pareto zamiast osobnych runów per N, granulacja N 4..10, weryfikacja konfiguracji na prawdziwych płytkach (`TRAINING_REPORT.md` §6).
- Potrzebny ponowny sweep na `spikes_v2` z `--metric recall_fa`. **Niepewne:** czy `master_pipeline` już go wykonał (w repo brak `runs/`).

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.
