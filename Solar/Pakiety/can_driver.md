---
typ: solar-pakiet
repo: "[[l0hwd]]"
build: ament_cmake
tags: [solar, pakiet]
zaktualizowano: 2026-10-04
---
# can_driver

> [!skrot] W 30 sekund
> Węzeł `CanDriver` odbiera surowe ramki z `/from_can_bus` (od [[ros2_socketcan]]), dekoduje je funkcjami wygenerowanymi z `barka.dbc` ([[can-msgs]]) i publikuje tryb pracy łodzi, nastawy silników, kanały aparatury RC, stan peryferiów oraz napięcia i dane BMS. Na `master` nic nie wysyła na CAN.

## Węzły
### CanDriver
- Plik: `l0hwd/can_driver/src/can_driver.cpp`, nagłówek `include/can_driver.hpp`, `main` w `src/node.cpp`. Klasa `CanDriver : rclcpp::Node`, nazwa węzła `"CanDriver"`. Executable `CanDriver` (`CMakeLists.txt`; źródła: `src/can_driver.cpp`, `src/node.cpp`, `../can-msgs/src/barka.c`).
- **Subskrybuje:** `/from_can_bus` (`can_msgs/msg/Frame`, depth 100) → `CanCallback` (`can_driver.cpp:38-39`). Pozostałe subskrypcje (`/gnss`, `filter/velocity`, `filter/euler`, `/next_waypoint`, `/odom`…) są zakomentowane (`:41-67`).
- **Publikuje** (`:110-142`, depth 10):

| topik | typ | źródło ramki |
|---|---|---|
| `/robot/state` | `std_msgs/msg/String` | `VESSEL_OP_MODE` (0x03): 0→`"killed"`, 1→`"manual"`, 2→`"auto"` (`:245-271`) |
| `/thrusters/setpoints` | `std_msgs/msg/Float32MultiArray` | `MOTOR_SPEED_SP_HL` (0x05): `[speed_left, speed_right]` (`:272-285`) |
| `/peripheral` | `std_msgs/msg/Float32MultiArray` | `PUMP_CTRL` 0x07, `ARM_CTRL` 0x08, `CANNON_CTRL` 0x09. Tablica 7 elementów: `[pump_dir, pump_cmd, pump_timeout_ms, arm_dir, arm_cmd, cannon_dir, cannon_cmd]`, wartości buforowane między ramkami (`:303-361`) |
| `/rc_chan` | `std_msgs/msg/Float32MultiArray` | `BB_RC_CHAN_1..4` (0x0C–0x0F): 16 kanałów, każda ramka aktualizuje 4 i publikuje całą tablicę (`:363-441`) |
| `/bms/voltage` | `l0_interfaces/msg/BmsVoltages` | `BMS_CENTRAL/SLAVE_LEFT/SLAVE_RIGHT_CELL_VOLTAGES` (0x40/0x42/0x44) → `central/left/right.cell1..4`, `frame_id="bms"` |
| `/bms/data` | `l0_interfaces/msg/BmsData` | `BMS_*_DATA` (0x41/0x43/0x45) → `temperature, current, power, soc` |

- **Timer:** `ControlLoop` co 20 ms (`:18-19`). Ciało jest całe zakomentowane (dawny GUI: `GuiGraph`, `VesselState`, waypointy).
- **Ramki rozpoznawane, ale ignorowane:** `RKS_STATUS`, `MKS_STATUS`, `MOTOR_SPEED_SP_BB` (dekodowana, publikacja zakomentowana), `UTC_1`, wszystkie SBG (`IMU_ACCEL`, `EKF_*`, `GPS1_*`), większość XSENS, `BB_CURRENT_MEAS`, `BB_GPIO_STATUS`, `BB_PWM_OUT`.
- **`EULER_ANGLES_XSENS` (0x27):** wywołuje `euler_pub->publish()` (`:577`), ale `euler_pub` nie jest tworzony (`:105` zakomentowane). Patrz [[l0hwd#Znane problemy / TODO]].
- Konsumenci: `/robot/state` czyta `l2dp/message_protobuf/message_protobuf/protobuf.py:62` i dashboard (`agh-dashboard-live/scripts/live-rosbridge-publisher.mjs:60`). `/bms/data` i `/bms/voltage` czyta dashboard (`live-rosbridge-publisher.mjs:41-42`, `scripts/bms_panel.py`). Subskrybentów `/rc_chan`, `/peripheral` i `/thrusters/setpoints` w `~/solar` nie znaleziono.

## Launch
Uruchamiany w `l0hwd/l0_launch/launch/l0hwd_launch.py` jako `Node(package="can_driver", executable="CanDriver", name="CanDriver")` → [[l0_launch]].

## Konfiguracja (yaml)
Brak parametrów.

## Zależności
`package.xml`: `l0_interfaces`. `CMakeLists.txt`: `rclcpp`, `can_msgs`, `sensor_msgs`, `std_msgs`, `geometry_msgs`, `nav_msgs`, `l0_interfaces`. Używa [[can_msgs]] (`Frame`), [[l0_interfaces]] i nagłówka `../../can-msgs/src/barka.h` ([[can-msgs]]).

## Interfejsy
Brak własnych.

## Różnice między branchami
- `music`: dodatkowo `/melody` (`std_msgs/msg/UInt16`) → ramka `BUZZER_CTRL` na `/to_can_bus` (patrz [[l0hwd#music]]).
- `sbg-info`: zupełnie inna rola. Subskrybuje topiki `sbg_driver`, wysyła je na CAN i publikuje `/gnss`, `/imu/data`, `/course/data`, `/true_heading/data`, `/velocity/data`, `/bms/*_voltage/data`, `/operation/data` (typy `l1_interfaces`).
- W `master` publikacje `/true_heading/data`, `/course/data`, `/imu/data`, `/gnss` są zakomentowane, więc `sbt_axis_audit` (`/true_heading/data`, [[Pakiety/sbt_transforms|sbt_transforms]]) nie ma dziś źródła w L0.

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.
