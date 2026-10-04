---
typ: solar-pakiet
repo: "[[l2dp]]"
build: ament_python
tags: [solar, pakiet]
zaktualizowano: 2026-10-04
---
# viz_correction

> [!skrot] W 30 sekund (co robi, wejście → wyjście)
> Pomocniczy republisher do RViz: zamienia osie w ścieżce i markerach, żeby dało się je nałożyć na mapę (stos nie ma drzewa TF). `/path` → `/path_2` (x↔y), `/map_detections` i `/spatial_points` → `/map_detections_2` (x := y, y := −x). Dodany na linii Njord (nie ma go na `master`).

## Węzły
- `swap_xy_node` (`viz_correction/viz_correction.py`, executable `viz_correction`): subskrybuje `/path` (`nav_msgs/Path`), `/map_detections`, `/spatial_points` (`visualization_msgs/MarkerArray`); publikuje `/path_2` (`Path`), `/map_detections_2` (`MarkerArray`). `path_callback` zamienia x↔y, `markers_callback` robi obrót (x, y) → (y, −x) dla pozycji markerów i ich punktów.

## Launch
Brak.

## Konfiguracja (yaml)
Brak.

## Zależności
`package.xml` bez `depend` (tylko testy). Dane z [[planner]] (`/path`) i [[pose_graph]]/[[mapper]] (`map_detections`).

## Interfejsy
Brak.

## Różnice między branchami
Tylko na gałęziach Njord (`scan_matching`, `spirala_zjebania`, `feat/dock_costmap_fix`).

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.
