---
typ: solar-pakiet
repo: "[[l2dp]]"
build: ament_python
tags: [solar, pakiet]
zaktualizowano: 2026-10-04
---
# signals_detector

> [!skrot] W 30 sekund (co robi, wejście → wyjście)
> Pythonowa wersja wykrywania sygnałów dźwiękowych: PyAudio (urządzenie zawierające „pulse” lub domyślne) → FFT z dopasowaniem paraboli piku → wykrywanie dźwięków 600/800/1000 Hz i wzorców krótki/długi → `detected_signals` (`std_msgs/String`). Równoległa implementacja do [[blast_detection]].

## Węzły
- `signals_detector_node` — tworzony w `main()` funkcją `rclpy.create_node` (`signals_detector/signals_detector.py:99–101`), executable `start_listening` (`setup.py`).
- Publikuje `detected_signals` (`std_msgs/String`, depth 10, QoS domyślne).
- Stałe (`signals_detector.py`, początek pliku): `RATE 44100`, `CHUNK 4096`, `TARGET_FREQS [600, 800, 1000]`, pasmo 500–1100 Hz, `MIN_DB 20`, krótki 0.25–0.65 s, długi 0.70–1.4 s, `PATTERN_TIMEOUT 1.7` s, tolerancja 5%.

## Launch
Brak; `ros2 run signals_detector start_listening`.

## Konfiguracja (yaml)
Brak.

## Zależności
`exec_depend`: `rclpy`, `std_msgs`, `python3-numpy`, `python3-scipy`, `python3-matplotlib`, `python3-pyaudio`. Odbiorca: [[basic_controller]] (`my_bt_node` — ustawia `blasts`, `is_alert_active` na blackboardzie; subskrypcja transient_local — **niepewne**, czy zgodna z domyślnym QoS tego publishera).

## Interfejsy
Brak.

## Różnice między branchami
Zmergowany z `origin/signals_detector` (2026-02).

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.
