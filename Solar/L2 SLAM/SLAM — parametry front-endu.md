---
typ: solar-slam
tags: [solar, slam, l2]
zaktualizowano: 2026-10-08
---
# SLAM — parametry front-endu

> [!skrot] W 30 sekund
> - `FeatureExtractor` deklaruje **82 parametry** (`pose_graph/src/feature_extractor.cpp:44-555`, `:755-758`); wszystkie czytane raz w konstruktorze — **brak** `add_on_set_parameters_callback`, zmiana w locie (`ros2 param set`) nie działa.
> - Plik na łodzi: `pose_graph/config/slam.yaml` (sekcja `feature_extractor.ros__parameters`); alternatywa autora: `pose_graph/baseline_params.yaml`. Front-endowe wartości obu plików są **identyczne** poza `tracking.max_extrapolation` (0,50 vs 0,05) i `tracking.tracking_threshold` (3,0 vs 5,0), a `slam.yaml` nie ma sekcji `slam_odom` (działają domyślne z kodu, równe tym z baseline).
> - W żadnym yaml nie ma `trajectory_extrapolation.*` → horyzont **6,0 s** z kodu (nagłówek sugeruje 3,0).
> - Najbardziej odbiegają od domyślnych w kodzie: `matcher.plane_weight` 1,0 → **0,5**, `matcher.line_weight` 0,70 → **0,5**, `keyframes.maximum_count` 25 → **10**, `keyframes.recovery_after_failures` 5 → **3**, `extrinsic.imu_lidar.z` −0,30 → **−0,40**.
> - `tracking.max_extrapolation` **nie zmienia zachowania** — jest używany tylko w treści ostrzeżenia (`:2886-2898`).

Kontekst: [[SLAM — architektura węzła i przepływ danych]]. Parametry `global_map.*` i `costmap.*` (ta sama sekcja yaml, ten sam węzeł) opisuje [[SLAM — global map builder i costmapy]]; znaczenie parametrów matchera w szczegółach: [[SLAM — scan matching (matematyka)]] i [[SLAM — degeneracja i jakość dopasowania]].

Uruchomienie: brak pliku launch w repo (niepewne, jak dokładnie podawano yaml na łodzi; [[pose_graph]] → „Launch”). Typy całkowite: `declare_parameter<int>` zwraca `int64_t`, porównywane z `1L`/`2L` — OK na 64-bit Linux (Jetson aarch64).

Legenda kolumn: **kod** = domyślna w `declare_parameter` (z ewentualnym ograniczeniem), **yaml** = `config/slam.yaml` (drzewo robocze; front-endowa część bez zmian względem HEAD), **base** = `baseline_params.yaml`; „—” = brak w pliku.

## Ramy, debug, extrinsic, deskew

