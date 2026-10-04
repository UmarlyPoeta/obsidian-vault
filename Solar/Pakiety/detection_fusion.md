---
typ: solar-pakiet
repo: "[[l1od]]"
build: ament_cmake
tags: [solar, pakiet]
zaktualizowano: 2026-10-04
---
# detection_fusion

> [!skrot] W 30 sekund (co robi, wejście → wyjście)
> Starsza fuzja: łączy klastry z lidaru ([[obstacle_detector]]) z detekcjami YOLO ([[vision_detector]]) przez porównanie pozycji i odległości. Wejście: `/l1/obstacle_detection` + `yolo_topic` → wyjście: `/fused_detection` + `/fusion_visualization`. Uruchamiana przez launch na `master`; na `refined-fusion` zastąpiona przez [[detection_pointcloud_fusion]] (pakiet nadal jest w repo i w `l1od.yaml`/`sim.yaml`).

## Węzły

### DetectionFusion
`detection_fusion/src/detection_fusion.cpp`, klasa `DetectionFusion`, executable `DetectionFusion`.
- **Subskrybuje** (`message_filters` `ApproximateTime`, kolejka 20, `setAgePenalty(5.0)`): `/l1/obstacle_detection` (`VectorOfClusters`) i `yolo_topic` (`VectorOfDetections`, domyślnie `/l1/detection_buoy_detector`).
- **Publikuje:** `/fused_detection` (`VectorOfFusedDetections`, depth 10), `/fusion_visualization` (`MarkerArray`, tekst w `hesai_lidar`, 0,5 s).
- **Parametry:** `fusion_angle_threshold 0.07`, `fusion_distance_threshold 1.0`, `yolo_topic`, `real_or_sim "real"`, `camera_fov 1.4825` (`DEFAULT_CAMERA_FOV`, komentarz „110 degrees, new zed2i ACTUAL CAMERA FOV”), `camera_width 960`. `camera_fov`, `camera_width`, `real_or_sim` i `fusion_angle_threshold` są tylko logowane — logika ich nie używa (refined-fusion).
- **Logika** (`DetectionCallback`, `:52-148`):
  1. Dla każdego klastra szuka pierwszej detekcji YOLO, dla której `IsYoloDetectionAndObstacleDetectionSameAngle` (`:161-171`): `|yolo.x − cluster.center.y| < 0.3` (na refined-fusion; na master `cluster.center.x`). Warunek zapisano dwukrotnie dla `x` — `y` nie jest sprawdzany (błąd).
  2. Trafienie → `FusedDetection` z pozycją klastra, `is_fused_angle = true`, `is_fused = |yolo.distance − |cluster.center|| < fusion_distance_threshold`; detekcja usuwana z kopii listy.
  3. Klaster bez pary → `is_fused_angle = is_fused = false`, pozycja klastra.
  4. Następnie **wszystkie** detekcje YOLO z oryginalnej wiadomości (`msg2->detections`, nie pozostała kopia) dopisywane jako `is_fused = false` z pozycją `yolo.x, yolo.y` — sparowane detekcje pojawiają się więc dwa razy.
- README opisuje starszą wersję (porównanie kąta, próg 10 m dla `is_fused`) — kod tego już nie robi.

## Launch
- `master`: `l1_launch/launch/l1od_launch.py` — parametry `fusion_angle_threshold`, `fusion_distance_threshold`, `real_or_sim`, `yolo_topic = "/l1/detection_" + vision_detector`, `camera_fov`.
- `l1od.yaml`/`sim.yaml` (oba branche): w `sim.yaml` remapy `/gps/nav_sat_fix`→`/wamv/sensors/gps/gps/fix`, `/imu/data`→`/wamv/sensors/imu/imu/data`, `/l1/detection_buoy_detector`→`/l1/detection_buoy_detector_simulation`.

## Konfiguracja (yaml)
Brak.

## Zależności
`l1_interfaces` ([[ros-sbt-interfaces]]); CMake: `rclcpp`, `message_filters`, `visualization_msgs`.

## Interfejsy
Brak własnych (zob. [[detection_pointcloud_fusion]] — pola `FusedDetection`).

## Różnice między branchami
- `refined-fusion` vs `master`: tylko zamiana osi klastra w porównaniu (`center.y`, `−center.x` zamiast `center.x`, `center.y`).
- `auto_fov_camera`: subskrypcja `/zed/zed_node/rgb/color/rect/camera_info`, `camera_fov = 2·atan(width/(2·fx))` z pierwszej wiadomości, callback fuzji pomijany do czasu odebrania `camera_info`.

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.
