---
typ: solar-dokument
warstwa: narzędzia
tags: [solar, dokumentacja, raport]
zaktualizowano: 2026-10-04
---
# Dokumenty i raporty

> [!skrot] W 30 sekund
> - `~/solar/docs/architektura.html` to przewodnik po całym stosie ROS 2 łodzi (L0–L4, pipeline od ramki CAN do śruby, pułapki osi). Napisany wcześniej z lektury kodu (stan na 11.08.2026). **To dokument pomocniczy, nie źródło prawdy.** Część komend jest w nim błędna (niżej).
> - `~/solar/docs/dock_costmap_check/` — 6 obrazków PNG z testu „gaszenia inflacji na doku” w costmapie SLAM-u (gałąź `feat/dock_costmap_fix` w `l2dp_docking_fix`). Szczegóły w [[l2dp]].
> - `~/solar/2026-09-16-200748-partner-with-nav2-sponsorship-opportunities-pr.txt` — **to nie dokument o partnerstwie z Nav2**, tylko eksport sesji Claude Code o naprawie inflacji na doku. Nazwa pliku wzięła się z pierwszej linii wklejonej strony dokumentacji Nav2.

Powiązane: [[Uruchamianie i launch]] · [[Środowisko developerskie (Docker ROS2)]] · [[l2dp]] · [[sbt_transforms]].

## `docs/architektura.html` — „Stos ROS 2 łodzi — architektura i pipeline”
78 kB, samodzielna strona HTML (diagramy mermaid w kodzie). Stopka: „Dokument wygenerowany z lektury kodu w `~/ros2_ws/src`. Wszystkie odniesienia plik:linia odpowiadają stanowi repozytorium na 11 sierpnia 2026”. Nagłówek podaje: 45 pakietów ROS, 265 miejsc pub/sub w C++, 5 warstw L0–L4, „ROS 2 Humble / Jazzy”.

Co zawiera (rozdziały):
1. **Mapa warstw** — L0 [[l0hwd]] (sterowniki, CAN), L1 [[l1od]] (percepcja), L2 [[l2dp]] (lokalizacja, mapa, planowanie, regulacja), L3 [[l3nav]] (drzewo zachowań), L4 [[ros-sbt-interfaces]] (wiadomości) + pakiety poboczne ([[sbt_transforms]], `fusion_camera_lidar`, [[dongle-service]], [[agh-dashboard-live]]). Diagram pętli sterowania.
2. **Skąd biorą się dane** — `ros2_socketcan` (`/from_can_bus`, `/to_can_bus`), `CanDriver` dekoduje ramki z DBC; pułapka: publikacje IMU/GNSS w `can_driver.cpp` są zakomentowane, a `/imu/data`, `/gnss`, `/filter/*` daje sterownik Xsens po USB. LiDAR Hesai (`/lidar_points`), ZED 2i, echosonda ECT400, mikrofon (`blast_detection`).
3. **L1 percepcja** — `vision_detector` (YOLO w Dockerze) + `obstacle_detector` (PCL) → `detection_pointcloud_fusion` → `/fused_detection` → `tracking_detection`.
4. **L2 lokalizacja** — `odometry` (Kalman 4-stanowy), `pose_graph` (SLAM LOAM-podobny: `/slam/boat_pose`, `/slam/global_costmap`), starszy `mapper`; pułapka: dwie costmapy (`/costmap` vs `/slam/global_costmap`) i dwa publishery `tracked_detections`.
5. **L2 planowanie** — `planner` (A* na `OccupancyGrid`), `trajectory_planner` (w praktyce regulator kursu co 200 ms: `speed = exp(0.015·d) − 0.95`), tabela węzłów pomocniczych.
6. **L3** — `basic_controller` (BehaviorTree.CPP v4, XML-e z `config/` ładowane automatycznie, `main_tree_name`), blackboard (`waypoint_vector`), węzeł `NextWaypoint` i przełącznik `use_mapper`.
7. **L0 wykonanie** — napęd różnicowy (`lewa = linear + angular`, `prawa = linear − angular`), trzy węzły piszące na `/to_can_bus`; pułapka `angular/=1,5;` w `new_diff_thrust_driver.cpp:58` (operator przecinka).
8. **Życie waypointa** — diagram sekwencji.
9. **Osie** — konwencje domowe NEU/FRU zamiast REP-103, `-atan2` w ~30 plikach, `sbt_transforms` jako jedyne miejsce na konwersje.
10. **Telemetria** — `message_protobuf`, TUI drzewa (`/bt_tui_state`), dashboard, dongle.
11. **Uruchamianie i diagnostyka** — kolejność startu, zmienne środowiska, tabela „objaw → gdzie szukać”.
12. **Ścieżka nauki** — 6 plików do przeczytania + ćwiczenia; ściąga „kto co publikuje”.

> [!blad] Co się nie zgadza z kodem (sprawdzone)
> - §11: `ros2 run odometry OdoCpp` → executable to `odo_cpp_node` (`l2dp/odometry/CMakeLists.txt`); `pose_graph <exec>` → `feature`; `trajectory_planner <exec>` → `TrajectoryPlannerNode`.
> - §10: „dongle-service … nie zawiera węzłów ROS” — gałąź `Dongle-Statistic` ma pakiet `dongle_statistic` z węzłem `dongle_statistic_node`.
> - §1: „L4 = ros-sbt-interfaces” — w kodzie jest też repo [[l4sim]] (symulacja), którego dokument nie opisuje.
> - Dokument odnosi się do stanu z 11.08.2026. Po zawodach (kopia z Jetsona, 23.08) gałęzie się zmieniły, np. `l0hwd/regulator` słucha `/slam/odom` (sesja z pliku txt, linie ~5260). Każdą liczbę i linię sprawdzaj w kodzie.

