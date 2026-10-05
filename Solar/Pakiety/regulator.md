---
typ: solar-pakiet
repo: "[[l0hwd]]"
build: ament_cmake
tags: [solar, pakiet]
zaktualizowano: 2026-10-04
---
# regulator

> [!skrot] W 30 sekund
> `Regulator` ma w środku model Simulink `code_gen_1`: Pure Pursuit i regulator rozmyty, wygenerowane do C przez Simulink Coder. Węzeł dostaje pozycję (`/slam/odom`), kurs (`/filter/euler`), prędkości i ścieżkę (`/path_points`), co 20 ms liczy ciąg lewego i prawego silnika i wysyła `MOTOR_SPEED_SP_HL` (`0x05`) na `/to_can_bus`. Z L3 przełącza się go tematem `/regulator_or_thrust_driver`, a prędkość zadaną ustawia `/regulator_speed`.

## Węzły
### Regulator
- Plik: `l0hwd/regulator/src/regulator.cpp`, nagłówek `include/regulator.hpp`, `main` w `src/node.cpp`. Klasa `Regulator`, węzeł `"Regulator"`, executable `Regulator`. Do pliku wykonywalnego kompilowane są `code_gen_1_ert_rtw/code_gen_1.c`, `code_gen_1_data.c` i `../can-msgs/src/barka.c` (`CMakeLists.txt`).
- **Parametr:** `sim` (bool, domyślnie `false`) (`:21-22`).
- **Subskrybuje** (drzewo robocze, `:44-75`):

| topik | typ | do czego |
|---|---|---|
| `/slam/odom` | `nav_msgs/msg/Odometry` | `rtU.pos_x/pos_y` (w commicie `fd33338`: `/odom`) |
| `/filter/velocity` | `geometry_msgs/msg/Vector3Stamped` | `rtU.vel_x/vel_y` |
| `/imu/data` albo przy `sim=true` `/wamv/sensors/imu/imu/data` | `sensor_msgs/msg/Imu` | `rtU.angular_velocity = angular_velocity.z` |
| `/filter/euler` | `geometry_msgs/msg/Vector3Stamped` | `rtU.heading = vector.z` |
| `/path_points` | `geometry_msgs/msg/PoseArray` | 100 punktów do `waypoint_x/y[]`; brakujące wypełniane ostatnim punktem. `rtU.end_heading` = yaw ostatniej pozy w stopniach (`+atan2`, `:133-169`) |
| `/regulator_or_thrust_driver` | `std_msgs/msg/String` | `use = (data == "regulator")` |
| `/regulator_speed` | `std_msgs/msg/Float64` | `rtU.desired_vel` (domyślnie 1.0, `:41`) |

- **Publikuje:** `/to_can_bus` (`can_msgs/msg/Frame`).
- **Timer:** `ControlLoop` co 20 ms (`:36-39`). Po pierwszej odometrii i ścieżce, jeśli `use`: `code_gen_1_step()` i `SendControl()`. Ostrzeżenie, gdy `/odom` lub `/path_points` nie przyszły od 1 s (`CheckTopicsStatus`, `:95-108`).
- **SendControl** (`:171-214`): `MotorSetpoints{right_thrust, left_thrust}` (kolejność odwrócona względem nazw pól), `id=5`, `dlc=8`, `memcpy` 4 B, plus log stanu co 1 s.
- **Model Simulink** (`code_gen_1_ert_rtw/code_gen_1.h`): „Code generated for Simulink model 'code_gen_1'”, wersja modelu 1.29, Simulink Coder 24.2 (R2024b), wygenerowano „Sun May 10 05:10:19 2026”.
  - Wejścia `ExtU`: `pos_x, pos_y, angular_velocity, heading, desired_vel, end_heading, vel_x, vel_y`.
  - Wyjścia `ExtY`: `LeftThruster, RightThruster, target_dir, actual_vel, heading_act, omega_act, heading_raw, v_x, v_y, x, y`.
  - W środku blok `nav_slalgs_internal_PurePursuit` (`MaxAngularVelocity = 1.0`, `code_gen_1.c:631`, lookahead i prędkość liczone w kroku) i podsystem „Fuzzy Logic Controller” (`code_gen_1.c:1006`). Domyślne `waypoint_x` w `code_gen_1.c:31` to prosta 0…29.7 m co 0.3 m.

## Launch
Brak w launchu. `ros2 run regulator Regulator [--ros-args -p sim:=true]`.

## Konfiguracja (yaml)
Brak plików YAML.

## Zależności
`package.xml`: `rclcpp, geometry_msgs, nav_msgs, can_msgs, sensor_msgs, tf2, tf2_geometry_msgs` → [[can_msgs]], [[can-msgs]]. Źródła `/path_points` i `/slam/odom` są w `l2dp`.

## Interfejsy
Brak.

## Różnice między branchami
- `sim_controller` (`8c481dc`): `/odom` zamiast `/slam/odom`, bez `/filter/velocity`, bez przełącznika i `/regulator_speed`, inna wersja wygenerowanego modelu.
- Commit `fd33338` na `master`: `/odom`, bez `use` i `/regulator_speed` (to dodaje dopiero drzewo robocze).

> [!blad] Znane problemy
> - `regulator.cpp:30`: `bool use = true;` to zmienna lokalna, a pole `use` zostaje niezainicjalizowane.
> - Według `sbt_transforms/REVIEW.md` (B2, B9) `rtU.end_heading` jest w konwencji ENU-CCW (`+atan2`), a `rtU.heading` z `/filter/euler` to kurs innej konwencji. Do tego orientacje w `/path` z `l2dp/planner` są liczone wstecz. Szczegóły w [[Drzewo TF i układy współrzędnych]].
> - ~~Brak przycinania wyjścia modelu do ±1000~~: model ma saturację ±1000 (`code_gen_1.c:1236-1264`), sprawdzone 2026-10-05.
> - `rtU.end_heading` jest w stopniach, a model odejmuje od niego kurs w radianach. Więcej w [[Regulator — TODO, błędy i oscylacje]].

## Zobacz też
- **Dla początkujących (dużo wykresów):** [[Regulator od zera — 1. Co to jest i Simulink]] → [[Regulator od zera — 2. Matematyka]] → [[Regulator od zera — 3. Logika rozmyta]] → [[Regulator od zera — 4. Dlaczego się buja]].
- [[Regulator — hierarchia i działanie]]: rozbiór kodu C++ i modelu Simulink blok po bloku.
- [[Regulator — TODO, błędy i oscylacje]]: lista TODO, przyczyny oscylacji, jak uprościć ścieżkę.

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.
