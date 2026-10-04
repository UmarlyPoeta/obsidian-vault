---
typ: solar-zbiorcza
tags: [solar, interfejsy]
zaktualizowano: 2026-10-04
---
# Interfejsy (msg, srv, action)

> [!skrot] W 30 sekund
> - Wszystkie własne interfejsy ROS 2 łodzi leżą w repo [[ros-sbt-interfaces]]. Jest tam pięć pakietów: [[l0_interfaces]], [[l1_interfaces]], [[l2_interfaces]], [[l3_interfaces]], [[l4_interfaces]].
> - Razem **36 msg i 13 srv, 0 action** (l0: 12+2, l1: 7+5, l2: 6+2, l3: 10+4, l4: 1+0) (stan `main` @ `f1de70b`).
> - Poza tym repo w `~/solar` są jeszcze: `can_msgs/msg/Frame` ([[can_msgs]] w [[l0hwd]]), `ros2_socketcan_msgs/msg/FdFrame` ([[ros2_socketcan]]) i `dongle_statistic/msg/TrafficStats` ([[dongle_statistic]]).
> - Kolumna „kto” pochodzi z grepa po `~/solar` (bez `build/`, `install/`, `log/`, `node_modules/`, `.git`). Kopie `bash_do_uruchomienia/*` i `l2dp_docking_fix` są zwinięte do nazwy repo.

## Jak czytać tabelę
- **pola**: request **→** response dla srv. Stałe (`int32 X = …`) pominięte, są w notatkach pakietów.
- **kto**: `pub`/`sub` + topik dla wiadomości, `serwer`/`klient` + nazwa dla serwisów. „Zagnieżdżony” znaczy, że typ występuje tylko jako pole innej wiadomości albo w kodzie pomocniczym. Ścieżki źródłowe są w notatkach repo i pakietów.

## l0_interfaces
| interfejs | pola | kto |
|---|---|---|
| `l0_interfaces/msg/SpeedVector` | `float64 linear`, `float64 angular` | **pub** `/l2/speed_vector`: `l2dp/planner` (`planner.cpp:213`), `l2dp/trajectory_planner` (`PUB_SPEED_VECTOR_TOPIC_NAME="/l2/speed_vector"`, `trajectory_planner.cpp:407`), `l3nav/basic_controller` (`breaking`, `spin`, `drift_around`, `look_for_gate*`), [[swim_sequence]]; **sub**: [[diff_thrust_driver]], `l4sim/sim_thrust_driver` |
| `l0_interfaces/msg/MissionState` | `string mission`, `string state`, `string transition` | **pub** `/mission_state/data`: `l3nav/basic_controller` (`steer_to_dock`, `hardcoded_slalom`, `wait`, `look_for_gate_speed`, `gnss_waypoint`), `l2dp/trajectory_planner:413`; **sub**: `l2dp/trajectory_planner:358` |
| `l0_interfaces/msg/BmsData` | `std_msgs/Header header`, `BmsUnitData central`, `BmsUnitData left`, `BmsUnitData right` | **pub** `/bms/data`: [[can_driver]]; **sub**: `agh-dashboard-live` (`scripts/bms_panel.py`, `live-rosbridge-publisher.mjs:41`) |
| `l0_interfaces/msg/BmsUnitData` | `float32 temperature`, `float32 current`, `float32 power`, `float32 soc` | zagnieżdżony w `BmsData` |
| `l0_interfaces/msg/BmsVoltages` | `std_msgs/Header header`, `CellVoltages central`, `CellVoltages left`, `CellVoltages right` | **pub** `/bms/voltage`: [[can_driver]]; **sub**: dashboard (`live-rosbridge-publisher.mjs:42`, `bms_panel.py`) |
| `l0_interfaces/msg/CellVoltages` | `float32 cell1`, `cell2`, `cell3`, `cell4` | zagnieżdżony w `BmsVoltages` |
| `l0_interfaces/msg/Course` | `std_msgs/Header header`, `float64 course` | **sub** `/true_heading/data`: `sbt_axis_audit` ([[Pakiety/sbt_transforms|sbt_transforms]]); zagnieżdżony/include: `l2dp/trajectory_planner.hpp`; publisher w [[can_driver]] zakomentowany |
| `l0_interfaces/msg/GuiGraph` | `float64 energy_consumption`, `battery_percentage_left`, `_right`, `_central` | tylko zakomentowany kod [[can_driver]] |
| `l0_interfaces/msg/VesselState` | `float64 latitude`, `longitude`, `heading`, `speed`, `distance_to_waypoint` | tylko zakomentowany kod [[can_driver]] |
| `l0_interfaces/msg/Measurement` | `std_msgs/Header header`, `float64 temperature`, `current`, `power`, `soc` | tylko zakomentowany kod [[can_driver]] |
| `l0_interfaces/msg/VoltageMeasurement` | `std_msgs/Header header`, `float64 voltage` | tylko zakomentowany kod [[can_driver]] |
| `l0_interfaces/msg/MeasureOld` | `std_msgs/Header header`, `int16 source`, `float64 data` | brak użyć |
| `l0_interfaces/srv/PumpControl` | `bool pump_req`, `int32 timeout` **→** `bool pomp_res` (stałe `POMP_START=1`, `POMP_STOP=0`) | **serwer** `/srv/pump_control`: [[peripherals_handler]]; **klient**: `l3nav/waypoint_sequencer` (`:36`), `l3nav/basic_controller/use_peripheral_node` (`:6`) |
| `l0_interfaces/srv/CannonControl` | `uint8 cannon_direction`, `uint8 cannon_command` **→** `int32 cannon_res` | **serwer** `/srv/cannon_control`: [[peripherals_handler]]; **klient**: `l3nav/waypoint_sequencer` (`:37`), `use_peripheral_node` (`:7`) |

