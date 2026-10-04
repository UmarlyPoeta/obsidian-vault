---
typ: snn-modul
sciezka: praca_wtorek_2026_08_04/
tags: [snn, modul, sprzet, pomiary]
zaktualizowano: 2026-10-05
---
# praca_wtorek_2026_08_04

> [!skrot] W 30 sekund
> - **Co to:** pakiet pracy dla 3 osób na dzień ewaluacji sprzętowej, **wtorek 2026-08-04** („po wtorku nie ma dostępu do sprzętu”). Cel: przejść od „działa w symulacji” do „zmierzone na sprzęcie” dla sieci 8 płytek Lu.i (`README.md:1-22`). Do tego tor software (enkoder, input sieci, trening) do wykonania na laptopie.
> - **Wejście → wyjście:** zadania, protokół do wypełnienia ołówkiem i narzędzia → mają powstać wypełniony `08_PROTOKOL_POMIAROWY.md` oraz surowe dane w `pomiary/`.
> - **Gdzie działa:** stanowisko: mikrofon MAX4466 → Arduino Uno (`encoder_v2.ino`, wyjścia D2–D8) → 8 płytek Lu.i → wyjście D na **D9**. Tor software działa na PC.

> [!wazne] Stan w gicie: wszystko jest niezacommitowane
> Cały katalog to **pliki nieśledzone** (`git status`: `??`). `git log --all -- praca_wtorek_2026_08_04` nie zwraca nic, więc żadna gałąź go nie zawiera. Pliki powstały 2026-07-30 (mtime). Należą do 36 niezacommitowanych zmian drzewa roboczego, obok `docs/konferencja_2026/` i `uv.lock`. Wśród nich nie ma żadnych zmodyfikowanych plików śledzonych.

## Struktura (drzewo z opisem plików)
Wszystko poniżej to drzewo robocze (nieśledzone):
```
praca_wtorek_2026_08_04/
├── README.md                      # plan, harmonogram, role A/B/C, definicja „zrobione” (7 punktów)
├── 00_REVIEW_PROJEKTU.md          # review kodu: znaleziska P0/P1/P2 z plik:linia (34 kB)
├── 01_DECYZJE_WHAT_WHY_WHATFOR.md # rejestr decyzji A1–D3 (architektura, enkoder, trening, sprzęt)
├── 02_ZADANIA_A_tor_sygnalu.md    # osoba A: MAX4466, firmware, wierność bliźniaka
├── 03_ZADANIA_B_warstwa_H.md      # osoba B: kalibracja H0–H3, τ, RV4/RV6
├── 04_ZADANIA_C_G_D_integracja.md # osoba C: G0–G2 + D, integracja, metryki, pomiar prądu
├── 05_FAQ.md                      # pytania (README mówi o 28, priorytety w Q33)
├── 06_LITERATURA.md, 07_CO_POLEPSZYC_POTEM.md
├── 08_PROTOKOL_POMIAROWY.md       # ★ deliverable: tabele do wypełnienia
├── narzedzia/
│   ├── mic_check.ino              # ustawienie gainu MAX4466 bez oscyloskopu
│   └── capture_serial.py          # zapis strumienia spików z encoder_v2.ino do CSV + statystyki
├── pomiary/README.md              # tylko README: „tu wrzucacie surowe dane” (pomiarów brak)
└── software/                      # tor software: 00_SETUP…06_FOLLOWUP, requirements-sw.txt, wyniki/README.md
```

## Jak działa

### Co miało zostać zmierzone (definicja „zrobione”, `README.md`)
1. Tor wejściowy: DC offset, p-p w ciszy i na szkle, **zero przycięć**, pozycja pokrętła MAX4466.
2. Wierność bliźniaka: % ramek ze spikiem na kanał, sprzęt kontra `encoder_twin.py`, oraz czy `hf_lo/hf_hi` strzelają częściej na szkle.
3. Wszystkie 8 płytek przechodzi test n\* (zapisane n\*, pot%, LED%).
4. τ zmierzone dla co najmniej 2 płytek i rozstrzygnięte RV4/RV6.
5. Metryki zdarzeniowe na sprzęcie (N nagrań szkła / M nagrań tła) porównane z symulacją.
6. **Prąd warstwy analogowej** (µA na płytkę i łącznie) oraz prąd Arduino, „liczba, na której stoi wniosek grantowy”.
7. Surowe dane: CSV, zdjęcia trymerów, wideo, odczyty multimetru → `pomiary/`.

### Protokół (`08_PROTOKOL_POMIAROWY.md`)
Część 1, przed wtorkiem: A-2 tor MAX4466 → A0 (kolumny `dc, p-p, clip_hi, clip_lo, hf_pp`; dla ciszy cel dc 500–525 i p-p ≤ 15; dla szkła p-p 300–800 i clip = 0), B-1/C-1 sanity check płytek, B-2 kalibracja H (paski docelowe H0 10%, H1 10%, H2 15,5%, H3 22,4%), C-2 G/D (G0 17,1%, G1 10%, G2 10%, D 26,4%), B-3 stałe czasowe, B-4 RV4 kontra RV6, B-5 krzywa pot% → waga, C-3 integracja warstwami, bramka GO/NO-GO w poniedziałek wieczorem. Część 2, wtorek: warunki pomiaru, **C-4 metryki zdarzeniowe** (playlista 20+20), B#1 krzywa czułości RV5, B#4 zasięg, B#6 ablacja `hf_hi`, A-3 wierność bliźniaka, A-4 budżet ISR, B#5 FA/h na realnym tle, **C-6 prąd** (P_analog = I × 3,0 V, P_arduino = I × 5,0 V, czas pracy z CR2032 ok. 220 mAh), checklista B#2 surowych danych. Część 3 to podpis wyników.

