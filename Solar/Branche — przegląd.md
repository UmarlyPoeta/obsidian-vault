---
typ: solar
tags: [solar, git, branche]
zaktualizowano: 2026-10-04
---
# Branche — przegląd wszystkich repozytoriów

> [!skrot] W 30 sekund
> - Wygenerowane z lokalnych kopii w `~/solar` (bez `git fetch` — gałęzie zdalne w stanie z ostatniego pobrania, ok. 2026-10-01; dla kopii bez `FETCH_HEAD` stan nieznany).
> - „+N/−M” = commity przed / za gałęzią główną (`origin/main` lub `origin/master`). ✅ = zmergowana do głównej. **Pogrubione** = wymeldowana w którejś kopii. „tylko lokalnie” = brak gałęzi na GitLabie; „lokalna ≠ origin” = lokalna ma inne commity niż zdalna (np. niewypchnięte).
> - Co która gałąź zmienia i która jest stanem z zawodów — w notatce repo (link w nagłówku).

## [[dongle-service]]
Remote: `git@gitlab.com:agh-solar-boat/ab/dongle-service.git` · gałąź główna: `main` · kopie: `~/solar/bash_do_uruchomienia/dongle-service` (**main**), `~/solar/dongle-service` (**Dongle-Statistic**)

| Gałąź | Ostatni commit | Autor | Data | +/− vs główna | Zmergowana | Uwagi | Temat |
|---|---|---|---|---|---|---|---|
| QoL/fixes_hotplug_watchdog_doctor | `f261e26` | patryk_kozlowski | 2026-07-30 | +2/−0 |  |  | wizard cli + diagnostics |
| **Dongle-Statistic** | `67505b4` | Your Name | 2025-10-21 | +2/−0 |  |  | ROS2 Node for dongle statistic |
| **main** | `cbfd591` | Przemysław Domagała | 2025-09-25 | +0/−0 | ✅ |  | Merge branch 'New_IP_for_Jetson' into 'main' |

## [[fusion_camera_lidar]]
Remote: `git@gitlab.com:agh-solar-boat/ab/fusion_camera_lidar.git` · gałąź główna: `main` · kopie: `~/solar/bash_do_uruchomienia/fusion_camera_lidar` (**main**)

| Gałąź | Ostatni commit | Autor | Data | +/− vs główna | Zmergowana | Uwagi | Temat |
|---|---|---|---|---|---|---|---|
| **main** | `9c91863` | Szymon Nowak | 2025-01-25 | +0/−0 | ✅ |  | Merge branch 'branch' into 'main' |

## [[l1od]]
Remote: `git@gitlab.com:agh-solar-boat/ab/l1od.git` · gałąź główna: `master` · kopie: `~/solar/bash_do_uruchomienia/l1od` (**master**), `~/solar/l1od` (**refined-fusion**)

