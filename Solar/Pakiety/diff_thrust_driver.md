---
typ: solar-pakiet
repo: "[[l0hwd]]"
build: ament_cmake
tags: [solar, pakiet]
zaktualizowano: 2026-10-04
---
# diff_thrust_driver

> [!skrot] W 30 sekund
> `DiffThrustDriver` zamienia `l0_interfaces/msg/SpeedVector` z `/l2/speed_vector` (publikowane przez `l2dp/planner`, `l2dp/trajectory_planner`, węzły BT `l3nav` i [[swim_sequence]]) na ramkę `MOTOR_SPEED_SP_HL` (`0x05`). W drzewie roboczym nadaje tylko wtedy, gdy `l3nav` przełączy sterowanie na `"diff_thrust_driver"`. Nie ma go w launchu (tam jest [[new_diff_thrust_driver]]).

## Węzły
### DiffThrustDriver
- Plik: `l0hwd/diff_thrust_driver/src/diff_thrust_driver.cpp`, klasa `DiffThrustDriver`, węzeł `"DiffThrustDriver"`, executable `DiffThrustDriver`.
- **Subskrybuje:**
  - `/l2/speed_vector` (`l0_interfaces/msg/SpeedVector`, depth 10) → `TwistCallback`;
  - `/regulator_or_thrust_driver` (`std_msgs/msg/String`, depth 10): `use = (data == "diff_thrust_driver")` (**niezacommitowane**).
- **Publikuje:** `/to_can_bus` (`can_msgs/msg/Frame`).
- **Timer:** co 50 ms `ControlLoop`, który publikuje ramkę tylko gdy `use` (drzewo robocze; w commicie `fd33338` publikował zawsze).
- **Logika:** `CalculateThrust(linear, angular)`: `left = linear + angular`, `right = linear − angular`, przycięte do ±1000, potem `Scale(1000)`. Ramka jak w [[new_diff_thrust_driver]] (`id=5`, `dlc=8`, `memcpy` 4 B).
- `use` startuje jako `false`, więc bez komunikatu z L3 węzeł nic nie wysyła.

## Launch
Brak w `l0hwd_launch.py` (pakiet jest tylko w `depend` pakietu [[l0_launch]]). Uruchamianie: `ros2 run diff_thrust_driver DiffThrustDriver` (`l0hwd/README.md`).

## Konfiguracja (yaml)
Brak.

## Zależności
`package.xml`: `can_msgs`, `rclcpp`, `l0_interfaces` → [[can_msgs]], [[l0_interfaces]], [[can-msgs]].

## Interfejsy
Brak.

## Różnice między branchami
Zmiana z przełącznikiem `use` istnieje tylko w niezacommitowanym drzewie roboczym `master`.

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.
