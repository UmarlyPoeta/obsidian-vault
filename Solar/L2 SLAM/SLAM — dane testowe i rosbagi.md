---
typ: solar-slam
tags: [solar, slam, l2]
zaktualizowano: 2026-10-08
---
# SLAM — dane testowe i rosbagi

> [!skrot] W 30 sekund
> - Lokalnie są **dwa bagi** z prawdziwej łodzi (sqlite3, ROS 2 Humble):
>   - `docking_regulator_2`: 48 s, dokowanie z 2026-08-08. Kopie w `~/solar/rosbagi/` i `~/ros2-humble-gpu/ws/bags/`.
>   - `bag_islamista`: 270 s, 9.06 GB, dane z 2026-08-08 ~07:49, `~/ros2-humble-gpu/ws/bag_islamista/`.
> - Oba mają tylko `/odom` + `/lidar_points` (+ `/fused_detection` albo `/costmap`, `/path`, `/trajectory`). **Brak `/imu/data`, `/filter/velocity`, `/gnss`**. SLAM da się puścić (potrzebuje tylko `/odom` i `/lidar_points`), ale odometrii i czynnika GNSS już nie.
> - **Do testu loop closure nadaje się `bag_islamista`**. Łódź przez 4.5 min krąży po obszarze ~19 × 14 m (155 m drogi) i wielokrotnie wraca w te same miejsca: pary keyframe'ów < 0.2 m od siebie i 60–130 s później, np. t = 105 s ↔ 217 s (0.10 m), 46 s ↔ 143 s (0.13 m, różnica kursu 34°). Policzone z `/odom` w bagu skryptem bez ROS-a.
> - `docking_regulator_2` daje tylko jeden powrót, pod prąd: t = 21 s ↔ 45 s, 0.03 m, kurs różny o ~140°. Nadaje się na test pętli „z przeciwnego kierunku”.
> - Pułapka: w `bag_islamista` stemple nagłówków są o **74 283 s (20.6 h) starsze** niż czas nagrania (bag nagrany z odtwarzania), więc `--clock` z baga nie zgadza się ze stemplami.

Powiązane: [[SLAM — historia i gałęzie]], [[L2 — odometria, sensory i fuzja]], [[L2 — konsumenci pozy i mapy (co się stanie po korekcie)]], [[pose_graph]].

Metoda: bagi czytane wyłącznie przez `sqlite3` w trybie tylko do odczytu, CDR dekodowany ręcznie (`struct`). Skrypty: `/tmp/claude-1000/-home-patryk/6eff1144-6ce4-4d8c-9ac9-7777f07c88ff/scratchpad/slam/bag/{odom.py,twist.py,revisit.py,lidar.py,lidar2.py,span.py}` (scratchpad sesji, nietrwały). ROS-a nie uruchamiano.

## Inwentarz

| bag | ścieżka | start (czas nagrania) | czas | topiki (liczba wiadomości, częstotliwość) |
|---|---|---|---|---|
| `docking_regulator_2` | `~/solar/rosbagi/docking_regulator_2/scan_08082026_bag_0.db3` (1.6 GB); ta sama kopia w `~/ros2-humble-gpu/ws/bags/docking_regulator_2/` | 2026-08-08 03:23:51 CEST | 48.37 s | `/odom` 661 (13.7 Hz), `/lidar_points` 484 (10.0 Hz), `/costmap` 484 (10 Hz, z [[mapper]]), `/trajectory` 480, `/path` 84 (1.7 Hz), `/map_detections` 0 |
| `bag_islamista` | `~/ros2-humble-gpu/ws/bag_islamista/bag_islamista_0.db3` (9.06 GB) | nagranie 2026-08-09 04:27:28 CEST; stemple nagłówków 2026-08-08 07:49:25 CEST | 269.9 s (stemple: 271.3 s) | `/odom` 3426 (12.7 Hz), `/lidar_points` 2714 (10.0 Hz), `/fused_detection` 1077 (`l1_interfaces/VectorOfFusedDetections`, 4 Hz, `frame_id = hesai_lidar`) |

Bagi wspomniane, ale **niedostępne lokalnie**:
- `docking_trajectory` (127 s) i `docking_parallel_bag` (41 s) w `~/ros2_ws/rosbagi` na innej maszynie (transkrypt `~/solar/2026-09-16-200748-partner-with-nav2-sponsorship-opportunities-pr.txt:2722`, `3415-3430`: „Bagi z dockingu nie mają `/scan` — mają tylko `/lidar_points` i `/odom`”).
- Planowane w `sbt_transforms/KURS_OSI_I_MIGRACJA.md:660-668`: `bag_real_straight`, `bag_real_turn`, `bag_real_buoys`. Komenda nagrania obejmuje `/imu/data /filter/velocity /filter/euler …`. Nie znaleziono ich w `~/solar` ani w `~/ros2-humble-gpu` (**niepewne**, czy powstały).

