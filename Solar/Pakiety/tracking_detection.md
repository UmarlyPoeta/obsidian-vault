---
typ: solar-pakiet
repo: "[[l1od]]"
build: ament_cmake
tags: [solar, pakiet]
zaktualizowano: 2026-10-04
---
# tracking_detection

> [!skrot] W 30 sekund (co robi, wejście → wyjście)
> Nadaje stałe ID obiektom: przelicza pozycję detekcji z układu łodzi do globalnego (pozycja i kurs z `/odom`) i jeśli w promieniu `tracking_tolerance` jest już zarejestrowany obiekt — używa jego ID. Wejście: `/fused_detection` + `/odom` (+ `/imu/data`) → `/tracking_detection` + `/tracking_visualization`. W `~/solar` nikt nie subskrybuje `/tracking_detection`.

## Węzły

### TrackingDetection
`tracking_detection/src/tracking_detection.cpp`, executable `TrackingDetection`.
- **Subskrybuje:**
  - `/fused_detection` — `l1_interfaces/VectorOfFusedDetections`, depth 10
  - `/odom` — `nav_msgs/Odometry` (pozycja x,y i yaw z kwaternionu, `:90-94`)
  - `/imu/data` — **`std_msgs/Float32`** (kurs w stopniach → rad, `:96-98`). Typ niezgodny z `sensor_msgs/Imu`, który publikuje reszta systemu (np. [[l2dp]] `odometry` subskrybuje `/imu/data` jako IMU) — subskrypcja się nie połączy.
- **Publikuje:** `/tracking_detection` (`VectorOfFusedDetections`), `/tracking_visualization` (`MarkerArray`, `frame_id` `wamv/wamv/base_link`, tekst 60 s, kula 600 s, x↔y zamienione).
- **Parametry:** `tracking_tolerance 1.5` [m] — deklarowany PO rejestracji subskrypcji (`:8-11`).
- **Logika** (`DetectionCallback`, `:38-70`): przepuszcza tylko detekcje z `is_fused_angle == true`; `object_global_from_local` = obrót o yaw + przesunięcie; nowy obiekt → `id = liczba_obiektów + 1`, zapis do `tracked_objects`; znany → log. Uwaga: `detection.id` ustawiane tylko dla nowych obiektów; dla znanych zostaje 0. `object_global_from_local` zwraca `new double[2]` bez `delete` (wyciek pamięci).
- Ponieważ [[detection_pointcloud_fusion]] nie ustawia `is_fused_angle`, na gałęzi `refined-fusion` węzeł publikuje puste wektory (wniosek z kodu).

## Launch
- `master`: `l1_launch/launch/l1od_launch.py` (parametr `tracking_tolerance`).
- `l1od.yaml`, `sim.yaml` (oba branche). Na `refined-fusion` `l1od_launch.py` go nie uruchamia.

## Konfiguracja (yaml)
Brak.

## Zależności
`rclcpp`, `std_msgs`, `nav_msgs`, `tf2`, `tf2_geometry_msgs`, `l1_interfaces`, `visualization_msgs`. `/odom` z [[l2dp]] (`odometry/src/OdoCpp.cpp:38`).

## Interfejsy
Brak własnych; `include/tracked_object.hpp`: struct `TrackedObject {id, detection_type, x_global, y_global}`.

## Różnice między branchami
Brak zmian w kodzie pakietu w diffach gałęzi.

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.