### Narzędzia
- `narzedzia/capture_serial.py`: `--port --out --seconds` albo `--stats plik.csv`, wymaga pyserial, 115200 baud. Czyta linie CSV z firmware'u (pomija `#` i nagłówek `frame`), przyjmuje 8 pól `frame,b0..b6` z bitami 0/1 i zapisuje CSV `frame,s0..s6` zgodny z `SpikeClips` w `snn_hw_pipeline.py`. Kanały: `peak, peak_cnt, cv, zcr, flux, hf_lo, hf_hi`. Nominał to ok. 100,2 ramki/s (192 próbki przy 19231 Hz). Odniesienie z symulacji wpisane w kod: hf_lo szkło 25,9% kontra negatywy 2,6–4,3%, hf_hi 18,5% kontra 0,6–2,1%.
- `narzedzia/mic_check.ino`: MAX4466 OUT → A0, 115200 baud, wypisuje `dc, min, max, p-p, clip_hi, clip_lo, hf_pp` (górne pasmo liczone tym samym filtrem `lp += (x-lp)>>1` co enkoder, cutoff ok. 2,2 kHz).
- Tryb `CALIB` firmware'u (`C <pin> <n> 100`) generuje impulsy bez mikrofonu (`encoder_v2.ino:165-183`, wg README).

> [!blad] Pin D8
> Stara dokumentacja (`architektura_i_kalibracja.md:74`) każe podłączyć wyjście D do Arduino D8, ale w `encoder_v2.ino` D8 to wyjście kanału `hf_hi`. Prawidłowo: **D → D9** (znalezisko P0-1, decyzja D3).

### Tor software (`software/`)
Trzy równoległe tory: ENC (enkoder i wierność bliźniaka), IN (okno, topologia, liczba płytek), TRN (trening, rozrzut seedów). Pracują **tylko** na `architecture_14_neurons_patryk_09_07/snn_hw_pipeline.py`, a `snn_pipeline/` uznano za legacy (`software/README.md`). Setup zakłada `git checkout feat/testing-ideas` i `uv venv --python 3.12` albo `python3 -m venv .venv` (`software/00_SETUP_I_SMOKE_TEST.md`). Zależności w `software/requirements-sw.txt`: torch, numpy, librosa, scipy, soundfile, matplotlib, opcjonalnie snntorch. Wyniki mają trafiać do `software/wyniki/{ENC,IN,TRN,00_SYNTEZA}.md`, a dziś jest tam tylko README.

## Jak uruchomić
```sh
python3 praca_wtorek_2026_08_04/narzedzia/capture_serial.py --port /dev/ttyACM0 --out hw_glass_01.csv --seconds 20
python3 praca_wtorek_2026_08_04/narzedzia/capture_serial.py --stats hw_glass_01.csv
```

## Konfiguracja i parametry
Sieć 7→4→3→1, nastawy z `architecture_14_neurons_patryk_09_07/hw7_config.json` (deklaruje `source_checkpoint: hw7_s2.pt`, którego nie ma w repo, P0-3).

## Wyniki
**Pomiarów brak.** `pomiary/` zawiera tylko README, `software/wyniki/` tylko README, protokół jest niewypełnionym szablonem. Wartości odniesienia z symulacji, cytowane przez pakiet:

| Źródło | F1 okien. | recall klipów szkła | FA klipów | F1_min |
|---|---|---|---|---|
| `README.md` („Stan wejściowy”) | 0,609 | 72% | **20%** | 0,550 |
| `software/wyniki/README.md` (`hw7_config.json`, zbiór `spikes_manifest7`) | 0,609 | 0,721 | **0,237** | 0,550 |

Rozbieżność FA (20% kontra 23,7%) jest w samych plikach. **niepewne:** która liczba jest aktualna. Pełne metryki: [[Wyniki i metryki]].

## Powiązania
[[lu.i-neuron-pcb]] (płytki), [[encoder]] (`encoder_v2.ino`, `encoder_twin.py`), [[Architektury sieci]] (`architecture_14_neurons_patryk_09_07`), [[Dane i zbiory]] (`spikes_manifest7`, `voice_extracted`), [[Sprzęt i wdrożenie]].

## Na gałęziach
Nie ma go na żadnej gałęzi (zob. ramkę wyżej). Tor software odsyła do gałęzi `feat/testing-ideas` (szczegóły: [[Branche — szczegóły]]).

## Znane problemy / TODO
- Znaleziska P0 z `00_REVIEW_PROJEKTU.md`: P0-1 pin D8, P0-2 docstring kontra kod członu `(1-β)`, P0-3 brak checkpointu `hw7_s2.pt` (`.gitignore: **/*.pt`), P0-4 zepsuty submoduł Lu.i (później naprawiony w W0, ale katalog nadal jest pusty).
- `capture_serial.py` czyta format CSV z `encoder_v2.ino`, a nie protokół v1 (`$F…*CRC`) z [[rpi_agents]]. To dwa różne formaty serial.
- **niepewne:** czy ewaluacja 2026-08-04 w ogóle się odbyła. W repo nie ma śladu wyników.

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.
