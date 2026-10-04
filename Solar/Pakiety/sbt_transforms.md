---
typ: solar-pakiet
repo: "[[Repozytoria/sbt_transforms]]"
build: ament_cmake
tags: [solar, pakiet]
zaktualizowano: 2026-10-04
---
# sbt_transforms

> [!skrot] W 30 sekund (co robi, wejście → wyjście)
> Biblioteka konwencji osi i trzy węzły. `sbt_tf_bridge` robi z `/odom` drzewo TF. `sbt_axis_normalizer` republikuje topiki w zadanej konwencji. `sbt_axis_audit` wykrywa w czasie działania odbite lub obrócone dane. Repo opisuje notatka [[Repozytoria/sbt_transforms|repo sbt_transforms]] w `Repozytoria/`, a tło [[Drzewo TF i układy współrzędnych]].

## Węzły
### sbt_tf_bridge
- `src/tf_bridge_node.cpp`, executable `sbt_tf_bridge`.
- **Subskrybuje:** `odom.topic` = `/odom` (`nav_msgs/msg/Odometry`, `SensorDataQoS`) (`:76-78`).
- **TF statyczne** (`publishStaticTransforms`, `:190-211`):
  - `base_link → hesai_lidar` (xyz z `extrinsics.lidar_xyz`, obrót z konwencji `RFU`, czyli −90° yaw);
  - `base_link → zed_left_camera_frame` (`camera_xyz`, `FLU`);
  - `zed_left_camera_frame → zed_left_camera_optical_frame` (quat `(0.5,−0.5,0.5,−0.5)` wg README).
- **TF dynamiczne** przy każdej `/odom` (`:235-252`): `map → odom` (identyczność, gdy `publish_map_to_odom`) i `odom → base_link`. Pozycja NEU jest zamieniana na ENU (`x=East`, `y=North`), orientacja NWU na yaw ENU.
- Wynik zweryfikowany w README dla łodzi N=10, E=5, kurs 090: `odom→base_link xyz=(5,10,0) enu_yaw=0`.
- **Parametry:** `odom.topic`, `odom.position_convention` (`NEU`), `odom.orientation_convention` (`NWU`), `frames.*`, `extrinsics.lidar_xyz`, `camera_xyz`, `lidar_convention` (`RFU`), `camera_convention` (`FLU`), `publish_map_to_odom` (`true`).

### sbt_axis_normalizer
- `src/axis_normalizer_node.cpp`, executable `sbt_axis_normalizer`.
- Parametr `streams` (lista nazw), a dla każdego strumienia: `type`, `in_topic`, `out_topic`, `in_convention` (domyślnie `FRU`), `out_convention` (`FLU`), `in_yaw` (`COMPASS`), `out_yaw` (`ENU_CCW`), `to_world`, `out_frame_id`.
- Obsługiwane typy (`:227-364`): `VectorOfFusedDetections`, `VectorOfClusters`, `VectorOfDetections`, `TrackedDetectionsVector`, `Odometry`, `PoseArray`, `Path`.
- Subskrypcja `/odom` służy do `to_world`. Wejścia mają `SensorDataQoS`, wyjścia depth 10.
- Strumienie z YAML:

| strumień | typ | wejście → wyjście | konwencja |
|---|---|---|---|
| `fused_to_rep103` | `VectorOfFusedDetections` | `/fused_detection` → `/canonical/fused_detection` | FRU → FLU, `base_link` |
| `fused_to_world` | `VectorOfFusedDetections` | `/fused_detection` → `/canonical/fused_detection_map` | FRU → ENU, `to_world`, `map` |
| `clusters_to_rep103` | `VectorOfClusters` | `/l1/obstacle_detection` → `/canonical/obstacle_detection` | RFU → FLU |
| `tracked_from_slam` | `TrackedDetectionsVector` | `/slam/tracked_detections_raw` → **`/tracked_detections`** | ENU → NEU, `map_neu` |
| `odom_to_rep103` | `Odometry` | `/odom` → `/canonical/odom` | NEU/COMPASS → ENU/ENU_CCW |

- `tracked_from_slam` zakłada, że `l2dp/pose_graph` zostanie przepięty na `/slam/tracked_detections_raw`. Bez tego normalizer byłby **trzecim** publisherem `/tracked_detections`.

### sbt_axis_audit
- `src/axis_audit_node.cpp`, executable `sbt_axis_audit`.
- **Subskrybuje** (`SensorDataQoS`):
  - `/odom` (`nav_msgs/msg/Odometry`);
  - `/true_heading/data` (`l0_interfaces/msg/Course`);
  - `/tracked_detections` (`l2_interfaces/msg/TrackedDetectionsVector`);
  - `/fused_detection` (`l1_interfaces/msg/VectorOfFusedDetections`).
- Wykrywa (komentarz `:1-15`):
  1. wielu producentów jednego topiku w odbitych konwencjach;
  2. kurs niezgodny z COG o ~180° (brak minusa) albo ~90° (ENU zamiast kompasu);
  3. wysoki odsetek `is_fused == false` / detekcje po złej burcie;
  4. przypomnienie o `frame_id` bez TF (tylko log).
- Parametry: `heading_tolerance_deg=15`, `min_speed_for_course_check=0.8`, `report_period_s=10`.
- Raport co 10 s.

## Launch
`launch/sbt_transforms.launch.py`: argumenty `params_file` (domyślnie `share/sbt_transforms/config/sbt_transforms.yaml`), `enable_audit` (`true`), `enable_normalizer` (`true`). Uruchamia trzy węzły z tymi samymi parametrami, bez remapów i namespace.

## Konfiguracja (yaml)
- `config/sbt_transforms.yaml`: patrz wyżej i [[Repozytoria/sbt_transforms|repo]].
- `config/frames.yaml`: dokumentacja w formie YAML (nie parametry). Dla każdego topiku: producent, konwencja, yaw, `status: ok|mislabelled|inconsistent|broken`. Przykłady:
  - `/imu/data`: NWU, `mislabelled`;
  - `/filter/velocity`: ENU, `ok`;
  - `/l1/detection`: FRU, `frame_id camera_det_frame`;
  - `/l1/obstacle_detection`: RFU, `hesai_lidar`;
  - `/odom`: `frame_id odom` „claims REP-105 odom, which is ENU”.

## Zależności
`rclcpp`, `geometry_msgs`, `nav_msgs`, `sensor_msgs`, `std_msgs`, `tf2`, `tf2_ros`, `tf2_geometry_msgs`, `l0_interfaces`, `l1_interfaces`, `l2_interfaces` (→ [[l0_interfaces]], [[l1_interfaces]], [[l2_interfaces]]); exec: `rclpy`, `python3-numpy`; test: `ament_cmake_gtest`, `ament_cmake_pytest`. Biblioteka eksportowana jako `INTERFACE` (`add_library(sbt_transforms INTERFACE)`).

## Interfejsy
Brak własnych.

## Różnice między branchami
Brak gita, więc brak gałęzi.

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.