## l1_interfaces
| interfejs | pola | kto |
|---|---|---|
| `l1_interfaces/msg/Detection` | `int32 detection_class`, `int32 detection_type`, `string detection_string`, `float64 confidence`, `float64 bbox_x0`, `bbox_x1`, `bbox_y0`, `bbox_y1`, `float64 heading_diff`, `distance`, `area`, `x`, `y`, `z` (+ stałe klas 1–8 i typów 101–401) | zagnieżdżony w `VectorOfDetections`; `l1od/vision_detector`, `detection_fusion*`, `detection_pointcloud_fusion` |
| `l1_interfaces/msg/VectorOfDetections` | `std_msgs/Header header`, `l1_interfaces/Detection[] detections` | **pub** `/l1/detection_<nazwa_węzła>`: `l1od/vision_detector/detector.py:119`; **sub**: `l1od/detection_fusion*`, `detection_pointcloud_fusion`; normalizer [[Pakiety/sbt_transforms|sbt_transforms]] (typ obsługiwany, nieużyty w YAML) |
| `l1_interfaces/msg/Cluster` | `geometry_msgs/Point center`, `min`, `max` | zagnieżdżony w `VectorOfClusters` |
| `l1_interfaces/msg/VectorOfClusters` | `std_msgs/Header header`, `l1_interfaces/Cluster[] clusters` | **pub** `/l1/obstacle_detection`: `l1od/obstacle_detector` (`:309`); **pub** `/global/clusters`: `l2dp/pose_graph/feature_extractor:587`; **sub** `/l1/obstacle_detection`: `l2dp/mapper`, `l2dp/obstacle_avoidance`, `l2dp/pose_graph`, `l3nav/look_for_object`, `bash_do_uruchomienia/l2dp/colision_detector`; **sub** `/global/clusters`: `l2dp/colision_detector`; normalizer → `/canonical/obstacle_detection` |
| `l1_interfaces/msg/FusedDetection` | `std_msgs/Header header`, `int32 id`, `bool is_fused_angle`, `bool is_fused`, `int32 detection_class`, `int32 detection_type`, `string detection_string`, `float64 confidence`, `geometry_msgs/Point position`, `geometry_msgs/Point[] yolo_bbox_corners`, `cluster_bbox_corners`, `spatial_points` (+ stałe jak w `Detection`) | zagnieżdżony w `VectorOfFusedDetections` |
| `l1_interfaces/msg/VectorOfFusedDetections` | `std_msgs/Header header`, `l1_interfaces/FusedDetection[] fused_detection` | **pub** `/fused_detection`: `l1od/detection_fusion`, `detection_fusion_projection`, `detection_pointcloud_fusion`; **pub** `/tracking_detection`: `l1od/tracking_detection`; **sub** `/fused_detection`: `l1od/tracking_detection`, `l2dp/mapper`, `l2dp/pose_graph`, `l2dp/message_protobuf`, `sbt_axis_audit`; normalizer → `/canonical/fused_detection(_map)` |
| `l1_interfaces/msg/PlotPackage` | `std_msgs/Header header`, `string model`, `string[] det_str`, `float32 duration`, `float32[] area`, `confidences`, `x_centres`, `y_centres`, `depths` | brak użyć |
| `l1_interfaces/srv/SaveImagesParams` | `int32 requested_cmd`, `string save_path`, `bool save_all_img`, `bool resize_for_yolo`, `int32 frequency`, `int32 buffer` **→** `bool executed_cmd` | **serwer** `/srv/save_images`: `l1od/images_collector` (`:53`) |
| `l1_interfaces/srv/MakeVideoParams` | `string input_directory`, `string film_filepath`, `int32 framerate`, `bool delete_images` **→** `bool executed_cmd` | **serwer** `/srv/make_video`: `l1od/images_collector` (`:55`) |
| `l1_interfaces/srv/MakeDetectionsParams` | `string input_directory`, `string output_directory`, `bool delete_images` **→** `bool executed_cmd` | **serwer** `/srv/make_detections`: `l1od/images_collector` (`:54`) |
| `l1_interfaces/srv/RequestCMD` | `int32 requested_cmd` **→** `bool executed_cmd` | brak użyć |
| `l1_interfaces/srv/RequestString` | `int32 requested_cmd`, `string requested_string` **→** `bool executed_cmd` | brak użyć |

