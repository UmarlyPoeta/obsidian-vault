---
typ: solar-pakiet
repo: "[[l4sim]]"
build: ament_python
tags: [solar, pakiet]
zaktualizowano: 2026-10-04
---
# boat_teleop

> [!skrot] W 30 sekund (co robi, wejście → wyjście)
> Sterowanie ręczne łodzią w symulacji z klawiatury (WASD) → `/cmd_vel` (`Twist`), przeliczane na pędniki przez `vel_to_thrusters` z [[cpp_nodes]] (lub `new_diff_thrust_driver` z [[l0hwd]] — **niepewne**).

Opis wg `~/solar/bash_do_uruchomienia/l4sim@main` (`7a789a2`), katalog `src/l4_teleop/src/boat_teleop`.

## Węzły
- `boat_teleop` (`boat_teleop/boat_teleop.py`, executable `boat_teleop`): publikuje `/cmd_vel`; `w`/`s` → `linear.x = ±1.0`, `a`/`d` → `angular.z = ±10.0`, `q` → zatrzymanie liniowe.

## Launch
Brak; `ros2 run boat_teleop boat_teleop`.

## Konfiguracja (yaml)
Brak.

## Zależności
`rclpy`, `geometry_msgs`. Katalog `src/l4_teleop/src/boat_teleop`.

## Interfejsy
Brak własnych.

## Różnice między branchami
Bez zmian.

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.
