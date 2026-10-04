---
typ: snn-modul
sciezka: encoder/
tags: [snn, modul, enkoder]
zaktualizowano: 2026-10-05
---
# encoder

> [!skrot] W 30 sekund
> - **Co to:** katalog zespołu enkodera, czyli audio → spiki. Zawiera trzy rzeczy:
>   - **bank cech** `feature_bank/`: 18 kanałów, bliźniak cyfrowy, statystyki separowalności i koszt w cyklach MCU;
>   - **stare szkice Arduino**: `snn_encoder.ino` (rate/TTFS na jednym pinie) i `snn_decoder.ino` (licznik spików);
>   - plan `ENCODER_PLAN.md`.
> - **Aktualny enkoder sprzętowy jest gdzie indziej:** `architecture_14_neurons_patryk_09_07/encoder_v2.ino` i bliźniak `encoder_twin.py` (zob. [[Architektury sieci]], [[Model SNN — od dźwięku do decyzji]]). Zamrożony wariant `encoder_v3` / `encoder_v2_swap.ino` jest na gałęzi `feat/encoder-features`.
> - **Wejście → wyjście:** WAV → macierz cech/spików na ramkę 10 ms (`feature_bank.npz`); mikrofon (A0) → impulsy TTL.
> - **Gdzie działa:** PC (bank, statystyki); Arduino Uno (`.ino`).

## Struktura
```
encoder/
├── ENCODER_PLAN.md               „QUICK START”: mikrofon WM-64 na A0, wysyłka spike'ów do RPi przez SPI (piny 10–13)
├── snn_encoder/snn_encoder.ino   szkic: amplituda p-p w oknie 50 ms; MODE_TTFS (domyślny: spike przy delta>30, refrakcja 100 ms) lub MODE_RATE_CODING (1–10 impulsów); impuls 0,5 ms na D2
├── snn_decoder/snn_decoder.ino   przerwanie na D3 (FALLING), liczba spików na sekundę na Serial
└── feature_bank/
    ├── __init__.py               eksport: ALL_CHANNELS, HW_CHANNELS, NEW_TIME_CHANNELS, FFT_CHANNELS, encode_signal, encode_wav_file
    ├── feature_metrics.py        18 funkcji cech (FEATURE_FUNCTIONS)
    ├── digital_twin_encoder.py   bliźniak encoder_v2 z kanałami rozszerzonymi; McuComplexity (szacunek cykli)
    ├── build_feature_bank.py     WAV → feature_bank.npz
    ├── feature_bank_stats.py     sanity, Cohen's d per kanał, baseline regresji logistycznej (split po plikach)
    ├── esos_time_analysis.py     szacunek czasu ramki per cecha
    └── README.md                 wyniki powyższych (liczby poniżej)
```

## Jak działa
- `digital_twin_encoder.py`:
  - `FS_HZ_HW = 19231`, `HOP_MS = 10`, `DC_ALPHA = 1/512`, `PRIME_FRAMES = 50` (`:134-138`);
  - grupy kanałów `HW_CHANNELS` / `NEW_TIME_CHANNELS` / `FFT_CHANNELS` (`:140-160`);
  - `encode_signal` (`:258`), `encode_wav_file` (`:459`).
- 18 cech (`feature_metrics.py:17-99`): peak, peak_cnt, crest, cv, zcr, flux, hjorth_mobility, tkeo_mean, curve_length, autocorr_lag1, kurtosis, spectral_centroid, dominant_freq, band_energy_low/mid/high, spectral_flatness, spectral_flux.
- Z banku powstaje artefakt `spikes_ext` (14 kanałów), budowany przez `ga_neuron_search/build_ext_dataset.py`.

## Jak uruchomić
```bash
python encoder/feature_bank/build_feature_bank.py     # → feature_bank.npz
python encoder/feature_bank/feature_bank_stats.py
python encoder/feature_bank/esos_time_analysis.py
```
Szkice `.ino`: Arduino IDE, Board Arduino Uno (`ENCODER_PLAN.md`).

## Wyniki (z `feature_bank/README.md`)
- Bank: 925 435 ramek, 18 kanałów, 13 401 plików; pozytywne 297 008 / negatywne 628 427 ramek.
- Cohen's d (|d| > 0,2 = sygnał):
  - najlepsze: spectral_flatness +1,305, spectral_centroid +1,269, band_energy_low −1,245, zcr +1,154, hjorth_mobility +1,140, autocorr_lag1 −1,041;
  - martwe: peak −0,011, peak_cnt −0,051, kurtosis +0,003.