### Szczegóły formatu (z pierwszych wiadomości)
- `/lidar_points`: `frame_id = hesai_lidar`, `height = 1`, `width = 128000` (chmura nieuporządkowana). Pola: `x,y,z,intensity` float32 (offset 0/4/8/12), `ring` uint16 (16), `timestamp` float64 (18). Opóźnienie nagrania względem stempla: 13–19 ms (`docking_regulator_2`).
- `/odom`: `frame_id = odom`, `child_frame_id` puste. Odstępy stempli: mediana 61–66 ms, maksimum 170–240 ms. Pary prędkość + GNSS mają odstęp ~0.3 ms (zob. [[L2 — odometria, sensory i fuzja]]).
- `/costmap` (mapper): rozdzielczość 0.30 m, 333 × 333, origin (−49.95, −49.95), `frame_id = map`.

## Trajektorie (z `/odom`, x = północ, y = wschód, metry od 1. fixa)

### `docking_regulator_2`
- Obwiednia: N ∈ [−8.28, −4.63], E ∈ [9.94, 17.84]. Droga: 23.7 m.
- Przebieg: 0–32 s na wschód (kurs ~90°, 0.2 m/s), zakręt przez północ (32–41 s, kurs 55° → 332°), powrót na południowy zachód (44–48 s, kurs ~228°, szybciej).
- Największe kroki pozycji: 0.25 m przy t ≈ 44 s (przyspieszenie).
- Powrót: t = 21.45 s ↔ 44.90 s w odległości 0.034 m, kursy ~93° i ~228°.

### `bag_islamista`
- Obwiednia: N ∈ [19.5, 38.9], E ∈ [3.0, 16.6]. Droga: 155.3 m w 270 s (średnio 0.58 m/s). Łódź zawraca i krąży (kurs co 10 s skacze między ~50° a ~340°).
- Największe kroki: 0.44 m przy t ≈ 254 s (skok GNSS albo szybki manewr, **niepewne**).
- Powroty w odległości < 3 m przy odstępie > 30 s: 2196 par próbek 1 Hz. Najlepsze, rozdzielone o > 60 s i < 1 m:

| t₁ [s] | t₂ [s] | odległość [m] | różnica kursu [°] |
|---|---|---|---|
| 105 | 217 | 0.10 | 72 |
| 152 | 269 | 0.10 | 88 |
| 46 | 143 | 0.13 | 34 |
| 166 | 249 | 0.13 | 100 |
| 82 | 212 | 0.16 | 89 |
| 107 | 198 | 0.22 | 85 |
| 97 | 213 | 0.35 | 45 |
| 85 | 204 | 0.45 | 120 |
| 111 | 208 | 0.84 | 23 |
| 28 | 135 | 0.88 | 172 |

> [!intuicja] Jak to czytać
> Odległości liczę z `/odom`, który jest przyklejony do GNSS. „0.10 m” oznacza, że GNSS mówi „to samo miejsce”. Prawdziwa rozbieżność mapy SLAM-u w tych chwilach to dokładnie to, co pętla ma wykryć i poprawić. Różnica kursu 70–120° oznacza, że lidar 360° widzi te same obiekty, ale obrócone. Detektor pętli musi być **niezmienniczy względem obrotu** (ScanContext jest, ICP z prior yaw z IMU też zadziała). Pary z małą różnicą kursu (46 ↔ 143 s, 111 ↔ 208 s) są najłatwiejsze na pierwszy test.

> [!wazne] Krótka historia, mały dryf
> 4.5 min i 155 m to mało, żeby front-end mocno zdryfował. Test pokaże raczej, czy pętla **nie psuje** (fałszywe dopasowania, skoki), niż ile poprawia. Do pomiaru zysku potrzebny jest dłuższy bag (≥ 15 min, kilka okrążeń trasy), nagrany razem z `/gnss`, `/imu/data` i `/filter/velocity`.

## Co trzeba odtworzyć na PC (Docker `~/ros2-humble-gpu`) — NIE uruchamiano

