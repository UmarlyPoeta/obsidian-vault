---
typ: solar-pakiet
repo: "[[l4sim]]"
build: ament_cmake
tags: [solar, pakiet]
zaktualizowano: 2026-10-04
---
# cpp_nodes

> [!skrot] W 30 sekund (co robi, wejście → wyjście)
> Pakiet z katalogu `src/new_odometry` (nazwa w `package.xml`: `cpp_nodes`): symulacyjna odometria `/odom2` z IMU i GPS WAM-V oraz `vel_to_thrusters` — `/cmd_vel` (np. z [[boat_teleop]]) → ciągi pędników.

Opis wg `~/solar/bash_do_uruchomienia/l4sim@main` (`7a789a2`), katalog `src/new_odometry`.

## Węzły
- `imu_odometry_node` (executable `odo_cpp_node`): subskrybuje `/wamv/sensors/imu/imu/data`, `/wamv/sensors/gps/gps/fix`, `/clock`; publikuje `/odom2` (`nav_msgs/Odometry`) — nazwa i executable kolidują z [[odometry]] z [[l2dp]] (ten sam `odo_cpp_node`, inny pakiet).
- `vel_node` (executable `vel_to_thrusters`, `src/VelToThrusters.cpp`): subskrybuje `/cmd_vel` (`Twist`), publikuje `/wamv/thrusters/{left,right}/thrust`.

## Launch
Brak; README: `ros2 run cpp_nodes vel_to_thrusters` (sterowanie ręczne).

## Konfiguracja (yaml)
Brak.

## Zależności
`rclcpp`, `sensor_msgs`, `nav_msgs`, `geometry_msgs`, `rosgraph_msgs`, Eigen. Odbiorca `/odom2`: [[odom_to_tf_sim]].

## Interfejsy
Brak własnych.

## Różnice między branchami
Na `origin/dockerized_detections` (2025-11) zmiany w `src/new_odometry` — gałąź martwa.

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.
