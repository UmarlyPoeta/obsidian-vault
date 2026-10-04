---
typ: solar-pakiet
repo: "[[l2dp]]"
build: ament_cmake
tags: [solar, pakiet]
zaktualizowano: 2026-10-04
---
# segment_scan

> [!skrot] W 30 sekund (co robi, wejście → wyjście)
> Detektor **slipu do dokowania** z 2D LaserScan. Z `/scan` składa odcinki prostych, szuka kształtu „U” (lewa ściana ⟂ tylna ściana ⟂ prawa ściana, równoległe boki) o zadanych wymiarach i publikuje punkt w środku slipu + punkt bezpieczny 5 m przed nim (`/dock_detection`, `l2_interfaces/Dock`) oraz wielokąt 4 narożników `/dock_polygon` dla [[pose_graph]] (gaszenie inflacji). Wszystko w ramce LiDAR-a (`hesai_lidar`) — przeliczenie do świata robi odbiorca. Szerokość slipu można zmieniać w locie z L3 przez `/choose_dock`.

Opis wg `l2dp_docking_fix@0d444ff`.

## Węzły
### `segment_scan_node` (klasa `SegmentScanNode`)
- Źródło: `segment_scan/src/segment_scan.cpp`, `include/segment_scan.hpp`, `src/main.cpp`. Executable `segment_scan`.
- **Parametry** (`segment_scan.cpp:52–55`; domyślne w nagłówku `segment_scan.hpp:44–47`, nadpisywane z YAML): `min_dock_width` (2.0 / YAML 1.0), `max_dock_width` (5.0 / YAML 3.0), `min_dock_depth` (1.0 / YAML 1.8), `max_dock_depth` (3.0 / YAML 3.0).
- **Subskrybuje:**
  - `/scan` (`sensor_msgs/LaserScan`, SensorDataQoS) → `LaserCallback` + `DockDetector` (`:59–65`).
  - `/scan_high` (`LaserScan`, SensorDataQoS) → `LaserCallback` + `MarinaDetector` (`:67–73`); `MarinaDetector` (`:694`) ma ciało w całości zakomentowane — nic nie publikuje.
  - `/choose_dock` (`geometry_msgs/Point`) — `x` → `min_dock_width`, `y` → `max_dock_width` (`:75–80`); wysyła je węzeł `SelectDock` z [[basic_controller]].
  - Subskrypcja `/odom` zakomentowana (`:100–108`) → `pose_x = pose_y = boat_yaw = 0`, czyli wszystkie wyniki są względem LiDAR-a.
- **Publikuje:** `/dock_detection` (`l2_interfaces/Dock`: `header`, `dock`, `safe`), `/dock_polygon` (`geometry_msgs/PolygonStamped`, frame `hesai_lidar`, 4 narożniki: lewy-przód, lewy-tył, prawy-tył, prawy-przód), `/dock_marker` (`visualization_msgs/Marker`, kula w punkcie dokowania), `/marina_entry`, `/marina_possible_entry` (`PoseArray`), `/marina_sides` (`l3_interfaces/MarinaSides`) — trzy ostatnie zadeklarowane, nieużywane (detektor mariny zakomentowany).

**Algorytm:**
1. `LaserCallback` (`:119–198`): co 2 promienie (`step = 2`) tworzy odcinek między kolejnymi punktami; jeśli kolejny odcinek jest równoległy (`AreParallelLines`), przedłuża bieżącą prostą; prosta z >2 segmentów trafia na listę. Promienie `inf` przerywają linię.
2. `DockDetector` (`:199–390`) — automat po liczbie znalezionych ścian (`switch (dock.size())`):
   - 0: pierwsza linia = lewa ściana;
   - 1: kolejne współliniowe (tolerancja 0.2) → dalej lewa; prostopadła (80–100°) → tylna ściana; sprawdza głębokość lewej ściany w [`min_dock_depth`, `max_dock_depth`];
   - 2: prostopadła do tylnej, równoległa do lewej, po tej samej stronie tylnej ściany → prawa ściana; szerokość (odległość tylnych narożników) w [`min_dock_width`, `max_dock_width`];
   - 3: koniec prawej ściany → sprawdza głębokość prawej; punkt dokowania = średnia ze środka boków i środka tylnych narożników; odrzuca, jeśli punkt leży <0.1 m od którejś ściany; punkt bezpieczny = punkt dokowania + 5 m wzdłuż normalnej tylnej ściany (`:362–366`); publikuje `/dock_detection`, `/dock_polygon`, `/dock_marker`.
   Każde niepowodzenie resetuje automat od bieżącej linii.

## Launch
`launch/segment_scan.launch.py`: argument `params_file` (domyślnie `share/segment_scan/config/segment_scan_params.yaml`), węzeł `segment_scan` o nazwie `segment_scan_node`, `output='screen'`. Uruchomienie: `ros2 launch segment_scan segment_scan.launch.py`.

## Konfiguracja (yaml)
`config/segment_scan_params.yaml` (`/**: ros__parameters`): `min_dock_width 1.0`, `max_dock_width 3.0` (na `0d444ff`, wcześniej 2.5), `max_dock_depth 3.0` („max. glebokosc kesonu”), `min_dock_depth 1.8` („min. glebokosc / prog odleglosci od sciany”).

## Zależności
`rclcpp`, `rclpy`, `nav_msgs`, `std_msgs`, `visualization_msgs`, `geometry_msgs`, `l2_interfaces`, `l3_interfaces` ([[ros-sbt-interfaces]]). Źródło `/scan`: [[fast_scan]] (domyślnie publikuje `/scan`) lub [[pointcloud_to_laserscan]]; w symulacji `scan_rotator` z [[l4sim]] ([[scan_rotator]]). Odbiorcy: [[basic_controller]] (`SteerToDock`, `UpdateSlipDetections`), [[pose_graph]] (`/dock_polygon`).

## Interfejsy
Brak własnych (`Dock.msg` w `l2_interfaces`).

## Różnice między branchami
- `master`/`bash_do_uruchomienia/l2dp`: brak `config/` i `launch/`, brak parametrów, `/choose_dock` i `/dock_polygon` (dodane na linii Njord: „segment_scan njord mati troubleshooting” `ce292f4`, `0d444ff`, `f8f0b00`).
- `origin/celka_odo`: zmiany `segment_scan.cpp` (+55/−…) z Monako — **niepewne**, nieprzeanalizowane.
- `origin/marina_lookout` (2025-12): „changes in segment scan, marina entry added” — wtedy powstał detektor mariny, dziś zakomentowany.
- Niezacommitowane w `l2dp_docking_fix`: stempel wielokąta = stempel skanu (`last_scan_stamp`) zamiast `now()` — potrzebne do interpolacji pozy w GlobalMapBuilder.

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.
