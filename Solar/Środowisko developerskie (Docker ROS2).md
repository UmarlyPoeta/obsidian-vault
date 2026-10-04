---
typ: solar-uruchamianie
warstwa: narzędzia
tags: [solar, docker, ros2, wsl, gpu]
zaktualizowano: 2026-10-04
---
# Środowisko developerskie (Docker ROS2)

> [!skrot] W 30 sekund
> - **Co to:** `~/ros2-humble-gpu` — kontener Docker z ROS 2 Humble (Ubuntu 22.04) na PC/WSL2 z kartą NVIDIA. Host to Ubuntu 24.04, a Humble nie ma pakietów na 24.04 (`README.md`, „Dlaczego kontener”).
> - **Główne narzędzie:** skrypt `./ros2dev` (shell, build-ws, rviz, gazebo, gpu…). Obraz `ros2-humble-gpu:latest`, kontener `ros2-humble`, użytkownik `dev`.
> - **Kod:** `~/ros2-humble-gpu/ws` na hoście = `/home/dev/ws` w kontenerze. Obecnie `ws/src` zawiera kopię repozytoriów koła (to samo co `~/solar`, starsza data), a zbudowane są interfejsy i pakiety dokowania z [[l2dp]] (`l2dp_docking_fix`).
> - **Nie jest repozytorium git** (`git status` → „not a git repository”).

Powiązane: [[Uruchamianie i launch]] · [[Git — praca z repozytoriami koła]] · [[Dokumenty i raporty]].

## Struktura
```
~/ros2-humble-gpu/
├── setup.sh              przygotowanie hosta: Docker Engine, NVIDIA Container Toolkit, budowa obrazu (idempotentny)
├── ros2dev               sterowanie środowiskiem (główne wejście)
├── docker-compose.yml    usługa „humble”: GPU, WSLg, wolumeny, sieć host
├── docker/
│   ├── Dockerfile        obraz: CUDA 12.6 + cuDNN (nvidia/cuda:12.6.3-cudnn-devel-ubuntu22.04) + ros-humble-desktop-full
│   ├── entrypoint.sh     source ROS + ws, wykrywanie ścieżki GPU w WSLg
│   ├── ros2-env.sh       → /etc/profile.d/ros2.sh (ROS dla powłok logowania)
│   ├── bashrc-extra      aliasy cb, cbp, ct, cc, sw
│   └── gpu-check.sh      diagnostyka CUDA + OpenGL
├── .devcontainer/devcontainer.json   VS Code Dev Containers (ten sam compose)
├── wslconfig.example     szablon C:\Users\<Ty>\.wslconfig (memory=16GB, processors=10, swap=8GB)
├── build.log             log budowy obrazu (930 kB)
├── scripts/              pusty
└── ws/                   workspace montowany do kontenera
    ├── src/              kopie repo koła (patrz niżej)
    ├── build/ install/ log/
    ├── tools/            skrypty testu dokowania na bagu (+ zrzuty, siatki .npy)
    ├── bags/docking_regulator_2   (1,6 GB)
    ├── bag_islamista/    (8,5 GB, sqlite3, 270 s: /odom, /lidar_points, /fused_detection …)
    └── .check/ .check2/  osobne mini-workspace’y (build/install/log/src) z wybranymi pakietami
```

## Jak zbudować i odpalić
Z `README.md` („Szybki start”):
```bash
cd ~/ros2-humble-gpu
./setup.sh            # Docker + nvidia-container-toolkit + budowa obrazu
./ros2dev gpu         # czy GPU działa
./ros2dev smoke       # test: PCL + Eigen + RViz2 (pakiet ws/src/smoke_test)
./ros2dev shell       # wejście do kontenera
```
Jeśli `setup.sh` dopisał Cię do grupy `docker` — w PowerShellu `wsl --shutdown` i nowy terminal. `ros2dev` sam próbuje `sg docker`, a jak się nie da — `sudo docker` (funkcja `need_docker`).

> [!wazne] `smoke_test` zniknął z `ws/src`
> W `ws/build/` i `ws/install/` jest `smoke_test`, ale w `ws/src/` już go nie ma (obecnie same repo koła). `./ros2dev smoke` w obecnym stanie nie zbuduje pakietu.