## `docs/dock_costmap_check/` — test gaszenia inflacji na doku
| Plik | Rozmiar | Co pokazuje |
|---|---|---|
| `rviz_przed_po.png` | 123 kB | trzy kadry RViz: „1. przed detekcją: slip zalany inflacją”, „2. po detekcji + lidar: wielokąt i korytarz”, „3. koniec baga: slip i korytarz wolne” |
| `before_after.png` | 129 kB | costmapa przed/po — po prawej zielony wielokąt doku z wyczyszczonym wnętrzem |
| `dock_check.png`, `F.png`, `lid12.png`, `s12_crop.png` | 17–57 kB | kolejne zrzuty z przebiegów (`F.png`, `lid12.png` = kopie z `~/ros2-humble-gpu/ws/tools/` i `tools/shots/`) |

Powstały skryptami `~/ros2-humble-gpu/ws/tools/run_dock_test.sh` / `rviz_shot.sh` na bagu `docking_regulator_2` (patrz [[Środowisko developerskie (Docker ROS2)]]). Sama zmiana: `segment_scan` publikuje wielokąt doku (`geometry_msgs/PolygonStamped`), a `global_map_builder` w `pose_graph` zeruje koszt wewnątrz wielokąta i obniża go w pasie 0,5 m wokół (commity `f8f0b00` „Gaszenie inflacji wewnatrz wykrytego slipu” i `0d444ff` na `feat/dock_costmap_fix`). Opis szczegółowy — [[l2dp]] / [[pose_graph]] / [[segment_scan]].

## Plik `2026-09-16-200748-partner-with-nav2-sponsorship-opportunities-pr.txt`
323 kB, 5812 linii, eksport rozmowy z Claude Code (Opus 5, katalog `~/ros2_ws/src` na laptopie). Ta sama kopia leży w `~/ros2-humble-gpu/ws/src/`. Przebieg:

1. **Linie 6–174:** wklejona strona dokumentacji Nav2 „Navigation Concepts” (action servery, lifecycle, behavior trees, costmapy) i objaśnienie global vs local costmap — stąd nazwa pliku.
2. **Linie 175–1300:** cel rozmowy: gdy `segment_scan` wykryje dok, zmniejszyć inflację w tym miejscu `/slam/global_costmap`. Ustalenia: `/dock_detection` był martwy — `segment_scan` publikuje `l2_interfaces/msg/Dock`, a konsumenci w L3 (`steer_to_dock.cpp`, `update_slip_detections.cpp`) subskrybują `geometry_msgs/msg/Point`; `segment_scan` publikuje w ramce LiDAR-u, costmapa jest w `map`.
3. **Linie 1308–2780:** implementacja (nowy topik z wielokątem `PolygonStamped`, zmiany w `segment_scan` i `pose_graph/global_map_builder`), plan, przegląd, budowanie `colcon build --packages-select segment_scan pose_graph`.
4. **Linie 2681–4900:** weryfikacja na bagach `docking_trajectory` i `docking_regulator_2`: konfiguracje RViz `dock_map.rviz`/`dock_lidar.rviz`, problem „wielokąty na wodzie” (zła transformacja LiDAR→mapa), test 18 kombinacji znaków/zamian osi, przejście na wzór z `update_slip_detections.cpp:45-49`, uwaga o `viz_correction` (republikuje `/path→/path_2`, `/map_detections→/map_detections_2` z zamienionymi osiami — zespół nie używa TF).
5. **Linie 5217–5400:** przegląd stosu po skopiowaniu z Jetsona („stan po zawodach”, drzewa z Njord w Trondheim najaktualniejsze): gałęzie l0hwd `master`, l1od `refined-fusion`, l2dp `spirala_zjebania`, l3nav `waypoint_first`; odkrycie, że `NextWaypoint slam_costmap="false"` przy wjeździe do slipu wyłącza costmapę w plannerze (`costmap.data[...]*slam_costmap`), czyli dziś łódka wjeżdża do doku „na ślepo”; `l0hwd/regulator` słucha `/slam/odom`; nieśledzony `l2_interfaces/msg/DockPolygonStamped.msg`.
6. **Linie 5400–5812:** dokument podsumowujący (artefakt), rebase pracy z `l2dp_docking_fix` na jetsonowe `spirala_zjebania` (`cb93270`) i dodanie marginesu 0,5 m wokół wielokąta (`dock_margin_ = 0.50`, `dock_margin_cost_ = 25` w `global_map_builder.hpp`).

Wniosek: plik to dziennik prac nad `feat/dock_costmap_fix` — przydatny jako uzasadnienie zmian w [[l2dp]], nie jako dokumentacja Nav2 (Nav2 nie jest używany w stosie; wspomina o nim tylko `NewDiffThrustDriver` na `/cmd_vel` w `docs/architektura.html` §7).

## Źródła
- `~/solar/docs/architektura.html`
- `~/solar/docs/dock_costmap_check/*.png`
- `~/solar/2026-09-16-200748-partner-with-nav2-sponsorship-opportunities-pr.txt`
- `~/solar/l2dp_docking_fix` (`git log`), `~/ros2-humble-gpu/ws/tools/`

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.
