---
typ: solar-pakiet
repo: "[[l4sim]]"
build: ament_cmake
tags: [solar, pakiet]
zaktualizowano: 2026-10-04
---
# sim_can_adapter

> [!skrot] W 30 sekund (co robi, wejście → wyjście)
> Podmienia magistralę CAN w symulacji: słucha `/to_can_bus` (to, co wysłałby `regulator` lub `diff_thrust_driver` z [[l0hwd]]) i zamienia ramkę nastaw silników na ciągi pędników WAM-V. Dzięki temu w symulacji działa ta sama ścieżka L0 co na łodzi.

Opis wg `~/solar/bash_do_uruchomienia/l4sim@main` (`7a789a2`), katalog `src/sim_can_adapter`.

## Węzły
- `SimCanAdapter` (klasa `CanAdapter`, `src/can_adapter.cpp` + `src/main.cpp`, executable `SimCanAdapter`).
- Subskrybuje `/to_can_bus` (`can_msgs/Frame`, depth 1); publikuje `/wamv/thrusters/left/thrust`, `/wamv/thrusters/right/thrust` (`Float64`, depth 1).
- `memcpy` danych ramki do struktury `MotorSetpoints` (dwa int16), clamp ±1000. **Nie sprawdza ID ramki** — każda ramka na `/to_can_bus` (np. z `peripherals_handler`) zostanie potraktowana jako nastawa silników.

## Launch
Uruchamiany przez `l4_launch/launch/sim.yaml` (razem z `CourseImu`) i ręcznie: `ros2 run sim_can_adapter SimCanAdapter` (README, ścieżka „Launch with Controller”).

## Konfiguracja (yaml)
Brak.

## Zależności
`rclcpp`, `can_msgs` ([[can_msgs]], lokalna kopia w `src/can_msgs`), `std_msgs`.

## Interfejsy
Brak własnych.

## Różnice między branchami
Bez zmian.

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.
