---
typ: solar-pakiet
repo: "[[l1od]]"
build: ament_python
tags: [solar, pakiet]
zaktualizowano: 2026-10-04
---
# vision_detector

> [!skrot] W 30 sekund (co robi, wejście → wyjście)
> Detektor obiektów (boje, doki, panele, przeszkody) na obrazie z kamery ZED. Model YOLO ładowany przez bibliotekę **Ultralytics** (`from ultralytics import YOLO`, `model.py:6`), preferowany silnik **TensorRT** (`.engine`), potem `.pt` (eksport do TensorRT FP16), na końcu `.onnx`. Dla każdego bboxa liczy pozycję 3D z chmury punktów ZED. Wejście: `/zed/zed_node/rgb/color/rect/image` + `/zed/zed_node/point_cloud/cloud_registered` → wyjście: `/l1/detection_<nazwa_executable>` (`l1_interfaces/VectorOfDetections`). Uruchamiany zwykle w Dockerze (`docker/run.sh`).

## Węzły
Jedna klasa `Detector(Node)` (`vision_detector/detector.py:41`), uruchamiana w wariantach (executable z `setup.py` → funkcja w `nodes.py` → `main_(DetectorVariant, nazwa)`):

| Executable | Wariant (`detector_config.py`) | Zmienna z modelem | Domyślny próg |
|---|---|---|---|
| `buoy_detector` | `BUOY_DETECTOR` | `BUOY_MODEL_PATH` | 0.7 |
| `buoy_detector_simulation` | `BUOY_DETECTOR_SIMULATION` | `BUOY_SIMULATION_MODEL_PATH` | 0.8 |
| `celka_slalom_gopro_detector` | `CELKA_SLALOM_GOPRO_DETECTOR` | `CELKA_SLALOM_GOPRO_MODEL_PATH` | 0.6 |
| `celka_taxi_gopro_detector` | `CELKA_TAXI_GOPRO_DETECTOR` | `CELKA_TAXI_GOPRO_MODEL_PATH` | 0.6 |
| `celka_taxi_detector` | `CELKA_TAXI_DETECTOR` | `CELKA_TAXI_MODEL_PATH` | 0.6 |
| `celka_all_detector` | `CELKA_ALL_DETECTOR` | `CELKA_ALL_MODEL_PATH` | 0.6 |
| `base_detector` | `BASE_DETECTOR` | `BASE_DETECTOR_MODEL_PATH` | 0.5 |
| `pub_test_imgs` | — (`pub_test_imgs/im_pub.py`) | — | — |

Nazwa węzła = nazwa executable, więc topik wyjściowy to np. `/l1/detection_buoy_detector`.

### Detector (stan `refined-fusion`)
- **Subskrybuje** (przez `message_filters.Subscriber`, QoS: KEEP_LAST depth 1, BEST_EFFORT, VOLATILE — `detector.py:33-38`):
  - `/zed/zed_node/rgb/color/rect/image` (`sensor_msgs/Image`)
  - `/zed/zed_node/point_cloud/cloud_registered` (`sensor_msgs/PointCloud2`)
  - obie przez `TimeSynchronizer` (DOKŁADNA zgodność stempli), `queue_size = sync_queue_size` (`detector.py:103-111`).
- **Publikuje:**
  - `/l1/detection_<nazwa>` — `l1_interfaces/VectorOfDetections`, depth 5 (`detector.py:119`); header = stempel i `frame_id` obrazu.
  - `/l1/visualization/bb_320_compressed_<nazwa>` — `sensor_msgs/Image` (mimo nazwy NIE `CompressedImage`), 320×320 z narysowanymi bboxami, RELIABLE depth 5, tylko gdy ktoś subskrybuje (`detector.py:130-134, 243-254`); `frame_id` = `camera_det_frame`. Wersja 640×640 zakomentowana („Turned off for dashboard in norway”, `detector.py:256`).
