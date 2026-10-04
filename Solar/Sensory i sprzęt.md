---
typ: solar-przeglad
tags: [solar, sprzet]
zaktualizowano: 2026-10-04
---
# Sensory i sprzęt

> [!skrot] W 30 sekund
> - **Kamera:** Stereolabs ZED (w kodzie L1 „zed2i”), wrapper [[zed-ros2-wrapper-humble-v5.1.0]] → `/zed/zed_node/rgb/color/rect/image`, `/zed/zed_node/point_cloud/cloud_registered`; watchdog [[zed-service]].
> - **Lidar:** Hesai (model nieustalony), driver `hesai_ros_driver` dołączany z [[l0hwd]] → `/lidar_points` (~10 Hz w rosbagu), frame `hesai_lidar`, montaż obrócony o −90° (RFU).
> - **INS/GNSS/IMU:** Xsens (topiki `/filter/*`, `/imu/data`, `/gnss`), wcześniej SBG; sterownika nie ma w `~/solar`.
> - **Echosonda:** Echologger ECT400 po UART — pakiet [[sonar]] w [[l0hwd]].
> - **Komputer:** NVIDIA Jetson Orin (host `jetson`, użytkownik `solar`), obrazy Docker na L4T r36.2.0 (JetPack 6.0 DP). Jest też Jetson Nano.

## Kamera ZED
- **Model:** w [[l1od]] wielokrotnie „zed2i” (`README.md`: „Processes data from zed2i camera”; `env_setup.sh`: „1.76278254 - (101 degrees, new zed2i)”; `detection_fusion.hpp:28`: „1.4825 - (110 degrees, new zed2i ACTUAL CAMERA FOV)”; README `images_collector`: „default ZED2i resolution”). Watchdog [[zed-service]] domyślnie startuje `camera_model:=zedx` (`zed-pls-dont-die.sh:41`) — **niepewne**, który model jest obecnie na łodzi. Komentarze wspominają też „old zed” z FOV 72° (`env_setup.sh`, `detection_pointcloud_fusion.hpp:45-51`).
- **Sterownik:** `zed_wrapper` 5.1.0 (Stereolabs, wymaga ZED SDK 5.1). Konfiguracja w `zed2i.yaml`: grab HD1080 @ 15 FPS; `common_stereo.yaml`: publikacja przeskalowana ×0,5 (960×540), 15 Hz, chmura 10 Hz, `depth_mode NEURAL_LIGHT`. Intrinsics w kodzie fuzji (z `camera_info`): fx=fy=362,97, cx=486,27, cy=274,10 dla 960×540 (`detection_pointcloud_fusion.hpp:38-43`).
- **Topiki surowe używane w systemie:** `/zed/zed_node/rgb/color/rect/image` (`sensor_msgs/Image`), `/zed/zed_node/point_cloud/cloud_registered` (`sensor_msgs/PointCloud2`), `/zed/zed_node/rgb/color/rect/camera_info` (gałąź `auto_fov_camera`), serwis `/zed/zed_node/set_parameters` ([[zed_controller]]).
- **Obsługujące pakiety:** [[vision_detector]], [[images_collector]], [[fusion_visualization]], [[zed_controller]], skrypty [[agh-dashboard-live]] (stream RTSP), [[fusion_camera_lidar]] (stara nazwa `left/image_rect_color`).
- **Kalibracja:** `l1od/config/camera_intrinsic_calibration.yaml` (2025-01-22, obraz 1280×720), `l1od/config/camera_extrinsic_calibration.yaml` („dzien przed testami barki 8 luty”), `fusion_camera_lidar/config/camera_extrinsic_calibration_24_01.yaml`.
- **Montaż:** kamera 11 cm pod lidarem (`detection_pointcloud_fusion.cpp:148`, `y_cam = -z_lidar - 0.11f`). [[sbt_transforms]] (`README.md`): `base_link → zed_left_camera_frame` z=0,650 m, `base_link → hesai_lidar` z=0,780 m, yaw −90°.

