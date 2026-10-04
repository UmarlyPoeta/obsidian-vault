---
typ: solar-pakiet
repo: "[[fusion_camera_lidar]]"
build: ament_python
tags: [solar, pakiet]
zaktualizowano: 2026-10-04
---
# ros2_camera_lidar_fusion

> [!skrot] W 30 sekund (co robi, wejście → wyjście)
> Narzędzia kalibracji kamera–lidar (upstream CDonosoK, MIT): intrinsics z szachownicy, zapis zsynchronizowanych par obraz+chmura, ręczny wybór odpowiadających sobie punktów, wyznaczenie macierzy lidar→kamera i podgląd rzutu chmury na obraz.

## Węzły
Executables z `setup.py`:
- `get_intrinsic_camera_calibration` (`get_intrinsic_camera_calibration.py`) — subskrybuje `camera.image_topic` z configu (`/output_zed`), kalibracja z szachownicy 7×9 → `camera_intrinsic_calibration.yaml`.
- `save_data` (`save_sensor_data.py`) — subskrybuje `/lidar_points` i republikuje z nowym stemplem na `/output_lidar_points`; synchronizuje `/zed/zed_node/left/image_rect_color` z `lidar.lidar_topic`, zapisuje pary `.png` + `.pcd` do `data_folder` (max `max_file_saved`).
- `extract_points` (`extract_points.py`) — okno Open3D „Select points on the cloud” (1280×720) + wybór punktów na obrazie → plik korespondencji.
- `get_extrinsic_camera_calibration` (`get_extrinsic_camera_calibration.py`) — z pliku korespondencji i intrinsics liczy `extrinsic_matrix`; ścieżki na sztywno `/home/solar/fusion_ws/src/ros2_camera_lidar_fusion/{data,config}/` (`:22-24`).
- `lidar_camera_projection` (`lidar_camera_projection.py`) — subskrybuje `/lidar_points` → publikuje `/output_lidar_points` (stempel = now); subskrybuje `/zed/zed_node/left/image_rect_color` → publikuje `/output_zed` (stempel = now); `ApproximateTimeSynchronizer(queue 5, slop 0.07)` na `/output_zed` + `/output_lidar_points` → rzut (`T_lidar_to_cam`, `cv2.projectPoints`) → `/fusion_projection` (`Image`, depth 1).

## Launch
Brak.

## Konfiguracja (yaml)
`config/general_configuration.yaml` (opis w [[fusion_camera_lidar]]), `config/camera_extrinsic_calibration_24_01.yaml` (macierz 4×4, translacja ok. (0,035; −0,134; −1,029)).

## Zależności
`package.xml` — tylko test_depend; w kodzie: `rclpy`, `message_filters`, `cv_bridge`, OpenCV, numpy, Open3D, PyYAML.

## Interfejsy
Brak.

## Różnice między branchami
Jedna gałąź `main`.

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.