- **Parametry:** `confidence_threshold` (domyślnie z wariantu; jeśli 0.0 → domyślny wariantu, `detector.py:74`), `camera_fov` (z env `CAMERA_FOV`; w kodzie refined-fusion nie jest już używany do obliczeń), `sync_queue_size` = 10 (musi być > 0).
- **Logika callbacku** `synchronized_detector_callback` (`detector.py:136-266`):
  1. `cv_bridge` → BGR8.
  2. `Model.postprocess()` → `YOLO(image, conf=próg)`, lista `Detection(detection_str, bbox, confidence, area)` (`model.py:81-108`).
  3. Jeśli są detekcje — zero-copy widok `PointCloud2` jako tablica (H, W) pól x,y,z (`detector_utils.py:18-78`).
  4. Dla każdej detekcji `mk_detection_msg`: `detection_class`/`detection_type` z `DetectorMapper` (słownik nazwa klasy → stała z `Detection.msg`; 80 klas COCO mapowanych na `OBSTACLE`), `heading_diff` = **środek bboxa w pikselach (x)**, `area`, oraz `x,y,z,distance` z `calculate_coordinates`.
  5. `calculate_coordinates` (`detector_utils.py:81-167`): środek bboxa przeskalowany do rozdzielczości chmury, okno ±10 px (21×21), odrzuca NaN/inf i punkty z `x ≤ 0.3 m`, bierze punkt o medianie `x` (`np.argpartition`), zwraca `(x, -y, z)` — czyli z układu ZED (FLU) robi FRU (x przód, y w prawo). Brak danych → `x=y=z=0`, `distance=-1`.
  6. Log czasu przetwarzania (`Vision Detection time`).

### Ładowanie modelu (`model.py:40-78`, refined-fusion)
- `torch.cuda.is_available()` → `device=0` (GPU) albo `'cpu'`.
- Z `BUOY_MODEL_PATH` itp. bierze nazwę bez rozszerzenia i sprawdza kolejno: `<nazwa>.engine` → ładuje TensorRT; `<nazwa>.pt` → `model.export(format='engine', device, half=True)` i ładuje powstały `.engine`; `<nazwa>.onnx` → ładuje ONNX. Inaczej `FileNotFoundError`.
- Na `master` (`55ded22` „Refine running models on CPU.”) logika rozdzielona: na CPU kolejność `.pt` → `.onnx`, a `.engine` na CPU rzuca `ValueError`.
- Lokalnie istnieje `model/njord26/navier30k.engine`, a `env_setup.sh` (refined-fusion) wskazuje `navier30k.pt` → w praktyce ładowany jest silnik TensorRT.

### Modele (`vision_detector/model/`, ~1,4 GB)
Tylko inwentaryzacja nazw/rozmiarów, bez czytania wag.
- `njord26/navier30k.{pt,onnx}` (+ lokalny `.engine`) — **aktualny** `BUOY_MODEL_PATH` na refined-fusion.
- `RB26/` — `roboboat.pt`, `roboboat_training.{pt,onnx}`, `roboboat_new_lights.{pt,onnx}`, `roboboat_lights_plackards.pt` (+ lokalne `.onnx`, `.engine`) — `BUOY_MODEL_PATH` na `master`.
- `RB25/real/*.onnx` (`24_nav_ftp_speed_predock`, `blue_black(_nano)`, `ftp(_nano)`, `all_5_epoch`), `RB25/simulation/` (`simulation.pt`, `merged_objects_*-yolov10.onnx`).
- `Bagry/` (boje „bagry”, m.in. `bagry_buoys.{pt,onnx}` + lokalny `.engine`, `nano_buoys.onnx`, `obstacle_bagry_07_20.onnx`).
- `Celka/` (Monako: `monaco_green`, `monaco_zlote_gacie`, `monaco-98fcj_*`, `ai_taxi_2_gopro_taxi`, `slalom-pu3ar_3_old_photos`, `biala-boja_*`).
- `njord/` (+ `class_list_*.txt`: `ar_tag`, `njord_buoys`, `vehicle`, symulacja: `all`, `cardinals`, `colors`, `docking_simulation`), `njord25/navier_1_alpha.onnx`.
- `roboboat/` — modele `yolov8n` (docking, nav_and_follow, octagon, speed_challenge, sim) z `*_class_list.txt`.
- `swimming_pool_detector/` (`yolov8n_swimming_pool_320.onnx` itd.), `base_models/base_yolov10{n,s,m}.onnx`, `yolo11n.pt`, `yolo11s.pt` (+ lokalne `.engine`/`.onnx`).
- README: modele „based on yolov10 architecture (trained in cnn-models repo)” — nazwy plików wskazują też YOLOv8n i YOLO11. **Niepewne:** architektura i klasy `navier30k` (nie czytano wag).

