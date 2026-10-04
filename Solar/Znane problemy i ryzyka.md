---
typ: solar
tags: [solar, problemy]
zaktualizowano: 2026-10-04
---
# Znane problemy i ryzyka (zbiorczo)

> [!skrot] W 30 sekund
> - Zebrane z analizy kodu wszystkich repo (2026-10-04, stan po zawodach). Każdy punkt ma źródło (plik:linia) i link do notatki, w której jest opisany szerzej.
> - „Błąd” = wynika wprost z kodu. „Ryzyko” / „niepewne” = może być problemem, zależnie od tego, jak łódź faktycznie jest uruchamiana.
> - Przed zawodami / startem łodzi: przejdź sekcję **Krytyczne**.

## Krytyczne — kod tylko lokalnie, bez kopii na GitLabie
> [!wazne] Stan z zawodów istnieje tylko na dysku
> - [[l3nav]]: gałąź `waypoint_first` nie ma odpowiednika na GitLabie, a drzewo robocze ma dodatkowo niezacommitowane zmiany (SelectDock, `/which_map`, `/regulator_speed`, `spisslonn.xml`).
> - [[l2dp]]: 2 niewypchnięte commity na `spirala_zjebania` (+ niezacommitowane zmiany = stan z Jetsona) i 5 na `feat/dock_costmap_fix` w kopii `l2dp_docking_fix` (+ niezacommitowany korytarz podejścia do slipu).
> - [[dongle-service]]: `ROS_DOMAIN_ID=69` i ustawienia Cyclone są tylko w niezacommitowanej kopii roboczej (w gicie `0`).
> - `sbt_transforms` w ogóle nie jest repozytorium git ([[Repozytoria/sbt_transforms]]).
> - Szczegóły gałęzi: [[Branche — przegląd]].

## L0 — sterowanie i CAN ([[l0hwd]], [[Magistrala CAN]])
- **Błąd (prawdopodobny crash):** `l0hwd/can_driver/src/can_driver.cpp:577` publikuje na niezainicjalizowanym `euler_pub`. Gdy XSENS nada ramkę `EULER_ANGLES_XSENS`, węzeł najpewniej padnie. [[can_driver]]
- **Ryzyko:** ramkę `MOTOR_SPEED_SP_HL` (0x05) wysyłają 4 węzły: NewDiffThrustDriver, DiffThrustDriver, Regulator, can_sequence. Przełącznik `/regulator_or_thrust_driver` (z l3nav) obsługują tylko Regulator i DiffThrustDriver, a NewDiffThrustDriver z launchu nadaje stale co 50 ms (`new_diff_thrust_driver.cpp:47`).
- **Błąd:** `new_diff_thrust_driver.cpp:58` ma `angular/=1,5`, czyli operator przecinka, więc dzielenie przez 1. [[new_diff_thrust_driver]]
- **Błąd (niezacommitowany):** `regulator.cpp:30`, lokalne `bool use = true` przesłania pole klasy. **Niepewne:** `regulator.cpp:185` podaje lewy i prawy ciąg w odwrotnej kolejności. [[regulator]]
- **Niepewne:** DLC 8 bajtów w węzłach, a w DBC 4. Bitrate `can0` nie jest nigdzie skonfigurowany w repo.
- **Literówka:** `l0hwd_launch.py:5` importuje `PythonLaunchArgumetsSource` (nie sprawdzone, czy import przechodzi). [[l0_launch]]
- Gałąź `music`: CanDriver subskrybuje `/melody` jako `UInt16`, a l3nav publikuje `Int16`, więc się nie połączą.

## L1 — percepcja ([[l1od]], [[Sensory i sprzęt]])
- **Błąd logiczny:** na `refined-fusion` nowa fuzja ([[detection_pointcloud_fusion]]) nigdy nie ustawia `is_fused_angle`, a [[tracking_detection]] przepuszcza tylko detekcje z tą flagą (`tracking_detection.cpp:45`). Tracking publikuje więc puste wektory, a `/tracking_detection` i tak nikt nie subskrybuje.
- **Ryzyko:** `detection_pointcloud_fusion` ma zakodowane intrinsics 960×540 i offset kamery 0,11 m. Nie korzysta z plików kalibracji w `l1od/config/`.
- **Niepewne:** model kamery. Kod L1 mówi „zed2i”, a watchdog domyślnie startuje `zedx` ([[zed-service]]).
- **Błąd:** `zed.service` wywołuje `zed-healthcheck.sh`, który w repo jest pusty (0 B). Logika watchdoga jest w `zed-pls-dont-die.sh`. W unicie jest też luźna linia `BindsTo=`.
- Kopia `l1od` straciła bit `+x` w skryptach Dockera, więc `l1od_launch.py` nie uruchomi `run.sh` bez `chmod +x` ([[Uruchamianie i launch]]).

## L2/L3 — planowanie i misja ([[l2dp]], [[l3nav]], [[Nawigacja i autonomia — przepływ]])
- **Błąd:** [[planner]] `planner.cpp:135`: kara za skręt liczona dzieleniem całkowitym, więc zawsze 0. Planowanie startuje dopiero, gdy przyjdzie `/slam/global_costmap` (`:195–199`).
- **Błąd:** [[colision_detector]] `/colision_direction` jest zawsze „right” (`colision_detector.cpp:256–266`).
- **Błąd typu:** `detected_signals`: [[blast_detection]] publikuje `BlastDetection`, a `my_bt_node` subskrybuje `std_msgs/String`. Drzewo nie dostaje sygnałów.
- **Ryzyko:** drzewa Njord z `use_mapper=false` domyślnie wybierają regulator, a ten potrzebuje `/path_points`, które w tym trybie nie powstają ([[basic_controller]]).
- **Ryzyko:** stos ROS nie egzekwuje trybów killed/manual/auto. `/robot/state` czyta tylko raport `message_protobuf`.
- **Niepewne:** kto publikuje `/scan` na łodzi. [[fast_scan]] czyta `/debug/deskewed`, którego nikt nie publikuje. Brak launchów L2, a `slam.yaml` nie jest instalowany przez CMake.
- Zależności `nav2_*` w `l3nav/basic_controller/package.xml` są nieużywane, bo L2 to własny stos, nie Nav2.

## Sieć, DDS, wdrożenie ([[Uruchamianie i launch]])
- **Ryzyko:** rozjazd `ROS_DOMAIN_ID`: Dockerfile_gpu ma 68, l4sim w Dockerze 42, PC 0, reszta 69.
- **Ryzyko:** CycloneDDS jest zamknięty na `lo`, więc laptop nie zobaczy topików łodzi przez DDS. Podgląd idzie przez dashboard i rosbridge.
- **TF:** na łodzi nikt nie publikuje TF poza mostem [[Pakiety/sbt_transforms|sbt_tf_bridge]], którego nic nie uruchamia. ZED z domyślnymi ustawieniami TF kolidowałby z mostem: dwa źródła `map→odom` ([[Drzewo TF i układy współrzędnych]]).
- [[agh-dashboard-live]]: skrypt startowy szuka `battery_status_publisher.py`, a w katalogu jest `bms_panel.py`. Do tego `package.json` leży w `scripts/`, a `npm run` jest wołane z katalogu głównego.
- **Niepewne:** jak startuje driver Xsens (`/imu/data`, `/gnss`, `/filter/*`) i `hesai_ros_driver`. Obu nie ma w `~/solar`.

## Automatycznie wykryte niezgodności topików
Pełna lista (topiki bez nadawcy lub odbiorcy, różne typy): [[Mapa topików i serwisów]], sekcja „Do sprawdzenia”.

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.
