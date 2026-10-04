---
typ: snn-modul
sciezka: main.py, test_forward.py
tags: [snn, modul]
zaktualizowano: 2026-10-05
---
# main.py i test_forward.py

> [!skrot] W 30 sekund
> - **Co to:** dwa pliki w korzeniu repo.
>   - `main.py` (6 linii) to szablon z `uv init`, wypisuje „Hello from snn-agent!”.
>   - `test_forward.py` (41 linii) to ręczny skrypt diagnostyczny starego modelu `GlassBreakSNN` (nie test pytest).
> - **Wejście → wyjście:** `test_forward.py`: sztuczny burst 5 spików (1 kanał, 10 kroków) → wypisuje progi, wagi, trigger i spiki N1/N2/N3/N_inh.
> - **Gdzie działa:** PC.

## Struktura
```
main.py           def main(): print("Hello from snn-agent!")
test_forward.py   GlassBreakSNN(quantize_mode="none") → forward na x[0,0,0:5]=1 → print
```

## Jak działa
- `test_forward.py` dokłada `.` do `sys.path` i importuje `snn_pipeline.snn_model.GlassBreakSNN` oraz `snn_pipeline.config.DEVICE`. Wypisuje `vth_*`, `w_*`, a potem wynik `model(x)` (trigger = częstość spików N3). Model: [[snn_pipeline]].
- Import `snn_pipeline.config` tworzy katalogi `data/`, `output/`, `checkpoints/` (efekt uboczny `PathConfig`).

## Jak uruchomić
```bash
python main.py
python test_forward.py      # z korzenia repo; wymaga torch + snntorch
```
Nie uruchamiano, bo import tworzy katalogi w repo (zasada „repo tylko do odczytu”).

## Konfiguracja i parametry
Brak. Wagi i progi startowe pochodzą z `BASELINE_NEURONS` (`snn_pipeline/config.py:166-195`).

## Wyniki
Brak zapisanych.

## Powiązania
[[snn_pipeline]] · [[tests]] (prawdziwe testy: `tests/`, `snn_pipeline/tests/`)

## Na gałęziach
Historia (`git log`): a12e4866 (2026-02-23, vityk-dev, „initial structure”) oraz 6caf6689 (2026-03-23, Patryk, „hat + qat 1 iteration”), czyli razem z pierwszą wersją `snn_pipeline`. Od tamtej pory bez zmian. **Niepewne:** który z tych commitów wprowadził który plik.

## Znane problemy / TODO
- Nazwa `test_forward.py` sugeruje test, ale pytest go nie zbierze sensownie: nie ma funkcji `test_*`, a kod wykonuje się przy imporcie.
- Testuje model historyczny, a nie `LuiNet`.

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.