| parametr | kod | yaml | base | jedn. | co robi / gdzie | ryzyko złej wartości |
|---|---|---|---|---|---|---|
| `frames.convert_ned_odom_to_enu_map` | false | false | false | — | inicjalna `T_map_odom` = permutacja NED→ENU (`:44-79`) | `true` psuje konwencję oczekiwaną przez detekcje (komentarz `:2981-2986`) i `/slam/odom` |
| `debug.publish_clouds` | false | false | false | — | publikacja `/slam/debug/*_scan` (`:2313-2359`, `:2456-2463`, `:2539-2546`) | `true` = kopie chmur i serializacja co skan → wolniej |
| `extrinsic.imu_lidar.x` | 0,40 | 0,40 | 0,40 | m | translacja $T_{imu\_lidar}$ (`:165-175`), użyta w deskew (`deskewer.cpp:305`, `:352`) | zła → błędna predykcja pozy lidaru przy obrotach (ramię dźwigni), smużenie przy deskew; osie ramy niepewne ([[SLAM — deskewing]]) |
| `extrinsic.imu_lidar.y` | 0,05 | 0,05 | 0,05 | m | j.w. | j.w. |
| `extrinsic.imu_lidar.z` | −0,30 | **−0,40** | −0,40 | m | j.w.; znak zależy od konwencji osi Xsensa (`REVIEW_AND_TEST_PLAN.md`, „Extrinsic translation caveat”) | błąd wysokości mapy; przy kołysaniu błąd XY |
| `extrinsic.imu_lidar.qw/qx/qy/qz` | 1/0/0/0 | 1/0/0/0 | 1/0/0/0 | — | rotacja $T_{imu\_lidar}$, normalizowana, zły → jednostkowa (`:177-204`) | obrót mapy względem trajektorii — najgroźniejszy błąd; jednostkowa „działa” przez zbieg konwencji (patrz deskewing) |
| `deskew.header_stamp_reference` | "end" | end | end | — | kotwica czasu punktów: `start`/`end` (`:206-227`, `deskewer.cpp:225-235`) | zła → stałe przesunięcie czasu ≈ czas skanu (~0,1 s) między lidarem a odometrią |
| `deskew.motion_buffer_length` | 5,0 (≥ 0,5) | 5,0 | 5,0 | s | długość bufora `/odom` (`deskewer.cpp:54-55`, `:100-105`) | za krótki → `SCAN_NOT_COVERED` przy opóźnionym lidarze |
| `deskew.maximum_scan_duration` | 0,50 (≥ 0; 0 = brak limitu) | 0,50 | 0,50 | s | odrzucenie skanu o rozpiętości czasów > limit (`deskewer.cpp:217-223`) | 0 → przepuści czasy w złych jednostkach |

## Cechy i próbkowanie

| parametr | kod | yaml | base | jedn. | co robi / gdzie | ryzyko |
|---|---|---|---|---|---|---|
| `features.scan_voxel_size` | 0,10 (≥ 0,01) | 0,10 | 0,10 | m | voxel skanu przed PCA (`:2379`) | mniejszy → wolniej (kNN), większy → mniej szczegółów, cechy „grubsze” |
| `features.neighbors` | 20 (≥ 3) | 20 | 20 | — | k w kNN normalnych i klasyfikatora (`:2381`, `:2399-2402`, `classifier.cpp:32-36`) | mały → szum PCA; duży → wolniej i mniej punktów spełnia promień |
| `features.maximum_neighbor_radius` | 0,75 | 0,75 | 0,75 | m | max odległość k-tego sąsiada (`classifier.cpp:38-41`, `:61-65`) | mały → brak cech daleko od lidaru |
| `features.minimum_linearity` | 0,45 | 0,45 | 0,45 | — | próg $L$ dla EDGE (`classifier.cpp:109-111`) | zbyt niski → „linie” z szumu |
| `features.minimum_planarity` | 0,35 | 0,35 | 0,35 | — | próg $P$ dla PLANE (`classifier.cpp:113-115`) | j.w. |
| `features.maximum_scattering` | 0,12 | 0,12 | 0,12 | — | max $S$ dla obu typów | zbyt wysoki → roślinność/woda jako cechy |
| `features.plane_dominance_margin` | 0,02 | 0,02 | 0,02 | — | rozstrzyganie PLANE vs EDGE (`classifier.cpp:131-133`) | mała waga |
| `matcher.plane_sample_leaf` | 0,25 (≥ 0,01) | 0,25 | 0,25 | m | komórka próbkowania płaszczyzn (`:2429-2433`) | mała → więcej punktów, wolniej |
| `matcher.line_sample_leaf` | 0,20 (≥ 0,01) | 0,20 | 0,20 | m | j.w. dla linii (`:2435-2439`) | j.w. |
| `matcher.maximum_plane_samples` | 3500 (≥ 1) | 3500 | 3500 | szt. | limit punktów płaszczyzn do matchera | duży → czas matchera rośnie liniowo |
| `matcher.maximum_line_samples` | 2000 (≥ 1) | 2000 | 2000 | szt. | j.w. dla linii | j.w. |

## Lokalna mapa i keyframe’y

