---
typ: solar-pakiet
repo: "[[l3nav]]"
build: ament_cmake
tags: [solar, pakiet]
zaktualizowano: 2026-10-04
---
# basic_controller

> [!skrot] W 30 sekund (co robi, wejście → wyjście)
> Warstwa decyzyjna L3: **drzewa zachowań** (BehaviorTree.CPP v4) wykonujące misje konkursowe. Jeden węzeł ROS `my_bt_node` ładuje wszystkie pliki XML z `config/` (poza `config/tests`), tworzy drzewo wskazane parametrem `main_tree_name` i tyka je 10 Hz. Ok. 60 własnych węzłów BT czyta percepcję (`/tracked_detections`, `/dock_detection`, `/colision_detector`, `/slam/odom`), zamienia ją na waypointy na blackboardzie (`waypoint_vector`), a `NextWaypoint` wysyła je do L2: serwisem `/srv/waypoint_navigation_state` ([[trajectory_planner]]) albo topikiem `/goal` ([[planner]]). Wybiera też, kto steruje silnikami (`regulator_or_thrust_driver` → [[l0hwd]]).

Opis wg `~/solar/l3nav` — gałąź `waypoint_first` (`e336cd3`, 2026-08-12) **plus niezacommitowane zmiany w drzewie roboczym** (21 plików + 4 nowe), które odpowiadają stanowi z zawodów Njord (m.in. `SelectDock`, `/which_map`, `/regulator_speed` — por. commit `0d444ff` w [[l2dp]]). Pliki: `basic_controller/...`.

## Węzły
### `my_bt_node` (klasa `MyBTNode`, executable `basic_controller_exec`)
- Źródło: `src/my_bt_node.cpp`, `src/main.cpp`, `src/bt_registration.cpp`, `include/my_bt_node.hpp`.
- `main.cpp`: `node->initTree(); node->run();` — własna pętla, nie `rclcpp::spin`.
- `initTree()` (`my_bt_node.cpp:60–123`): `registerMyBTNodes(factory_, node)` (rejestracja węzłów C++ z lambdą wstrzykującą `rclcpp::Node`), `initMyBlackboard(...)`, rekurencyjnie `registerBehaviorTreeFromFile` dla każdego `*.xml` w `share/basic_controller/config` z pominięciem `config/tests` (`:85–101`), `createTree(main_tree_name)` — parametr `main_tree_name`, domyślnie `SimpleTree` (`:107`); TUI logger i timer 200 ms publikujący `/bt_tui_state`.
- `run()` (`:125–155`): pętla 10 Hz `spin_some` + `tree_.tickOnce()` aż do `SUCCESS`; flaga blackboardu `restart_tree` → `restartTree()` (halt, ponowne `createTree`).
- **Subskrybuje:** `detected_signals` (`std_msgs/String`, QoS `QoS(1).transient_local().reliable()`) → blackboard `blasts` (int z treści), `is_alert_active=true` (jednorazowo, `:13–26`); `/odom` (`nav_msgs/Odometry`) → `pose_x`, `pose_y` (`:28–33`).
- **Publikuje:** `/bt_tui_state` (`l3_interfaces/TuiState`).
- **Blackboard startowy** (`bt_registration.cpp:290–326`): `waypoint_vector`, `buoy_waypoints`, `exit`, `task_waypoint_map`, `gate_waypoints`, `past_buoys`, `active_subtree`, `blasts 0`, `is_alert_active false`, `pose_x/pose_y 0`, `restart_tree false`, `speed_limit (2.5, 1.5)`, `colision false`, `slip_data`, `last_gate_stamp_sec`, `last_debris_stamp_sec`, `node` …

### `bt_tui_viewer` (executable, `src/tui/bt_tui_viewer.cpp`)
Terminalowy podgląd `/bt_tui_state`: panele CURRENT, RECENT ACTIONS, NODE INFO, PROGRESS, WARNINGS/FAILURES, DETECTION FEED; klawisze `p`, `Tab`, `j/k`, `q` (README `l3nav`). Logowanie z węzłów BT: `tui_.info/warn/error/progress(name(), fmt, …)` (`include/tui/node_tui_logger.h`).

