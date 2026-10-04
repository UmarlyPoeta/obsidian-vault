---
typ: solar-pakiet
repo: "[[l0hwd]]"
build: ament_cmake
tags: [solar, pakiet]
zaktualizowano: 2026-10-04
---
# can_sequence

> [!skrot] W 30 sekund
> Narzędzie testowe do silników. Węzeł `can_sequence` co 100 ms wysyła na `/to_can_bus` ramkę `MOTOR_SPEED_SP_HL` (`0x05`) według zadanej w parametrach sekwencji (lewy, prawy, czas trwania), a na końcu ramkę stop `0/0`. Omija całe ROS-owe sterowanie.

## Węzły
### can_sequence
- Plik: `l0hwd/can_sequence/src/can_sequence.cpp` (+ `include/can_sequence.hpp`, `src/main.cpp`), klasa `CanSequence`, węzeł `"can_sequence"`, executable `can_sequence`.
- **Parametry** (`:8-10`):
  - `left_thruster` = `[200, 400, 600, 800, 1000, 0]`
  - `right_thruster` = `[-200, -400, -600, -800, -1000, 0]`
  - `durations` = `[5, 5, 5, 5, 5, 10]` s
  - Krótsze wektory są dopełniane: prawy zerami, czasy wartością 1.0.
- **Publikuje:** `/to_can_bus` (`can_msgs/msg/Frame`).
- **Timer** 100 ms (`:32`): w bieżącym kroku wysyła `{left, right}` jako `int16` (bez przycinania). Po upływie `durations[i]` przechodzi do następnego kroku. Po ostatnim wysyła `MotorSetpoints{0,0,0}` i anuluje timer (`:42-58`).
- Domyślna sekwencja kręci silnikami w **przeciwnych** kierunkach, więc łódź obraca się w miejscu.

## Launch
Brak. `ros2 run can_sequence can_sequence --ros-args -p left_thruster:="[...]"`.

## Konfiguracja (yaml)
Brak.

## Zależności
`rclcpp`, `can_msgs` → [[can_msgs]], nagłówek [[can-msgs]]. Maintainer: filipnaw (`package.xml`).

## Interfejsy
Brak.

## Różnice między branchami
Jest na `master` i `music`, nie ma go na `sim_controller`, `sbg-info` ani `old_architecture`.

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.