| parametr | kod | yaml | base | jedn. | co robi / gdzie | ryzyko |
|---|---|---|---|---|---|---|
| `map.voxel_size` | 0,40 (≥ 1e-6) | 0,40 | 0,40 | m | voxel mapy (`local_voxel_map.hpp:195-199`) | promień wyszukiwania $r = \lfloor 0{,}8/s\rfloor + 1$ zależy od niego → koszt $(2r+1)^3$ |
| `map.keyframe_voxel_size` | 0,18 (≥ 0,01) | 0,18 | 0,18 | m | przerzedzenie chmury keyframe’u (`:2093-2094`) | duży → za mało punktów na voxel (< 5) → pusta mapa |
| `map.minimum_points_per_voxel` | 5 (≥ 3) | 5 | 5 | szt. | min punktów do prymitywu (`local_voxel_map.hpp:104`) | duży → mapa rzadka |
| `map.minimum_linearity` | 0,45 | 0,45 | 0,45 | — | próg LINE w mapie (`:143-145`) | — |
| `map.minimum_planarity` | 0,32 | 0,32 | 0,32 | — | próg PLANE w mapie (`:147-149`) | — |
| `map.maximum_scattering` | 0,12 | 0,12 | 0,12 | — | j.w. | — |
| `map.plane_dominance_margin` | 0,02 | 0,02 | 0,02 | — | j.w. (`:151-153`) | — |
| `keyframes.translation` | 0,80 | 0,80 | 0,80 | m | próg XY nowego keyframe’u (`:2081`) | mały → okno 10 keyframe’ów jeszcze krótsze |
| `keyframes.yaw_deg` | 4,0 | 4,0 | 4,0 | ° | próg kursu (`:2082`) | mały → na kołysaniu/obrotach w miejscu ciągłe przebudowy mapy |
| `keyframes.time` | 2,0 | 2,0 | 2,0 | s | próg czasu (z min. ruchem) (`:2083-2084`) | — |
| `keyframes.minimum_timed_translation` | 0,25 | 0,25 | 0,25 | m | min. ruch dla progu czasu | 0 → keyframe co 2 s na postoju |
| `keyframes.maximum_count` | 25 (≥ 2) | **10** | 10 | szt. | długość okna (`:2107-2108`) | mały → szybka utrata zaufanej mapy; duży → wolniejsza odbudowa mapy |
| `keyframes.recovery_after_failures` | 5 (≥ 1) | **3** | 3 | szt. | po ilu porażkach recovery-keyframe (`:2608-2621`) | mały → mapa szybko zapełnia się niezaufanymi; bardzo duży → brak recovery (test 1 w `START_HERE.md`) |

## Matcher (przekazywane do `ScanMatcherConfig`, `:353-521`)