## Lidar Hesai
- **Model:** nieustalony z plików — w `~/solar` nie ma kodu ani configu `hesai_ros_driver` (tylko include w `l0hwd/l0_launch/launch/l0hwd_launch.py:20-27`: `get_package_share_directory("hesai_ros_driver")`, `launch/start.py`).
- **Topik surowy:** `/lidar_points` (`sensor_msgs/PointCloud2`), w rosbagu 484 wiadomości / 48,4 s ≈ 10 Hz, QoS RELIABLE depth 100 ([[Rosbagi]]). Frame `hesai_lidar` (markery w [[obstacle_detector]], domyślny `frame_id` w [[detection_pointcloud_fusion]]).
- **Układ osi:** RFU — x w prawo, y do przodu, z w górę (`sbt_transforms/config/sbt_transforms.yaml:21`: „Hesai mounted 90 deg off: its +y is forward”). Stąd w [[obstacle_detector]] `y_axis_min = 0` odcina tył, a fuzja zamienia `x_robot = y_lidar`.
- **Obsługujące pakiety:** [[obstacle_detector]], [[detection_pointcloud_fusion]], [[fusion_visualization]], [[ros2_camera_lidar_fusion]], w L2: [[l2dp]] — `/lidar_points` występuje w `pose_graph/src/feature_extractor.cpp` i `pointcloud_to_laserscan/launch/sample_pointcloud_to_laserscan_launch.py`.

## INS / GNSS / IMU
- **Xsens:** [[agh-dashboard-live]] (`scripts/start_jetson_live.sh:62-70`) używa `/filter/positionlla`, `/filter/euler`, `/filter/velocity` (`geometry_msgs/Vector3Stamped`) i typu `xsens_mti_ros2_driver/msg/XsStatusWord` (status RTK) → driver `xsens_mti_ros2_driver`, którego nie ma w `~/solar`. [[sbt_transforms]] opisuje `/imu/data` jako kwaternion Xsensa względem NWU.
- `/imu/data` (`sensor_msgs/Imu`), `/filter/velocity` — subskrybuje [[l2dp]] `odometry` (`OdoCpp.cpp:25-28`) i [[l0hwd]] `regulator`; `/gnss` (`NavSatFix`) — `odometry` (`OdoCpp.cpp:32`), `message_protobuf`, dashboard.
- W [[l0hwd]] `can_driver` publikacje `/gnss` i `/imu/data` są zakomentowane („currently unused, will be used if INS through CAN again”, `can_driver.cpp:73-77`); ramki Xsens na CAN mają ID 0x20–0x34 (`l0hwd/can-msgs/README.md`). Szczegóły: [[can_driver]], [[l0hwd]].
- Starsza gałąź `sbg-info` w [[l0hwd]] używała SBG (`sbg_driver`).
- [[tracking_detection]] subskrybuje `/imu/data` jako `std_msgs/Float32` (niezgodny typ).

## Echosonda
Echologger ECT400 po porcie szeregowym (`/dev/ttyUSB5`, 115200, NMEA DPT/DBT) → `/echologger/depth` (`sensor_msgs/Range`), `/echologger/raw_echo` — pakiet [[sonar]] w [[l0hwd]]. Submoduł `echosounderapi-python` nieużywany.

## Komputer pokładowy (Jetson)
- **Jetson Orin:** `l1od/README.md` (`ssh solar@jetson`), `dongle-service/README.md` („Jetson Orin”, `solar@jetson`; jest też „Jetson Nano”, `solar@jetson-nano`), autor commita `JetsonOrin` w [[fusion_camera_lidar]].
- **JetPack / L4T:** `l1od/vision_detector/docker/Dockerfile_jetson:2` — `FROM nvcr.io/nvidia/l4t-jetpack:r36.2.0` (L4T R36.2 = JetPack 6.0 Developer Preview), torch z `developer.download.nvidia.com/compute/redist/jp/v60dp/` (`:71`), Python 3.10 (`cp310`, aarch64). To wersja obrazu kontenera; wersja L4T hosta nie wynika z plików — **niepewne**, czy host ma tę samą.
- **Inferencja:** TensorRT przez Ultralytics (`.engine` FP16 eksportowane z `.pt`), `onnxruntime_gpu 1.18.0` w obrazie Jetson ([[vision_detector]]).
- **Sieć ROS:** CycloneDDS, `ROS_DOMAIN_ID=69` ([[l1od]] Dockerfile, [[zed-service]]); kontener detektora ograniczony do `lo` (`cyclonedds_lo.xml`).

## Symulacja (WAM-V)
Odpowiedniki topików w symulacji (z [[l1_launch]]): kamera `/wamv/sensors/cameras/front_left_camera_sensor/{image_raw,points}`, lidar `/wamv/sensors/lidars/lidar_wamv_sensor/points`, GPS `/wamv/sensors/gps/gps/fix`, IMU `/wamv/sensors/imu/imu/data`.

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.
