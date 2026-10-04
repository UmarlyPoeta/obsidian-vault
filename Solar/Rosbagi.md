---
typ: solar-przeglad
tags: [solar, rosbag]
zaktualizowano: 2026-10-04
---
# Rosbagi

> [!skrot] W 30 sekund
> W `~/solar/rosbagi/` jest **jeden** bag: `docking_regulator_2` (sqlite3, 1,6 GB, 48,4 s, 2193 wiadomości, nagrany 2026-08-08). Zawiera chmurę z lidaru oraz wyjścia L2 (odometria, trajektoria, ścieżka, costmapa) — brak obrazu z kamery i brak topików L1. Innych `metadata.yaml` w `~/solar` nie ma.

## docking_regulator_2
Źródło: `~/solar/rosbagi/docking_regulator_2/metadata.yaml`.

| Pole | Wartość |
|---|---|
| Format | rosbag2 `version: 5`, `storage_identifier: sqlite3`, bez kompresji |
| Plik | `scan_08082026_bag_0.db3` — 1 667 088 384 B (≈1,55 GiB) |
| Start | `1786152231831168749` ns = **2026-08-08 01:23:51 UTC** (03:23:51 czasu polskiego) |
| Czas trwania | 48 370 528 313 ns ≈ **48,4 s** |
| Wiadomości | 2193 |

| Topik | Typ | Liczba | ≈ Częstotliwość | Kto publikuje (z kodu) |
|---|---|---|---|---|
| `/lidar_points` | `sensor_msgs/msg/PointCloud2` | 484 | 10 Hz | driver lidaru Hesai ([[l0hwd]], `hesai_ros_driver`) |
| `/costmap` | `nav_msgs/msg/OccupancyGrid` | 484 | 10 Hz | [[l2dp]] `mapper` (`mapper.cpp:173`, `"costmap"`) |
| `/odom` | `nav_msgs/msg/Odometry` | 661 | 13,7 Hz | [[l2dp]] `odometry` (`OdoCpp.cpp:38`) |
| `/trajectory` | `geometry_msgs/msg/PoseArray` | 480 | 9,9 Hz | [[l2dp]] `odometry` (`OdoCpp.cpp:44`) |
| `/path` | `nav_msgs/msg/Path` | 84 | 1,7 Hz | [[l2dp]] `planner` (`planner.cpp:210`, `"path"`) |
| `/map_detections` | `visualization_msgs/msg/MarkerArray` | 0 | — | [[l2dp]] `mapper`/`pose_graph` (`"map_detections"`) |

QoS nagrane: wszystkie `reliability: 1` (RELIABLE), `durability: 2` (VOLATILE), `depth` 10, dla `/lidar_points` `depth 100`. Częstotliwości policzone jako liczba / 48,4 s.

**Przypuszczenia (nie wynikają wprost z plików):**
- Nazwa „docking_regulator_2” i zestaw topików (lidar + costmapa + ścieżka planera) sugerują test regulatora/planowania dokowania na podstawie samego lidaru — przydatny do odtwarzania [[l2dp]] (`mapper`, `planner`) i [[obstacle_detector]] bez łodzi.
- `/map_detections` z 0 wiadomości → percepcja wizyjna (L1) nie działała lub nie była nagrywana; brak `/fused_detection` i `/l1/obstacle_detection`.
- Godzina 03:23 może oznaczać niezsynchronizowany zegar komputera (Jetson bez RTC/NTP) — **niepewne**. Data w nazwie pliku (`08082026`) zgadza się z datą stempla.
- Dzień przed ostatnimi commitami w [[l1od]] `refined-fusion` z 2026-08-09…08-12 („Publishing fusion with a pointcloud stamp”, „Obstacle avoidance changes”) — zbieżność czasowa, związek niepotwierdzony.

## Odtwarzanie
```bash
ros2 bag info ~/solar/rosbagi/docking_regulator_2
ros2 bag play ~/solar/rosbagi/docking_regulator_2 --topics /lidar_points   # np. tylko lidar dla obstacle_detector
```
Uwaga: rosbagi wspominane w kodzie, ale nieobecne w `~/solar`: „2026-03-21 bags” (`l1od` gałąź `zed_exposure_controller`, `zed_exposure_controller.yaml` — dobór ROI dla [[zed_controller]]).

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.
