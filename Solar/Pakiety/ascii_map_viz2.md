---
typ: solar-pakiet
repo: "[[l2dp]]"
build: ament_python
tags: [solar, pakiet]
zaktualizowano: 2026-10-04
---
# ascii_map_viz2

> [!skrot] W 30 sekund (co robi, wejście → wyjście)
> Podgląd costmapy w terminalu (ASCII, opcjonalnie w kolorze) z naniesioną łodzią, ścieżką i śledzonymi detekcjami — przydatny przez SSH na Jetsonie bez RViz.

## Węzły
- `ascii_costmap_viz` (`ascii_map_viz2/ascii_map_viz2.py`, executable `ascii_map_viz2`).
- Parametry (`:70–77`): `costmap_topic` `/costmap`, `odom_topic` `/odom`, `path_topic` `/path`, `detections_topic` `/tracked_detections`, `max_size` 125, `rate_hz` 5.0, `occupied_thresh` 40, `use_color` True.
- Subskrybuje: `OccupancyGrid` (costmap_topic), `Odometry`, `Path`, `l2_interfaces/TrackedDetectionsVector`; timer `1/rate_hz` → `render`.
- Domyślnie czyta costmapę [[mapper]] (`/costmap`); dla stanu zawodów trzeba podać `-p costmap_topic:=/slam/global_costmap` (wniosek z nazw topików, nie z dokumentacji).

## Launch
Brak.

## Konfiguracja (yaml)
Brak.

## Zależności
`package.xml` tylko testy (brak `depend`). W kodzie komentarz „TODO: Import your actual message type here”.

## Interfejsy
Brak.

## Różnice między branchami
Drzewa robocze `~/solar/l2dp` i `l2dp_docking_fix` mają zmieniony tylko tryb pliku `ascii_map_viz2.py`.

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.
