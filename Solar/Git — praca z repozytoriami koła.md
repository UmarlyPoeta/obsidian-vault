---
typ: solar-uruchamianie
warstwa: narzędzia
tags: [solar, git, gitlab]
zaktualizowano: 2026-10-04
---
# Git — praca z repozytoriami koła

> [!skrot] W 30 sekund
> - Kod koła jest na **GitLabie w grupie `agh-solar-boat`**: podgrupa `ab` (autonomia — wszystkie warstwy ROS), `delta` (`embedded-system`) i `embedded-software-development/autonomous-boat` (`can-msgs`, submoduł [[l0hwd]]). Dostęp przez SSH `git@gitlab.com:agh-solar-boat/...`.
> - Lokalnie: główne kopie w `~/solar/<repo>` (stan z Jetsona po zawodach), kopie „na gałęziach głównych” w `~/solar/bash_do_uruchomienia/<repo>`, kopie robocze w `~/ros2-humble-gpu/ws/src/<repo>`.
> - Tożsamość: w każdym repo `user.email` jest ustawiony lokalnie na adres w domenie koła (`@solarboat.agh.edu.pl`); globalnie jest adres studencki AGH. Klucz SSH `~/.ssh/solar` istnieje i `~/.ssh/config` wskazuje go dla `gitlab.com`.
> - Konwencji commitów praktycznie nie ma (dowolne, często żartobliwe opisy po polsku/angielsku); zmiany wchodzą do `main/master` przez Merge Requesty GitLaba.

Powiązane: [[Uruchamianie i launch]] · [[Środowisko developerskie (Docker ROS2)]].

## Lista repozytoriów (z `git remote -v`)
| Repo | URL | Gałąź główna | Lokalne kopie | Notatka |
|---|---|---|---|---|
| l0hwd | `git@gitlab.com:agh-solar-boat/ab/l0hwd.git` | `master` | `~/solar/l0hwd` | [[l0hwd]] |
| l1od | `git@gitlab.com:agh-solar-boat/ab/l1od.git` | `master` | `~/solar/l1od`, `bash_do_uruchomienia/l1od` | [[l1od]] |
| l2dp | `git@gitlab.com:agh-solar-boat/ab/l2dp.git` | `master` | `~/solar/l2dp`, `~/solar/l2dp_docking_fix`, `bash_do_uruchomienia/l2dp` | [[l2dp]] |
| l3nav | `git@gitlab.com:agh-solar-boat/ab/l3nav.git` | `master` | `~/solar/l3nav`, `bash_do_uruchomienia/l3nav` | [[l3nav]] |
| l4sim | `git@gitlab.com:agh-solar-boat/ab/l4sim.git` | `main` | `bash_do_uruchomienia/l4sim` | [[l4sim]] |
| ros-sbt-interfaces | `git@gitlab.com:agh-solar-boat/ab/ros-sbt-interfaces.git` | `main` | `~/solar/ros-sbt-interfaces`, `bash_do_uruchomienia/ros-sbt-interfaces` | [[ros-sbt-interfaces]] |
| dongle-service | `git@gitlab.com:agh-solar-boat/ab/dongle-service.git` | `main` | `~/solar/dongle-service`, `bash_do_uruchomienia/dongle-service` | [[dongle-service]] |
| zed-ws | `git@gitlab.com:agh-solar-boat/ab/zed-ws.git` | `main` | `~/solar/zed-ws`, `bash_do_uruchomienia/zed-ws` | [[zed-ws]] |
| fusion_camera_lidar | `git@gitlab.com:agh-solar-boat/ab/fusion_camera_lidar.git` | `main` | `bash_do_uruchomienia/fusion_camera_lidar` | — |
| embedded-system | `git@gitlab.com:agh-solar-boat/delta/embedded-system.git` | `main` | `~/solar/embedded-system` | [[embedded-system]] |
| can-msgs (submoduł l0hwd) | `git@gitlab.com:agh-solar-boat/embedded-software-development/autonomous-boat/can-msgs.git` | `main`/`master` | `~/solar/l0hwd/can-msgs` (detached HEAD `27a40a6`) | [[l0hwd]] |
| ros2_socketcan (submoduł l0hwd) | `https://github.com/autowarefoundation/ros2_socketcan` | — | `~/solar/l0hwd/ros2_socketcan` (detached `8996cdb`, 1.2.0) | upstream |
| echosounderapi-python (submoduł l0hwd) | `https://github.com/Echologger/echosounderapi-python.git` | `main` | `~/solar/l0hwd/echosounderapi-python` | upstream |
| zed-ros2-wrapper (submoduł zed-ws) | `https://github.com/stereolabs/zed-ros2-wrapper` | — | **niezainicjalizowany** (pusty katalog) | upstream |

