---
typ: solar-pakiet
repo: "[[l2dp]]"
build: ament_cmake
tags: [solar, pakiet]
zaktualizowano: 2026-10-04
---
# trajectory_planner

> [!skrot] W 30 sekund (co robi, wejście → wyjście)
> Mimo nazwy **nie planuje trajektorii** — to regulator „jedź do punktu” tykający co 200 ms. Cel dostaje serwisem `/srv/waypoint_navigation_state` (pojedynczy punkt z [[basic_controller]] `NextWaypoint` przy `use_mapper=false` albo z [[waypoint_sequencer]]) lub listą `/path_points` z [[planner]]. Liczy kurs do celu, publikuje `/l2/speed_vector` (`linear`, `angular` w [-1,1]) dla `diff_thrust_driver` w [[l0hwd]], a po dojściu — `/l3/waypoint_reached`.

Opis wg `l2dp_docking_fix@0d444ff`.

## Węzły
### `TrajectoryPlannerNode` (klasa `TrajectoryPlanner`)
- Źródło: `trajectory_planner/src/trajectory_planner.cpp`, `include/trajectory_planner.hpp`, `utils/src/utils.cpp`, `utils/include/utils.hpp`; `src/main.cpp` (`rclcpp::spin`, jednowątkowy). Executable `TrajectoryPlannerNode`.
- **Subskrybuje** (stałe w `trajectory_planner.hpp:51–62`):
  - `/slam/odom` (`nav_msgs/Odometry`, depth 1) — x, y bez zamiany; kurs `-atan2(...)` w stopniach [0,360) (`.cpp:299–314`).
  - `/path_points` (`geometry_msgs/PoseArray`, depth 1) — ładuje listę, `pathIndex=0`, `steering=true` (`:315–327`).
  - `/l3/waypoint_reached` (`std_msgs/Bool`, depth 1) — zatrzymuje (speed 0,0) i `steering=false` (`:329–337`). Uwaga: ten sam topik węzeł sam publikuje.
  - `/mission_state/data` (`l0_interfaces/MissionState`) — zapamiętuje mission/state/transition (`:358–366`), dziś nieużywane (kod omijania zakomentowany).
  - `/l3/pause_wp_driver` (`std_msgs/Bool`) — pauza: nie publikuje prędkości (`:396–401`, `:196–206`).
  - `/colision_detector` — **zakomentowane** (`:339–356`).
- **Publikuje:** `/l2/speed_vector` (`l0_interfaces/SpeedVector`, depth 1), `/l3/waypoint_reached` (`Bool`), `/mission_state/data` (`MissionState`), `/wp_distance` (`std_msgs/Float32`, odległość do celu), `/gui_waypoint_reached` (`geometry_msgs/Point`).
- **Serwisy (serwer):** `/srv/waypoint_navigation_state` (`l2_interfaces/srv/WaypointNavigationState`: `x`, `y`, `stay_in_this_wp_for_30s`, `reverse`, limity prędkości — limity są ignorowane, `:263–274`); `/srv/remove_wp` (`std_srvs/Empty`) — publikuje „osiągnięty” i przestaje sterować (`:276–282`).
- **Timer:** 200 ms → `NavigateToWaypointLoop` (`:438–442`).

**Prawo sterowania (`SteerToWaypointControlValue`, `:100–207`):**
1. `heading_to_wp = GetHeadingTo(...)` (stopnie, `utils.cpp`), `Δ = heading_to_wp − kurs_łodzi`, normalizacja do (−180,180]; przy `reverse` +180°.
2. `angle = Δ/180` → clamp [−1,1].
3. Jeżeli `|Δ| > 90°` → `speed = 0` (obrót w miejscu), w przeciwnym razie `speed = exp(0.015·d) − 0.95` (komentarz: „1 for 45m, 0.05 for 0m”) → clamp [0,1].
4. `reverse`: `linear *= −3`, clamp [−1,0].
5. Czysty człon P na kursie, brak I/D; limity prędkości z serwisu nieużyte.

**Dojście (`NavigateToWaypointLoop`, `:32–98`):** `dist <= THRESHOLD_WAYPOINT` (**1.5 m**, `utils.hpp:9`) → `/gui_waypoint_reached`, opcjonalnie `Timeout(30)` (blokujący `sleep_for` w callbacku timera, `:290–295`), `pathIndex++`; po ostatnim punkcie `/l3/waypoint_reached=true`, prędkość (0,0), `steering=false`. Przy celu z serwisu `pathSize` nie jest ustawiane — **niepewne:** zachowanie zależy od poprzedniej wartości `pathSize`/`pathIndex` (niezainicjowane w konstruktorze).

## Launch
Brak; `ros2 run trajectory_planner TrajectoryPlannerNode`.

## Konfiguracja (yaml)
Brak. Stałe w `utils/include/utils.hpp`: `SPEED 0.15`, `ANGLE_SCALE 0.5`, `THRESHOLD_WAYPOINT 1.5`, `WP_RANGE 20.0`, `WP_VEC_SIZE 5`, `TIMEOUT_30_S 30`; w `trajectory_planner.cpp` używane są tylko `THRESHOLD_WAYPOINT` (`:234`) i `TIMEOUT_30_S` (`:57`).

## Zależności
`l0_interfaces`, `l2_interfaces`, `l3_interfaces`, `rclcpp`, `sensor_msgs`, `std_srvs` ([[ros-sbt-interfaces]]). Współpraca: [[pose_graph]] (`/slam/odom`), [[planner]], [[basic_controller]], [[waypoint_sequencer]], `diff_thrust_driver` w [[l0hwd]], `sim_thrust_driver` w [[l4sim]] ([[sim_thrust_driver]]).

## Interfejsy
Brak własnych.

## Różnice między branchami
- `master`/`bash_do_uruchomienia/l2dp`: odometria z `/odom`, „osiągnięty” nasłuchiwany na `/wp_reached` (planer), `THRESHOLD_WAYPOINT = 1.0`, obrót w miejscu już przy `|Δ| > 10°`, minimalny ciąg `DRIF_FORCE_3 = 0.35`, `speed = exp(0.015·d) − 0.95 − angle`, min. 0.2 dla punktów pośrednich, zwiększanie `speedMultiplier` przy „utknięciu” (`git diff origin/master feat/dock_costmap_fix -- trajectory_planner`).
- Linia Njord (`feat/dock_costmap_fix`): uproszczony regulator jak wyżej, `/slam/odom` (od `0d444ff`), próg 1.5 m.
- `origin/celka_odo` (Monako): +121 linii w `trajectory_planner.cpp`, „tp for controller” — **niepewne**, szczegóły nieprzeanalizowane.

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.
