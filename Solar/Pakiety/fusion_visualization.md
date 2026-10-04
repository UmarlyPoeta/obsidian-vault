---
typ: solar-pakiet
repo: "[[l1od]]"
build: ament_python
tags: [solar, pakiet]
zaktualizowano: 2026-10-04
---
# fusion_visualization

> [!skrot] W 30 sekund (co robi, wejście → wyjście)
> Narzędzie diagnostyczne kalibracji: nakłada punkty lidaru na obraz kamery (kolor wg głębokości, mapa `coolwarm_r`, 2–15 m). Wejście: `/zed/zed_node/rgb/color/rect/image` + `/lidar_points` → `/lidar_camera_projection` (`sensor_msgs/Image`, 640×640).

## Węzły

### fusion_visualization
`fusion_visualization/fusion_visualization/fusion_visualization.py`, klasa `FusionVisualization`, executable `fusion_visualization` (`setup.py`).
- **Subskrybuje:** `/zed/zed_node/rgb/color/rect/image` (`Image`) i `/lidar_points` (`PointCloud2`) przez `ApproximateTimeSynchronizer(queue_size=5, slop=0.07)`.
- **Publikuje:** `/lidar_camera_projection` (`Image`, depth 10).
- **Konfiguracja:** `os.getcwd()/src/l1od/config/camera_extrinsic_calibration.yaml` i `camera_intrinsic_calibration.yaml` — uruchamiać z katalogu workspace.
- **Logika:** obraz skalowany do 640×640 (`:53`), punkty `[x y z 1] · Tᵀ`, filtr `z_cam > 0`, `cv2.projectPoints(K, dist)`, kółka r=2 px. Uwaga: rzut liczony intrinsics z kalibracji (1280×720), a rysowany na obrazie przeskalowanym do 640×640 — **niepewne:** czy to celowe.

## Launch
Brak w repo; `ros2 run fusion_visualization fusion_visualization`.

## Konfiguracja (yaml)
Pliki kalibracji z `l1od/config/` (opis w [[l1od]]).

## Zależności
Tylko test_depend w `package.xml`; w kodzie: `rclpy`, `cv_bridge`, `message_filters`, OpenCV, numpy, matplotlib, PyYAML. Pokrewne: [[fusion_camera_lidar]] (`lidar_camera_projection.py` — prawie ten sam kod).

## Interfejsy
Brak.

## Różnice między branchami
Na `refined-fusion` commit `6c5264e` „Align the fusion visualization axes with /lidar_points” dotyczył wizualizacji w [[detection_pointcloud_fusion]]; pliki tego pakietu nie występują w diffie `master...refined-fusion`.

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.
