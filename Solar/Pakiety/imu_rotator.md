---
typ: solar-pakiet
repo: "[[l4sim]]"
build: ament_cmake
tags: [solar, pakiet]
zaktualizowano: 2026-10-04
---
# imu_rotator

> [!skrot] W 30 sekund (co robi, wejście → wyjście)
> Obraca orientację IMU z Gazebo tak, by yaw=0 oznaczał północ (oś X – północ, Y – wschód, konwencja projektu).

Opis wg `~/solar/bash_do_uruchomienia/l4sim@main` (`7a789a2`), katalog `src/imu_rotator`.

## Węzły
- `imu_rotator` (executable `ImuRotator`, biblioteka `imu_rotator_lib`). Parametr `yaw_offset_deg` −90.0. Subskrybuje `/wamv/sensors/imu/imu/data`, publikuje `/wamv/sensors/imu/imu/data_rotated` (`sensor_msgs/Imu`).

## Launch
Brak; `ros2 run imu_rotator ImuRotator`.

## Konfiguracja (yaml)
Brak.

## Zależności
`rclcpp`, `sensor_msgs`, tf2 (**niepewne**).

## Interfejsy
Brak własnych.

## Różnice między branchami
Wprowadzony w `8f706d1` „Nodes rotating axes” (2025-11-12).

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.