| Gałąź | Ostatni commit | Autor | Data | +/− vs główna | Zmergowana | Uwagi | Temat |
|---|---|---|---|---|---|---|---|
| **refined-fusion** | `52dabf2` | juliadcz | 2026-08-12 | +26/−2 |  |  | Obstacle avoidance changes to obstacle detector. |
| zed_exposure_controller | `01677ee` | UmarlyPoeta | 2026-08-05 | +21/−11 |  |  | dodanie brakującego analyze_bag.py |
| vlm-detection | `1d89c9f` | juliadcz | 2026-07-10 | +14/−2 |  |  | Jak ktoś to postawi kiedyś to postawię mu flaszke |
| dbscan_clustering | `c4f951f` | NSTMS | 2026-05-11 | +14/−11 |  |  | Adaptive DBSCAN, changed static parameters to adaptive in 4 zones(can  |
| **master** | `55ded22` | Mateusz | 2026-04-27 | +0/−0 | ✅ |  | Refine running models on CPU. |
| fix/docker-optimization | `04a04fd` | Aleksander Młynarski | 2026-03-17 | +5/−3 |  |  | Docker up to date with changes on master |
| depth-in-simulation | `9c39c68` | Mateusz | 2026-03-10 | +0/−4 | ✅ |  | Resolve conflicts. |
| quantized_model | `518420e` | juliadcz | 2026-03-10 | +1/−9 |  |  | minor env_setup change |
| auto_fov_camera | `d1a524f` | jankusa | 2026-02-14 | +6/−19 |  |  | fix: check camera_info_set in DetectionCallback |

## [[l2dp]]
Remote: `git@gitlab.com:agh-solar-boat/ab/l2dp.git` · gałąź główna: `master` · kopie: `~/solar/bash_do_uruchomienia/l2dp` (**master**), `~/solar/l2dp` (**spirala_zjebania**), `~/solar/l2dp_docking_fix` (**feat/dock_costmap_fix**)

| Gałąź | Ostatni commit | Autor | Data | +/− vs główna | Zmergowana | Uwagi | Temat |
|---|---|---|---|---|---|---|---|
| **feat/dock_costmap_fix** | `0d444ff` | patryk_kozlowski | 2026-08-23 | +33/−1 |  | lokalna `0d444ff` ≠ origin `b27f9c6` | Niezacommitowane zmiany z Jetsona (stan po zawodach) |
| **spirala_zjebania** | `b27f9c6` | patryk_kozlowski | 2026-08-14 | +29/−1 |  | lokalna `b27f9c6` ≠ origin `3b73cff` | wszyscy pijani zasneli na kac vegas |
| jetson/spirala_zjebania | `cb93270` | juliadcz | 2026-08-12 | +30/−1 |  | tylko lokalnie | colision avoidance smigaaaaa auuuuu japierdole |
| scan_matching | `b858a54` | juliadcz | 2026-08-10 | +10/−1 |  | lokalna `b858a54` ≠ origin `ed210d2` | Merge branch 'scan_matching' of gitlab.com:agh-solar-boat/ab/l2dp into |
| workingDockingBackup | `5c07476` | juliadcz | 2026-08-10 | +8/−1 |  | tylko lokalnie | mergowansko |
| njord/tracked_timestamp_matching | `db9caeb` | Mateusz | 2026-08-09 | +13/−1 |  |  | Merge branch 'njord/tracked_timestamp_matching' of gitlab.com:agh-sola |
| **master** | `6ae4389` | Mateusz | 2026-08-01 | +0/−0 | ✅ |  | poprawka na osie jazwieckiego |
| celka_odo | `6b02df0` | juliadcz | 2026-07-21 | +7/−0 |  |  | kodzik po monaco2026 |
| formatting | `87dded7` | janSochaOfficial | 2026-06-10 | +4/−0 |  |  | Updated README.md with linting and formatting tutorials |
| lio_sam | `7f0930d` | jankusa | 2026-05-11 | +5/−84 |  |  | fix deskewing |
| jenkinsfile | `72d36f2` | Filnaw | 2026-04-26 | +3/−1 |  |  | fix Jenkinsfile |
| mapper_multitype_fusion | `ea12802` | janSochaOfficial | 2026-03-21 | +2/−4 |  |  | added confidence for tracked detections |
| point2cloud | `2995dc4` | Natalia Guzdek | 2026-03-18 | +1/−4 |  |  | rewrite pointcloud_to_laserscan |
| mapper_sim | `cc0852f` | MKalandykM | 2026-03-11 | +14/−105 |  |  | usa dump |
| communication_protobuf | `8a2fd80` | Piotr Kędziora | 2026-02-13 | +0/−28 | ✅ |  | adapted delivery and speed chellenge topics |
| signals_detector | `030a17d` | Blazej | 2026-02-10 | +0/−114 | ✅ |  | cleaning, move some functions to utils, add highpass filter |
| temp_ | `e1a520a` | witNie | 2026-01-27 | +16/−84 |  |  | test ci wrong formatting cpp |
| costmap_viewer | `f3f3a31` | Natalia Guzdek | 2026-01-21 | +1/−84 |  |  | OccupancyGrid plot |
| pipeline-testing | `b332e42` | witNie | 2026-01-06 | +1/−84 |  |  | Added mapper packet to .gitlab-ci.yml |
| marina_lookout | `ca4351e` | MKalandykM | 2025-12-13 | +8/−105 |  |  | unbuilding marina detection |
| docker_l2 | `58441ab` | Alek | 2025-12-11 | +6/−87 |  |  | Add tmux support to Docker and update README instructions |
| mapper | `47cc319` | MKalandykM | 2025-11-30 | +0/−51 | ✅ |  | fixed mapper |
| feature/global-map-sync | `338794f` | Mateusz | 2025-11-27 | +2/−99 |  |  | start task |
| mapper_sim_l2_divided_fix | `205f82e` | patryk_kozlowski | 2025-11-19 | +6/−105 |  |  | wreszcie sie mapper buduje |
| quantize_model | `f73c6e0` | filnaw | 2025-10-31 | +12/−127 |  |  | Commented out rarely used functionality. |
| tracking_debug | `359883b` | szymon_nowak | 2025-01-17 | +3/−305 |  |  | fix |

## [[l3nav]]
Remote: `git@gitlab.com:agh-solar-boat/ab/l3nav.git` · gałąź główna: `master` · kopie: `~/solar/bash_do_uruchomienia/l3nav` (**master**), `~/solar/l3nav` (**waypoint_first**)

| Gałąź | Ostatni commit | Autor | Data | +/− vs główna | Zmergowana | Uwagi | Temat |
|---|---|---|---|---|---|---|---|
| melody | `d6db1c1` | juliadcz | 2026-08-12 | +27/−0 |  |  | otter na żyletki |
| **waypoint_first** | `e336cd3` | juliadcz | 2026-08-12 | +27/−0 |  | tylko lokalnie | piotrek rozjebal |
| njord_trees | `35b6cb4` | juliadcz | 2026-07-31 | +8/−0 |  |  | rewrite cmake for a faster build |
| docking_njord | `5a85118` | juliadcz | 2026-07-25 | +13/−0 |  |  | good gnss for sim, tree works well |
| **master** | `dd07b54` | Martyna Czerniak | 2026-07-19 | +0/−0 | ✅ |  | merge monaco-cleanup and apply final fixes |
| maneuvering_njord26 | `9a86f89` | juliadcz | 2026-06-14 | +7/−13 |  |  | important: detect bouy parameters, waypoint needs changing, change in  |
| formating | `24bbbed` | janSochaOfficial | 2026-05-27 | +8/−30 |  |  | added install script for formatting and lintig dependecies |
| tui_drzewkowe | `48178ac` | Martyna Czerniak | 2026-04-28 | +0/−39 | ✅ |  | readme for tui |
| entry_exit_after_rb | `e94ca03` | Piotr Kędziora | 2026-04-26 | +14/−29 |  |  | include waypoint.h |
| mock_testing | `08ca407` | Kacper Kustra | 2026-04-21 | +1/−30 |  |  | add next_waypoint retry and stress test trees |
| bt_visualization | `7c56333` | janSochaOfficial | 2026-02-25 | +2/−118 |  |  | fixed building issue with btccpv4 and changed loggin logic |
| tests_tree | `a3747ab` | filnaw | 2026-02-12 | +0/−131 | ✅ |  | merged branches tests_tree and object-delivery |
| foxglove_audio_output | `dc657a3` | Martyna Czerniak | 2025-12-13 | +7/−143 |  |  | migration to btv4 fix |
| Docker_l3 | `db953e9` | Martyna Czerniak | 2025-12-13 | +7/−143 |  |  | migration to btv4 fix |

## [[l4sim]]
Remote: `git@gitlab.com:agh-solar-boat/ab/l4sim.git` · gałąź główna: `main` · kopie: `~/solar/bash_do_uruchomienia/l4sim` (**main**)

| Gałąź | Ostatni commit | Autor | Data | +/− vs główna | Zmergowana | Uwagi | Temat |
|---|---|---|---|---|---|---|---|
| collision-world | `d49626f` | Martyna Czerniak | 2026-07-27 | +2/−4 |  |  | smaller moving boat model |
| **main** | `7a789a2` | juliadcz | 2026-05-25 | +0/−0 | ✅ |  | monaco figure 8 |
| depth-camera | `dfc897c` | Mateusz | 2026-03-10 | +0/−4 | ✅ |  | Add working depth camera. Add RB26_LIGHT world to docker sim. |
| axes_rotated | `27a2a8e` | filnaw | 2026-02-18 | +0/−6 | ✅ |  | add follow the path |
| small-sim | `851370e` | Kacper Kustra | 2025-11-23 | +1/−13 |  |  | make barka smaller |
| dockerized_detections | `02892b0` | patryk_kozlowski | 2025-11-19 | +11/−14 |  |  | wybor swiata w solar3000 start-full |

## [[ros-sbt-interfaces]]
Remote: `git@gitlab.com:agh-solar-boat/ab/ros-sbt-interfaces.git` · gałąź główna: `main` · kopie: `~/solar/bash_do_uruchomienia/ros-sbt-interfaces` (**main**), `~/solar/ros-sbt-interfaces` (**main**)

| Gałąź | Ostatni commit | Autor | Data | +/− vs główna | Zmergowana | Uwagi | Temat |
|---|---|---|---|---|---|---|---|
| **main** | `f1de70b` | juliadcz | 2026-08-10 | +0/−0 | ✅ |  | Merge branch 'main' of gitlab.com:agh-solar-boat/ab/ros-sbt-interfaces |
| jenkins | `81e5c2c` | juliadcz | 2026-05-31 | +2/−5 |  |  | jenkinsfile update |
| l3_tui_msgs | `ea8f48e` | Martyna Czerniak | 2026-04-28 | +0/−12 | ✅ |  | tui progress msg update |
| tracking_debug | `bfe713f` | szymon_nowak | 2025-03-07 | +2/−75 |  |  | detection types and classes update |

## [[zed-ws]]
Remote: `git@gitlab.com:agh-solar-boat/ab/zed-ws.git` · gałąź główna: `main` · kopie: `~/solar/bash_do_uruchomienia/zed-ws` (**main**), `~/solar/zed-ws` (**main**)

| Gałąź | Ostatni commit | Autor | Data | +/− vs główna | Zmergowana | Uwagi | Temat |
|---|---|---|---|---|---|---|---|
| **main** | `d72d973` | przemekdomagala | 2024-11-27 | +0/−0 | ✅ |  | readme |

## [[embedded-system]]
Remote: `git@gitlab.com:agh-solar-boat/delta/embedded-system.git` · gałąź główna: `main` · kopie: `~/solar/embedded-system` (**main**)

| Gałąź | Ostatni commit | Autor | Data | +/− vs główna | Zmergowana | Uwagi | Temat |
|---|---|---|---|---|---|---|---|
| user-input-board | `5cf5325` | Kacper Ślusarczyk | 2026-07-08 | +14/−3 |  |  | add project for testing the encoder in isolation |
| **main** | `d34894f` | treborsky | 2026-06-10 | +0/−0 | ✅ |  | rm CI for now... |

## [[l0hwd]]
Remote: `git@gitlab.com:agh-solar-boat/ab/l0hwd.git` · gałąź główna: `master` · kopie: `~/solar/l0hwd` (**master**)

| Gałąź | Ostatni commit | Autor | Data | +/− vs główna | Zmergowana | Uwagi | Temat |
|---|---|---|---|---|---|---|---|
| **master** | `fd33338` | juliadcz | 2026-08-05 | +0/−0 | ✅ |  | jetson changes |
| music | `1873814` | juliadcz | 2026-08-03 | +1/−3 |  |  | music in CanDriver |
| sim_controller | `8c481dc` | filnaw | 2026-02-12 | +1/−6 |  |  | working controller in simulation |
| next_gen_drive_controller | `63f086d` | Mikslo | 2025-10-20 | +1/−13 |  |  | Initial commit |
| sbg-info | `9ba0e0c` | akzceluj | 2025-06-26 | +7/−28 |  |  | jetson changes |
| old_architecture | `0a4b961` | przemekdomagala | 2024-11-27 | +2/−67 |  |  | readme |

## [[can-msgs]]
Remote: `git@gitlab.com:agh-solar-boat/embedded-software-development/autonomous-boat/can-msgs.git` · gałąź główna: `main` · kopie: `~/solar/l0hwd/can-msgs` (**odłączony HEAD @27a40a6**)

| Gałąź | Ostatni commit | Autor | Data | +/− vs główna | Zmergowana | Uwagi | Temat |
|---|---|---|---|---|---|---|---|
| BMS_V3.0 | `5937bfa` | gyzmaul | 2026-07-15 | +2/−0 |  |  | forgor to generate source files |
| njord_2025 | `27a40a6` | Michał Zawada | 2025-08-07 | +1/−0 |  |  | fixed state od charge values |
| sbg-info | `575dda5` | filnaw | 2025-06-02 | +1/−0 |  |  | add sbg info |
| main | `021049a` | Damian Brzana | 2025-03-02 | +0/−0 | ✅ |  | added can message for calculated yaw heading |
| Njord | `bacc89b` | Michal | 2024-08-20 | +2/−30 |  |  | zmiana jakichś gówno nazw z repo, na te śmigające na BMSie |
| master | `de0495d` | treborsky | 2024-01-21 | +1/−33 |  |  | updated naming so there are no conflicts |

## echosounderapi-python
Remote: `https://github.com/Echologger/echosounderapi-python.git` · gałąź główna: `main` · kopie: `~/solar/l0hwd/echosounderapi-python` (**main**)

| Gałąź | Ostatni commit | Autor | Data | +/− vs główna | Zmergowana | Uwagi | Temat |
|---|---|---|---|---|---|---|---|
| **main** | `1c4a4be` | ultrasonics-kr | 2025-11-04 | +0/−0 | ✅ |  | Fix issue with read working frequency back when working frequency chan |

## ros2_socketcan
Remote: `https://github.com/autowarefoundation/ros2_socketcan` · gałąź główna: `main` · kopie: `~/solar/l0hwd/ros2_socketcan` (**odłączony HEAD @8996cdb**)

| Gałąź | Ostatni commit | Autor | Data | +/− vs główna | Zmergowana | Uwagi | Temat |
|---|---|---|---|---|---|---|---|
| add-destructor | `eea7dea` | mitsudome-r | 2026-07-31 | +1/−0 |  |  | feat: add destructor to socket_can_receiver_node |
| main | `e16bb19` | Tetsuhiro Kawaguchi | 2026-03-19 | +0/−0 | ✅ |  | feat(`agnocast`): adopt `CallbackIsolatedAgnocastExecutor` (CIE) (#72) |
| jazzy | `a920407` | M. Fatih Cırıt | 2024-07-16 | +0/−9 | ✅ |  | chore: release 1.3.0 (#49) |
| iron | `a920407` | M. Fatih Cırıt | 2024-07-16 | +0/−9 | ✅ |  | chore: release 1.3.0 (#49) |
| humble | `a920407` | M. Fatih Cırıt | 2024-07-16 | +0/−9 | ✅ |  | chore: release 1.3.0 (#49) |
| reorg-for-canfd-support | `16e835f` | Joshua Whitley | 2022-10-03 | +1/−25 |  |  | Reorganize folders for adding ros2_socketcan_msgs. |
| add-nodes | `b4768b2` | Joshua Whitley | 2021-02-27 | +2/−57 |  |  | Add basic receiver node |

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.