### `test_runner` (executable, `tests/test_runner.cpp`)
`ros2 run basic_controller test_runner <TreeName> <MockJsonFile.json>` — uruchamia drzewo z podmianą węzłów na mocki wg `config/tests/*.json` (`SubstitutionRules`, `TestNodeConfigs`), np. `nav_test.json`, `entry_test.json`.

### Węzeł BT `NextWaypoint` — zwrotnica do L2 (`src/next_waypoint_node.cpp`)
`BT::StatefulActionNode`. Porty (`:115–133`): wejścia `waypoint_vector`, `speed_limit`, `reverse`, `use_mapper`, `steering` (`regulator`|`thrust_driver`), `play_music`, `stop_music`, `slam_costmap` (domyślnie true), `speed_regulator` (domyślnie 1.0); wyjścia `last_waypoint`, `colision_x`, `colision_y`, `colision`, `safe_passage`.
- Tworzy: klient `/srv/waypoint_navigation_state`; subskrypcje `/wp_reached`, `/l3/waypoint_reached` (`Bool`), `/slam/odom`, `/colision_detector` (`Point`), `/colision_direction` (`String`); publishery `/goal` (`PoseArray`), `melody` (`Int16`), `regulator_or_thrust_driver` (`String`), `/which_map` (`Bool`), `/regulator_speed` (`Float64`) (`:3–76`).
- `onStart` (`:135–224`): pusty wektor → `FAILURE` (idiom „powtarzaj aż kolejka pusta”); publikuje `/which_map = slam_costmap`, `/regulator_speed = speed_regulator`, `regulator_or_thrust_driver` = `"regulator"` (domyślnie / brak portu / nieznana wartość) albo `"diff_thrust_driver"` (gdy `steering="thrust_driver"`); melodia (3× `melody`); `sendWaypointToL2()`; zwraca `RUNNING` (z ustawieniem portów kolizji, jeśli `/colision_detector` ≠ 0).
- `sendWaypointToL2` (`:311–360`): pierwszy punkt kolejki; `use_mapper=true` → `PoseArray` z jednym punktem na `/goal` (orientacja = bieżący kurs łodzi); `use_mapper=false` → `async_send_request` serwisu (x, y, limity z `speed_limit`, `reverse`, `stay_in_this_wp_for_30s=false`).
- `onRunning` (`:226–302`): `use_mapper=true` czeka na `/wp_reached` (z [[planner]]); `false` czeka na `/l3/waypoint_reached` (z [[trajectory_planner]]) i co tick publikuje `/which_map=false`; po dojściu `erase(begin())`, opcjonalnie `stop_music` (ton −9), `SUCCESS`.
- Melodie → numery tonów buzzera (`melody_to_tone`, `:362–430`; komentarz: id z `Embedded/Barka/V2.0/Core/Src/task_master_buzzer.c`).

### Pozostałe węzły BT (rejestracja w `src/bt_registration.cpp`)
Zarejestrowane nazwy: `NextWaypoint`, `LookForGateSpeed`, `MoveAlongHeading`, `LookForGate`, `LookForGatePathFinding`, `ResetUsage`, `LookForBeacon`, `LookForTracked`, `ExitMarina`, `ReturnToCenter`, `SteerToDock`, `Breaking`, `Wait`, `LookForBuoyTracked`, `HandleHarborAlert`, `Success`, `Failure`, `Log`, `IsTrue`, `SendWaypoint`, `RetryWithRecoveryUntilFail`, `RetryWithRecoveryUntilSuccess`, `RetryThenSuccess`, `LookForBoatTracked`, `UsePeripheral`, `ApproachBeacon`, `LookForBuoyAndBeacon`, `SendGatePass`, `AwaitDockSelection`, `ComputeSlipWaypoint`, `BuildModel`, `LookForMarina`, `UpdateSlipDetections`, `SearchForMarina`, `CalculateDockSelection`, `LookForDebris`, `LoadReturnWaypoints`, `ReturnHome`, `ResetProgress`, `SetTask`, `SendEntryExit`, `DriveForwardFromGate`, `Spin`, `AddCurrentPose`, `PublishTransition`, `StoreWaypoints`, `DetectBuoy`, `SetBuoyWaypoints`, `SafeSquareWaypoints`, `ApproachGNSSBuoy`, `WaypointMiddleOfCoords`, `HardcodedSlalom`, `GNSSToLocal`, `Figure8WaypointGenerator`, `LookForObject`, `DriftAround`, `DeleteWaypoints`, `Melody`, `SelectDock` (ostatni — niezacommitowany).