### Komendy `ros2dev`
| Komenda | Co robi (z `ros2dev`) |
|---|---|
| `build [--no-cache]` | `docker compose build` z logiem do `build.log` (25–50 min) |
| `up` / `stop` / `restart` / `rm` / `clean` | cykl życia kontenera; `clean` = `down -v --rmi local` (pyta, `ws/` zostaje) |
| `shell` (domyślne) | `docker exec -it ros2-humble bash -l` |
| `exec <cmd>` / `root` | polecenie w kontenerze / powłoka roota |
| `status` | stan kontenera, obrazu, lista pakietów w `ws/src` |
| `build-ws [args]` | `colcon build --symlink-install --cmake-args -DCMAKE_BUILD_TYPE=RelWithDebInfo -DCMAKE_EXPORT_COMPILE_COMMANDS=ON [args]` w `/home/dev/ws` |
| `test-ws` | `colcon test` + `colcon test-result --verbose` |
| `rosdep` | `rosdep install --from-paths src --ignore-src -r -y` |
| `gpu` | `gpu-check` |
| `rviz`, `gazebo [świat]`, `ign [świat]`, `rqt`, `plotjuggler`, `foxglove` | GUI; `ign` domyślnie z `--render-engine ogre` (ogre2 pada na Mesa d3d12 po ~6 s), `foxglove` = `foxglove_bridge` na `ws://localhost:8765` |
| `demo` | talker/listener — test DDS |

Wszystko poza `build`/`clean` samo startuje kontener (`ensure_up` → `docker compose up -d`).

## Montowania i ustawienia kontenera (`docker-compose.yml`)
- `./ws → /home/dev/ws` (rw) — kod, `build/`, `install/`, `log/` żyją na hoście.
- WSLg: `/tmp/.X11-unix`, `/mnt/wslg`; sterowniki GPU: `/usr/lib/wsl` (ro); urządzenie `/dev/dxg`.
- Wolumeny nazwane: `ccache → /home/dev/.ccache`, `bash-history → /home/dev/.history`.
- `/dev/bus/usb` zakomentowane (USB przez usbipd-win — do kamery/LiDAR-u).
- `network_mode: host`, `ipc: host`, `cap_add: SYS_PTRACE`, `seccomp:unconfined`, `ulimits nofile 65536`.
- GPU: `deploy.resources.reservations.devices` driver `nvidia`, `capabilities: [gpu, compute, utility, graphics, display]`.
- Zmienne: `DISPLAY`, `WAYLAND_DISPLAY`, `LD_LIBRARY_PATH=/usr/lib/wsl/lib`, `MESA_D3D12_DEFAULT_ADAPTER_NAME=NVIDIA`, `ROS_DOMAIN_ID=${ROS_DOMAIN_ID:-0}`, `RMW_IMPLEMENTATION` **zakomentowane**.

## GPU i GUI
`README.md`, „Jak działa GPU”: dwie ścieżki — compute (CUDA: `libcuda.so` z `/usr/lib/wsl/lib` → `/dev/dxg`; sprawdzasz `nvidia-smi`, `nvcc`) i render (OpenGL przez Mesa gallium **d3d12**; `glxinfo -B` powinno dać `D3D12 (NVIDIA GeForce RTX 3060)`). `llvmpipe` = render na CPU → sprawdź `/dev/dxg`, montowanie `/usr/lib/wsl/lib`, `export GALLIUM_DRIVER=d3d12`, `wsl --update`. Okna GUI (RViz, Gazebo) otwierają się jako natywne okna Windows przez WSLg; test: `./ros2dev exec xeyes`.

Obraz (README „Co jest w obrazie”): `ros-humble-desktop-full`, Gazebo Classic 11, Ignition Fortress (`ros_gz`), Nav2, slam_toolbox, robot_localization, Cartographer, PCL 1.12, OpenCV 4.5, Open3D, `pointcloud_to_laserscan`, CycloneDDS i FastDDS (`Dockerfile:162`), `numpy<2` przypięte (`Dockerfile:168-173`).

