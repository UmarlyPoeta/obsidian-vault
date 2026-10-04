---
typ: solar-pakiet
repo: "[[l3nav]]"
build: ament_cmake
tags: [solar, pakiet]
zaktualizowano: 2026-10-04
---
# waypoint_sequencer

> [!skrot] W 30 sekund (co robi, wejście → wyjście)
> Najprostsza misja bez drzewa zachowań: lista punktów (x, y) z YAML wysyłana po kolei serwisem `/srv/waypoint_navigation_state` do [[trajectory_planner]]; kolejny punkt po `/l3/waypoint_reached`. Punkty specjalne odpalają peryferia: (−1000, −1000) → pompa, (−2000, −2000) → działko. Po ostatnim punkcie 10× `/temporary/transition = true` co 300 ms.

Opis wg `~/solar/l3nav@waypoint_first` (pakiet bez niezacommitowanych zmian).

## Węzły
### `waypoint_sequencer_node` (klasa `WaypointSequencer`)
- Źródło: `waypoint_sequencer/src/waypoint_sequencer.cpp`, `include/waypoint_sequencer/waypoint_sequencer.hpp`, `src/main.cpp`. Executable `waypoint_sequencer_node`. W katalogu leży też `src/waypoint_sequencer.cpp.save` (kopia edytora).
- Parametr `waypoints` (`double[]`, płaska lista x,y); przy braku — lista domyślna w kodzie (zaczyna się od (−2000, −2000), kończy (−1000, −1000)).
- **Klienci serwisów:** `/srv/waypoint_navigation_state` (`l2_interfaces/srv/WaypointNavigationState`, limity `linear 1.0`, `angular 40.0`), `/srv/pump_control` (`l0_interfaces/srv/PumpControl`: `pump_req=true`, `timeout=3000`), `/srv/cannon_control` (`l0_interfaces/srv/CannonControl`: `cannon_direction=1`, `cannon_command=1`).
- **Subskrybuje:** `/l3/waypoint_reached` (`std_msgs/Bool`, SensorDataQoS).
- **Publikuje:** `/temporary/transition` (`std_msgs/Bool`, stała `PUB_TRAN_TOPIC_NAME`, `waypoint_sequencer.hpp:46`).
- Start: osobny wątek czeka na serwis i wysyła pierwszy punkt.

## Launch
Brak. Uruchomienie np. `ros2 run waypoint_sequencer waypoint_sequencer_node --ros-args --params-file waypoint_sequencer/config/waypoints.yaml` (**niepewne** — komendy nie ma w repo; YAML ma sekcję `waypoint_sequencer_node`).

## Konfiguracja (yaml)
`config/waypoints.yaml`: `waypoint_sequencer_node.ros__parameters.waypoints` = (0,0), (−1,−24), (7,−6), (3,0), (−1,−4), (−6,−16), (−4.5,−25), (−3,−30), (10,−25), (−1000,−1000).

## Zależności
`rclcpp`, `std_msgs`, `l0_interfaces`, `l2_interfaces`, `ament_index_cpp`. Współpraca: [[trajectory_planner]], `peripherals_handler` w [[l0hwd]].

## Interfejsy
Brak.

## Różnice między branchami
Bez zmian na gałęziach Njord względem `master` (zmiany dotyczą `basic_controller`).

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.
