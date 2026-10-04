---
typ: solar-pakiet
repo: "[[l4sim]]"
build: ament_cmake
tags: [solar, pakiet]
zaktualizowano: 2026-10-04
---
# sim_thrust_driver

> [!skrot] W 30 sekund (co robi, wejście → wyjście)
> Symulacyjny odpowiednik `diff_thrust_driver` z [[l0hwd]]: `/l2/speed_vector` (z [[trajectory_planner]] lub węzłów BT) → ciągi lewego/prawego pędnika WAM-V w Gazebo (`/wamv/thrusters/{left,right}/thrust`). Zamiast CAN — bezpośrednio topiki VRX.

Opis wg `~/solar/bash_do_uruchomienia/l4sim@main` (`7a789a2`), katalog `src/sim_thrust_driver`.

## Węzły
- `SimThrustDriver` (`src/sim_thrust_driver.cpp`, executable `SimThrustDriver`).
- Parametry: `linear_gain` 300.0, `angular_gain` 750.0, `max_thrust` 1000.0.
- Subskrybuje `/l2/speed_vector` (`l0_interfaces/SpeedVector`); publikuje `/wamv/thrusters/left/thrust`, `/wamv/thrusters/right/thrust` (`std_msgs/Float64`).
- `left = linear·linear_gain + angular·angular_gain/2`, `right = linear·linear_gain − angular·angular_gain/2`, clamp ±max_thrust.

## Launch
Brak; README: `ros2 run sim_thrust_driver SimThrustDriver` (ścieżka „Launch with Trajectory Planner”).

## Konfiguracja (yaml)
Brak plików.

## Zależności
`rclcpp`, `std_msgs`, `l0_interfaces` ([[ros-sbt-interfaces]]). Nie ma go w `Dockerfile.sim` (budowany ręcznie w workspace).

## Interfejsy
Brak własnych.

## Różnice między branchami
Bez zmian na gałęziach.

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.
