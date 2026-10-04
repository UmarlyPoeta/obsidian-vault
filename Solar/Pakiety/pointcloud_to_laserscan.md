---
typ: solar-pakiet
repo: "[[l2dp]]"
build: ament_cmake
tags: [solar, pakiet]
zaktualizowano: 2026-10-04
---
# pointcloud_to_laserscan

> [!skrot] W 30 sekund (co robi, wejście → wyjście)
> Wersja standardowego pakietu ROS `pointcloud_to_laserscan` (licencja i `CHANGELOG.rst` z upstreamu), przepisana na `master` w commitach „hand written pcl to laserscan” (`997f3a7`, 2026-03-29). Zamienia `cloud_in` (`PointCloud2`) na `scan` (`LaserScan`). Przykładowy launch remapuje to na `/lidar_points` → `/scan`. Alternatywa dla [[fast_scan]]; w symulacji [[l4sim]] README uruchamia właśnie ten launch.

Opis wg `l2dp_docking_fix@0d444ff`.

## Węzły
- `pointcloud_to_laserscan_node` (`src/pointcloud_to_laserscan_node.cpp`, klasa `pointcloud_to_laserscan::PointCloudToLaserScanNode`, nazwa węzła `pointcloud_to_laserscan`): subskrybuje `cloud_in` (`sensor_msgs/PointCloud2`, uruchamiany dopiero gdy ktoś słucha `scan` — `:120–128`), publikuje `scan` (`LaserScan`, SensorDataQoS, `:78`). Parametry: `target_frame` "", `transform_tolerance` 0.01, `min_height`, `max_height`, `angle_min` −π, `angle_max` π, `angle_increment` π/180, `scan_time` 1/30, `range_min` 0.0, … (`:61–73`). Używa tf2 (`target_frame`).
- `laserscan_to_pointcloud_node` (`src/laserscan_to_pointcloud_node.cpp`) — odwrotna konwersja.
- `dummy_pointcloud_publisher` (`src/dummy_pointcloud_publisher.cpp`) — testowy publisher (executable z `ament_auto_add_executable`).

## Launch
- `launch/sample_pointcloud_to_laserscan_launch.py`: argument `scanner` (nieużywany), węzeł `pointcloud_to_laserscan_node`, remapy `cloud_in → /lidar_points`, `scan → /scan`, parametry `target_frame hesai_lidar`, `min_height −0.6`, `max_height 0.1`, `angle_increment 0.0087`, `scan_time 0.3333`, `range_min 0.6`, `range_max 30.0`, `use_inf True`, `inf_epsilon 1.0`.
- `launch/sample_laserscan_to_pointcloud_launch.py` — próbka odwrotnej konwersji.
- W katalogu leży plik wymiany vima `.sample_pointcloud_to_laserscan_launch.py.swp`.

## Konfiguracja (yaml)
Brak (parametry w launchu).

## Zależności
`laser_geometry`, `message_filters`, `rclcpp`, `rclcpp_components`, `sensor_msgs`, `tf2`, `tf2_ros`, `tf2_sensor_msgs`. Odbiorca `/scan`: [[segment_scan]].

## Interfejsy
Brak.

## Różnice między branchami
- `origin/point2cloud` (`2995dc4`, Natalia Guzdek, 2026-03-18): osobny rewrite (−292/+87) — niezmergowany, martwy.
- Gałąź zawodów: 4 linie zmian w launchu względem `master`.

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.