- Regresja logistyczna (split po plikach): acc 0,835 (baseline 0,698), bal. acc 0,766, F1 0,684, ROC-AUC 0,858.
- Czas ramki (szacunek): 172,64 µs z 10 ms budżetu (1,7% CPU).
- **Niepewne:** na jakim zbiorze zbudowano bank (13 401 plików nie zgadza się z v1.0.0 ani v2.0.0). Prawdopodobnie stary `dataset/combined`.

> [!blad] Zły procesor w szacunku
> `esos_time_analysis.py` i `McuComplexity.cycles_cortex_m4f` liczą cykle dla „Cortex-M4F @ 64 MHz”. Projekt używa Arduino Uno (ATmega328P, 8-bit, bez FPU), więc te czasy nie przenoszą się na Uno. Na gałęzi `feat/encoder-features` dodano `esos_time_analysis_avr.py` i pomiary na AVR (zob. niżej).

## Powiązania
- Bliźniak produkcyjny: `architecture_14_neurons_patryk_09_07/encoder_twin.py` ↔ `encoder_v2.ino`. Opis w [[Architektury sieci]].
- Kontrakt kanałów `EncoderProfile` i `encoder_hash`: [[contracts]].
- GA (selekcja cech na `spikes_ext`): [[ga_neuron_search]].
- Runtime na RPi czyta ramki z Uno: [[snn_runtime]], [[rpi_agents]].
- Stanowisko pomiarowe: [[praca_wtorek_2026_08_04]].

## Na gałęziach
- `origin/main`: tylko `ENCODER_PLAN.md` i dwa szkice `.ino`. `feature_bank/` przyszedł z `feat/encoder` (import 30f5bccd, 2026-08-17, marcel).
- **`origin/feat/encoder-features`** (kuleczkomen, 2026-09-22..26; włączona też do `origin/feat/testing-ideas` przez PR #50) dodaje `encoder/features-improvement/` i `encoder/contract/`:
  - wariant **`swap`**: kanał 1 `peak_cnt` → `hjorth_mobility`, kanał 2 `cv` → `autocorr_lag1`, progi `mob_thr = 2,4973`, `ac_thr = −0,2336`;
  - Faza 0: Cohen's d +1,046 / −1,125, ΔAUC GBM +0,028 (CI95 [+0,016; +0,041]);
  - pomiary ISR na Uno: prescaler 32 → fs ok. 27 050 Hz i 99,73% CPU; **prescaler 64 → 19 230,5 Hz**, 71% CPU; `swap_full` 35,54 µs/ISR (569 cykli), 68,3% CPU, `late=0`;
  - A/B na `spikes_v2` (5 seedów): `swap_s2` test F1 0,683 vs najlepszy `base` 0,648; FA/h 78–260, budżet nieosiągalny (`WNIOSKI.md`);
  - test parity C++ ↔ Python w `simavr` (`tools/parity_test.py`, `parity.json`);
  - **K1 ukończone:** zamrożony `encoder_config.json` / `encoder-profile.json` (470286a5, 5ab343d1). `master_pipeline/freeze_manifest.json` podaje `profile_id encoder_v3` = `encoder_v2_swap.ino`, `encoder_hash sha256:fec5ee94…`.
- `origin/feat/master-pipeline` importuje ten zamrożony enkoder (229ae9da).

## Znane problemy / TODO
- `snn_encoder.ino` wysyła wszystko jednym pinem (D2) z impulsem 0,5 ms. Nie zgadza się to z siecią 7-kanałową (impulsy 6 ms, D2–D8). Szkic jest historyczny.
- `ENCODER_PLAN.md` zakłada łącze SPI do RPi. Aktualna architektura to USB serial Uno → RPi (`CURRENT_ASSUMPTIONS.md`).
- Rozjazd nazw kanałów: w eksportach ramienia `swap` widnieją stare nazwy `peak_cnt`/`cv` (`WNIOSKI.md` §5). Naprawione na `feat/master-pipeline` (72e667f1, „Fix feature-channel name drift”).

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.
