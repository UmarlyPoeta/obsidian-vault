---
typ: solar-pakiet
repo: "[[l4sim]]"
build: ament_cmake
tags: [solar, pakiet]
zaktualizowano: 2026-10-04
---
# course_imu

> [!skrot] W 30 sekund (co robi, wejście → wyjście)
> Starszy konwerter IMU symulatora na kurs i prędkość jako `std_msgs/Float32` (`/sim/imu/course`, `/sim/imu/velocity`) — z czasów, gdy [[trajectory_planner]] czytał kurs z `/sim/imu/course` (zakomentowana stała w `trajectory_planner.hpp:53`).

Opis wg `~/solar/bash_do_uruchomienia/l4sim@main` (`7a789a2`), katalog `src/course_imu`.

## Węzły
- `CourseImuConverter` (executable `CourseImu`): subskrybuje `/wamv/sensors/imu/imu/data`, publikuje `/sim/imu/course`, `/sim/imu/velocity`.

## Launch
Uruchamiany w `l4_launch/launch/sim.yaml`.

## Konfiguracja (yaml)
Brak.

## Zależności
`rclcpp`, `sensor_msgs`, `std_msgs`.

## Interfejsy
Brak własnych.

## Różnice między branchami
Bez zmian.

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.