Węzły kluczowe dla nawigacji i dokowania:
- **`GNSSToLocal`** (`src/gnss_to_local.cpp`) — współrzędne geograficzne z XML (`gnss_points="lat,lon"`) → punkty lokalne względem `/start_pose`, z uwzględnieniem `/slam/odom_transform` ([[pose_graph]]); dopisuje do `waypoint_vector`.
- **`SelectDock`** (`src/select_dock.cpp`, niezacommitowany) — porty `min_width_dock` (1.0), `max_width_dock` (2.5); publikuje 8× `choose_dock` (`geometry_msgs/Point`, x=min, y=max) → [[segment_scan]] zmienia wymiary, `SteerToDock` czyści swoje detekcje.
- **`SteerToDock`** (`src/steer_to_dock.cpp`) — zbiera `/dock_detection` ([[segment_scan]], ramka LiDAR) i przelicza do świata pozą z `/slam/odom` (`GetLocationOfDetection`, `:57–64`: `x = pose_x + d.y·cos(yaw) − d.x·sin(yaw)`, `y = pose_y + d.y·sin(yaw) + d.x·cos(yaw)`); detekcje bliżej niż 1 m się scalają (licznik); `tick`: wybiera detekcję z największym licznikiem, wymaga ≥2 (`:135`), ustawia `speed_limit (1.0, 1.0)` i dopisuje do kolejki punkt w slipie i punkt bezpieczny; inaczej `FAILURE`.
- **`Breaking`** (`src/breaking.cpp`) — przełącza na `diff_thrust_driver`, publikuje `/l2/speed_vector` z `linear = reverse_power` przez `breaking_ticks` ticków (FAILURE w trakcie), potem 0 i `SUCCESS`.
- **`DeleteWaypoints`**, **`MoveAlongHeading`** (punkt względem kursu łodzi), **`Spin`**, **`DriftAround`** (publikuje `/bow_thrusters`), **`Wait`** — manewry.
- **`LookForGate`**, **`DetectBuoy`**, **`SetBuoyWaypoints`**, **`WaypointMiddleOfCoords`** — percepcja → punkty (bramki z boi czerwonej/zielonej, obejście boi wg `directions`); zgłaszają zużyte detekcje na `/used_detections`.
- **`RetryWithRecoveryUntilSuccess`** — dekorator z portem `colision`: przy kolizji uruchamia gałąź recovery (obejście).
- **`SetTask`** → `/robot/task`; **`PublishTransition`** → `/celka/temporary/transition`; **`UsePeripheral`** → klienci `/srv/pump_control`, `/srv/cannon_control` ([[l0hwd]] `peripherals_handler`).

## Launch
Brak plików launch. Uruchamianie (README `l3nav`):
```
ros2 run basic_controller basic_controller_exec --ros-args -p main_tree_name:=<ID drzewa>
ros2 run basic_controller bt_tui_viewer
```

## Konfiguracja (yaml)
Drzewa XML w `config/` (ID drzewa w nawiasie):
- ogólne: `behavior_tree.xml` (SimpleTree), `bh_dummy.xml`, `bh_example.xml`, `bh_test.xml`;
- `monaco_trees/` (Monako 2026): `celka_osemka*.xml` (CelkaOsemka, …Auto, …GNSS, …Manual, …Tracked — ósemka wokół dwóch boi), `bh_slalom.xml`, `slalom_alternative.xml`, `sprint*.xml`, `bh_docking_monaco.xml`, `safety_inspection*.xml`, `regulator_test.xml` (RegTest), `baobab.xml`, `pinii.xml`;
- `njord_trees/` (Njord 2026): `collision_njord.xml` (CollisionNjord), `maneuvering_njord*.xml` (Maneuvering, ManNjord1, ManNjord2, ManNjord2C), `maneuvering_surprise.xml` (ManSurprise, nieśledzony), `normal_docking_njord.xml` (DockNjordNorm; drugi ID DockingNjordNormal w bloku komentarza), `parallel_docking_njord.xml` (DockNjordParl), `spisslonn.xml` (Spisslonn = `DockNjordParl` → `ManNjord2` → `DockNjordNorm`, nieśledzony), `bjork.xml` (MoveAlongHeading/NextWaypoint/Spin), `rogn.xml` (same Log/Sleep); `surprisetask.xml` usunięty w drzewie roboczym;
- `rb_trees/` (RoboBoat): entry/exit, docking, object delivery, speed challenge, nav channel.
- Współrzędne GNSS w drzewach Njord (np. `63.440950,10.423950`) — okolice Trondheim.

