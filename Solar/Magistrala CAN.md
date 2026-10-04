---
typ: solar-zbiorcza
tags: [solar, can]
zaktualizowano: 2026-10-04
---
# Magistrala CAN

> [!skrot] W 30 sekund
> - Łódź autonomiczna (barka, Njord) ma jedną magistralę CAN opisaną w `barka.dbc` z repo [[can-msgs]]. Jetson widzi ją jako interfejs SocketCAN **`can0`** (domyślny argument `socket_can_bridge.launch.xml` w [[ros2_socketcan]]).
> - ROS ↔ CAN: węzły L0 publikują `can_msgs/msg/Frame` na **`/to_can_bus`**, a `socket_can_sender` wysyła je na `can0`. `socket_can_receiver` publikuje ramki z `can0` na **`/from_can_bus`**, gdzie dekoduje je [[can_driver]].
> - Ramki standardowe (11-bit), klasyczny CAN, FD wyłączone. **Bitrate barki nie wynika z żadnego pliku w `~/solar`.** 125 kbit/s jest ustawione tylko dla innej łodzi (Delta/Celka, [[embedded-system]]).

## Węzły magistrali (z `BU_`)
`barka.dbc:35`: `BARKA SBG BB HL BMS_MASTER BMS_SLAVE_LEFT BMS_SLAVE_RIGHT XSENS` (ramki BMS 0x40/0x41 mają w praktyce nadawcę `BMS_CENTRAL`).
- **HL**: według nazw ramek (`MOTOR_SPEED_SP_HL`, kierunek `HL2BB`) to komputer wysokiego poziomu, czyli Jetson z [[l0hwd]] (**rozwinięcie skrótu niepewne**).
- **BB**: płytka, która wysyła kanały RC, `MOTOR_SPEED_SP_BB`, `BUZZER_CTRL`, `HEADING_CALCULATED`. README wspomina „barka-board's readme”, stąd prawdopodobnie „barka board” (**niepewne**). Jej firmware nie ma w `~/solar`.
- **BARKA**: nadawca statusów (`BMS_STATUS`, kill switche, tryb pracy, radio). **Niepewne**, która to fizycznie płytka.
- **SBG** i **XSENS**: dwa INS-y. SBG zajmuje 0x10–0x1F (little-endian), XSENS 0x20–0x34 (sygnały `@0`, big-endian, jak w natywnym formacie Xsens CAN).
- **BMS_***: trzy BMS-y (centralny, lewy, prawy), po 4 ogniwa.

## Tabela ramek (`barka.dbc`, wersja przypięta w l0hwd = `njord_2025`)
Format sygnału: `nazwa(bity, s/u = signed/unsigned, × skala, + offset, jednostka)`.

