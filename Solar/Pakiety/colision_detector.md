---
typ: solar-pakiet
repo: "[[l2dp]]"
build: ament_cmake
tags: [solar, pakiet]
zaktualizowano: 2026-10-04
---
# colision_detector

> [!skrot] W 30 sekund (co robi, wejście → wyjście)
> Wykrywanie kolizji z **poruszającym się obiektem** (inną łodzią) — zadanie „collision” na zawodach Njord. Buduje „tunel” 0,5 m wokół ekstrapolowanego toru własnej łodzi (`/slam/trajectory_extrapolation` z [[pose_graph]]), śledzi klaster przeszkody z `/global/clusters`, z 3 kolejnych położeń estymuje prędkość i prędkość kątową obcej łodzi, ekstrapoluje jej łuk (tunel 1,0 m) i sprawdza przecięcie (Boost.Geometry). Wynik: punkt `/colision_detector` (0,0 = bezpiecznie) i kierunek obejścia `/colision_direction` — konsumuje je `NextWaypoint` w [[basic_controller]].

Opis wg `l2dp_docking_fix@0d444ff` (ostatnia zmiana: `cb93270` „colision avoidance smigaaaaa…”, 2026-08-12).

## Węzły
### `colision_detector_node` (klasa `ColisionDetector`)
- Źródło: `colision_detector/src/colision_detector.cpp`, `include/colision_detector.hpp`, `src/main.cpp`. Executable `colision_detector`.
- **Subskrybuje:** `/global/clusters` (`l1_interfaces/VectorOfClusters`), `/slam/trajectory_extrapolation` (`geometry_msgs/PoseArray`), `/slam/odom` (`nav_msgs/Odometry` — pozycja i kurs `-atan2`, `:29–40`).
- **Publikuje:** `/colision_detector` (`geometry_msgs/Point`; współrzędne zamienione x↔y, 0/0 = brak kolizji), `/colision_direction` (`std_msgs/String`), `/tunnel_marker` (`visualization_msgs/Marker` — tunel własny zielony/czerwony, tunel obcej łodzi, kula w punkcie kolizji).
- Brak parametrów (stałe w kodzie).

**Logika:**
- `TrajectoryCallback` (`:50–106`): `LineString` z punktów trajektorii (z zamianą x↔y), `bg::buffer` z promieniem 0.5 m → `tunnel_polygons`.
- `ClusterCallback` (`:116–308`): ignoruje, gdy poza łodzi = 0; próbkuje co ≥0,2 s; jeśli klaster przeskoczył więcej niż `6.0 m/s · dt`, uznaje go za nowy obiekt; po zebraniu 3 położeń liczy średnią prędkość i prędkość kątową, generuje 20 punktów łuku (`x = clu.x + v/ω·(sin(h+(i/2+0.3)ω) − sin h)`, analogicznie y, `:169–175`), tunel 1.0 m; przecięcie z tunelem własnym → centroid przecięcia.
- Gdy różnica kursów <30° (obiekt płynie w tym samym kierunku) punkt kolizji = centroid przecięcia, w przeciwnym razie = pozycja klastra; **w obu gałęziach** `/colision_direction = "right"` (`:256–266`).
- Brak kolizji → publikuje (0,0) i „Safe.”.

## Launch
Brak; `ros2 run colision_detector colision_detector`.

## Konfiguracja (yaml)
Brak. Stałe: tunel własny 0.5 m, tunel obcej łodzi 1.0 m, 3 próbki, próg „nowego obiektu” 6 m/s, min. odstęp próbek 0.2 s.

## Zależności
`rclcpp`, `rclpy`, `nav_msgs`, `std_msgs`, `visualization_msgs`, `geometry_msgs`, `l1_interfaces`; Boost (Geometry). Dane z [[pose_graph]]; odbiorca [[basic_controller]] (drzewo `CollisionNjord` — `RetryWithRecoveryUntilSuccess` + `SetBuoyWaypoints` z `directions={safe_passage}` i `side_offset 5.0`). Starsza wersja: [[obstacle_avoidance]]. Zakomentowana obsługa w [[trajectory_planner]].

## Interfejsy
Brak własnych.

## Różnice między branchami
- `master`/`bash_do_uruchomienia/l2dp`: wejścia `/l1/obstacle_detection` (zamiast `/global/clusters`), `/trajectory` z [[odometry]] (zamiast `/slam/trajectory_extrapolation`) i `/odom` (zamiast `/slam/odom`); brak `/colision_direction` (`git show origin/master:colision_detector/src/colision_detector.cpp:12–27`).
- `cb93270` (lokalna `spirala_zjebania`, Jetson) vs `origin/feat/dock_costmap_fix` (`b27f9c6`): różnica 45 linii w `colision_detector.cpp` — wersja z `cb93270` jest w gałęzi zawodów.
- `origin/celka_odo` (Monako): −112/+… linii w `colision_detector.cpp` — **niepewne**, osobna wersja.

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.
