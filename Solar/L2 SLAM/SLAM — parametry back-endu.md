---
typ: solar-slam
tags: [solar, slam, l2]
zaktualizowano: 2026-10-08
---
# SLAM — parametry back-endu

> [!skrot] W 30 sekund
> Wszystkie parametry `matcher.*` (deklarowane w `FeatureExtractor`, `pose_graph/src/feature_extractor.cpp:353–521`) oraz `global_map.*` i `costmap.*` (deklarowane w `GlobalMapBuilder`, `pose_graph/src/global_map_builder.cpp:227–689`), wszystkie w przestrzeni `feature_extractor.ros__parameters`. Porównanie: domyślna w kodzie / `config/slam.yaml` (drzewo robocze = łódź) / `baseline_params.yaml` / `global_map_builder_parameters.yaml`. Najważniejsze: w slam.yaml wagi matchera 0.5/0.5 (kod: 1.0/0.70), a w builderze `minimum_support_growth_events: 1` + `minimum_static_source_points: 1` → **każdy nowy woksel jest od razu „potwierdzony”**, czyli filtr trwałości jest w praktyce wyłączony. `global_map_builder_parameters.yaml` ma blok `dock:` w złym miejscu (nie pod `costmap`) — byłby ignorowany.

Powiązane: [[SLAM — scan matching (matematyka)]], [[SLAM — degeneracja i jakość dopasowania]], [[SLAM — global map builder i costmapy]], [[pose_graph]].

Skróty: **SM** = `src/scan_matcher.cpp`, **FE** = `src/feature_extractor.cpp`, **GMB** = `src/global_map_builder.cpp`, **hpp** = `include/global_map_builder.hpp`. „—” = brak w pliku (obowiązuje domyślna). Pliki yaml: `slam.yaml` = `config/slam.yaml` (224 linie, drzewo robocze), `baseline` = `baseline_params.yaml` (bez sekcji buildera), `gmb.yaml` = `global_map_builder_parameters.yaml` (tylko builder).

## Matcher (`matcher.*`)

Domyślne z `ScanMatcherConfig` (`include/scan_matcher.hpp:80–127`). W slam.yaml i baseline **wszystkie wartości matchera są identyczne** (slam.yaml:49–95, baseline:64–107). Walidacja w FE: tylko `max` dla iteracji, liczników i wag oraz clamp rzędu; reszta bez sprawdzenia znaku.

