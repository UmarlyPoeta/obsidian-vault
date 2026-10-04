---
typ: solar-pakiet
repo: "[[l2dp]]"
build: ament_cmake
tags: [solar, pakiet]
zaktualizowano: 2026-10-04
---
# obstacle_avoidance

> [!skrot] W 30 sekund (co robi, wejście → wyjście)
> Starsza, równoległa implementacja wykrywania kolizji — praktycznie ta sama logika „tunel własnej trajektorii vs ekstrapolowany tor obiektu” co [[colision_detector]], ale na danych sprzed SLAM: `/trajectory` z [[odometry]], `/l1/obstacle_detection` z [[l1od]], `/odom`. Publikuje na **ten sam** topik `/colision_detector` i `/tunnel_marker` — uruchomienie obu naraz da dwóch publisherów.

Opis wg `l2dp_docking_fix@0d444ff` (pakiet nie zmieniony na linii Njord).

## Węzły
### `obstacle_avoidance_node` (klasa `ObstacleAvoidanceNode`)
- Źródło: `obstacle_avoidance/src/obstacle_avoidance.cpp`, `src/main.cpp`; executable `obstacle_avoidance` budowany tylko z `src/main.cpp` (`CMakeLists.txt:25`).
- **Subskrybuje:** `/l1/obstacle_detection` (`l1_interfaces/VectorOfClusters`), `/trajectory` (`geometry_msgs/PoseArray`), `/odom` (`nav_msgs/Odometry`) (`obstacle_avoidance.cpp:12–27`).
- **Publikuje:** `/tunnel_marker` (`visualization_msgs/Marker`), `/colision_detector` (`geometry_msgs/Point`).
- `TrajectoryCallback` (`:41`): tunel 0.5 m wokół `/trajectory` (bez zamiany x↔y, frame z wiadomości); `ClusterCallback` (`:107`): estymacja ruchu obiektu i przecięcie tuneli.
- `src/obstacle_avoidance_algorithm.cpp` (`ComputeSubtarget` — wyznaczanie punktu pośredniego omijania) i `include/obstacle_avoidance_algorithm.hpp`: kod obecny w repo, **niepewne** czy linkowany (nie występuje w `add_executable`); zawiera komentarz „//USUN TO BLAGAM!!!!1111” przy globalnym `rclcpp::Node* nodePtr` (`:5–6`).

## Launch
Brak.

## Konfiguracja (yaml)
Brak.

## Zależności
`rclcpp`, `rclpy`, `nav_msgs`, `std_msgs`, `visualization_msgs`, `geometry_msgs`, `l1_interfaces`; Boost.Geometry. README pakietu to szablon GitLaba („# Odometry”).

## Interfejsy
Brak.

## Różnice między branchami
Na `origin/formatting` i `origin/temp_` — tylko formatowanie. Na `origin/mapper_sim_l2_divided_fix` — poprawki includów po podziale interfejsów (2025-11). Martwy kod względem stanu zawodów.

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.
