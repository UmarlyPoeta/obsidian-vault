---
typ: solar-pakiet
repo: "[[l1od]]"
build: ament_cmake
tags: [solar, pakiet]
zaktualizowano: 2026-10-04
---
# l1_launch

> [!skrot] W 30 sekund (co robi, wejście → wyjście)
> Pliki launch warstwy L1. Detektor wizyjny NIE jest uruchamiany jako `Node`, tylko jako proces `ExecuteProcess` wołający `$DEV_WS_DIR/src/l1od/vision_detector/docker/run.sh` (kontener Docker). Na `refined-fusion` launch łodzi = detektor + [[detection_pointcloud_fusion]].

## Węzły
Brak (pakiet tylko z `launch/`).

## Launch

### `launch/l1od_launch.py` (refined-fusion)
- Argumenty: `camera_fov` (1.76278254, nieużywany), `confidence_threshold` (0.7), `vision_detector` (`buoy_detector`), `vision_detector_CB` (0).
- Uruchamia: `run.sh <vision_detector> CB=<..> CT=<..>` (`shell=True`) oraz `detection_pointcloud_fusion/DetectionPointCloudFusion` z `real_or_sim: real`.
- Wymaga zmiennej `DEV_WS_DIR` (i `VISION_DETECTOR_DOCKER_DEVICE` dla `run.sh`).

### `launch/sim_launch.py` (refined-fusion)
- `run.sh buoy_detector_simulation` z remapami `CTR=/zed/zed_node/rgb/color/rect/image:=/wamv/sensors/cameras/front_left_camera_sensor/image_raw` i `DTR=/zed/zed_node/point_cloud/cloud_registered:=/wamv/sensors/cameras/front_left_camera_sensor/points`.
- `DetectionPointCloudFusion` z parametrami symulacji (960×720, fx=fy=397,77, cx 480, cy 370, `lidar_topic /wamv/sensors/lidars/lidar_wamv_sensor/points`, `yolo_topic /l1/detection_buoy_detector_simulation`, `frame_id wamv/wamv/base_link/lidar_wamv_sensor`).

### `launch/l1od_launch.py` (master)
Argumenty dla [[obstacle_detector]] (x ±15, y 0..15, z −1,5..5, `tolerance 0.2`, rozmiary klastrów), [[detection_fusion]] (`fusion_angle_threshold 0.07`, `fusion_distance_threshold 1.0`, `real_or_sim`), [[tracking_detection]] (`tracking_tolerance 1.5`); uruchamia detektor (Docker), `ObstacleDetector`, `DetectionFusion`, `TrackingDetection`.

### `launch/l1od.yaml` i `launch/sim.yaml` (oba branche, starsze)
Uruchamiają `vision_detector` jako zwykły węzeł (`buoy_detector` / `buoy_detector_simulation`), `ObstacleDetector`, `DetectionFusion`, `TrackingDetection` i `gps_calculator/GpsConverter` (pakiet z [[l2dp]]). `sim.yaml` remapuje obraz, lidar, `/gps/nav_sat_fix`, `/imu/data` na topiki `/wamv/sensors/...`.

## Konfiguracja (yaml)
Brak.

## Zależności
`package.xml`: [[vision_detector]], [[obstacle_detector]], [[detection_fusion]], [[tracking_detection]], `gps_calculator` ([[l2dp]]). Brak zależności od [[detection_pointcloud_fusion]], choć launch na refined-fusion go uruchamia.

## Interfejsy
Brak.

## Różnice między branchami
`refined-fusion` i `vlm-detection`: `l1od_launch.py` i `sim_launch.py` skrócone do detektor + `DetectionPointCloudFusion` (commit `ec0f624` „Make l1od launch file compatible with new fusion implementation”). `dbscan_clustering`: zmiany w `sim_launch.py` (25 linii).

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.