| Parametr | Kod | slam.yaml | Jedn. | Co robi / gdzie | Ryzyko złej wartości |
|---|---|---|---|---|---|
| `maximum_iterations` | 8 (≥1, FE:355) | 8 | – | limit iteracji GN, SM:650 | za mało → brak zbieżności (i tak akceptowane); za dużo → czas callbacku LiDAR-u |
| `minimum_total_matches` | 80 | 80 | – | min. łącznie dopasowań, SM:668, 881 | za wysoko → częste `not_enough_matches` na otwartej wodzie |
| `minimum_plane_matches` | 40 | 40 | – | wsparcie typu, SM:300, 308 | przy obu typach działa OR z liniami |
| `minimum_line_matches` | 20 | 20 | – | wsparcie typu, SM:302, 314 | j.w. |
| `maximum_plane_distance` | 0.25 | 0.25 | m | bramka $|n^T(p-c)|$ w `findNearestPlane`, SM:342 | za duże → złe korespondencje; za małe → brak przy dryfie |
| `maximum_line_distance` | 0.35 | 0.35 | m | bramka odległości od linii, SM:430 | j.w. |
| `maximum_center_distance` | 0.80 | 0.80 | m | bramka odległości od centroidu woksela; też promień przeszukiwania $\lfloor d/0.4\rfloor+1 = 3$ → 343 woksele/punkt (`local_voxel_map.hpp:458–467`), SM:343, 431 | większe = sześcienny wzrost kosztu |
| `plane_huber_delta` | 0.10 | 0.10 | m | próg Hubera, SM:380 | ≤0 wyłącza Hubera (`SM:102`) |
| `line_huber_delta` | 0.15 | 0.15 | m | SM:480 | j.w. |
| `plane_weight` | **1.0** (≥0, FE:424) | **0.5** | – | waga typu; 0 wyłącza płaszczyzny, SM:295, 330, 395, 542 | skaluje $H$ → przesuwa progi obserwowalności |
| `line_weight` | **0.70** (≥0, FE:418) | **0.5** | – | j.w. dla linii, SM:418, 495 | j.w. |
| `minimum_plane_horizontal_normal` | 0.08 | 0.08 | – | min $\|n_{xy}\|$, SM:348 | za duże → tracimy pochylone płaszczyzny |
| `damping` | 1e-4 | 0.0001 | – | $\lambda I$ w zredukowanym układzie, SM:749 | ujemne → LDLT może się wysypać |
| `maximum_translation_step` | 0.20 | 0.20 | m | obcięcie kroku, SM:792 | za małe → wolna zbieżność |
| `maximum_rotation_step_deg` | 1.5 | 1.5 | ° | obcięcie yaw, SM:800 | j.w. |
| `translation_convergence` | 0.002 | 0.002 | m | stop, SM:839 | – |
| `rotation_convergence_deg` | 0.02 | 0.02 | ° | stop, SM:841 | – |
| `odometry_prior_sigma_xy` | 0.30 (≥1e-6, SM:713) | 0.30 | m | prior $\Lambda_{xx} = 1/\sigma^2$, SM:725–729 | małe → matcher „przyklejony” do odometrii |
| `odometry_prior_sigma_yaw_deg` | 2.0 | 2.0 | ° | prior yaw, SM:717, 731 | j.w. |
| `maximum_rmse` | 0.20 | 0.20 | m | bramka `HIGH_RMSE`, SM:896 | RMSE ≤ bramek odległości, więc > 0.35 nic nie daje |
| `minimum_match_ratio` | 0.10 | 0.10 | – | bramka `LOW_MATCH_RATIO`, SM:902 | – |
| `minimum_trusted_match_fraction` | 0.25 | 0.25 | – | bramka `INSUFFICIENT_TRUSTED_REFERENCE`, SM:908 | za wysoko → po długim recovery nigdy nie wraca |
| `maximum_correction_translation` | 0.35 | 0.35 | m | bramka korekty vs predykcja, SM:916 | za nisko → odrzuca prawdziwe korekty po dryfie; blokuje loop closure |
| `maximum_correction_yaw_deg` | 3.0 | 3.0 | ° | SM:918 | j.w. |
| `maximum_final_to_initial_rmse_ratio` | 1.05 | 1.05 | – | bramka `COST_INCREASED`, SM:928 | patrz „Podejrzane” w notatce o matematyce |
| `minimum_translation_information` | 0.01 | 0.01 | – (ważone) | próg $\lambda_i$, SM:198 | zależy od wag! |
| `minimum_translation_eigenvalue_ratio` | 0.02 | 0.02 | – | próg $\lambda_i/\lambda_{max}$, SM:204 | – |
| `minimum_observable_translation_rank` | 1 (clamp 1..2, FE:513–519) | 1 | – | bramka `DEGENERATE_GEOMETRY`, SM:687, 888 | 2 → odrzuca korytarze (ale wtedy bez częściowych korekt) |
| `minimum_yaw_information` | 0.005 | 0.005 | – | próg yaw, SM:238 | – |
| (brak parametru) `minimum_effective_jacobian_norm` | 1e-8 | — | – | odrzut $\|J\|^2$, SM:371, 469 | – |

Parametry `matcher.*` należące do front-endu (próbkowanie wejścia matchera, FE:100–122, użyte FE:2429–2439): `plane_sample_leaf` 0.25 m, `line_sample_leaf` 0.20 m, `maximum_plane_samples` 3500, `maximum_line_samples` 2000 — wszędzie te same wartości.

## Global map (`global_map.*`)

