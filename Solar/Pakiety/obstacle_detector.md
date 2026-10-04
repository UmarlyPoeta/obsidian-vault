---
typ: solar-pakiet
repo: "[[l1od]]"
build: ament_cmake
tags: [solar, pakiet]
zaktualizowano: 2026-10-04
---
# obstacle_detector

> [!skrot] W 30 sekund (co robi, wejście → wyjście)
> Wykrywanie przeszkód z lidaru bez uczenia: chmura `/lidar_points` → przycięcie do prostopadłościanu (CropBox) → VoxelGrid 2 cm → klasteryzacja euklidesowa (PCL) → filtr rozmiaru → `/l1/obstacle_detection` (`l1_interfaces/VectorOfClusters`: środek, min, max każdego klastra) + markery RViz. Układ współrzędnych: surowy lidar Hesai (`hesai_lidar`, RFU wg [[sbt_transforms]]). Odbiorcy: [[l2dp]] (`mapper`, `pose_graph`, `obstacle_avoidance`, `colision_detector`).

## Węzły

### ObstacleDetector
Plik `obstacle_detector/src/obstacle_detector.cpp`, klasa `ObstacleDetector`, executable `ObstacleDetector` (`CMakeLists.txt`), nazwa węzła `ObstacleDetector`.
- **Subskrybuje:** `POINT_CLOUD_TOPIC` = `/lidar_points` (`sensor_msgs/PointCloud2`, depth 10).
- **Publikuje:**
  - `OBSTACLE_DETECTION_TOPIC` = `/l1/obstacle_detection` — `l1_interfaces/VectorOfClusters`, depth 10; tylko gdy jest ≥1 klaster; header = stempel i frame chmury (`:192-196`).
  - `MARKER_TOPIC` = `/obstacle_markers` — `visualization_msgs/MarkerArray`, BEST_EFFORT depth 10, co 10. chmurę (`:289`).
  - `FILTERED_CLOUD_TOPIC` = `/filtered_cloud` — publisher istnieje, publikacja zakomentowana w `52dabf2`.
  - `/ground_cloud` — publisher istnieje, kod RANSAC-u zakomentowany.
- **Parametry (domyślne w kodzie, refined-fusion):** `x_axis_min −20`, `x_axis_max 20`, `y_axis_min 0`, `y_axis_max 40`, `z_axis_min −1.5`, `z_axis_max 5`, `tolerance 0.5`, `cluster_size_min 5`, `cluster_size_max 10000`, `cluster_z_min −1.0`, `cluster_z_max 5.0`, `cluster_x_min 0`, `cluster_x_max 4`, `cluster_y_min 0`, `cluster_y_max 4`, `ground_tolerance 0.3`, nazwy topików jw. (`:8-28`).
- **Algorytm** (`PCCallback`, `:61-197`):
  1. `pcl::fromROSMsg` → `PointXYZI`.
  2. `CropBox` [x_min..x_max]×[y_min..y_max]×[z_min..z_max] — przy montażu Hesai y = przód, więc `y_axis_min=0` odcina wszystko za rufą.
  3. `VoxelGrid` 0,02 m.
  4. (zakomentowane) RANSAC płaszczyzny wody z `ground_tolerance`.
  5. `EuclideanClusterExtraction`: tolerancja `tolerance`, rozmiar [`cluster_size_min`, `cluster_size_max`] punktów.
  6. `IsClusterValid` (`:199-211`): odrzuca klastry ze środkiem w prostokącie x∈(−0,5; 0,5), y∈(−1; 0,5) (sama łódź); akceptuje, gdy wysokość > `cluster_z_min` i szerokość x < `cluster_x_max` i y < `cluster_y_max`. Przy domyślnym `cluster_z_min = −1.0` warunek wysokości jest zawsze spełniony.
  7. Dla każdego klastra: `compute3DCentroid` → `center`, `getMinMax3D` → `min`, `max`.
  8. Markery: pudełka `LINE_LIST`, `frame_id` na sztywno `hesai_lidar`, czas życia 0,5 s; zielone gdy przekątna w XY ∈ (0,3; 5) m i wysokość ∈ (0,1; 3) m, inaczej różowe.

### obstacle_detector.py (skrypt)
`scripts/obstacle_detector.py`, instalowany przez `install(PROGRAMS …)`. Węzeł `minimal_subscriber`: subskrybuje `wamv/sensors/lidars/lidar_wamv_sensor/points`, DBSCAN z `sklearn` + Open3D, publikuje `dbscan_viz`. Eksperyment do symulacji; nie jest uruchamiany przez żaden launch.

## Launch
- `launch/obstacle_detector.launch.py`: argument `config_file` (domyślnie `share/obstacle_detector/config/sim_config.yaml`), uruchamia `ObstacleDetector` z tym plikiem parametrów.
- Na `master` `l1_launch/launch/l1od_launch.py` uruchamia `ObstacleDetector` z argumentami (domyślne tam: x ±15, y 0..15, z −1,5..5, `tolerance 0.2`, `cluster_z_min 0.1`, `cluster_x/y_max 2.0`). Na `refined-fusion` launch L1 go NIE uruchamia.
- `l1_launch/launch/sim.yaml`: remap `/lidar_points` → `/wamv/sensors/lidars/lidar_wamv_sensor/points`.

## Konfiguracja (yaml)
`config/sim_config.yaml`: x,y ±20, z ±3, `tolerance 0.5`, rozmiar klastra 10–1000, `cluster_{x,y,z}_max 2.0`, `cluster_z_min 0.0` oraz `point_cloud_topic: "wamv/sensors/lidars/lidar_wamv_sensor/points"` itd. Uwaga: węzeł deklaruje parametry topików WIELKIMI literami (`POINT_CLOUD_TOPIC`), więc klucze topików z tego pliku nie działają.

## Zależności
`package.xml`: `rclcpp`, `rclpy`, `sensor_msgs`, `geometry_msgs`, `l1_interfaces` ([[ros-sbt-interfaces]]); CMake dodatkowo `PCL 1.3`, `pcl_conversions`, `pcl_ros`, `visualization_msgs`. Źródło danych: driver lidaru `hesai_ros_driver` uruchamiany z [[l0hwd]].

## Interfejsy
Brak własnych. `l1_interfaces/Cluster`: `geometry_msgs/Point center, min, max`; `VectorOfClusters`: `header`, `clusters[]`.

## Różnice między branchami
- `refined-fusion` (`52dabf2`, „Obstacle avoidance changes”): zmiana domyślnych (`y_axis_min −10→0`, `cluster_size_min 50→5`, `cluster_x/y_max 3→4`), wyłączony `/filtered_cloud`, nowy warunek koloru markera.
- `dbscan_clustering` (i `zed_exposure_controller`): pakiet rozbity na `PclPreprocess`, `DbscanClustering` (adaptacyjny DBSCAN w 4 strefach), `EuclideanClustering`, wspólne `clustering_utils`; topiki `/l1/obstacle_detection/pcl_preprocessed`, `/l1/obstacle_detection/{dbscan,euclidean}/{clusters,markers}`; szczegóły w [[l1od]] → Branche.

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.