Bez gita (luźne kopie): `~/solar/sbt_transforms` ([[sbt_transforms]]), `~/solar/agh-dashboard-live` ([[agh-dashboard-live]]), `~/solar/zed-service`, `~/solar/zed-ros2-wrapper-humble-v5.1.0` (rozpakowany zip), `~/solar/docs`.

Specjalny przypadek: `~/solar/l2dp_docking_fix` ma **dwa** remote’y: `origin` (GitLab l2dp) i `jetson → /home/umarly-poeta/ros2_ws/src/l2dp` (ścieżka lokalna z laptopa, na którym kopia z Jetsona leżała jako osobny klon — z niego pobrano `spirala_zjebania` do rebase’u; zapis w `2026-09-16-…txt`, linie ~5460–5500).

### Stan kopii w `~/solar` (wymeldowana gałąź, z `git status`)
| Repo | Gałąź | HEAD | Niezacommitowane | Względem upstream | Ostatni fetch |
|---|---|---|---|---|---|
| l0hwd | `master` | `fd33338` 2026-08-05 „jetson changes” | 7 (m.in. `regulator`, `diff_thrust_driver`, submoduły) | równo z `origin/master` | 2026-10-01 |
| l1od | `refined-fusion` | `52dabf2` 2026-08-12 | 5 (głównie utracony bit `+x` skryptów, model `.onnx`) | równo | 2026-10-01 |
| l2dp | `spirala_zjebania` | `cb93270` 2026-08-12 | 11 (+ nieśledzony `COLCON_IGNORE`) | **2 przed** `origin/spirala_zjebania` (nie wypchnięte) | 2026-10-01 |
| l2dp_docking_fix | `feat/dock_costmap_fix` | `0d444ff` 2026-08-23 „Niezacommitowane zmiany z Jetsona (stan po zawodach)” | 7 | 5 przed, 1 za `origin/feat/dock_costmap_fix` | 2026-10-01 |
| l3nav | `waypoint_first` | `e336cd3` 2026-08-12 | 25 | **brak gałęzi zdalnej** (tylko lokalnie) | 2026-10-01 |
| ros-sbt-interfaces | `main` | `f1de70b` 2026-08-10 | 0 | równo | 2026-10-01 |
| dongle-service | `Dongle-Statistic` | `67505b4` 2025-10-21 | 8 (2 zmienione, 6 nieśledzonych) | równo | 2026-10-01 |
| zed-ws | `main` | `d72d973` 2024-11-27 | 0 | równo | 2026-10-01 |
| embedded-system | `main` | `d34894f` 2026-06-10 | 25 | — | brak `FETCH_HEAD` |

> [!wazne] Praca, która istnieje tylko lokalnie
> `l3nav` `waypoint_first` (stan z Jetsona, 25 zmienionych plików), 2 commity na `l2dp` `spirala_zjebania` i 5 na `l2dp_docking_fix` `feat/dock_costmap_fix` nie są na GitLabie (stan na fetch z 1.10.2026). Utrata tych katalogów = utrata stanu po zawodach.

## Tożsamość i klucze
- `git config user.email` w **każdym** repo w `~/solar` (i w `bash_do_uruchomienia`): ustawiony lokalnie na adres w domenie `solarboat.agh.edu.pl` — czyli commity idą pod adresem koła. Globalnie (`~/.gitconfig`): adres `@student.agh.edu.pl`, `user.name = UmarlyPoeta`. Nowy klon **nie dostanie** adresu koła automatycznie — trzeba `git config user.email <adres koła>` w repo (albo `includeIf` w `~/.gitconfig`; tego nie ma).
- `~/.ssh/solar` i `~/.ssh/solar.pub` istnieją (zawartości nie czytano). `~/.ssh/config`: `Host gitlab.com` → `User git` + `IdentityFile …/solar`; `Host github.com` → `id_ed25519`.
- Historia ma też commity z nieustawioną tożsamością (`Your Name` w `dongle-service` `67505b4`) — warto sprawdzać `git config user.name` przed commitem.

