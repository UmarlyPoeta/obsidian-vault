---
typ: solar-pakiet
repo: "[[l1od]]"
build: ament_cmake
tags: [solar, pakiet]
zaktualizowano: 2026-10-04
---
# detection_pointcloud_fusion

> [!skrot] W 30 sekund (co robi, wejście → wyjście)
> „Refined fusion” — aktualna fuzja kamera–lidar na gałęzi `refined-fusion`. Każdy punkt chmury z lidaru rzutuje modelem otworkowym na obraz i przypisuje do bboxów YOLO, w które trafił; dla każdej detekcji bierze najliczniejszy „pas głębokości” (punkty posortowane po odległości, przerwa > 0,25 m dzieli grupy) i uśrednia go → pozycja 3D obiektu. Wejście: `/l1/detection_buoy_detector` + `/lidar_points` → wyjście: `/fused_detection` (`l1_interfaces/VectorOfFusedDetections`) + `/fusion_visualization`. Istnieje tylko na gałęziach `refined-fusion` i `vlm-detection`.

## Węzły

### DetectionPointCloudFusion
Pliki `detection_pointcloud_fusion/src/detection_pointcloud_fusion.cpp`, `include/detection_pointcloud_fusion.hpp`, `src/node.cpp`; executable `DetectionPointCloudFusion` (`CMakeLists.txt`).
- **Subskrybuje** (`message_filters`, `ApproximateTime`, kolejka 20, `setAgePenalty(1.0)`, `setMaxIntervalDuration(25 ms)` — `:57-79`):
  - `yolo_topic` (domyślnie `/l1/detection_buoy_detector`) — `l1_interfaces/VectorOfDetections`
  - `lidar_topic` (domyślnie `/lidar_points`) — `sensor_msgs/PointCloud2`
- **Publikuje:**
  - `/fused_detection` — `l1_interfaces/VectorOfFusedDetections`, depth 10; header = header chmury, `frame_id` = parametr `frame_id` (`:125-126`, commit `3959b42` „Publishing fusion with a pointcloud stamp”).
  - `/fusion_visualization` — `visualization_msgs/MarkerArray` (punkty `SPHERE_LIST` 1 s + etykieta tekstowa 0,25 s, tylko dla `is_fused`).
- **Parametry (domyślne):** `yolo_topic`, `lidar_topic`, `real_or_sim = "real"`, `frame_id = "hesai_lidar"`, `camera_fx = camera_fy = 362.9652`, `camera_cx = 486.2684`, `camera_cy = 274.1009`, `camera_width = 960`, `camera_height = 540` (komentarz: „from zed/zed_node/camera_info intricsics (K matrix)”, `.hpp:37-43`). Zakomentowany zestaw dla „old zed narrow FOV 72 deg” (fx 957,97).
- **Logika** (`DetectionCallback`, `:87-118`; `mapYoloToLidar`, `:120-283`):
  1. Odrzuca parę, jeśli |Δt| stempli > 25 ms; odrzuca ramkę z > 500 detekcjami („Garbage data”).
  2. Dla każdego punktu lidaru:
     - `real`: oś robota `x_robot = y_lidar` (przód), `y_robot = x_lidar` (prawo), `z_robot = z_lidar`; oś kamery `z_cam = y_lidar`, `x_cam = x_lidar`, `y_cam = −z_lidar − 0.11` (kamera 11 cm pod lidarem; komentarz „kalandyk's axes”).
     - `sim`: z URDF WAM-V (kamera z=0,65, lidar z=0,78 → `dz = −0.13`), `z_cam = x`, `x_cam = −y`, `y_cam = −(z+0.13)`; `x/y/z_robot` = surowe.
     - pomija punkty z `z_cam ≤ 0.3`; `u = x_cam·fx/z_cam + cx`, `v = y_cam·fy/z_cam + cy`; poza obrazem → pomija.
     - dopisuje punkt do każdej detekcji, której bbox zawiera (u, v).
  3. Dla każdej detekcji: brak punktów → `is_fused=false`, pozycja (0,0,0). W przeciwnym razie sortuje po `distance_sq`, szuka najdłuższego ciągu punktów bez skoku odległości > `depth_gap_threshold = 0.25 m`, wpisuje te punkty do `spatial_points`, średnia → `position`.
  4. `is_fused = IsSamePosition(yolo, position)` — |Δx| < 0,5, |Δy| < 0,5, |Δz| < 1,0 względem `x,y,z` z [[vision_detector]] (pozycja z chmury ZED) (`:389-412`).
  5. Kopiuje `detection_class/type/string`, `confidence`, rogi bboxa do `yolo_bbox_corners`. `fused_det.header.stamp = now()`.
- Pola **nieustawiane**: `is_fused_angle` (zawsze false), `id`, `cluster_bbox_corners`. Skutek: [[tracking_detection]] (filtruje po `is_fused_angle`) odrzuci wszystko.
- Kolory markerów wg `detection_type`: 101 czerwony, 102 zielony, 103 niebieski, 104 żółty, 152–155 (kardynalne) różne, 105 czarny.

## Launch
- `l1_launch/launch/l1od_launch.py` (refined-fusion): `parameters=[{'real_or_sim': 'real'}]`.
- `l1_launch/launch/sim_launch.py`: `real_or_sim: sim`, `camera_width 960`, `camera_height 720`, `cx 480`, `cy 370`, `fx = fy = 397.77`, `lidar_topic: /wamv/sensors/lidars/lidar_wamv_sensor/points`, `yolo_topic: /l1/detection_buoy_detector_simulation`, `frame_id: wamv/wamv/base_link/lidar_wamv_sensor`.

## Konfiguracja (yaml)
Brak; wszystko parametrami. Nie korzysta z `l1od/config/camera_*_calibration.yaml` — przesunięcie kamera–lidar (0,11 m real / 0,13 m sim) i orientacja osi są zakodowane w kodzie.

## Zależności
`package.xml`: tylko `l1_interfaces` ([[ros-sbt-interfaces]]); CMake: `rclcpp`, `message_filters`, `visualization_msgs`. Wejście z [[vision_detector]] i lidaru ([[l0hwd]], `hesai_ros_driver`). Odbiorcy `/fused_detection`: [[tracking_detection]], [[l2dp]] (`mapper.cpp:144`, `feature_extractor.cpp:591`, `message_protobuf/protobuf.py:58`).

## Interfejsy
Brak własnych. `l1_interfaces/FusedDetection`: `header`, `id`, `is_fused_angle`, `is_fused`, `detection_class`, `detection_type`, `detection_string`, `confidence`, `position` (Point), `yolo_bbox_corners[]`, `cluster_bbox_corners[]`, `spatial_points[]`.

## Różnice między branchami
- Nie istnieje na `master`, `dbscan_clustering`, `zed_exposure_controller`, `auto_fov_camera`, `quantized_model`, `fix/docker-optimization`.
- `vlm-detection` ma wcześniejszą wersję (343 linie zamiast 412; odbita 2026-06-04 przed poprawkami synchronizacji z lipca/sierpnia).
- Historia na refined-fusion: `3e1e1ed` „Initial new fusion setup” (2026-03-24) → `3f51797` parametryzacja intrinsics → `b382d92` symulacja → `b61a78d` „Fix synchronization issue” → `279b65b` „X do przodu, Y w prawo” → `3959b42` stempel chmury → `52dabf2`.
- Repo [[sbt_transforms]] (`config/frames.yaml`) opisuje `/fused_detection` jako `status: inconsistent` (real: FRU ze stemplem `hesai_lidar`; sim: FLU).

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.