| ID dec / hex | ramka | DLC | nadawca (DBC) | sygnały | ROS (l0hwd `master`) |
|---|---|---|---|---|---|
| 0 / 0x00 | `BMS_STATUS` | 2 | BARKA | SOURCE(8bu: MASTER/SLAVE_LEFT/SLAVE_RIGHT), STATUS(8bu: OK/FAULT) | — |
| 1 / 0x01 | `RKS_STATUS` | 1 | BARKA | RKS_STATUS(8bu: KS_OFF/KS_ON), zdalny kill switch | rozpoznawana, ignorowana |
| 2 / 0x02 | `MKS_STATUS` | 1 | BARKA | MKS_STATUS(8bu), ręczny kill switch | rozpoznawana, ignorowana |
| 3 / 0x03 | `VESSEL_OP_MODE` | 1 | BARKA | VESSEL_OPERATION_MODE(8bu: 0 OFF, 1 MANUAL, 2 AUTO) | → `/robot/state` (`"killed"`/`"manual"`/`"auto"`) |
| 4 / 0x04 | `RADIO_STATUS` | 1 | BARKA | CONNECTION(8bu) | — |
| 5 / 0x05 | `MOTOR_SPEED_SP_HL` | 4 | HL | speed_left, speed_right (16bs, ‰, −1000…1000) | **nadają**: [[new_diff_thrust_driver]], [[diff_thrust_driver]], [[regulator]], [[can_sequence]]; **odbiera** [[can_driver]] → `/thrusters/setpoints` |
| 6 / 0x06 | `MOTOR_SPEED_SP_BB` | 4 | BB | speed_left, speed_right (16bs ‰) | dekodowana, publikacja zakomentowana |
| 7 / 0x07 | `PUMP_CTRL` | 4 | HL | DIRECTION(HL2BB/BB2HL), COMMAND(OFF/ON/DONE), TIMEOUT(16bu ms) | **nadaje** [[peripherals_handler]] (`/srv/pump_control`); odbiera CanDriver → `/peripheral[0..2]` |
| 8 / 0x08 | `ARM_CTRL` | 2 | HL | DIRECTION, ARM_COMMAND(NONE/HOME/LATCH/UNLATCH/PICKUP/PUTDOWN) | odbiera CanDriver → `/peripheral[3..4]`; nadawcy w ROS brak |
| 9 / 0x09 | `CANNON_CTRL` | 2 | HL | DIRECTION, COMMAND(NO_ACTION/SHOOT/DONE) | **nadaje** PeripheralsHandler (`/srv/cannon_control`); odbiera CanDriver → `/peripheral[5..6]` |
| 10 / 0x0A | `BUZZER_CTRL` | 4 | BB | DIRECTION(2BB/2HL/2BMSM), COMMAND(STOP_PLAYING/PLAY/DONE), TONE_NUMBER(16bu) | tylko gałąź `music`: CanDriver `/melody` → ta ramka |
| 12–15 / 0x0C–0x0F | `BB_RC_CHAN_1..4` | 8 | BB | chan1…chan16 (16bu, 0–2047, CRSF) | → `/rc_chan` (16 floatów) |
| 16 / 0x10 | `UTC_1` | 8 | SBG | data/czas | ignorowana |
| 17–30 / 0x11–0x1E | `IMU_ACCEL`, `IMU_GYRO`, `EKF_QUAT`, `EKF_EULER`, `EKF_ORIENTATION_ACC`, `EKF_POS`, `EKF_VEL_NED`, `SHIP_MOTION_0`, `MAG_1`, `ODO_VEL`, `GPS1_VEL`, `GPS1_VEL_COURSE`, `GPS1_POS`, `GPS1_HDT` | 2–8 | SBG | patrz DBC (`barka.dbc:126-191`) | dekodowanie częściowe, publikacje (`/gnss`, `/course/data`, `/true_heading/data`) zakomentowane |
| 31 / 0x1F | `RESERVED_FOR_IMU` | 8 | — | RESERVED(64bu) | — |
| 32–37 / 0x20–0x25 | `WARNING/ERROR/SAMPLE_TIME/GROUP_COUNTER/UTC/STATUS_WORD_XSENS` | 1–8 | XSENS | statusy | ignorowane |
| 38 / 0x26 | `QUATERION_XSENS` | 8 | XSENS | Q0..Q3 (16bs ×3.0519e‑5) | dekodowana, bez publikacji |
| 39 / 0x27 | `EULER_ANGLES_XSENS` | 6 | XSENS | Roll, Pitch, Yaw (16bs ×0.0078 deg) | `euler_pub->publish` na **niezainicjalizowanym** publisherze (`can_driver.cpp:577`) |
| 40–49 / 0x28–0x31 | `DELTA_V`, `RATE_OF_TURN`, `DELTA_Q`, `ACCEL`, `FREE_ACCEL`, `MAGNETIC_FIELD`, `TEMPERATURE`, `BARO_PRESSURE`, `ACCEL_HR`, `RATE_OF_TURN_HR` (`_XSENS`) | 2–8 | XSENS | patrz DBC | ignorowane / bez publikacji |
| 50 / 0x32 | `LATITUDE_LONGITUDE_XSENS` | 8 | XSENS | latitude (32bs ×5.96e‑8), longitude (32bs ×1.19e‑7) | publikacja `/gnss` zakomentowana |
| 51 / 0x33 | `ALTITUDE_ELLIPSOID_XSENS` | 4 | XSENS | altEllipsoid | ignorowana |
| 52 / 0x34 | `VELOCITY_XSENS` | 6 | XSENS | velX/Y/Z (16bs ×0.015625 m/s) | publikacja `/filter/velocity` zakomentowana |
| 64 / 0x40 | `BMS_CENTRAL_CELL_VOLTAGES` | 8 | BMS_CENTRAL | C1..C4 (12bu ×0.001 +1 V), SUM (16bu ×0.001 V) | → `/bms/voltage.central` |
| 65 / 0x41 | `BMS_CENTRAL_DATA` | 8 | BMS_CENTRAL | TEMPERATURE (×0.01 °C), CURRENT (×0.01 A), POWER (×0.1 W), SOC (×0.01 %) | → `/bms/data.central` |
| 66 / 0x42 | `BMS_SLAVE_LEFT_CELL_VOLTAGES` | 8 | BMS_SLAVE_LEFT | jw. | → `/bms/voltage.left` |
| 67 / 0x43 | `BMS_SLAVE_LEFT_DATA` | 8 | BMS_SLAVE_LEFT | jw. | → `/bms/data.left` |
| 68 / 0x44 | `BMS_SLAVE_RIGHT_CELL_VOLTAGES` | 8 | BMS_SLAVE_RIGHT | jw. | → `/bms/voltage.right` |
| 69 / 0x45 | `BMS_SLAVE_RIGHT_DATA` | 8 | BMS_SLAVE_RIGHT | jw. | → `/bms/data.right` |
| 70 / 0x46 | `BB_CURRENT_MEAS` | 2 | — | current_meas (16bu ×0.001 A) | ignorowana |
| 71 / 0x47 | `BB_GPIO_STATUS` | 4 | — | gpio1..4 | ignorowana |
| 72 / 0x48 | `BB_PWM_OUT` | 8 | — | pwm1..4 (%) | publikacja `/thrusters/pwm` zakomentowana |
| 80 / 0x50 | `HEADING_CALCULATED` | 8 | BB | HDG1..HDG4 (16bu ×0.01 deg) | brak obsługi w CanDriver |