| Parametr | Kod (hpp) | slam.yaml | gmb.yaml | Jedn. | Co robi / gdzie | Ryzyko |
|---|---|---|---|---|---|---|
| `frame_id` | `map` (hpp:366) | map | map | – | wymagana ramka wejścia i wyjść, GMB:1244, 2326, 2769 | inna → wszystko ignorowane |
| `input_topic` | `/slam/local_map_points` | = | = | – | GMB:696 | – |
| `boat_pose_topic` | `/slam/boat_pose` | = | = | – | GMB:705 | – |
| `cloud_topic` | `/slam/global_map_points` | = | = | – | GMB:796 | – |
| `voxel_size` | 0.30 (≥0.05, GMB:247) | 0.30 | 0.30 | m | woksel mapy trwałej, GMB:1312 | mniejsze → więcej wokseli (limit 500k) |
| `cloud_leaf_size` | **0.40** (≥voxel, GMB:255) | **0.30** | 0.40 | m | downsampling chmury do RViz, GMB:2277 | tylko wizualizacja |
| `publish_confirmed_only` | **true** | **false** | false | – | czy publikować niepotwierdzone, GMB:2171 | tylko wizualizacja |
| `use_source_intensity` | true | true | true | – | filtr po `intensity`, GMB:1273, 1329 | false → recovery keyframe’y wchodzą do mapy |
| `minimum_source_intensity` | 0.50 | 0.50 | 0.50 | – | próg (1.0 zaufane, 0.25 recovery), GMB:1306 | >1 → pusta mapa |
| `maximum_voxels` | 500000 (≥1000) | 500000 | 500000 | – | limit wokseli, GMB:1410 | po osiągnięciu nowe przeszkody pomijane |
| `maximum_observations_per_voxel` | 50 (≥1) | 50 | 50 | – | okno średniej kroczącej, GMB:1445–1465 | – |
| `minimum_support_growth_events` | **2** (≥1) | **1** | **1** | – | próg potwierdzenia, GMB:1428–1432, 1481–1486 | **1 = każdy nowy woksel od razu potwierdzony** |
| `tentative_stale_updates` | **3** (≥1) | **10** | 3 | wiad. | wiek niepotwierdzonych do usunięcia, GMB:2087–2092 | przy growth=1 prawie nieużywane |
| `active_clearing_radius` | **30.0** (≥0) | **12.0** | 30.0 | m | promień aktywnego czyszczenia, GMB:1494–1530 | 0 wyłącza czyszczenie (duchy łodzi zostają) |
| `confirmed_misses_before_removal` | **5** (≥1) | **10** | **3** | wiad. | liczba „pudeł” do usunięcia, GMB:1534 | małe → znikają przesłonięte przeszkody |
| `publish_rate_hz` | 2.0 (≥0.1) | 2.0 | 2.0 | Hz | timer (wall), GMB:806–814 | większe → CPU |
| `publish_only_when_changed` | true | true | true | – | GMB:2221–2223, 2255 | costmapa i tak „brudna” przy każdej pozie |

## Costmap (`costmap.*`)