## Launch
Pakiet nie ma własnych launchy. Uruchamiany:
- przez `docker/run.sh <executable> [CB=1] [CT=0.7] [CTR=from:=to] [DTR=from:=to]` → `entrypoint.sh` → `ros2 run vision_detector <executable> --ros-args -p confidence_threshold:=<CT> [--remap CTR] [--remap DTR]`; `CB=1` robi `colcon build --packages-select l1_interfaces vision_detector` w kontenerze; `run.sh` najpierw zatrzymuje działające kontenery z obrazu `vision_detector_image:latest`;
- z [[l1_launch]] (`ExecuteProcess` uruchamia `run.sh`), w symulacji z remapami na `/wamv/sensors/cameras/front_left_camera_sensor/{image_raw,points}`.

### Docker
| Plik | Baza | Kluczowe |
|---|---|---|
| `Dockerfile_jetson` | `nvcr.io/nvidia/l4t-jetpack:r36.2.0` | ROS 2 Humble desktop, torch `2.2.0a0+6a974be.nv23.11` (`jp/v60dp`), torchvision 0.18.0a0, `ultralytics<8.3.237`, `onnxruntime_gpu-1.18.0` (wheel z kontekstu budowania), CycloneDDS `cyclonedds_lo.xml`, `ROS_DOMAIN_ID=69` |
| `Dockerfile_gpu` | `nvidia/cuda:12.1.1-runtime-ubuntu22.04` | cuDNN 9.11, torch 2.3.0, `tensorrt==10.0.1`, `ultralytics==8.2.103`, `ROS_DOMAIN_ID=68` |
| `Dockerfile_cpu` | `ros:humble` | `onnxruntime`, buduje `vision_detector l2_interfaces` (błąd: powinno być `l1_interfaces`) |

`run.sh` dla JETSON: `--network host --runtime=nvidia --ipc host --pid host`, montuje `$DEV_WS_DIR/src` jako `/ros2_ws/src`.

## Konfiguracja (yaml)
Brak yaml; konfiguracja przez zmienne środowiskowe z `l1od/env_setup.sh` (źródłowany w `entrypoint.sh`) i parametry ROS.

## Zależności
`package.xml`: `rclpy`, `std_msgs`, `sensor_msgs`, `onnxruntime`, `l1_interfaces` ([[ros-sbt-interfaces]]), `message_filters`. `setup.py install_requires`: `cv_bridge`, `matplotlib`, `numpy`, `onnx==1.16.1`, `opencv_python==4.10.0.84`, `pandas`, `seaborn` (Ultralytics/torch instalowane w Dockerfile). Odbiorcy: [[detection_pointcloud_fusion]], [[detection_fusion]], [[detection_fusion_projection]].

## Interfejsy
Brak własnych. Używa `l1_interfaces/msg/Detection` (pola: `detection_class`, `detection_type`, `detection_string`, `confidence`, `bbox_x0/x1/y0/y1`, `heading_diff`, `distance`, `area`, `x`, `y`, `z`) i `VectorOfDetections` (`header`, `detections[]`).

## Różnice między branchami
- `master`: osobne subskrypcje obrazu i chmury (depth 1, bez synchronizacji), publikuje dodatkowo `/l1/visualization/bb_<nazwa>`; pozycja liczona z ostatniej odebranej chmury.
- `refined-fusion`: `TimeSynchronizer` obraz+chmura, zero-copy, mediana ROI (`3820f37` „Optimize zed pointcloud handling”).
- `vlm-detection`: `model.py` zamieniony na VLM `nvidia/LocateAnything-3B` (transformers), kategorie i prompt z env.
- `dbscan_clustering`/`zed_exposure_controller`: usunięte `docker/inference.py`, dodany `plotter/plotter.py`, zmiany w Dockerfile.

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.