| parametr | kod | yaml = base | jedn. | użycie w `scan_matcher.cpp` | rola (skrót) |
|---|---|---|---|---|---|
| `matcher.maximum_iterations` | 8 (≥ 1) | 8 | — | `:651` | iteracje Gaussa-Newtona |
| `matcher.minimum_total_matches` | 80 (≥ 0) | 80 | szt. | `:668`, `:881` | min. korespondencji |
| `matcher.minimum_plane_matches` | 40 | 40 | szt. | `:301`, `:309` | wsparcie płaszczyzn |
| `matcher.minimum_line_matches` | 20 | 20 | szt. | `:303`, `:315` | wsparcie linii |
| `matcher.maximum_plane_distance` | 0,25 | 0,25 | m | `:342` | bramka punkt–płaszczyzna |
| `matcher.maximum_line_distance` | 0,35 | 0,35 | m | `:430` | bramka punkt–linia |
| `matcher.maximum_center_distance` | 0,80 | 0,80 | m | `:343`, `:431` | bramka do centroidu + promień wyszukiwania |
| `matcher.plane_huber_delta` | 0,10 | 0,10 | m | `:380` | Huber płaszczyzn |
| `matcher.line_huber_delta` | 0,15 | 0,15 | m | `:480` | Huber linii |
| `matcher.plane_weight` | 1,0 (≥ 0) | **0,5** | — | `:295`, `:395`, `:640` | waga; 0 wyłącza płaszczyzny |
| `matcher.line_weight` | 0,70 (≥ 0) | **0,5** | — | `:296`, `:495`, `:641` | waga; 0 wyłącza linie |
| `matcher.minimum_plane_horizontal_normal` | 0,08 | 0,08 | — | `:349` | odrzuca płaszczyzny poziome (nie wiążą x, y, yaw) |
| `matcher.damping` | 1e-4 | 1e-4 | — | `:749` | tłumienie LM |
| `matcher.maximum_translation_step` | 0,20 | 0,20 | m | `:793` | limit kroku |
| `matcher.maximum_rotation_step_deg` | 1,5 | 1,5 | ° | `:802` | limit kroku |
| `matcher.translation_convergence` | 0,002 | 0,002 | m | `:840` | zbieżność |
| `matcher.rotation_convergence_deg` | 0,02 | 0,02 | ° | `:842` | zbieżność |
| `matcher.odometry_prior_sigma_xy` | 0,30 | 0,30 | m | `:714` | prior do predykcji |
| `matcher.odometry_prior_sigma_yaw_deg` | 2,0 | 2,0 | ° | `:719` | prior do predykcji |
| `matcher.maximum_rmse` | 0,20 | 0,20 | m | `:896` | akceptacja |
| `matcher.minimum_match_ratio` | 0,10 | 0,10 | — | `:902` | akceptacja |
| `matcher.minimum_trusted_match_fraction` | 0,25 | 0,25 | — | `:908` | akceptacja; blokuje mapę z samych recovery |
| `matcher.maximum_correction_translation` | 0,35 | 0,35 | m | `:916` | max korekta względem predykcji — **front-end nie wchłonie większego dryfu** |
| `matcher.maximum_correction_yaw_deg` | 3,0 | 3,0 | ° | `:919` | j.w. dla kursu |
| `matcher.maximum_final_to_initial_rmse_ratio` | 1,05 | 1,05 | — | `:928` | odrzuca, gdy RMSE wzrosło |
| `matcher.minimum_translation_information` | 0,01 | 0,01 | — | `:199` | obserwowalność kierunku XY |
| `matcher.minimum_translation_eigenvalue_ratio` | 0,02 | 0,02 | — | `:204` | j.w., względnie |
| `matcher.minimum_yaw_information` | 0,005 | 0,005 | — | `:239` | obserwowalność kursu |
| `matcher.minimum_observable_translation_rank` | 1 (clamp 1–2) | 1 | — | `:688`, `:889` | 1 = dopuszcza korektę tylko w jednym kierunku |

`ScanMatcherConfig::minimum_effective_jacobian_norm` (1e-8, `scan_matcher.hpp:98`) **nie jest** parametrem ROS.

## `/slam/odom`, ekstrapolacja, śledzenie

