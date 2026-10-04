---
typ: solar-pakiet
repo: "[[l2dp]]"
build: ament_cmake
tags: [solar, pakiet]
zaktualizowano: 2026-10-04
---
# blast_detection

> [!skrot] W 30 sekund (co robi, wejście → wyjście)
> Detektor sygnałów dźwiękowych (krótki/długi gwizd) z mikrofonu: PortAudio (domyślne urządzenie wejściowe) → spektrogram FFT 300–900 Hz → progi magnitudy/energii/SNR z histerezą → klasyfikacja długości dźwięku → `detected_signals` (`l2_interfaces/BlastDetection`, QoS transient_local). Zadanie „sound signal” (RoboBoat).

## Węzły
### `blast_detector_node` (klasa `BlastDetectorNode`)
- Źródło: `blast_detection/src/blast_detector_node.cpp`, `src/spectro.cpp`, `src/main.cpp`, `include/config.hpp`, `include/spectro.hpp`, `include/utils.hpp`. Executable `blast_detector`.
- Parametry: `target_freq` 600.0 Hz, `target_tol_pct` 0.05 (`blast_detector_node.cpp:46–47`).
- Publikuje `detected_signals` (`l2_interfaces/BlastDetection`: `header`, `blast_type`, `frequency_hz`, `zone`), QoS `QoS(1).transient_local().reliable()` (`:58–59`).
- Timer 50 ms → `detect_blast` (`:61`).
- Stałe (`include/config.hpp`): 44100 Hz, bufor 4096, pasmo 300–900 Hz, progi włączenia `MAG −46 dB`, `E −49`, `SNR 13`, wyłączenia `−47/−50/11`, `HOLD_S 0.1`, `TIMEOUT 0.7`, krótki dźwięk 0.3–0.7 s, długi ≥0.8 s.

## Launch
Brak.

## Konfiguracja (yaml)
Brak.

## Zależności
`rclcpp`, `std_msgs`, `l2_interfaces`; PortAudio i FFTW (w `package.xml` zakomentowane `portaudio19-dev`, `libfftw3-dev` — trzeba doinstalować ręcznie).

> [!blad] Niezgodność typu
> [[basic_controller]] (`my_bt_node.cpp:13–26`) subskrybuje `detected_signals` jako `std_msgs/String` (QoS transient_local), a ten węzeł publikuje `l2_interfaces/BlastDetection` na tym samym topiku — typy się nie zgadzają, więc BT nie odbierze tych wiadomości. Pasujący typ (`String`) publikuje [[signals_detector]] (ale z QoS domyślnym — **niepewne**, czy zgodnym z transient_local subskrybenta).

## Interfejsy
Brak własnych.

## Różnice między branchami
Zmergowana gałąź `origin/signals_detector` (2026-02): parametryzacja częstotliwości, SNR, QoS publishera, filtr górnoprzepustowy.

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.