W kontenerze jest już przygotowany workspace: `~/ros2-humble-gpu/ws/src/l2dp_docking_fix` (osobny klon, HEAD `0d444ff`, te same zmiany robocze; `diff -rq` dla `pose_graph` nie pokazuje różnic) i zbudowany `install/pose_graph`. Gotowy łańcuch dla doku: `~/ros2-humble-gpu/ws/tools/run_dock_test.sh`, który uruchamia `pointcloud_to_laserscan` → `segment_scan` → `pose_graph feature --params-file …/slam.yaml -p use_sim_time:=true`, a potem `ros2 bag play --clock`.

> [!zadanie] Procedura testu loop closure (do wykonania ręcznie)
> 1. `cd ~/ros2-humble-gpu && ./ros2dev shell`, w środku `source ~/ws/install/setup.bash`.
> 2. Węzeł SLAM jak w `run_dock_test.sh`: `ros2 run pose_graph feature --ros-args --params-file ~/ws/src/l2dp_docking_fix/pose_graph/config/slam.yaml -p use_sim_time:=true`. Na start z `-p debug.publish_clouds:=true` (`slam.yaml:8-9`), żeby widzieć `/slam/debug/*`.
> 3. Nagrać wyjście: `ros2 bag record /slam/odom /slam/boat_pose /slam/debug/map_to_odom /slam/odom_transform /slam/local_map_points /slam/global_map_points /slam/global_costmap -o slam_out_<data>`.
> 4. Odtworzyć: `ros2 bag play ~/ws/bag_islamista --clock --rate 0.5` (sqlite 9 GB; przy problemach z I/O `--read-ahead-queue-size`).
> 5. Ocena bez pętli (baseline): rozjazd `T_map_odom` w parach z tabeli wyżej, czyli $\lVert t(T_{map\_odom}(t_2)) - t(T_{map\_odom}(t_1))\rVert$ z `/slam/debug/map_to_odom`. Dla zgodnej mapy i dobrego GNSS powinien być bliski 0. Do tego wizualnie „podwójne ściany” w `/slam/global_map_points` w miejscach powrotów.
> 6. Z pętlą: te same miary plus liczba przyjętych/odrzuconych pętli i maksymalny skok `T_map_odom` (wymagania progów w [[L2 — konsumenci pozy i mapy (co się stanie po korekcie)]]).

> [!blad] Pułapka czasu w `bag_islamista`
> Stemple nagłówków (`/odom`, `/lidar_points`, `/fused_detection`) są o 74 283 s starsze niż czas nagrania (`span.py`, `lidar2.py`). Przy `--clock` zegar ROS-a idzie więc ~20.6 h przed stemplami. Front-end liczy na stemplach nagłówków i to mu nie przeszkadza. `GlobalMapBuilder` porównuje jednak `node_.now()` ze stemplami: przycinanie warstwy na żywo (`global_map_builder.cpp:2232-2240`, `prune_time = max(now, latest_live_stamp)`) i timeout wielokąta doku (`2193-2200`, `1692`). Prawdopodobnie cała warstwa przeszkód na żywo zostanie przycięta od razu, a wielokąty doku wygasną (**do sprawdzenia**). Rozwiązania: przepisać czasy nagrania na stemple nagłówków (np. skryptem `rosbag2_py`, reader → writer z `timestamp = header.stamp`) albo odtwarzać bez `--clock` z `use_sim_time:=false` (wtedy z kolei `now()` = październik 2026). Pierwsze jest czyste. `docking_regulator_2` nie ma tego problemu (różnica 0.3–19 ms).

### Czego brakuje w bagach i co nagrać przy następnym wyjściu na wodę
| brak | po co | uwagi |
|---|---|---|
| `/gnss` (`NavSatFix` z kowariancją) | czynnik GNSS w grafie, ocena dryfu względem GNSS | `/odom` ma GNSS „wtopiony” z R = 0.005 |
| `/imu/data`, `/filter/velocity`, `/filter/euler` | ponowne puszczenie [[odometry]] z poprawkami (stemple, R z kowariancji) | bez tego `/odom` z baga jest „zamrożony” |
| `/tf_static` lub zmierzona ekstrynsyka | weryfikacja `T_imu_lidar` | dziś tylko w `slam.yaml:11-19` |
| `/slam/*` z łodzi | porównanie z odtworzeniem na PC (determinizm) | na łodzi SLAM działał od 2026-08-09; bagów z `/slam/*` nie ma |
| trasa | ≥ 2 pełne okrążenia tej samej pętli ≥ 200 m, w tym przejście obok nabrzeża w obu kierunkach | żeby dryf zdążył urosnąć |

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.
