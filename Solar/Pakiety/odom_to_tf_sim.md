---
typ: solar-pakiet
repo: "[[l4sim]]"
build: ament_cmake
tags: [solar, pakiet]
zaktualizowano: 2026-10-04
---
# odom_to_tf_sim

> [!skrot] W 30 sekund (co robi, wejście → wyjście)
> Rozgłasza TF `odom → wamv/wamv/base_link` z `/odom2` (odometria symulatora z [[cpp_nodes]]) — dla RViz w symulacji.

Opis wg `~/solar/bash_do_uruchomienia/l4sim@main` (`7a789a2`), katalog `src/odom_to_tf`.

## Węzły
- `odom_to_tf_node` (`src/OdomToTf.cpp`, executable `OdomToTf`): subskrybuje `/odom2` (`nav_msgs/Odometry`), `tf2_ros::TransformBroadcaster` → `odom` → `wamv/wamv/base_link`.

## Launch
Brak.

## Konfiguracja (yaml)
Brak.

## Zależności
`rclcpp`, `nav_msgs`, `geometry_msgs`, `tf2_ros`. Katalog `src/odom_to_tf`.

## Interfejsy
Brak własnych.

## Różnice między branchami
Bez zmian.

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.
