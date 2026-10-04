---
typ: solar-pakiet
repo: "[[l1od]]"
build: ament_cmake
tags: [solar, pakiet]
zaktualizowano: 2026-10-04
---
# zed_controller

> [!skrot] W 30 sekund (co robi, wejście → wyjście)
> Regulator ekspozycji kamery ZED na odblaski słońca na wodzie („solar flare”). Mierzy w ROI pod horyzontem odsetek prześwietlonych pikseli; gdy średnia z kilku klatek przekroczy próg — wyłącza auto-ekspozycję ZED i steruje `video.exposure`/`video.gain` regulatorem PI w skali log2 (EV), po ustąpieniu oddaje sterowanie auto-ekspozycji. Istnieje **tylko na gałęzi `zed_exposure_controller`** repo [[l1od]] (autor: Patryk, 2026-05…08).

## Węzły

### zed_exposure_controller
`zed_controller/src/zed_controller.cpp` (766 linii), klasa `ZedExposureController`, executable `zed_exposure_controller` (`CMakeLists.txt`), nazwa węzła w launchu `zed_exposure_controller`.
- **Subskrybuje:** `image_topic` = `/zed/zed_node/rgb/color/rect/image` (`SensorDataQoS`).
- **Klient serwisu:** `<zed_node>/set_parameters` (`rcl_interfaces/srv/SetParameters`), domyślnie `/zed/zed_node/set_parameters` — ustawia `video.auto_exposure_gain`, `video.exposure`, `video.gain`, opcjonalnie `video.gamma`. Używa `service_is_ready()` (nie blokuje callbacku), z ponowieniem wysyłki (`resend_pending_`).
- **Serwis (server):** `command_service` = `/srv/zed_exposure_mode` (`l1_interfaces/srv/RequestCMD`): `0` = wymuś normalny (auto-ekspozycja ZED), `1` = wymuś tryb flare, `2` = auto.
- **Publikuje:** `diag_topic` = `/zed_exposure/diag` (`std_msgs/Float32MultiArray`, co `diag_every_n = 5` klatek).
- **Parametry** (`launch/zed_exposure_controller.yaml`, te same co domyślne w kodzie): ROI `roi_x 0.0, roi_y 0.55, roi_w 1.0, roi_h 0.4` (komentarz: ROI musi być pod horyzontem, „Measured on the 2026-03-21 bags”); `white_threshold 245`, `hi_percentile 0.95`, `analysis_width 320`; `flare_on_ratio 0.15`, `flare_off_ratio 0.08`, `cooldown_on_sec 1.0`, `cooldown_off_sec 4.0`, `history_frames 5`; `target_p_hi 220`, `sat_max 0.02`, `sat_guard_step 0.5`, `median_min 60`, `kp 0.6`, `ki 0.15`, `max_step_ev 0.5`, `settle_frames 3`; `exposure_min 1`, `exposure_max 60`, `manual_exposure 20`, `seed_from_last true`; `flare_gain 20`, `gain_min 0`, `gain_step 5`, `gain_step_frames 10`; `set_gamma_in_flare false`, `manual_gamma 7`, `normal_gamma 8`; `log_every_n 30`. Parametry zmienialne w locie (`add_on_set_parameters_callback`).
- **Logika:**
  1. `onImage`: wycięcie ROI, `computeMetrics` — skalowanie do 320 px szerokości, histogram 256 binów szarości → `median`, `p_hi` (95. percentyl), `sat_ratio` (udział pikseli > 245). Średnia `sat_ratio` z `history_frames` klatek.
  2. Histereza: `avg_sat ≥ 0.15` i ≥ 1 s od przełączenia → `engage`; w trybie flare `avg_sat ≤ 0.08` i ≥ 4 s → `release` (zapamiętuje ostatnie EV, `seed_from_last`).
  3. `regulate` (PI): błąd `log2(target_p_hi / p_hi)`; przy `sat_ratio > sat_max` błąd ≤ −0,5 EV; „shadow floor”: gdy `median < 60` nie wolno ciemnieć (błąd ≥ 0, całka ≥ 0) i krok ograniczony predykcyjnie `log2(median_min/median)`; anti-windup (brak całkowania przy nasyceniu aktuatora); krok ≤ 0,5 EV; po komendzie czeka `settle_frames`.
  4. `updateGainLadder`: `video.gain` krokami po 5 co 10 klatek, tylko gdy ekspozycja dobiła do limitu.
- `scripts/analyze_bag.py` (406 linii) — analiza rosbagów pod dobór ROI/progów (README, „Picking the ROI”).

## Launch
`launch/zed_exposure_controller.launch.py` (32 linie) + `launch/zed_exposure_controller.yaml`.

## Konfiguracja (yaml)
Jw. — `zed_exposure_controller.yaml`, sekcja `zed_exposure_controller: ros__parameters`.

## Zależności
`package.xml`: `rclcpp`, `sensor_msgs`, `cv_bridge`, `opencv4`, `rcl_interfaces`, `l1_interfaces` ([[ros-sbt-interfaces]]), `std_msgs`; exec: `rclpy`, `rosbag2_py`, `rosidl_runtime_py`, numpy, yaml, matplotlib (dla `analyze_bag.py`). Steruje węzłem ZED z [[zed-ros2-wrapper-humble-v5.1.0]] (parametry `video.*` są dynamiczne w `common_stereo.yaml`).

## Interfejsy
Brak własnych.

## Różnice między branchami
Tylko `zed_exposure_controller` (commity `6e29f12` 2026-05-06 → `01677ee` 2026-08-05, m.in. „regulator njord try”). Nie ma go na `refined-fusion` ani `master`.

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.