| Parametr | Kod (hpp) | slam.yaml | gmb.yaml | Jedn. | Co robi / gdzie | Ryzyko |
|---|---|---|---|---|---|---|
| `topic` | `/slam/global_costmap` | = | = | – | GMB:801 | planner subskrybuje ten topic |
| `resolution` | 0.20 (≥0.05) | 0.20 | 0.20 | m | komórka, GMB:2416 i dalej | mniejsze → kwadratowo więcej komórek |
| `fixed_size` | false | false | false | – | GMB:2622 | – |
| `origin_x`, `origin_y` | −100, −100 | = | = | m | tylko przy `fixed_size` | – |
| `width_m`, `height_m` | 200, 200 (≥res) | = | = | m | tylko przy `fixed_size` | – |
| `dynamic_padding` | 5.0 (≥0) | 5.0 | 5.0 | m | margines wokół przeszkód i łodzi, GMB:2679–2697 | – |
| `maximum_width_cells`, `maximum_height_cells` | 2000 (≥10) | 2000 | 2000 | kom. | limit (400 m), GMB:2699–2745 | mapa > 400 m przycinana wokół łodzi |
| `obstacle_z_relative_to_boat` | true | true | true | – | $z_{ref}$ = boat z, GMB:2386 | false przy dryfującym z → znikają przeszkody |
| `minimum_obstacle_z` | −0.90 | −0.90 | −0.90 | m | dolna granica (swap gdy min>max, GMB:471), GMB:2408 | za nisko → woda jako przeszkoda |
| `maximum_obstacle_z` | 3.00 | 3.00 | 3.00 | m | górna granica, GMB:2409 | za nisko → mosty itp. pominięte (dobrze) |
| `support_radius` | **0.35** | **0.50** | 0.5 | m | sąsiedztwo agregacji, GMB:2471–2519 | większe → „rozmazanie” |
| `minimum_static_voxels` | 1 | 1 | 1 | – | GMB:2575 | – |
| `minimum_static_support_events` | **2** | **1** | **3** | – | GMB:2576 | 1 → brak filtra trwałości |
| `minimum_static_source_points` | **2** | **1** | 2 | – | GMB:2578 i potwierdzenie GMB:1431 | 1 → pojedynczy punkt potwierdza |
| `minimum_vertical_extent` | **0.10** | **0.05** | 0.10 | m | GMB:2568 | prawie martwe przez compact height |
| `minimum_compact_obstacle_height` | **−0.25** | **−0.45** | −0.45 | m | alternatywny warunek geometrii, GMB:2569–2571, 2590–2592 | blisko poziomu wody → odblaski jako przeszkody |
| `dynamic_obstacles_enabled` | true | true | true | – | warstwa żywa; wyłączana, gdy oba wejścia off (GMB:396–405) | – |
| `dynamic_use_edge_features` | true | true | true | – | GMB:760, 1897, 1976 | – |
| `dynamic_use_plane_features` | true | true | true | – | GMB:773, 1898, 1978 | – |
| `dynamic_edge_topic` | `/features/edges` | = | = | – | GMB:762 | – |
| `dynamic_plane_topic` | `/features/planes` | = | = | – | GMB:775 | – |
| `dynamic_lidar_pose_topic` | `/slam/debug/predicted_lidar_pose` | = | = | – | GMB:786 | poza sprzed korekty |
| `dynamic_voxel_size` | **0.15** (≥0.05) | **0.30** | 0.30 | m | woksel warstwy żywej, GMB:1964 | – |
| `dynamic_obstacle_timeout` | 1.5 (≥0.1) | 1.5 | 1.5 | s | wygasanie, GMB:2109–2125 | długie → ślady ruchomych łodzi |
| `dynamic_minimum_range` | 0.50 (≥0) | 0.50 | 0.50 | m | zasięg od LiDAR-u, GMB:1934–1936 | za mało → własna łódź jako przeszkoda |
| `dynamic_maximum_range` | 100.0 (≥min) | 100.0 | 100.0 | m | j.w. | – |
| `dynamic_maximum_pending_frames` | 12 (≥3) | 12 | 12 | – | bufor parowania, GMB:2070 | – |
| `dynamic_maximum_observations_per_voxel` | 20 (≥1) | 20 | 20 | – | okno średniej, GMB:2025–2045 | – |
| `minimum_dynamic_voxels` | 1 | 1 | 1 | – | GMB:2596 | – |
| `minimum_dynamic_observations` | 2 | 2 | 2 | skany | GMB:2597 | 1 → pojedyncze odblaski |
| `minimum_dynamic_points` | **3** | **2** | 2 | – | GMB:2599 | – |
| `minimum_dynamic_vertical_extent` | 0.08 | 0.08 | 0.08 | m | GMB:2588 | – |
| `inflation_radius` | 1.00 (≥0) | 1.0 | 1.0 | m | GMB:2838, 2868 | – |
| `inscribed_radius` | **0.45** (clamp 0..inflation) | **0.30** | 0.3 | m | koszt 99 do tej odległości, GMB:2873 | – |
| `inflation_cost_scaling_factor` | 3.0 (≥0) | 3.0 | 3.0 | 1/m | wykładnik, GMB:2879–2881 | – |
| `clear_radius_around_boat` | 0.75 (≥0) | **0.0** | 0.0 | m | czyszczenie wokół łodzi, GMB:3042 | >0 kasuje też przeszkody przy burcie |
| `unknown_as_free` | true | true | true | – | tło 0 albo −1, GMB:2784–2788 | – |

### `costmap.dock.*` (slip)

W gmb.yaml blok `dock:` jest wcięty **bezpośrednio pod `ros__parameters`** (`global_map_builder_parameters.yaml:107–113`), więc dałby parametry `dock.enabled` itd., których kod nie deklaruje — **ignorowane**. Wartości w nim (timeout 5.0, extension 1.5) odpowiadają HEAD, nie drzewu roboczemu.