## l2_interfaces
| interfejs | pola | kto |
|---|---|---|
| `l2_interfaces/msg/TrackedDetection` | `l2_interfaces/DetectionMapEntry[] detection_type_map`, `int32 detection_type`, `int32 detection_class`, `float64 x`, `float64 y`, `int32 usage`, `int32 id`, `int32 spatial_points_size` (+ stałe jak w `Detection`) | zagnieżdżony; `l2dp/mapper`, `l2dp/pose_graph`, `l3nav/basic_controller`, `l3_interfaces/Dock` |
| `l2_interfaces/msg/TrackedDetectionsVector` | `std_msgs/Header header`, `l2_interfaces/TrackedDetection[] tracked_detections` | **pub** `/tracked_detections`: `l2dp/pose_graph/feature_extractor:583` **i** `l2dp/mapper:172` (względne `tracked_detections`), czyli **dwóch producentów** (patrz [[Drzewo TF i układy współrzędnych]]); **sub**: 10 węzłów BT `l3nav/basic_controller` (`look_for_*`, `detect_buoy`), `l2dp/ascii_map_viz2`, `sbt_axis_audit`; normalizer: `/slam/tracked_detections_raw` → `/tracked_detections` |
| `l2_interfaces/msg/DetectionMapEntry` | `int32 detection_type`, `float64 confidence_sum`, `int64 detection_count` | zagnieżdżony w `TrackedDetection` |
| `l2_interfaces/msg/Dock` | `std_msgs/Header header`, `geometry_msgs/Point dock`, `geometry_msgs/Point safe` | **pub** `/dock_detection`: `l2dp/segment_scan` (`:85`); **sub**: `l3nav/basic_controller/steer_to_dock` (`:6`). Nowy na `main` (`57c73d4`, 2026-08-10 „docking njord”) |
| `l2_interfaces/msg/MetricPosition` | `std_msgs/Header header`, `float64 x`, `float64 y`, `float64 distance` | **pub** `/gps/metric_position`: `l2dp/gps_calculator/gps_converter.cpp:25`; subskrybenta nie znaleziono |
| `l2_interfaces/msg/BlastDetection` | `std_msgs/Header header`, `uint8 blast_type`, `uint32 frequency_hz`, `string zone` | **pub** `detected_signals` (względny): `l2dp/blast_detection/blast_detector_node.cpp:59` |
| `l2_interfaces/srv/SetHome` | `float64 latitude`, `float64 longitude` **→** `bool if_home_changed` | **serwer** `/srv/set_home_keyboard_input`: `l2dp/gps_calculator` (`:26`) |
| `l2_interfaces/srv/WaypointNavigationState` | `float64 x`, `float64 y`, `bool stay_in_this_wp_for_30s`, `float64 linear_speed_limit`, `float64 angular_speed_limit`, `bool reverse` **→** `bool reached` | **serwer** `/srv/waypoint_navigation_state`: `l2dp/trajectory_planner:427`; **klient**: `l3nav/waypoint_sequencer:35`, `l3nav/basic_controller/next_waypoint_node:7` |