Pole `SUM` w ramkach napięć jest dekodowane, ale nie trafia do `l0_interfaces/msg/CellVoltages` (ta wiadomość ma tylko `cell1..cell4`).

## Gdzie w l0hwd odbywa się tłumaczenie
- **CAN → ROS:** `l0hwd/can_driver/src/can_driver.cpp:236` `CanDriver::CanCallback`, czyli `switch (msg->id)` po stałych `BARKA_*_FRAME_ID` i funkcjach `barka_*_unpack` / `*_decode` z `can-msgs/src/barka.h`.
- **ROS → CAN:**
  - napęd: `BuildCanMotorPowerRequest()` w `new_diff_thrust_driver.cpp:81-104` i `diff_thrust_driver.cpp:84-107`, `Regulator::SendControl()` w `regulator.cpp:171-189`, `CanSequence::timerCallback()` w `can_sequence.cpp:35-89`. Wszystkie robią `memcpy` struktury `{int16 left; int16 right; int32 padding}` (packed) zamiast `barka_motor_speed_sp_hl_pack` (wersja z `pack` jest zakomentowana);
  - peryferia: `PeripheralsHandler::BuildPumpCanMessage` / `BuildCannonCanMessage` (`peripherals_handler.cpp:34-61`) przez `barka_*_pack`.
- **Most:** [[ros2_socketcan]] (`socket_can_receiver` → `from_can_bus`, `socket_can_sender` ← `to_can_bus`), interfejs `can0`, `interval_sec=0.01`, `timeout_sec=0.01`.
- **Symulacja:** `bash_do_uruchomienia/l4sim/src/sim_can_adapter/src/can_adapter.cpp` subskrybuje `/to_can_bus` i bez sprawdzania ID kopiuje `dlc` bajtów do `MotorSetpoints`, z czego publikuje `/wamv/thrusters/left|right/thrust`. Każda ramka na `/to_can_bus`, także pompa czy działko, zostanie więc w symulacji potraktowana jak nastawa silników.

> [!wazne] DLC 8 kontra 4
> DBC: `MOTOR_SPEED_SP_HL: 4`. Węzły ROS wysyłają `dlc = 8` i kopiują 4 bajty (reszta to zera z konstruktora). Adapter symulacyjny robi `memcpy(..., msg->dlc)` do 8-bajtowej struktury, więc DLC 8 pasuje do niego. **Niepewne**, czy firmware płytki BB akceptuje DLC ≠ 4.

## Bitrate i interfejs
- Interfejs: `can0` (`ros2_socketcan/launch/socket_can_bridge.launch.xml:3`, domyślne parametry węzłów `socket_can_*_node.cpp`). `l0hwd_launch.py` go nie nadpisuje.
- Bitrate barki: **nie ustalono**. W `~/solar` nie ma skryptu `ip link set can0 type can bitrate …` dla Jetsona.
- Dla porównania druga łódź, Delta/Celka ([[embedded-system]]): `bus-speed = <125000>` w `boards/solarboat/base_board/base_board.dts:45` i `can_test_board/boards/*.overlay`, osobny `communication/delta.dbc`, a GUI domyślnie `socketcan`/`can0` (`delta-gui/app/config.py:15-16`). **To nie jest ta sama magistrala co barka.**

## Warianty DBC na gałęziach can-msgs
- `BMS_V3.0`: 4 baterie, nowe 0x46/0x47 i przesunięcie `BB_*` na 0x48–0x4A.
- `sbg-info`: dodatkowe ramki GPS SBG, `IMU_ACCEL` przeniesiony na 0x4B.
- Szczegóły w [[can-msgs#Branche]]. Wdrożenie którejkolwiek wymaga zmian w [[can_driver]].

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.
