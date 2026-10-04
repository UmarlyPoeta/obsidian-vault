---
typ: solar-pakiet
repo: "[[l4sim]]"
build: ament_cmake
tags: [solar, pakiet]
zaktualizowano: 2026-10-04
---
# l4_launch

> [!skrot] W 30 sekund (co robi, wejście → wyjście)
> Pakiet z plikiem uruchomieniowym węzłów L4 w kontenerze i konfiguracją mostu Gazebo↔ROS dla kamery.

Opis wg `~/solar/bash_do_uruchomienia/l4sim@main` (`7a789a2`), katalog `src/l4_launch`.

## Węzły
Brak własnych węzłów.

## Launch
- `launch/sim.yaml` (launch YAML): `sim_can_adapter/SimCanAdapter` (name `SimCanAdapter`) i `course_imu/CourseImu` (name `CourseImu`). Wywoływany przez `src/start-simulation-test.sh` po starcie VRX: `ros2 launch l4_launch sim.yaml`.

## Konfiguracja (yaml)
- `config/bridge_config.yaml`: 2 mapowania ros_gz_bridge — `/wamv/sensors/cameras/front_left_camera_sensor/image_raw` (`Image`) i `…/points` (`PointCloud2`), QoS best_effort (kamera głębi z gałęzi `depth-camera`).

## Zależności
`launch`, `launch_ros` (**niepewne**).

## Interfejsy
Brak własnych.

## Różnice między branchami
Dodane w gałęzi `depth-camera` (zmergowana 2026-05-25).

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.