## l3_interfaces
| interfejs | pola | kto |
|---|---|---|
| `l3_interfaces/msg/LocalWaypoint` | `float32 x`, `float32 y`, `bool stay_in_this_wp_for_30s` | zagnieżdżony/kod: `l2dp/obstacle_avoidance`, `l2dp/trajectory_planner` |
| `l3_interfaces/msg/Dock` | `int32 number`, `char side`, `l2_interfaces/TrackedDetection plackard`, `light`, `boat`, `geometry_msgs/Point dock`, `float64 x`, `float64 y` | wewnętrznie w BT `l3nav/basic_controller` (`build_model`, `calculate_dock_selection`, `compute_slip_waypoint`, …) |
| `l3_interfaces/msg/MarinaEntry` | `geometry_msgs/Point along_start`, `along_end`, `across_start`, `across_end`, `char side` | **pub** `/marina_entry`: `l3nav/search_for_marina:23`; **sub**: `l3nav/look_for_marina:14` |
| `l3_interfaces/msg/MarinaSides` | `l3_interfaces/MarinaEntry north`, `l3_interfaces/MarinaEntry south` | **pub** `/marina_sides`: `l2dp/segment_scan` (`:91`); **sub**: `l3nav/search_for_marina:26` |
| `l3_interfaces/msg/Delivery` | `string boat_color`, `string delivery_type` | **pub** `/delivery`: `l3nav/look_for_boat_tracked:32` |
| `l3_interfaces/msg/SpeedReport` | `string team_id`, `string vehicle_id`, `uint32 seq`, `string event_type` („START”, „END”, „BEACON”), `float64 latitude`, `float64 longitude`, `string color` | **pub** `/robot/speed_challenge`: `l3nav/send_entry_exit:8`, `send_gate_pass:8` |
| `l3_interfaces/msg/TuiState` | `string active_running_node`, `tree_status`, `active_subtree`, `TuiTransition[] recent_actions`, `TuiEvent[] recent_detection`, `recent_node_info`, `recent_warnings`, `recent_progress` | **pub** `/bt_tui_state`: `l3nav/my_bt_node:35`; **sub**: `l3nav/tui/bt_tui_viewer:457` |
| `l3_interfaces/msg/TuiEvent` | `string node_name`, `string category`, `uint8 level` (`LEVEL_INFO=0`, `WARNING=1`, `ERROR=2`), `string message`, `builtin_interfaces/Time stamp` | zagnieżdżony w `TuiState` |
| `l3_interfaces/msg/TuiTransition` | `string node_name`, `node_type`, `prev_status`, `new_status`, `builtin_interfaces/Time stamp` | zagnieżdżony w `TuiState` |
| `l3_interfaces/msg/State` | `string task`, `previous_state`, `current_state`, `int32 buoy_detections`, `float64 closest_buoy_bbox_area`, `int32 cardinal_detections`, `float64 closest_cardinal_bbox_area`, `int32 ar_tag_detections`, `float64 closest_artag_bbox_area`, `string info`, `string decision` | brak użyć |
| `l3_interfaces/srv/ChangeTask` | `string next_state` **→** `bool success`, `string message` | brak użyć |
| `l3_interfaces/srv/CurrentStateTask` | (puste) **→** `string current_task`, `bool success` | brak użyć |
| `l3_interfaces/srv/LoadControl` | `int64 timestamp`, `int32 target_module`, `int32 target_pub_period` **→** `int64 timestamp` (stałe `DETECTOR_*`, `ESTIMATOR_*`) | brak użyć |
| `l3_interfaces/srv/MissionControl` | `int64 timestamp`, `int32 mission` **→** `int64 timestamp`, `int32 mission`, `int32 cause` (stałe `MISSION_*` 0–8, `CAUSE_*` 100–102) | brak użyć |

