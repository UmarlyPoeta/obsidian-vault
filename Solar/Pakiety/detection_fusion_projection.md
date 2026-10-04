---
typ: solar-pakiet
repo: "[[l1od]]"
build: ament_cmake
tags: [solar, pakiet]
zaktualizowano: 2026-10-04
---
# detection_fusion_projection

> [!skrot] W 30 sekund (co robi, wejście → wyjście)
> Wariant fuzji: środek klastra lidaru przekształca macierzą zewnętrzną (lidar→kamera) z `l1od/config/camera_extrinsic_calibration.yaml` i rzutuje `cv::projectPoints` z intrinsics i dystorsją z `camera_intrinsic_calibration.yaml`; jeśli piksel wpada w bbox YOLO → obiekt zfuzjowany. Wejście: `/l1/obstacle_detection` + `/l1/detection_buoy_detector` → `/fused_detection`. Nie jest uruchamiany przez żaden launch w repo.

## Węzły

### DetectionFusionProjection
`detection_fusion_projection/src/detection_fusion_projection.cpp`, executable `DetectionFusionProjection`.
- **Subskrybuje:** `/l1/obstacle_detection` (`VectorOfClusters`) + `/l1/detection_buoy_detector` (`VectorOfDetections`) — stałe w `.hpp:23-24`, `ApproximateTime` kolejka 20, age penalty 5.
- **Publikuje:** `/fused_detection` (`VectorOfFusedDetections`). Publisher `/fusion_visualization` zadeklarowany, ale nieutworzony (wizualizacja „//TODO”).
- **Parametry:** brak.
- **Konfiguracja:** ścieżki WZGLĘDNE `src/l1od/config/camera_intrinsic_calibration.yaml` i `…/camera_extrinsic_calibration.yaml` (`.hpp:28-29`) — działa tylko uruchomiony z katalogu workspace.
- **Logika:** `LidarPointToPixel` (`:162-185`): `extrMat · [x y z 1]` → `projectPoints(rvec=0, tvec=0, K, dist)`. `IsYoloDetectionAndObstacleDetectionSame` (`:147-160`): piksel w [bbox_x0..x1]×[bbox_y0..y1]. Pozycja zfuzjowana = (center.y, center.x, center.z). `std::remove` bez `erase` (`:98`) — detekcja nie jest faktycznie usuwana z listy.
- `calibration_check.ipynb` w `src/` — notebook do sprawdzania kalibracji.

## Launch
Brak.

## Konfiguracja (yaml)
`l1od/config/camera_intrinsic_calibration.yaml` (2025-01-22) i `camera_extrinsic_calibration.yaml` (komentarz „dzien przed testami barki 8 luty, w 106”) — opis w [[l1od]].

## Zależności
`package.xml`: `l1_interfaces`, `rclcpp`, `message_filters`, `visualization_msgs`; w kodzie także OpenCV, Eigen3, yaml-cpp. Kalibrację wygenerowano narzędziami z [[fusion_camera_lidar]] (ten sam format `extrinsic_matrix`, `camera_matrix`, `distortion_coefficients`) — **niepewne:** czy dokładnie tymi.

## Interfejsy
Brak własnych.

## Różnice między branchami
Brak różnic w kodzie tego pakietu między `master` i `refined-fusion` (nie występuje w diffie).

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.
