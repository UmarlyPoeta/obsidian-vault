---
typ: solar-pakiet
repo: "[[l4sim]]"
build: ament_cmake
tags: [solar, pakiet]
zaktualizowano: 2026-10-04
---
# vrx_gz

> [!skrot] W 30 sekund (co robi, wejście → wyjście)
> Pakiet VRX (Virtual RobotX, upstream OSRF) z lokalnymi zmianami: światy Gazebo Garden, modele, launch `competition.launch.py`. Tu dodano światy konkursów koła (MONACO26_FIGURE8, NJORD_*, RB26_*, RB_*) i modele (boje NJORD_*, `dock_njord1/2`, RB_*).

Opis wg `~/solar/bash_do_uruchomienia/l4sim@main` (`7a789a2`), katalog `src/vrx/vrx_gz`.

## Węzły
Węzły/pluginy upstream VRX (zadania, wiatr `vrx::USVWind`, fale). Nie analizowano szczegółowo.

## Launch
- `launch/competition.launch.py` — `ros2 launch vrx_gz competition.launch.py world:=<ŚWIAT>` (np. `RB_FOLLOW_THE_PATH`, `NJORD_NAV`, `MONACO26_FIGURE8`); także `spawn.launch.py`, `spawn_config.launch.py`, `vrx_environment.launch.py`, `usv_joy_teleop.py`.

## Konfiguracja (yaml)
Światy w `worlds/`: własne `MONACO26_FIGURE8.sdf` (`7a789a2`, 2026-05-25; dwie boje `NJORD_GREEN`, `coast_waves`, współrzędne sferyczne świata Sydney −33.72, 150.68; URI modeli lokalnych to ścieżki bezwzględne `file:///home/solar/l4sim/install/share/...` — świat działa tylko w takiej ścieżce), `NJORD_DOCKING1/2`, `NJORD_MAN`, `NJORD_NAV` (domyślny wg `docs/WORLDS.md`), `OCTAGON_COLLECTION/DELIVERY`, `RB26_FOLLOW_THE_PATH`, `RB26_LIGHT`, `RB26_SPEED`, `RB_24`, `RB_DOCKING`, `RB_DOCK_OCTAGON_BUOY`, `RB_FOLLOW_THE_PATH`, `RB_SPEED_CHALLENGE` + światy upstream (sydney_regatta, nbpark, *_task). 63 modele w `models/`.

## Zależności
Gazebo Garden (`gz-garden`, `ros-humble-ros-gzgarden`, `python3-sdformat13` — `Dockerfile.sim`).

## Interfejsy
Brak własnych.

## Różnice między branchami
`origin/collision-world` (2026-07-27, Martyna Czerniak; niezmergowana): świat kolizji Njord i model `NJORD_SMALL_MOVING_BOAT` (poruszająca się łódź do testu [[colision_detector]]). `origin/depth-camera` zmergowana (świat RB26_LIGHT).

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.
