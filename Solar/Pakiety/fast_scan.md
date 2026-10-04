---
typ: solar-pakiet
repo: "[[l2dp]]"
build: ament_cmake
tags: [solar, pakiet]
zaktualizowano: 2026-10-04
---
# fast_scan

> [!skrot] W 30 sekund (co robi, wejście → wyjście)
> Szybka konwersja chmury 3D (`PointCloud2`) na 2D `LaserScan` z odszumianiem: punkty filtrowane po wysokości i zasięgu, w każdym binie kątowym klastrowane po odległości, do skanu trafia najbliższy klaster mający wsparcie w sąsiednich binach. Wynik `/scan` jest wejściem detektora slipu [[segment_scan]].

Opis wg `l2dp_docking_fix@0d444ff` (na tej gałęzi plik przepisany: +1054 linii względem `master`).

## Węzły
### `fast_cloud_to_scan_clustered` (klasa `FastCloudToScan`)
- Źródło: `fast_scan/src/fast_scan.cpp` (942 linie), `include/fast_scan.hpp`, `src/main.cpp`. Executable `fast_scan`.
- **Parametry z domyślnymi** (`fast_scan.cpp:53–131`): `input_topic` `/debug/deskewed`, `scan_topic` `/scan`, `debug_topic` `/debug_points`, `min_z` −2.0, `max_z` 1.0, `min_range` 0.5, `max_range` 30.0, `angle_min` −π, `angle_max` π, `angle_increment` 0.008726646 (0.5°), `cluster_tolerance` 0.30, `neighbor_range_tolerance` 0.50, `max_clusters_per_bin` 4, `min_points_per_cluster` 3, `neighbor_bins` 2, `min_support_bins` 2, `scan_time` 0.0, `publish_debug` true, `max_debug_points` 100000, `debug_point_size` 0.05.
- **Subskrybuje:** `input_topic` (`sensor_msgs/PointCloud2`, KeepLast(10) reliable, `:230`).
- **Publikuje:** `scan_topic` (`sensor_msgs/LaserScan`, depth 10), `debug_topic` (`visualization_msgs/MarkerArray`).
- Logika (`callback`, `:496`): odrzucenie po z i zasięgu (`:594–615`), klastrowanie zasięgów w binie (dołącz do istniejącego / utwórz nowy / zastąp najdalszy, `:317–380`), wymóg wsparcia sąsiednich binów dla rzadkich chmur (`:465–480`), klasyfikacja punktów po klastrowaniu całej chmury (`:807`).

## Launch
Brak.

## Konfiguracja (yaml)
Brak w repo (commit `74c473e` „fast_scan config” na `master` zmienia parametry w kodzie).

## Zależności
`rclcpp`, `rclpy`, `sensor_msgs`, `visualization_msgs`. Odbiorca: [[segment_scan]]. Alternatywa: [[pointcloud_to_laserscan]].

## Interfejsy
Brak.

## Różnice między branchami
- `master`/`bash_do_uruchomienia/l2dp`: wejście na sztywno `/lidar_points`, wyjście `/scan`, debug `/debug_points` (depth 100) — bez parametrów.
- Gałąź zawodów: wejście parametryzowane, domyślnie `/debug/deskewed`. **Niepewne:** w przejrzanym kodzie nikt nie publikuje `/debug/deskewed` ([[pose_graph]] publikuje `/slam/debug/deskewed_scan`, i to tylko przy `debug.publish_clouds`), więc na łodzi najpewniej podawano `input_topic` przy starcie — brak skryptu, który by to potwierdzał.

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.
