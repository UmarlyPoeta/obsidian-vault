---
typ: solar-pakiet
repo: "[[l2dp]]"
build: ament_cmake
tags: [solar, pakiet]
zaktualizowano: 2026-10-04
---
# planner

> [!skrot] W 30 sekund (co robi, wejście → wyjście)
> Globalny planer ścieżki L2: własne **A\*** w 8-sąsiedztwie na siatce `nav_msgs/OccupancyGrid` z [[pose_graph]] (`/slam/global_costmap`). Dostaje listę celów `/goal` (`PoseArray`) od [[basic_controller]] (`NextWaypoint` z `use_mapper=true`), planuje od pozy łodzi przez kolejne cele i publikuje rzadką listę punktów `/path_points` dla regulatora ([[l0hwd]] `regulator`) i [[trajectory_planner]]. Gdy łódź dotrze do ostatniego celu — `/wp_reached`. **Nie jest to Nav2.**

Opis wg `l2dp_docking_fix@0d444ff` (stan po zawodach Njord).

## Węzły
### `planner` (klasa `PlannerNode`)
- Źródło: `planner/src/planner.cpp`, `planner/include/planner.h`, `planner/src/main.cpp` (`rclcpp::spin`, jednowątkowo). Executable `planner` (`planner/CMakeLists.txt`).
- **Subskrybuje:**
  - `/slam/odom` (`nav_msgs/Odometry`, depth 10) — poza łodzi; **zamiana osi**: `boatPose.x = odom.y`, `boatPose.y = odom.x` (`planner.cpp:173–176`), kurs `-atan2(...)` (`:181`).
  - `/goal` (`geometry_msgs/PoseArray`) — ustawia `planning=true`, usuwa kolejne duplikaty (`PreprocessGoals`, `:217–236`), `goalIndex=0` (`:187–193`).
  - `/slam/global_costmap` (`nav_msgs/OccupancyGrid`) — zapisuje costmapę i **wywołuje planowanie** `GetPathPlan()` (`:195–199`). Planowanie jest więc wyzwalane przyjściem costmapy (GlobalMapBuilder publikuje ją z `publish_rate_hz 2.0` — [[pose_graph]]), nie przyjściem celu.
  - `/which_map` (`std_msgs/Bool`) — `slam_costmap` (domyślnie `true`, `:158`); od `0d444ff` (`:201–205`).
- **Publikuje:** `path` (`nav_msgs/Path`, frame `map`, do RViz), `path_points` (`PoseArray`, frame `odom`, x/y zamienione z powrotem na konwencję odometrii — `:40–41`), `wp_reached` (`std_msgs/Bool`), `invalid_waypoints` (`PoseArray`, nieużywany — kod zakomentowany), `/l2/speed_vector` (`l0_interfaces/SpeedVector`, tylko zadeklarowany — wywołania zakomentowane), `path_markers` (`MarkerArray`). Nazwy bez `/` → w praktyce `/path`, `/path_points`, `/wp_reached`, `/path_markers` (brak namespace).
- Brak parametrów ROS, timerów, serwisów.

**Algorytm (`AStar`, `:100–154`):**
- Ruchy: 4 proste (koszt 1.0) + 4 ukośne (1.4142).
- Koszt: `g + koszt_ruchu + costmap.data[n]*slam_costmap + (abs(pastMoveIndex-i)%5)/5` (`:135`). Ostatni człon to dzielenie całkowite → **zawsze 0** (skręty nie są karane); komórki zajęte (100) nie są wykluczane, tylko drogie. Przy `slam_costmap=false` A* ignoruje costmapę → prosta linia (tak wjeżdża się do slipu — opis commita `0d444ff`).
- Heurystyka euklidesowa (`:5–7`). Brak wyniku → cicho `return`.
- `GetPathPlan` (`:238–310`): jeżeli łódź jest bliżej niż `goalThreshold = 1.0` m (`planner.h:39`) od bieżącego celu, `goalIndex++`; planuje odcinki od końca: ostatni cel ← przedostatni … ← poza łodzi; po osiągnięciu wszystkich celów publikuje `wp_reached=true` i `planning=false`.
- `reconstructPath` (`:9–98`): do `path_points` trafiają tylko punkty, w których kierunek zmienia się o >10° (`:48`); serie ≥3 kroków ukośnych są wycinane (`:76–96`). Stąd kilkanaście punktów zamiast setek.
- Jeżeli ścieżka pusta, publikuje sam punkt końcowy (`:306`).

## Launch
Brak. Uruchamianie: `ros2 run planner planner` (README [[l4sim]]).

## Konfiguracja (yaml)
Brak plików parametrów — wartości na sztywno (`goalThreshold 1.0`).

## Zależności
`package.xml`: `rclcpp`, `rclpy`, `std_msgs`, `nav_msgs`, `sensor_msgs`, `l2_interfaces`, `l0_interfaces`, `vizualize_msgs` (literówka w nazwie), `geometry_msgs`. Interfejsy z [[ros-sbt-interfaces]].
Współpraca: [[pose_graph]] (costmapa, `/slam/odom`), [[basic_controller]] (`/goal`, `/which_map`, `/wp_reached`), [[trajectory_planner]] i `regulator` z [[l0hwd]] (`/path_points`), [[viz_correction]] (republikuje `/path`), [[ascii_map_viz2]].

## Interfejsy
Brak własnych.

## Różnice między branchami
- `master` (`6ae4389`) i `bash_do_uruchomienia/l2dp` (`74c473e`): planer subskrybuje `/odom` i **`/costmap`** (costmapę z [[mapper]]), nie `/slam/odom` i `/slam/global_costmap` (`git diff origin/master feat/dock_costmap_fix -- planner/src/planner.cpp`). Przejście na SLAM nastąpiło na linii Njord.
- `0d444ff`: dodany `/which_map` i mnożnik `slam_costmap`.
- Drzewo robocze `~/solar/l2dp` zawiera te same zmiany jako niezacommitowane.

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.