## Konwencje (z historii)
**Gałęzie** — brak jednego schematu. Spotykane wzorce (z `git for-each-ref`):
- prefiks `feat/`, `fix/`: `feat/dock_costmap_fix` (l2dp), `feat/global-map-sync` (l2dp), `fix/docker-optimization` (l1od);
- prefiks zawodów/łodzi: `njord/tracked_timestamp_matching` (l2dp), `docking_njord`, `maneuvering_njord26`, `njord_trees` (l3nav), `Njord`, `njord_2025`, `BMS_V3.0` (can-msgs);
- `QoL/fixes_hotplug_watchdog_doctor`, `Dongle-Statistic`, `New_IP_for_Jetson`, `IP_changed` (dongle-service);
- opisowe snake_case: `refined-fusion`, `auto_fov_camera`, `zed_exposure_controller`, `vlm-detection` (l1od), `scan_matching`, `lio_sam`, `mapper_sim`, `marina_lookout`, `workingDockingBackup` (l2dp), `tui_drzewkowe`, `waypoint_first`, `mock_testing` (l3nav), `small-sim`, `dockerized_detections` (l4sim);
- gałęzie „robocze z zawodów” o niepoważnych nazwach (`spirala_zjebania`) — to na nich jest stan z łodzi.

**Commity** — z ~2100 commitów w l0hwd/l1od/l2dp/l3nav/ros-sbt-interfaces/embedded-system/l4sim tylko 16 ma format Conventional Commits (`feat:`/`fix:`…). W `embedded-system` dominuje styl „czasownik w trybie rozkazującym po angielsku” (`add …`, `fix …`, `rm …`). W repo ROS opisy są dowolne, po polsku i angielsku, często żartobliwe/wulgarne (np. `colision avoidance smigaaaaa …`, `piotrek rozjebal`).

**Merge Requesty** — liczba commitów „Merge branch '…' into '…'” (= MR z GitLaba): l2dp 67, l3nav 43, embedded-system 41, ros-sbt-interfaces 32, l0hwd 27, l4sim 26, dongle-service 4, l1od 1. Czyli l2dp/l3nav/l0hwd żyją na MR-ach, l1od pushuje bezpośrednio.

**CI** — `.gitlab-ci.yml` w l0hwd, l1od, l2dp, l3nav, ros-sbt-interfaces, dongle-service. Np. `l1od/.gitlab-ci.yml`: obraz `althack/ros2:humble-full`, klonuje [[ros-sbt-interfaces]] tokenem CI (zmienna `CI_WORKER_CI_TOKEN`) i robi `colcon build`.

## Jak bezpiecznie zsynchronizować (opis, nic nie wykonano)
1. **Najpierw zabezpiecz lokalny stan** — w każdym repo `git status`, `git stash list`, `git log @{u}..` (commity niewypchnięte). Dla stanu z Jetsona rozważ kopię katalogu albo `git bundle create ~/backup-<repo>.bundle --all`.
2. **Pobierz bez ruszania drzewa:** `git fetch --all --prune` (dla submodułów: `git submodule foreach git fetch`). Fetch nie zmienia plików roboczych.
3. **Zobacz różnice:** `git log --oneline HEAD..@{u}` (co przyszło), `git rev-list --left-right --count HEAD...@{u}`, `git diff HEAD...@{u} --stat`.
4. **Zacommituj lub odłóż własne zmiany** na osobnej gałęzi (`git switch -c <moja-gałąź>`; `git add -p`; `git commit`), zanim cokolwiek zaktualizujesz. Nie rób `pull` na brudnym drzewie.
5. **Aktualizuj:** `git pull --ff-only` (gdy nie masz własnych commitów) albo `git rebase @{u}` na swojej gałęzi; przy konfliktach `git rebase --abort` przywraca stan.
6. **Wypchnij pracę z zawodów:** `git push -u origin waypoint_first` (l3nav), `git push origin spirala_zjebania` (l2dp), następnie MR do `master` na GitLabie.
7. **Submoduły l0hwd:** `git submodule update --init --recursive` (README l0hwd zaleca klon `--recursive`). Uwaga: `can-msgs` ma lokalne zmiany (2) — `update` na brudnym submodule może je nadpisać.
8. **Nowa maszyna:** `git clone --recursive git@gitlab.com:agh-solar-boat/ab/<repo>.git`, potem `git config user.email <adres koła>`; wszystkie repo do jednego `ws/src/` razem z [[ros-sbt-interfaces]].

Kopie w `bash_do_uruchomienia` nie mają `FETCH_HEAD` (poza `ros-sbt-interfaces`, 2026-09-07) — nie zakładaj, że są aktualne.

## Źródła
- `git remote -v`, `git branch -a -vv`, `git status --porcelain`, `git log --all` w każdym repo w `~/solar` i `~/solar/bash_do_uruchomienia`
- `.git/FETCH_HEAD` (daty fetchu), `l0hwd/.gitmodules`, `zed-ws/.gitmodules`, `l1od/.gitlab-ci.yml`, `l0hwd/README.md`
- `~/.ssh/config` (tylko wpisy `Host`/`IdentityFile`), `git config --global user.email/user.name`

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.