## l4_interfaces
| interfejs | pola | kto |
|---|---|---|
| `l4_interfaces/msg/WorldObject` | `string name`, `uint64 id`, `float64 x`, `float64 y` | brak użyć (także w `bash_do_uruchomienia/l4sim`) |

## Interfejsy spoza ros-sbt-interfaces
| interfejs | pola | kto |
|---|---|---|
| `can_msgs/msg/Frame` ([[can_msgs]]) | `std_msgs/Header header`, `uint32 id`, `bool is_rtr`, `bool is_extended`, `bool is_error`, `uint8 dlc`, `uint8[8] data` | `/to_can_bus` i `/from_can_bus`, patrz [[Magistrala CAN]] |
| `ros2_socketcan_msgs/msg/FdFrame` | (upstream Autoware) | `/from_can_bus_fd`, `/to_can_bus_fd`, w łodzi nieużywane (FD off) |
| `dongle_statistic/msg/TrafficStats` ([[dongle_statistic]]) | `string timestamp`, `string interface_name`, `float64 today_received_gb`, `today_transmited_gb`, `today_total_gb`, `month_received_gb`, `month_transmited_gb`, `month_total_gb`, `uint32 ssh_connections` | **pub** `dongle_statistic/traffic_stats` |

## Obserwacje
- **Nieużywane** (brak odwołań poza definicją): `MeasureOld`, `PlotPackage`, `RequestCMD`, `RequestString`, `DetectionMapEntry` (tylko jako pole), `State`, `ChangeTask`, `CurrentStateTask`, `LoadControl`, `MissionControl`, `WorldObject`. `GuiGraph`, `VesselState`, `Measurement`, `VoltageMeasurement` występują tylko w zakomentowanym kodzie [[can_driver]].
- Stałe klas i typów detekcji są **skopiowane trzy razy** (`Detection`, `FusedDetection`, `TrackedDetection`), więc trzeba je utrzymywać ręcznie w zgodzie.
- `l3_interfaces` używa `geometry_msgs/Point` (`MarinaEntry`, `Dock`), ale nie ma `geometry_msgs` ani w `package.xml`, ani w `DEPENDENCIES` (`l3_interfaces/CMakeLists.txt`). Działa przypuszczalnie tranzytywnie przez `l2_interfaces` (**niepewne**).
- Pakiety mają ten sam numer wersji `0.0.0` i nie ma wersjonowania interfejsów.

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.