| parametr | kod | yaml | base | jedn. | co robi / gdzie | ryzyko |
|---|---|---|---|---|---|---|
| `slam_odom.frame_id` | "map" | — | map | — | `header.frame_id` `/slam/odom`, trajektorii, markera (`:1067`, `:1189`, `:1498`) | — |
| `slam_odom.child_frame_id` | "" | — | "" | — | puste = zachowaj z `/odom` (`:1069-1070`) | — |
| `slam_odom.correction_time_constant` | 0,10 (≥ 0) | — | 0,10 | s | $\tau$ domykania korekty (`:937-942`); 0 = natychmiast | duży → `/slam/odom` długo odstaje od SLAM |
| `slam_odom.maximum_translation_correction_rate` | 3,0 (≥ 0; 0 = brak) | — | 3,0 | m/s | limit prędkości domykania (`:950-959`) | mały → po loop closure długie „dojeżdżanie” |
| `slam_odom.maximum_rotation_correction_rate_deg` | 45 (≥ 0; 0 = brak) | — | 45 | °/s | j.w. dla obrotu (`:989-999`) | j.w. |
| `slam_odom.timestamp_reset_gap` | 1,0 (≥ 0,1) | — | 1,0 | s | reset filtra przy dziurze czasu (`:896-903`, `:1169`) | mały → resety przy zgubionych próbkach (skok) |
| `trajectory_extrapolation.time` | **6,0** (≥ 0) | — | — | s | horyzont `PoseArray` (`:1373-1398`) | — |
| `trajectory_extrapolation.dt` | 0,10 (≥ 0,01) | — | — | s | odstęp póz (`:1375`) | max 500 póz (`:1371`) |
| `trajectory_extrapolation.history` | 0,50 (≥ 0,05) | — | — | s | okno estymacji prędkości (`:1183-1185`) | — |
| `trajectory_extrapolation.minimum_history` | 0,10 (clamp 0,01–history) | — | — | s | min. zebranego czasu, inaczej prędkość 0 (`:1278-1292`) | — |
| `trajectory_extrapolation.publish_rate` | 20 (≥ 0; 0 = każdy `/odom`) | — | — | Hz | throttling (`:1173-1180`) | 0 przy 400 Hz → duży ruch na DDS |
| `tracking.tracking_threshold` | 5,0 | **3,0** | 5,0 | m | promień asocjacji detekcji (`:3152-3153`) | — (poza SLAM) |
| `tracking.pose_buffer_length` | 5,0 | 5,0 | 5,0 | s | bufor `T_map_boat` dla detekcji (`:2833-2838`) | krótki → detekcje `TOO_OLD` |
| `tracking.max_extrapolation` | 0,05 | **0,50** | 0,05 | s | **tylko tekst ostrzeżenia** (`:2886-2898`) | żadne — parametr martwy |
| `tracking.publish_rate` | 10,0 (≤ 0 wyłącza) | — | — | Hz | timer `/tracked_detections` (`:755-774`) | — |

## Stałe zaszyte w kodzie (kandydaci na parametry)

- min. zasięg punktu 0,1 m (`feature_extractor.cpp:1661`); wagi `score` 0,05 / 0,05 w wyszukiwaniu korespondencji (`local_voxel_map.hpp:355-358`, `:427-430`);
- krok całkowania ekstrapolacji ≤ 0,02 s i odrzucanie $dt > 0{,}25$ s w historii (`:1223`, `:1337-1338`);
- kolejki detekcji/klastrów 200 wiadomości (`:18-19`); filtr klastrów: rozmiar 0,3–5,0 m, wysokość 0,1–3,0 m, `min.z ≥ −2,0`, zasięg ≤ 40 m — z komentarzem „TODO: PARAMETRY BŁAGAM” (`:3505-3507`).

## Podejrzane / do naprawy

1. `tracking.max_extrapolation` jest martwy (tylko log) — różnica yaml/baseline (0,50 vs 0,05) nie ma skutków; usunąć albo zaimplementować ekstrapolację.
2. Rozbieżność domyślnych `trajectory_extrapolation.time`: nagłówek 3,0 (`feature_extractor.hpp:438`), `declare_parameter` 6,0 (`:323`), log startowy pokazuje faktyczną wartość. Ujednolicić i dopisać sekcję do `slam.yaml`.
3. `REVIEW_AND_TEST_PLAN.md` („Parameter tuning order”) podaje dla baseline `plane / line weight: 1.0 / 0.70`, a `baseline_params.yaml` i `slam.yaml` mają 0,5 / 0,5 — dokument ≠ plik. To samo z `extrinsic z`: dokument i `math_checks.py` używają −0,30, pliki −0,40.
4. Brak dynamicznej rekonfiguracji — każdy test parametru wymaga restartu węzła (a z nim utraty mapy, bo nic nie jest zapisywane).

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.
