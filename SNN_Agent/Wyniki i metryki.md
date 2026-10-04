---
typ: snn-zbiorcza
tags: [snn, wyniki, metryki]
zaktualizowano: 2026-10-05
---
# Wyniki i metryki

> [!skrot] W 30 sekund
> - **Metryka decyzyjna:** recall przy budżecie FA/h (fałszywe alarmy na godzinę tła), z rozbiciem po `kind` i CI bootstrapem po `group_id`. Próg „gotowe”: **recall ≥ 0,70 @ ≤ 6 FA/h na teście, żaden `kind` > 6 FA/h** (`docs/DATASET_CONTRACT.md`, sekcja „Próg akceptacji”).
> - **Czysty benchmark (`spikes_v2` / v2.0.0):** najlepszy zapisany wynik to recall **86,3% przy 219 FA/h** (k=1, test). recall @ 6 FA/h = **0** we wszystkich przebiegach, czyli budżet jest nieosiągalny (`models/WYNIKI.md` na gałęzi `origin/fah-metric-kn`).
> - **„Champion” GA clip-F1 0,658 / FA 0,158 (N=8, 7-4-3-1) jest WYCOFANY:** liczony na `spikes_manifest7`, który ma przeciek train/test i `clock_tick` zamiast szkła (issue #36).
> - Formalnie **nie ma zamrożonego championa** (`docs/project/CURRENT_ASSUMPTIONS.md`: „końcowy champion … wymaga weryfikacji”). Ranking championów jest dopiero w budowie (`master_pipeline/champion.py` na `origin/feat/master-pipeline`).

## Definicje metryk (wg kodu)

| metryka | definicja | kod |
|---|---|---|
| **F1 ramkowe/okienkowe** | okno 2 s jest pozytywne, gdy $\max_t V_D \ge V_{th}$; TP/FP/FN po oknach | `architecture_14_neurons_patryk_09_07/snn_hw_pipeline.py:419-433` |
| **clip_recall / clip_fa_rate** (pipeline) | klip alarmuje, gdy któreś jego okno ma ≥ k spików D; `clip_fa_rate` = **odsetek klipów tła** z alarmem (to nie jest „na godzinę”!) | `snn_hw_pipeline.py:436-465` |
| **clip_f1 / AP** (GA) | jak wyżej plus F1 po klipach; AP liczone po ciągłym score = max spików D w klipie | `ga_neuron_search/net.py:118-164` |
| **FA/h** | alarmy reguły (k, w) z refrakcją 500 ramek na klipach tła, podzielone przez godziny tła = Σ ramek klipów tła · 0,01 s / 3600. Liczone też per `kind` | `snn_pipeline/stream_eval.py:59-72`, `:128-139`, `:179-183` |
| **recall** (strumień) | odsetek klipów pozytywnych z ≥ 1 alarmem | `stream_eval.py:132` |
| **recall @ budżet B** | najwyższy recall spośród reguł (k, w), dla których FA/h łączne **i każdego** `kind` ≤ B. Gdy żadna reguła się nie mieści, wynik to 0,0 (niewykonalne) | `stream_eval.py:142-155` |
| **CI recall** | bootstrap 500× po `group_id`, percentyle 2,5 i 97,5 | `stream_eval.py:191-207` |
| **odporność** | F1 przy losowym rozrzucie trymerów, τ i V_leak (5 przebiegów), plus ensemble D z głosowaniem 2 z 3 | `snn_hw_pipeline.py:468-510` |
| precision/recall/F1/FNR (stary pipeline) | progowanie częstości spików N3 ≥ 0,5; precision = 1,0, gdy brak predykcji pozytywnych | `snn_pipeline/metrics.py:15-198` |

> [!blad] Pułapka: „FA” ≠ „FA/h”
> W raportach GA i `hw_config.json` „FA 0,158” oznacza odsetek klipów tła z alarmem (`clip_fa_rate`), a nie alarmy na godzinę. FA/h liczy tylko `stream_eval.py`, który powstał 2026-08-30 (commit `7861316d`, Karolina Nocek, #37).

> [!wazne] FA/h — rozjazd z protokołem
> `CURRENT_ASSUMPTIONS.md` (pkt 3) wymaga, by mianownikiem FA/h była „ekspozycja negatywna po odjęciu przedziałów zdarzeń i tolerancji” na **ciągłym strumieniu**. `stream_eval.py` liczy na osobnych klipach (całe klipy tła). Ciągły strumień testowy powstaje dopiero na `origin/feat/continuous-dataset` i `origin/feat/master-pipeline` (`dataset/continuous/`). **Niepewne:** czy jakiekolwiek wyniki policzono już na nim.

## Tabela wyników (wszystkie źródła)

### A. Czysty benchmark `spikes_v2` (dataset v2.0.0) — liczby obowiązujące

| data | run | topologia | recall (test) | FA/h (test) | inne | źródło |
|---|---|---|---|---|---|---|
| 2026-09-08 | Wiersz 1, baseline, `spk_w=0`, `pos_weight=3.0`, seed 0, k=1 | 7-4-3-1 (ręczna) | **86,3%** | **219** | k=2: 75,3% / 155; k=3: 69,3% / 139; recall @ 1 i @ 6 FA/h = 0; okienkowe test F1 0,603 (rec 0,890, prec 0,456) | `models/WYNIKI.md` @ `origin/fah-metric-kn` (e09456a8) |
| 2026-09-14 | Wiersz 2, `spk_w=0.5`, `pw=1.5`, s0 | 7-4-3-1 | 62,0% | 113 | speech 298 FA/h, animal 130, stationary 104, loud_event 62; okienkowe f1 0,72 / prec 0,60 | jw. |
| 2026-09-14 | Wiersz 3, `spk_w=0.3`, `pw=1.0`, s0 | 7-4-3-1 | 67,3% | 139 | speech **376** FA/h | jw. |
| 2026-09-22 | A/B enkodera `base` vs `swap` (5 seedów) | 7-4-3-1 | — | 78–260 (zależnie od reguły) | najlepszy `swap_s2`: F1 val 0,763 / **test 0,683** / odporność 0,755; `base` najlepszy s2/s4: test 0,648. Budżet 1 i 6 FA/h „całkowicie nieosiągalny” | `encoder/features-improvement/WNIOSKI.md` @ `origin/feat/encoder-features` (c9ff2346, kuleczkomen) |

Pliki modeli z wierszy 1–3 (`models/v2_*.pt`, `models/hw_v2_*.json`) są tylko na `origin/fah-metric-kn`. Zob. [[models]].

### B. Wyniki WYCOFANE (`spikes_manifest7` / `spikes_manifest`, issue #36), zachowane dla historii

| data | run | topologia | metryki | źródło |
|---|---|---|---|---|
| 2026-07-14 | „14 neuronów”, 6 kanałów, run A, pw 1,0, seed 1 | 6-4-3-1 | val F1 0,512, test F1 0,487; k=1 clip recall 0,726 / clip FA 0,636; odporność min 0,355 | `architecture_14_neurons_patryk_09_07/hw_config.json` (c1f38a7c) |
| 2026-07-14 | „15 neuronów”, 7 kanałów v3 widmowy, seed 2, pw 1,0 | 7-4-3-1 | test F1 **0,609** (rec 0,859, prec 0,472); k=1 clip recall 0,721 / FA 0,237; odporność min 0,550 | `hw7_config.json`, `DECYZJE_SESJI.md` E10 |
| 2026-08-24 | GA sweep N=4/6/8/10 (proxy AP) | N=8: 7-4-3-1 | proxy 0,4483; zwycięzca po 60 ep: clip-F1 0,605, rec 0,831, prec 0,476, FA 0,229, AP 0,473 (k=2, 649 klipów val) | `ga_neuron_search/TRAINING_REPORT.md` (4938d180, Wiktor Goszczyński) |
| 2026-08-24 | strojenie pw=1,5, k=3 | 7-4-3-1 | clip-F1 0,654, FA 0,170 | jw. (7a47a283) |
| 2026-08-24 | **„champion” pw=1,4, `tuned_k=3`** | 7-4-3-1 (N=8) | **clip-F1 0,658, rec 0,800, prec 0,559, FA 0,158, AP 0,510**; round-trip eksportu PASS | `TRAINING_REPORT.md` Follow-up 2, `wyniki_fine_tuned_hw_config_N8.json` (2edb9087) |
| 2026-08-24 | GA rankowany clip-F1 | N=5 7-4-1 | pełny trening 0,597 (pw 2,0) | `wyniki_best.json` |
| 2026-08-24 | A/B cech na `spikes_ext` (poligon, 14 kanałów) | 14-2-2-3-1 | fitness 0,665 (7 HW: 0,611) | `ga_neuron_search/WYNIKI.md` |

To, że te wyniki liczono na `spikes_manifest7`, potwierdzają `wyniki_real_run.log` (linie 1–2) i `run_real_sweep.bat:11-12`. Oznaczenie jako wycofane: `ga_neuron_search/WYNIKI.md`, `DECYZJE_SESJI.md` (commit adb3ada7, 2026-08-31).

> [!blad] Martwy odnośnik
> `DECYZJE_SESJI.md` i `ga_neuron_search/WYNIKI.md` odsyłają do `models/WYNIKI_v2.md`. Takiego pliku nie ma na żadnej gałęzi (`git ls-tree` wszystkich refów). Odpowiednikiem jest `models/WYNIKI.md` na `origin/fah-metric-kn`.

### C. Stary pipeline `snn_pipeline` (4 neurony, ESC-50)
Brak zapisanych liczb w repo. `run_pipeline.py` zapisuje raporty do `output/`, który nie jest śledzony. **Niepewne.**

## Aktualny „champion”

- **Na dziś (stan plików 2026-10-05):** brak championa spełniającego kontrakt.
- Najlepszy obowiązujący punkt na czystym zbiorze: **7-4-3-1, recall 86,3% @ 219 FA/h (k=1)** i 69,3% @ 139 FA/h (k=3), test v2.0.0, seed 0, 2026-09-08 (`models/WYNIKI.md` @ `origin/fah-metric-kn`). Tę samą liczbę cytują nieśledzone materiały konferencyjne `docs/konferencja_2026/_build/doc3.py:116`.
- Najwyższe okienkowe F1 na v2.0.0: `swap_s2` test **0,683** (`origin/feat/encoder-features`). Tam też FA/h ≥ 78.
- Mechanizm wyboru championa: `master_pipeline/champion.py` (`origin/feat/master-pipeline`, 93808d40, 2026-09-26, MarcelDoro) odrzuca porównania z różnym commitem, enkoderem lub manifestem. Przy recall_fa = 0 zwraca `status="infeasible"` zamiast zwycięzcy.

## Wnioski zespołu (z plików)
- Ścianą jest **mowa** (`speech` 298–376 FA/h), to problem rozróżnialności, a nie progu (`models/WYNIKI.md`, Wiersz 3).
- Wcześniej (lipiec) sufitem okazał się enkoder pełnopasmowy. Dodanie kanałów widmowych hf_lo/hf_hi zmniejszyło FA klipowe 3× (`DECYZJE_SESJI.md` D4, E10, liczby wycofane).
- Różnicy mniejszej niż szum (±0,060 clip-F1 wg `ga_neuron_search/calibrate-results.txt`) nie wolno nazywać poprawą (`DATASET_CONTRACT.md`). Rozrzut między seedami 0,53–0,59 val-F1 (`models/README.md`).

## Powiązania
[[Model SNN — od dźwięku do decyzji]] · [[Dane i zbiory]] · [[ga_neuron_search]] · [[models]] · [[snn_pipeline]] · [[Architektury sieci]] · [[Branche — szczegóły]]

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.
