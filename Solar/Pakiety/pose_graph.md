---
typ: solar-pakiet
repo: "[[l2dp]]"
build: ament_cmake
tags: [solar, pakiet]
zaktualizowano: 2026-10-04
---
# pose_graph

> [!skrot] W 30 sekund (co robi, wejście → wyjście)
> Własny SLAM LiDAR (LOAM-podobny) + budowa mapy globalnej i costmapy w jednym procesie. `FeatureExtractor` bierze chmurę Hesai `/lidar_points` i odometrię `/odom` z [[odometry]], usuwa zniekształcenie ruchu (deskew), klasyfikuje punkty na krawędzie/płaszczyzny, dopasowuje skan do lokalnej mapy wokselowej i publikuje skorygowaną pozę `/slam/odom` (~częstotliwość wejściowej odometrii) oraz śledzone obiekty `/tracked_detections`. `GlobalMapBuilder` z tych danych składa trwałą mapę i costmapę `/slam/global_costmap` (z inflacją i „gaszeniem” slipu z `/dock_polygon`) — jedyne wejście [[planner]]. Najnowszy i największy pakiet L2 (~15 tys. linii).

Opis wg `l2dp_docking_fix@0d444ff`. Pakiet nie istnieje na `master` — pochodzi z gałęzi `origin/scan_matching` (sierpień 2026).

## Węzły
Jeden executable `feature` (`pose_graph/CMakeLists.txt:45`), `src/main.cpp`: tworzy węzeł `FeatureExtractor` (nazwa ROS `feature_extractor`, `feature_extractor.cpp:37`) i dołącza do niego obiekt `GlobalMapBuilder` (ten sam węzeł); `MultiThreadedExecutor` z 3 wątkami. Grupy callbacków: lidar, odom, detekcje, klastry (`feature_extractor.cpp:557–579`).

### FeatureExtractor (`src/feature_extractor.cpp`, ~3600 linii; pomocnicze `deskewer.cpp`, `classifier.cpp`, `scan_matcher.cpp`, `local_voxel_map.hpp`, `hesai_point.cpp`)
- **Subskrybuje:**
  - `/lidar_points` (`sensor_msgs/PointCloud2`, QoS KeepLast(1) reliable) → `hesaiCallback` (`:2302`): deskew, cechy, dopasowanie, keyframe’y.
  - `/odom` (`nav_msgs/Odometry`, SensorDataQoS) → `odomCallback` (`:1574`): zamiana x↔y (`state.position = (y, x, z)`), bufor ruchu dla deskew i **publikacja `/slam/odom` przy każdej próbce** (komentarz: „~400 Hz input gives ~400 Hz /slam/odom”).
  - `/fused_detection` (`l1_interfaces/VectorOfFusedDetections`) → `DetectionCallback` (`:2997`) — z [[l1od]].
  - `/l1/obstacle_detection` (`l1_interfaces/VectorOfClusters`) → `ObstacleCallback` (`:3346`).
  - `/used_detections` (`std_msgs/Int32`) — zwiększa `usage` śledzonej detekcji o danym id; `/deuse_detections` (`std_msgs/Bool`) — zeruje `usage` (używane przez węzły BT w [[basic_controller]]).
