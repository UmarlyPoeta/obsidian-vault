---
typ: solar-pakiet
repo: "[[l2dp]]"
build: ament_cmake
tags: [solar, pakiet]
zaktualizowano: 2026-10-04
---
# mapper

> [!skrot] W 30 sekund (co robi, wejście → wyjście)
> Starsza ścieżka mapowania oparta na bibliotece `grid_map`: z detekcji L1 (`/fused_detection`, `/l1/obstacle_detection`), skanu 2D i `/odom` buduje mapę globalną 100×100 m (0.3 m) z warstwami `detections`, `boarders`, `cost`, `character`, śledzi obiekty i publikuje `costmap` (`OccupancyGrid`) oraz `tracked_detections`. Na `master` to **ta** costmapa zasilała [[planner]] (`/costmap`); na gałęzi zawodów planer czyta już `/slam/global_costmap` z [[pose_graph]], a `tracked_detections` publikuje też `pose_graph` (`/tracked_detections`).

Opis wg `l2dp_docking_fix@0d444ff`.

## Węzły
### `mapper` (klasa `MapperNode`)
- Źródło: `mapper/src/mapper.cpp` (711 linii), `include/mapper.h`, `src/main.cpp`. Executable `mapper`.
- **Subskrybuje:** `/scanchuj` (`sensor_msgs/LaserScan`, SensorDataQoS — na `master` było `/scan`; brak publishera `/scanchuj`, czyli w praktyce wyłączone), `/odom` (`nav_msgs/Odometry`), `/trajectory` (`geometry_msgs/PoseArray`), `/fused_detection` (`l1_interfaces/VectorOfFusedDetections`), `/l1/obstacle_detection` (`l1_interfaces/VectorOfClusters`), `/used_detections` (`std_msgs/Int32`), `/deuse_detections` (`std_msgs/Bool`) (`mapper.cpp:113–166`).
- **Publikuje:** `global_map`, `local_map` (`grid_map_msgs/GridMap`), `tracked_detections` (`l2_interfaces/TrackedDetectionsVector`), `costmap` (`nav_msgs/OccupancyGrid`, frame `map`), `/boat_mark` (`Marker`), `map_trajectory` (`Marker`), `map_detections` (`MarkerArray`), `/character_points` (`MarkerArray`) (`:169–178`). Nazwy względne → `/global_map`, `/costmap`, `/tracked_detections` itd. (brak namespace) — **kolizja** z `/tracked_detections` z [[pose_graph]], jeśli oba działają.
- **Timery:** 5000 ms → `PublishMap` (`:102`), 100 ms → `CreateCostmap` (`:106`).
- **`CreateCostmap` (`:397`):** dla komórek `boarders ≥ 2` dodaje `inflationCost` w kole 0.7 m i ustawia `obstacleCost` w komórce; następnie idzie po `/trajectory` i przy pierwszej kolizji z kosztem >0 dokłada dodatkowe koło 0.7 m; konwersja `GridMapRosConverter::toOccupancyGrid(globalMap, "cost", 0, obstacleCost, …)`.
- Inne: `UpdateGlobalMap` (`:208`), `sortDetections` (`:245`), `DetectionCallback` (`:341`), `ClusterCallback` (`:510`), `MyExtendToInclude` (rozszerzanie mapy, `:46`).

## Launch
Brak; `ros2 run mapper mapper` (README [[l4sim]]).

## Konfiguracja (yaml)
Brak; geometria na sztywno w konstruktorze (`:81–100`): globalna 100×100 m / 0.3 m, lokalna 20×20 m / 0.1 m. `mapper/README.md` — pusty plik (0 B).

## Zależności
`rclcpp`, `rclpy`, `grid_map_ros`, `grid_map_core`, `grid_map_msgs`, `sensor_msgs`, `l2_interfaces`, `l1_interfaces`, `vizualize_msgs`, `geometry_msgs`. Dane z [[l1od]], [[odometry]]. Odbiorcy: [[planner]] (na `master`), [[ascii_map_viz2]] (domyślnie `/costmap`), węzeł `SearchForMarina` w [[basic_controller]] wg `~/solar/docs/architektura.html` (**niepewne**, nie weryfikowano).

## Interfejsy
Brak.

## Różnice między branchami
- `master` (`6ae4389`, „poprawka na osie jazwieckiego”): ostatnia zmiana w `mapper.cpp` na master (9 linii, osie).
- Gałąź zawodów: +136 linii w `mapper.cpp`, `/scan` → `/scanchuj`, `/character_points`.
- Historyczne gałęzie mappera: `origin/mapper` (zmergowana), `origin/mapper_sim`, `origin/mapper_sim_l2_divided_fix`, `origin/mapper_multitype_fusion` (confidence detekcji), `origin/feature/global-map-sync` (usunięcie starej `UpdateGlobalMap`) — wszystkie martwe.
- Bag `~/solar/rosbagi/docking_regulator_2` (2026-08-08) zawiera `/costmap` (484 wiadomości) — mapper działał wtedy na łodzi.

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.
