---
typ: solar-pakiet
repo: "[[ros-sbt-interfaces]]"
build: ament_cmake
tags: [solar, pakiet]
zaktualizowano: 2026-10-04
---
# l3_interfaces

> [!skrot] W 30 sekund (co robi, wejście → wyjście)
> Interfejsy warstwy misji (L3, repo `l3nav` z drzewem zachowań). Obejmują wejście do mariny, wybór doku, dostawę, raporty „speed challenge” do organizatora (`/robot/speed_challenge`), stan TUI drzewa zachowań (`/bt_tui_state`) oraz nieużywane serwisy sterowania misją.

## Węzły
Brak.

## Launch
Brak.

## Konfiguracja (yaml)
Brak.

## Zależności
`package.xml`: `std_msgs`, `std_srvs`, `builtin_interfaces`, `l2_interfaces` → [[l2_interfaces]]. W `DEPENDENCIES` nie ma `geometry_msgs`, choć `MarinaEntry` i `Dock` używają `geometry_msgs/Point`.

## Interfejsy
| plik | pola |
|---|---|
| `msg/State.msg` | `string task`, `previous_state`, `current_state`, `int32 buoy_detections`, `float64 closest_buoy_bbox_area`, `int32 cardinal_detections`, `float64 closest_cardinal_bbox_area`, `int32 ar_tag_detections`, `float64 closest_artag_bbox_area`, `string info`, `string decision` |
| `msg/LocalWaypoint.msg` | `float32 x`, `float32 y`, `bool stay_in_this_wp_for_30s` |
| `msg/MarinaEntry.msg` | `geometry_msgs/Point along_start`, `along_end`, `across_start`, `across_end`, `char side` |
| `msg/MarinaSides.msg` | `l3_interfaces/MarinaEntry north`, `l3_interfaces/MarinaEntry south` |
| `msg/Dock.msg` | `int32 number`, `char side`, `l2_interfaces/TrackedDetection plackard`, `light`, `boat`, `geometry_msgs/Point dock`, `float64 x`, `float64 y` |
| `msg/Delivery.msg` | `string boat_color`, `string delivery_type` |
| `msg/SpeedReport.msg` | `string team_id`, `string vehicle_id`, `uint32 seq`, `string event_type` („START”, „END”, „BEACON”), `float64 latitude`, `float64 longitude`, `string color` |
| `msg/TuiEvent.msg` | `string node_name`, `string category`, `uint8 level`, `string message`, `builtin_interfaces/Time stamp`; `LEVEL_INFO=0`, `LEVEL_WARNING=1`, `LEVEL_ERROR=2` |
| `msg/TuiTransition.msg` | `string node_name`, `node_type`, `prev_status`, `new_status`, `builtin_interfaces/Time stamp` |
| `msg/TuiState.msg` | `string active_running_node`, `tree_status`, `active_subtree`, `TuiTransition[] recent_actions`, `TuiEvent[] recent_detection`, `recent_node_info`, `recent_warnings`, `recent_progress` |
| `srv/MissionControl.srv` | stałe `MISSION_UNUSED=-1`, `MISSION_START=0` … `MISSION_EXPLORE_CORAL_REEF=8`, `CAUSE_SYSTEM_OK=100`, `CAUSE_SYSTEM_STILL_BUSY=101`, `CAUSE_DATA_MISSING=102`; `int64 timestamp`, `int32 mission` → `int64 timestamp`, `int32 mission`, `int32 cause` |
| `srv/LoadControl.srv` | stałe `UNUSED=-1`, `DETECTOR_ALL=0` … `DETECTOR_DOCKING_PLATFORM=4`, `ESTIMATOR_ALL=100`, `ESTIMATOR_POSE=101`; `int64 timestamp`, `int32 target_module`, `int32 target_pub_period` → `int64 timestamp` |
| `srv/ChangeTask.srv` | `string next_state` → `bool success`, `string message` |
| `srv/CurrentStateTask.srv` | (puste) → `string current_task`, `bool success` |

Użycia: [[Interfejsy (msg, srv, action)#l3_interfaces]].

## Różnice między branchami
Wiadomości `Tui*` pochodzą z gałęzi `l3_tui_msgs` (zmergowanej). Między `main` a kopią `bash_do_uruchomienia` pakiet jest bez różnic.

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.