- **Publikuje (wybór):** `/slam/odom` (`Odometry`, KeepLast(20) reliable), `/slam/odom_transform` (`geometry_msgs/TransformStamped` — transformacja z ramki `/odom` do `/slam/odom`, czyta ją `GNSSToLocal` w [[basic_controller]]), `/slam/boat_pose` (`Odometry`), `/tracked_detections` (`l2_interfaces/TrackedDetectionsVector`, frame `map`), `/global/clusters` (`l1_interfaces/VectorOfClusters` — wejście [[colision_detector]]), `/slam/trajectory_extrapolation` (`PoseArray` — ekstrapolowany tor łodzi, wejście [[colision_detector]]), `/features/edges`, `/features/planes`, `/features/debris` (`PointCloud2`), `/local_voxel_map`, `/slam/local_map_points` (`PointCloud2`, transient_local), debug: `/slam/debug/raw_scan`, `/slam/debug/deskewed_scan`, `/slam/debug/predicted_scan_map`, `/slam/debug/candidate_scan_map`, `/slam/debug/predicted_lidar_pose`, `/slam/debug/candidate_lidar_pose`, `/slam/debug/map_to_odom`, `/slam/debug/frame_axes`; markery `/slam/odom_marker`, `/slam/boat_marker`, `map_detections`, `map_clusters`.
- **Timer:** `tracking.publish_rate` (domyślnie 10 Hz) → `PublishTrackedDetectionsPeriodic` (`:3583`) — dodane w `0d444ff`, wcześniej `/tracked_detections` publikowane tylko przy nowych detekcjach.
- **Parametry** (sekcja `feature_extractor.ros__parameters` w `config/slam.yaml`): `extrinsic.imu_lidar` (x 0.40, y 0.05, z −0.40, kwaternion jednostkowy), `tracking.*` (bufor póz 5 s, ekstrapolacja ≤0.5 s, próg śledzenia 3.0 m), `deskew.*`, `features.*` (voxel 0.10, 20 sąsiadów), `map.*` (voxel 0.40), `matcher.*`, `keyframes.*` (0.8 m / 4° / 2 s, max 10, `recovery_after_failures 3`), `frames.convert_ned_odom_to_enu_map: false` („use the coordinate setup that worked”).
- Kluczowe funkcje: `setMapToOdom` (`:822`), `updateStableSlamOdomPose` (`:867`), `publishSlamOdometry` (`:1031`), `publishTrajectoryExtrapolation` (`:1143`), `shouldAddKeyframe`/`addKeyframe`/`rebuildLocalMap` (`:2061–2134`), `sortDetections` (`:2693`), `ProcessDetectionMessage` (`:3142`).

### GlobalMapBuilder (`src/global_map_builder.cpp`, `include/global_map_builder.hpp`)
- **Subskrybuje:** `global_map.input_topic` = `/slam/local_map_points`, `global_map.boat_pose_topic` = `/slam/boat_pose`, `costmap.dynamic_edge_topic` = `/features/edges`, `costmap.dynamic_plane_topic` = `/features/planes`, `costmap.dynamic_lidar_pose_topic` = `/slam/debug/predicted_lidar_pose`, `costmap.dock.polygon_topic` = `/dock_polygon` (`geometry_msgs/PolygonStamped` z [[segment_scan]]) (domyślne w `global_map_builder.hpp:323–402`).
- **Publikuje:** `/slam/global_map_points` (`PointCloud2`), `/slam/global_costmap` (`nav_msgs/OccupancyGrid`), `/slam/dock_polygon_map` (`PolygonStamped`, wielokąt slipu w ramce map — do RViz).
- **Timer:** `global_map.publish_rate_hz` = 2.0 Hz → `publishTimerCallback` (`:1862`), `publish_only_when_changed: true`.
- **Costmapa (`publishCostmap`, `:2014`):** rozdzielczość 0.20 m, rozmiar dynamiczny (`fixed_size false`, `dynamic_padding 5.0`, max 2000×2000 komórek); warstwa statyczna (woksele potwierdzone) + warstwa „żywa” z cech krawędzi/płaszczyzn (`dynamic_obstacle_timeout 1.5` s); przeszkoda = 100; inflacja: w promieniu `inscribed_radius 0.30` koszt 99, dalej `round(1 + 98·exp(−3.0·(d − 0.30)))` w [1,98] do `inflation_radius 1.0` (`:2510–2519`); `unknown_as_free: true`.
- **Gaszenie slipu (`f8f0b00`):** `dockPolygonCallback` (`:1310`) przyjmuje wielokąt 4-punktowy, przelicza do mapy z bieżącą pozą łodzi, wydłuża wlot o `mouth_extension 1.5` m (`:1357–1368`), scala po centroidzie (`merge_radius 1.0`), wygasza po `polygon_timeout 5.0` s. W `publishCostmap` po pętli inflacji: wewnątrz wielokąta koszt obcinany do `dock.maximum_cost 0`, w pasie `dock.margin 0.50` m od obrysu do `dock.margin_cost 25`; komórki 100 pomijane (`:2600–2643`). Kolejność ma znaczenie: inflacja składa się przez maksimum, więc gaszenie przed nią byłoby nadpisane (opis commita).

