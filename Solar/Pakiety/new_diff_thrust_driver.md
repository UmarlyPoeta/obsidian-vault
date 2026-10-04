---
typ: solar-pakiet
repo: "[[l0hwd]]"
build: ament_cmake
tags: [solar, pakiet]
zaktualizowano: 2026-10-04
---
# new_diff_thrust_driver

> [!skrot] W 30 sekund
> `NewDiffThrustDriver` zamienia `/cmd_vel` (`geometry_msgs/msg/Twist`, np. z Nav2) na nastawy lewego i prawego silnika i co 50 ms wysyła ramkę `MOTOR_SPEED_SP_HL` (ID `0x05`) na `/to_can_bus`. To ten sterownik napędu startuje w launchu L0.

## Węzły
### NewDiffThrustDriver
- Plik: `l0hwd/new_diff_thrust_driver/src/new_diff_thrust_driver.cpp`, klasa `NewDiffThrustDriver`, nazwa węzła `"NewDiffThrustDriver"`. Executable `NewDiffThrustDriver` (`CMakeLists.txt`: `src/new_diff_thrust_driver.cpp src/node.cpp ../can-msgs/src/barka.c`).
- **Subskrybuje:** `/cmd_vel` (`geometry_msgs/msg/Twist`, depth 10) (`:40-41`).
- **Publikuje:** `/to_can_bus` (`can_msgs/msg/Frame`, depth 10) (`:42`).
- **Timer:** `ControlLoop` co `TimerBase = 50` ms (`include/new_diff_thrust_driver.hpp:36`). Wysyła **zawsze** ostatnią nastawę. Przy braku `/cmd_vel` jest to ostatnia zapamiętana wartość, bez timeoutu.
- **Logika** (`TwistCallback`, `:52-62`):
  1. `linear = msg.linear.x`. Dla `≥ 0` dzieli przez 2 (komentarz: „maks speed 2 m/s ustawione w nav2_params”), dla `< 0` mnoży przez 2 („maks wstecz 0,5 m/s”).
  2. `angular = msg.angular.z; angular/=1,5;`. Przez operator przecinka to dzielenie przez **1**, nie 1,5 (błąd).
  3. `CalculateThrust`: `left = linear + angular`, `right = linear − angular` (`:71-79`), przycięte do ±1000.
  4. `Scale(1000)` i ponowne przycięcie do ±1000.
- **Budowa ramki** (`:81-104`): `id = BARKA_MOTOR_SPEED_SP_HL_FRAME_ID` (5), `dlc = 8`, standardowe ID. `MotorSetpoints{int16 left, int16 right, int32 padding}` jest kopiowane przez `memcpy` na `BARKA_MOTOR_SPEED_SP_HL_LENGTH` (4) bajty, little-endian, zgodnie z DBC `speed_left : 0|16@1-`, `speed_right : 16|16@1-`.

## Launch
`l0hwd/l0_launch/launch/l0hwd_launch.py:35-39`: `package="new_diff_thrust_driver"  #changed to new`, `executable/name="NewDiffThrustDriver"`.

## Konfiguracja (yaml)
Brak. Stałe: `MIN/MAX_SETPOINT = ±1000`, `MIN/MAX_THRUST = ±1000`, `TimerBase = 50`.

## Zależności
`package.xml`: tylko `rclcpp`. CMake dodatkowo szuka `can_msgs`, `std_msgs`, `geometry_msgs` (braki w `package.xml`). Pakiety z repo: [[can_msgs]], nagłówek [[can-msgs]].

## Interfejsy
Brak.

## Różnice między branchami
Pakiet pojawił się z gałęzi `new_diff_thrust` (merge `1b96e41`, 2025-10-13; commit `97bee05` „converting cmd_vel to diff_thrust_driver”). Na `old_architecture` go nie ma.

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.
