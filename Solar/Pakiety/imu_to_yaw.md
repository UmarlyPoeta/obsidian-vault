---
typ: solar-pakiet
repo: "[[l4sim]]"
build: ament_cmake
tags: [solar, pakiet]
zaktualizowano: 2026-10-04
---
# imu_to_yaw

> [!skrot] W 30 sekund (co robi, wejście → wyjście)
> Liczy kurs z kwaternionu IMU symulatora i publikuje go jak sterownik Xsens: `/filter/euler` (`Vector3Stamped`, `vector.z` = yaw w stopniach, konwencja ENU).

Opis wg `~/solar/bash_do_uruchomienia/l4sim@main` (`7a789a2`), katalog `src/imu_to_yaw`.

## Węzły
- `imu_to_yaw` (`src/imu_to_yaw.cpp`): subskrybuje `/wamv/sensors/imu/imu/data`, publikuje `/filter/euler`; `yaw = atan2(...)` w stopniach, `vector.x = vector.y = 0`.

## Launch
Brak; `ros2 run imu_to_yaw imu_to_yaw`.

## Konfiguracja (yaml)
Brak.

## Zależności
`rclcpp`, `sensor_msgs`, `geometry_msgs`. Odbiorcy `/filter/euler`: [[regulator]], [[message_protobuf]].

## Interfejsy
Brak własnych.

## Różnice między branchami
Bez zmian.

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.