## Launch
Brak pliku launch. **Niepewne:** dokładna komenda z łodzi; `slam.yaml` nie jest instalowany, więc musi być podany ścieżką, np. `ros2 run pose_graph feature --ros-args --params-file pose_graph/config/slam.yaml`.

## Konfiguracja (yaml)
- `config/slam.yaml` (216 linii) — parametry jak wyżej (sekcje `debug`, `extrinsic`, `tracking`, `deskew`, `features`, `map`, `matcher`, `keyframes`, `frames`, `global_map`, `costmap`, `costmap.dock`). `debug.publish_clouds: false`.
- `global_map_builder_parameters.yaml` (113 linii), `baseline_params.yaml` (116 linii) — zestawy alternatywne/bazowe z audytu.
- `START_HERE.md`: procedura pierwszego testu na wodzie (wyłączyć wagi matchera, `keyframes.recovery_after_failures` bardzo wysoko, sprawdzić czy `/slam/boat_pose` podąża za `/odom`), rotacja LiDAR→IMU celowo jednostkowa. `REVIEW_AND_TEST_PLAN.md`: audyt przyczyn złego działania SLAM (degeneracja matchera, niszczący recovery czyszczący keyframe’y, błędy bookkeepingu linii, niespójna klasyfikacja cech, kotwiczenie czasu deskew). `math_checks.py` — 3 testy PASS. `changes.patch`.

## Zależności
`package.xml` (`0d444ff`): `rclcpp`, `rclpy`, `nav_msgs`, `sensor_msgs`, `l2_interfaces`, `l1_interfaces`, `vizualize_msgs`, `geometry_msgs`; CMake: PCL ≥1.3, `pcl_conversions`, `pcl_ros`, Eigen. `grid_map` usunięty w `f8f0b00`. Interfejsy z [[ros-sbt-interfaces]].
Współpraca: [[odometry]] (`/odom`), [[l1od]] (detekcje), [[segment_scan]] (`/dock_polygon`), [[planner]], [[trajectory_planner]], [[colision_detector]], [[basic_controller]] (`/slam/odom`, `/slam/odom_transform`, `/tracked_detections`, `/used_detections`), `regulator` w [[l0hwd]] (`/slam/odom`).

## Interfejsy
Brak własnych msg/srv.

## Różnice między branchami
- Brak na `master` i w `bash_do_uruchomienia/l2dp`.
- `origin/scan_matching` (`ed210d2`) — pierwsza wersja (local/global map, „quite cleanly working slam”).
- `njord/tracked_timestamp_matching` — dopasowanie detekcji do pozy w chwili ich znacznika czasu (`b1bff75`/`e88037b` UmarlyPoeta, „fixed calculating sorting det” patryk_kozlowski).
- `spirala_zjebania` — poprawki osi („Osieet działa”, „NAPRAWILI SLAMA”), `/slam/odom_transform`, ekstrapolacja trajektorii.
- `0d444ff` — okresowe `/tracked_detections`; `f8f0b00` — gaszenie slipu.
- Niezacommitowane w `~/solar/l2dp_docking_fix`: korytarz podejścia `/slam/dock_corridor_map`, `/choose_dock` i bufor póz z `/slam/odom` w GlobalMapBuilder, walidacja wymiarów slipu (szczegóły w [[l2dp]] → „Kopie lokalne”); obrazy `~/solar/docs/dock_costmap_check/`.

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.