**Dokowanie Njord (`normal_docking_njord.xml`):** `ResetUsage` → `SelectDock(1.0, 2.5)` → `SetTask normal_docking` → `GNSSToLocal` punkt podejścia → `NextWaypoint use_mapper=true slam_costmap=true` (A* omija przeszkody) → `ReactiveFallback`: [`SteerToDock` → `DeleteWaypoints(number=2, leave=true)` → `NextWaypoint use_mapper=true slam_costmap=false speed_regulator=0.7` (prosto do slipu, wolniej) → `Breaking(-1.0, 100 ticków)` → `Sleep 10 s`] albo (gdy slipu nie widać) manewry `MoveAlongHeading` i powrót na punkt GNSS.

**Collision Njord (`collision_njord.xml`):** `GNSSToLocal` → pętla `LookForGate` → `WaypointMiddleOfCoords` → `RetryWithRecoveryUntilSuccess colision={colision}`: `NextWaypoint use_mapper=true` (wyprowadza `colision_x/y`, `safe_passage`), recovery: `SetBuoyWaypoints buoy={colision}` `directions={safe_passage}` `side_offset=5.0` + `NextWaypoint`.

W `njord_trees/` + `monaco_trees/` jest 102 wystąpień `<NextWaypoint` (łącznie z zakomentowanymi): 54× `use_mapper="true"`, 48× `use_mapper="false"` — oba tryby są w użyciu.

## Zależności
`package.xml`: `rclcpp`, `rclcpp_lifecycle`, `rclcpp_action`, `l0_interfaces`, `l1_interfaces`, `l2_interfaces`, `l3_interfaces`, `pluginlib`, `nav2_core`, `nav2_msgs`, `std_srvs`, `nav_msgs`, `sensor_msgs`, `geometry_msgs`, `tf2_ros`, `nav2_costmap_2d`, `std_msgs`, `behaviortree_cpp`. Zależności Nav2 są pozostałością (w `src/` i `include/` brak odwołań do `nav2`; `plugin_description.xml` opisuje `NavigateToGoal` „using Nav2's NavigateToPose action”, ale taki węzeł nie jest rejestrowany; tag `archive/bt-testing` = „archived nav2 and slam approach”).
Współpraca: [[planner]], [[trajectory_planner]], [[pose_graph]], [[segment_scan]], [[colision_detector]], [[odometry]], `regulator`/`diff_thrust_driver`/`peripherals_handler` w [[l0hwd]], percepcja [[l1od]]; interfejsy [[ros-sbt-interfaces]].

## Interfejsy
Brak własnych msg/srv (TUI używa `l3_interfaces/TuiState`).

## Różnice między branchami
- `master` (`dd07b54`, 2026-07-19, „merge monaco-cleanup and apply final fixes”) — stan po Monako; brak `njord_trees`, `SelectDock`, `/which_map`, `/regulator_speed`.
- `njord_trees`, `origin/docking_njord` → wmergowane do `melody` (Njord): drzewa Njord, `DeleteWaypoints`, GNSS w dokowaniu, `detection_area` w `DetectBuoy`, przyspieszony CMake.
- `melody` (`d6db1c1`) vs `waypoint_first` (`e336cd3`): ta sama baza `95d3741`; `waypoint_first` dodaje zmiany w `detect_buoy.cpp`, `set_buoy_waypoints.cpp` („piotrek rozjebal”), `melody` — w `collision_njord.xml`, `move_along_heading.cpp`.
- Drzewo robocze (niezacommitowane): `SelectDock`, porty `slam_costmap`/`speed_regulator` w `NextWaypoint`, zmiany w drzewach dokowania i manewrów, `spisslonn.xml`, `maneuvering_surprise.xml`, poprawki `WaypointMiddleOfCoords` (+97/−…), `DetectBuoy`, `LookForGate`, `SetBuoyWaypoints`, `SteerToDock`.

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.