## Jak budować repozytoria z `~/solar`
Montowany jest tylko `~/ros2-humble-gpu/ws`, więc `~/solar` **nie jest widoczny w kontenerze**. Z plików wynika obecny sposób: repozytoria są skopiowane do `ws/src/` (`ws/src/l0hwd`, `l1od`, `l2dp`, `l2dp_docking_fix`, `l3nav`, `ros-sbt-interfaces`, `dongle-service`, `zed-ws`, `sbt_transforms`, `agh-dashboard-live`, `docs` — te same HEAD-y co w `~/solar`). Potem w kontenerze:
```bash
./ros2dev build-ws --packages-select l1_interfaces l2_interfaces l3_interfaces segment_scan pose_graph pointcloud_to_laserscan
./ros2dev shell
sw   # = source /home/dev/ws/install/setup.bash
```
Ten zestaw jest aktualnie zbudowany w `ws/build` (z `CMakeCache.txt`: interfejsy z `ros-sbt-interfaces`, reszta z `l2dp_docking_fix`). Pełnego stosu się tu nie buduje: `ws/src/l2dp/COLCON_IGNORE` wyłącza `l2dp` (duplikaty z `l2dp_docking_fix`), a `zed-ros2-wrapper-humble-v5.1.0/COLCON_IGNORE` wrapper ZED. Inny sposób (niesprawdzony, z `docker-compose.yml`): dopisać wolumen `~/solar/<repo>:/home/dev/ws/src/<repo>`.

Aliasy w kontenerze (`docker/bashrc-extra`): `cb` (build), `cbp <pkg>` (build `--packages-select`), `ct` (test), `cc` (`rm -rf build install log`), `sw` (source).

### Test dokowania na bagu (`ws/tools/`)
- `run_dock_test.sh [bag] [rate] [png] [args dla feature]` — w kontenerze: `pointcloud_to_laserscan_node` (`cloud_in:=/lidar_points`, `angle_increment=0.005`, `min_height=-1.0`, `max_height=2.0`, `range_max=50.0`), `segment_scan` z `l2dp_docking_fix/segment_scan/config/segment_scan_params.yaml`, `pose_graph feature` z `slam.yaml`, `dock_check.py` (zapis PNG), `ros2 bag play bags/docking_regulator_2`; na końcu liczy wykrycia w logu segment_scan.
- `validate_test.sh` — publikuje sztuczne wielokąty na `/dock_polygon` (`geometry_msgs/PolygonStamped`) i `/choose_dock`, sprawdza w logu `Rejected` / `Dock width`.
- `rviz_shot.sh` — RViz z `dock_map.rviz` + `lidar_to_map.py`, zrzuty okna co 4 s (`xwd` → PNG) podczas `run_dock_test.sh`.
Wyniki (PNG) skopiowano do `~/solar/docs/dock_costmap_check/` — patrz [[Dokumenty i raporty]].

## Łączenie z łodzią / innymi maszynami
> [!blad] Domyślnie kontener nie zobaczy stosu łodzi
> Kontener ma `ROS_DOMAIN_ID=0` i domyślny RMW, a stos łodzi używa `ROS_DOMAIN_ID=69` i `rmw_cyclonedds_cpp` (patrz [[Uruchamianie i launch]]). Trzeba: `echo "ROS_DOMAIN_ID=69" >> .env` (README „Konfiguracja”) i odkomentować `RMW_IMPLEMENTATION=rmw_cyclonedds_cpp` w `docker-compose.yml`. Dodatkowo CycloneDDS na Jetsonie jest zamknięty na `lo`, więc nawet wtedy topiki łodzi nie dotrą po sieci; discovery między WSL a Windows wymaga `networkingMode=mirrored` w `.wslconfig` (README „Problemy”).

## Znane problemy (z README)
- RAM WSL 7,7 GB → OOM przy Gazebo + RViz + `colcon build -j12`; podbij `memory` w `.wslconfig` albo `./ros2dev build-ws --parallel-workers 4`.
- Fortress z `ogre2` pada (`Ogre::UnimplementedException` w `GL3PlusTextureGpu::copyTo`) — używaj `--render-engine ogre`.
- `docker: permission denied` → `wsl --shutdown`.
- `numpy` 2 psuje `cv_bridge` i `rclpy`.
- Zmienne ROS są w `/etc/profile.d/ros2.sh`, nie w `.bashrc`, bo `docker exec … bash -lc` musi mieć ROS bez trybu interaktywnego.

## Źródła
- `~/ros2-humble-gpu/README.md`, `docker-compose.yml`, `ros2dev`, `setup.sh`, `docker/{Dockerfile,entrypoint.sh,ros2-env.sh,bashrc-extra,gpu-check.sh}`, `.devcontainer/devcontainer.json`, `wslconfig.example`
- `~/ros2-humble-gpu/ws/tools/{run_dock_test.sh,validate_test.sh,rviz_shot.sh}`, `ws/build/*/CMakeCache.txt`

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.
