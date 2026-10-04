---
typ: snn-modul
sciezka: snn_simulator/
tags: [snn, modul]
zaktualizowano: 2026-10-05
---
# snn_simulator

> [!skrot] W 30 sekund
> - **Co to:** wczesny szkic symulatora SNN z danymi syntetycznymi (HF/LF/obwiednia). Ma prosty neuron LIF w numpy i pętlę treningową w torch. **Nie działa samodzielnie:** `main.py` importuje nieistniejący moduł `models.snn_torch`.
> - **Wejście → wyjście:** losowe próbki (T=100, 3 kanały) → strata BCE; opcjonalnie WAV → cechy (T, 3).
> - **Gdzie działa:** PC. Status historyczny, zastąpiony przez `snn_hw_pipeline.py` (zob. [[Architektury sieci]]).

## Struktura
```
snn_simulator/
├── main.py                 from models.snn_torch import GlassSNN → train_torch(model, epochs=10)   (import nie istnieje)
├── config.py               T=100, INPUT_SIZE=3, HIDDEN_SIZE=8, OUTPUT_SIZE=2, BETA=0.9, THRESHOLD=1.0, LR=0.01, EPOCHS=10
├── neurons/lif.py          LIFNeuron: V = β·V + I; spike gdy V ≥ próg, reset V=0
├── training/train_torch.py Adam lr 0.003, BCEWithLogits, 100 próbek/epokę; woła model.clamp_weights(), model.quantize_weights()
├── training/loss.py        MSE częstości wyjść vs one-hot (2 klasy)
└── utils/
    ├── synthetic_data.py   „glass”: wysoki HF + burst obwiedni; „normal”: wysoki LF
    ├── data.py             generate_sample_torch — losowe spiki z przesunięciem dla etykiety
    └── audio_features.py   librosa: STFT (n_fft 512, hop 256, sr 16 kHz), energia >2 kHz, <500 Hz, onset envelope → (T,3)
```

## Jak działa
> [!wzor] Neuron w `neurons/lif.py`
> $V[t] = \beta\,V[t-1] + I[t]$, spike gdy $V \ge V_{th}$, potem $V \leftarrow 0$ (`lif.py:12-19`).

To prostszy model niż w `snn_hw_pipeline.py`: bez synapsy prądowej i bez $V_{leak}$. Pełny model: [[Model SNN — od dźwięku do decyzji]].

## Jak uruchomić
Nie da się bez dopisania `models/snn_torch.py` z klasą `GlassSNN` (metody `clamp_weights`, `quantize_weights`). Takiego pliku nie ma na żadnej sprawdzonej gałęzi (`main`, `origin/main`, `origin/dev`, `origin/hat-metrics-work`, `origin/snn/ready-for-test-pipeline`). Ścieżki importów są względne (`from training…`, `from utils…`), więc wymagają uruchamiania z katalogu `snn_simulator/`.

## Konfiguracja i parametry
`config.py`, wartości wyżej. Nie są używane przez `train_torch.py`, który ma lr 0,003 wpisane na sztywno.

## Wyniki
Brak.

## Powiązania
[[snn_pipeline]] · [[Architektury sieci]] · [[experiments]]

## Na gałęziach
Identyczny zestaw plików na lokalnym `main` (9408e71e, 2026-05-18, Karolina Nocek, „create training based on umarly-poeta notebooks”) i na `origin/main`. Od tamtej pory bez zmian.

## Znane problemy / TODO
- Brakuje `models/snn_torch.py`, a `main.py` rzuca ImportError.
- Korzeń `models/` w repo to katalog checkpointów, nie pakiet Pythona (zob. [[models]]). Kolizja nazw.
- **CURRENT_ASSUMPTIONS:** „Patryk kończy symulator i eksport championa”. Chodzi zapewne o symulator runtime ([[snn_runtime]]), a nie o ten katalog. **Niepewne.**

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.