| Parametr | Kod (hpp) | slam.yaml | Jedn. | Co robi / gdzie | Ryzyko |
|---|---|---|---|---|---|
| `enabled` | true | true | – | subskrypcje i gaszenie, GMB:714, 2931 | – |
| `polygon_topic` | `/dock_polygon` | = | – | GMB:744 | – |
| `maximum_cost` | 0 (clamp 0..99) | 0 | – | cap w wielokącie i korytarzu, GMB:3012, 3029 | – |
| `polygon_timeout` | 0.0 (≥0) | 0.0 | s | 0 = bez wygasania, GMB:2195–2211 | wielokąty na zawsze |
| `merge_radius` | 1.0 (≥0) | 1.0 | m | scalanie po centroidzie, GMB:1759 | – |
| `mouth_extension` | 0.0 (≥0) | 0.0 | m | wydłużenie wlotu, GMB:1725–1741 | – |
| `margin` | 0.50 (≥0) | 0.50 | m | pas wokół obrysu, GMB:3015–3020 | – |
| `margin_cost` | 25 (clamp 0..99) | 25 | – | cap w pasie | – |
| `pose_topic` (nowy) | `/slam/odom` | — | – | bufor póz, GMB:622, 726 | – |
| `pose_max_gap` (nowy) | 0.25 (≥0) | — | s | maks. luka interpolacji, GMB:946, 958 | – |
| `choose_dock_topic` (nowy) | `/choose_dock` | — | – | GMB:632, 735 | – |
| `min_width` / `max_width` (nowe) | 1.0 / 3.0 | 1.0 / 3.0 | m | walidacja szerokości; nadpisywane z `/choose_dock`, GMB:982–983, 1029 | – |
| `min_depth` / `max_depth` (nowe) | 1.8 / 3.0 | 1.8 / 3.0 | m | walidacja głębokości, GMB:1018 | – |
| `size_tolerance` (nowy) | 0.30 | 0.30 | m | GMB:1012–1013 | – |
| `angle_tolerance_deg` (nowy) | 20.0 (clamp 0..90) | 20.0 | ° | narożniki/boki, GMB:1056–1058 | – |
| `boat_width` (nowy) | 0.80 | 0.80 | m | szerokość korytarza, GMB:871, 2948 | – |
| `corridor_back_inset` (nowy) | 0.30 | 0.30 | m | GMB:1715–1719 | – |
| `corridor_approach` (nowy) | 3.0 | 3.0 | m | GMB:1720–1721 | – |

Stałe w kodzie (bez parametru): bufor `/slam/odom` 10 s (GMB:921–923), tematy `/slam/dock_polygon_map`, `/slam/dock_corridor_map` (hpp:446–447).

## Rozbieżności dokumentów z kodem i yaml

> [!blad] Dokumentacja autora vs wartości
> - `REVIEW_AND_TEST_PLAN.md:316` podaje bazowe wagi płaszczyzna/linia **1.0 / 0.70** — to domyślne w kodzie; `baseline_params.yaml:82–83` i slam.yaml mają **0.5 / 0.5**.
> - `REVIEW_AND_TEST_PLAN.md:59` i `math_checks.py:39` używają `z = -0.30` dla ekstrinsyki, slam.yaml:15 i baseline:12 mają **−0.40**.
> - Komentarz w gmb.yaml:30–31 mówi „two genuine support-growth events”, a wartość obok to **1**.
> - baseline zawiera `tracking.max_extrapolation: 0.05`, `tracking_threshold: 5.0` i sekcję `slam_odom` — slam.yaml ma 0.50 / 3.0 i nie ma `slam_odom` (obowiązują domyślne z FE:270–317, równe wartościom z baseline). Front-end — szczegóły u innego autora notatek.

> [!wazne] Efekt `minimum_support_growth_events: 1` w slam.yaml
> Nowy woksel dostaje `support_growth_events = 1` (GMB:1425) i od razu spełnia warunek potwierdzenia (GMB:1428–1432), bo `minimum_static_source_points` = 1. Wszystko, co trafi do zaufanego keyframe’u, staje się „statyczne”, a `tentative_stale_updates` nie ma czego usuwać. Jedyne filtry, które zostają: intensywność (recovery), zakres z, `minimum_compact_obstacle_height` i aktywne czyszczenie w 12 m. Zapewne świadomy wybór pod małe boje — ale oznacza też więcej szumu w costmapie.

## Moje notatki

> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.
