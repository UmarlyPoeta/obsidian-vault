---
typ: solar-pakiet
repo: "[[ros-sbt-interfaces]]"
build: ament_cmake
tags: [solar, pakiet]
zaktualizowano: 2026-10-04
---
# l2_interfaces

> [!skrot] W 30 sekund (co robi, wejście → wyjście)
> Interfejsy planowania i mapy (L2, repo `l2dp`). Należą tu śledzone detekcje na mapie (`TrackedDetection(sVector)` na `/tracked_detections`), wykryty dok (`Dock`), pozycja metryczna z GPS, detekcja sygnałów dźwiękowych oraz serwisy `SetHome` i `WaypointNavigationState`, przez które L3 zleca jazdę do punktu.

## Węzły
Brak.

## Launch
Brak.

## Konfiguracja (yaml)
Brak.

## Zależności
`std_msgs`, `std_srvs`, `sensor_msgs`, `geometry_msgs`. Na tym pakiecie opiera się [[l3_interfaces]].

## Interfejsy
| plik | pola |
|---|---|
| `msg/Dock.msg` | `std_msgs/Header header`, `geometry_msgs/Point dock`, `geometry_msgs/Point safe` |
| `msg/MetricPosition.msg` | `std_msgs/Header header`, `float64 x`, `float64 y`, `float64 distance` |
| `msg/DetectionMapEntry.msg` | `int32 detection_type`, `float64 confidence_sum`, `int64 detection_count` |
| `msg/TrackedDetection.msg` | stałe jak w [[l1_interfaces]] (`Detection`); `l2_interfaces/DetectionMapEntry[] detection_type_map`, `int32 detection_type`, `int32 detection_class`, `float64 x`, `float64 y`, `int32 usage`, `int32 id`, `int32 spatial_points_size` |
| `msg/TrackedDetectionsVector.msg` | `std_msgs/Header header`, `l2_interfaces/TrackedDetection[] tracked_detections` |
| `msg/BlastDetection.msg` | `std_msgs/Header header`, `uint8 blast_type`, `uint32 frequency_hz`, `string zone` |
| `srv/SetHome.srv` | `float64 latitude`, `float64 longitude` → `bool if_home_changed` |
| `srv/WaypointNavigationState.srv` | `float64 x`, `float64 y`, `bool stay_in_this_wp_for_30s`, `float64 linear_speed_limit`, `float64 angular_speed_limit`, `bool reverse` → `bool reached` |

Użycia: [[Interfejsy (msg, srv, action)#l2_interfaces]]. `/tracked_detections` ma dwóch producentów w różnych konwencjach osi, patrz [[Drzewo TF i układy współrzędnych]].

## Różnice między branchami
- `main` (`f1de70b`) względem kopii `bash_do_uruchomienia` (`c7322b5`): nowy `Dock.msg` (`57c73d4`) i pole `spatial_points_size` (`d9c4fa3`).
- `origin/tracking_debug`: inne pliki (`Detection.msg`, `VectorOfTrackedDetections.msg`) w tym pakiecie.

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.
