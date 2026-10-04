---
typ: solar-pakiet
repo: "[[l2dp]]"
build: ament_cmake
tags: [solar, pakiet]
zaktualizowano: 2026-10-04
---
# odometry

> [!skrot] W 30 sekund (co robi, wejście → wyjście)
> Podstawowa odometria łodzi: filtr Kalmana o stanie `[x, y, vx, vy]` (metry względem punktu startu). Predykcja z prędkości Xsensa `/filter/velocity`, korekcja z GNSS `/gnss` (odległości haversine od pierwszego fixa), orientacja przepisana z `/imu/data`. Publikuje `/odom`, punkt startu `/start_pose` (lat/lon pierwszego fixa) i krótką predykcję toru `/trajectory`. `/odom` jest wejściem SLAM-u ([[pose_graph]]), który z niego robi `/slam/odom`.

Opis wg `l2dp_docking_fix@0d444ff`.

## Węzły
### `imu_odometry_node` (klasa `IMUOdometryNode`)
- Źródło: `odometry/src/OdoCpp.cpp`, `include/OdoCpp.h`, `src/KalmanFilter.cpp`, `include/KalmanFilter.h`, `src/main.cpp`. Executable `odo_cpp_node` (`CMakeLists.txt:23`, linkuje `kalman_lib`).
- **Subskrybuje:** `/imu/data` (`sensor_msgs/Imu`) → orientacja i prędkość kątowa; `/filter/velocity` (`geometry_msgs/Vector3Stamped`) → predykcja; `/gnss` (`sensor_msgs/NavSatFix`) → korekcja; `/gnss_pose` (`geometry_msgs/PoseStamped`) → pusty callback.
- **Publikuje:** `/odom` (`nav_msgs/Odometry`, frame `odom`) — w obu callbackach (prędkości i GNSS), więc częstotliwość = suma obu; `/start_pose` (`geometry_msgs/Point`: x = lat, y = lon pierwszego fixa); `/trajectory` (`geometry_msgs/PoseArray`, frame `map`, 10 punktów łuku z bieżącej prędkości i `angular_velocity.z`).
- **Logika:**
  - `velCallback`: `vx = vector.y`, `vy = vector.x` (zamiana osi, `OdoCpp.cpp` w `velCallback`), krok predykcji z `dt` od ostatniej próbki, szum procesu z kowariancją 0.05.
  - `gpsCallback`: pierwszy fix → `startingGlobalPose`; pomiar `x` = odległość haversine wzdłuż szerokości (północ), `y` = wzdłuż długości (wschód), znak wg kierunku; `R = diag(0.005, 0.005)`; krok aktualizacji.
  - `imuCallback`: orientacja przepisana do `/odom` bez zmian; kurs liczony `-atan2(...)` w stopniach (tylko log i `/trajectory`).
  - Pozycja w `/odom` jest w układzie x = północ, y = wschód; orientacja to kwaternion z Xsensa (zob. opis osi w `~/solar/docs/architektura.html`, rozdz. 9 — **niepewne**, nie weryfikowano sterownika Xsens).

## Launch
Brak; `ros2 run odometry odo_cpp_node` (README [[l4sim]]).

## Konfiguracja (yaml)
Brak. Kowariancje na sztywno (0.05; GNSS 0.005).

## Zależności
`package.xml` deklaruje tylko `ament_cmake` i testy — brak `depend` na `rclcpp`, `nav_msgs`, `sensor_msgs` itd. (budowanie opiera się na `find_package` w `CMakeLists.txt`). Eigen3. Dane z Xsens MTi (sterownik poza repo; [[l0hwd]]). Odbiorcy `/odom`: [[pose_graph]], [[mapper]], [[obstacle_avoidance]], [[basic_controller]] (wiele węzłów BT), [[message_protobuf]]/[[ascii_map_viz2]].

## Interfejsy
Brak.

## Różnice między branchami
- `origin/celka_odo` (Monako 2026): obsługa SBG — subskrypcje `/sbg/gps_hdt` (`SbgGpsHdt`, status 64 = dobry kurs), `/sbg/gps_pos`, `/sbg/gps_vel`, `/sbg/ekf_quat` (fallback, gdy HDT zły), zależność `sbg_driver` (+215 linii). Linia Njord tego nie zawiera i używa Xsensa.
- `master` vs gałąź zawodów: `git diff --stat origin/master feat/dock_costmap_fix -- odometry` — brak różnic.

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.
