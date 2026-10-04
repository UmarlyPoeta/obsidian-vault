---
typ: solar-pakiet
repo: "[[l4sim]]"
build: ament_cmake
tags: [solar, pakiet]
zaktualizowano: 2026-10-04
---
# scan_rotator

> [!skrot] W 30 sekund (co robi, wejście → wyjście)
> Obraca kąty `LaserScan` (domyślnie o 90°), by skan z symulatora zgadzał się z montażem LiDAR-a na łodzi: `/scan_prerotated` → `/scan` (wejście [[segment_scan]]).

Opis wg `~/solar/bash_do_uruchomienia/l4sim@main` (`7a789a2`), katalog `src/scan_rotator`.

## Węzły
- `scan_rotator` (executable `ScanRotator`). Parametr `angle_offset_deg` 90.0. Subskrybuje `/scan_prerotated`, publikuje `/scan` (`sensor_msgs/LaserScan`). README podaje wyjście `/scan_rotated` — kod publikuje `/scan`.

## Launch
Brak; `ros2 run scan_rotator ScanRotator`. Wymaga, by konwerter chmury publikował `/scan_prerotated` (przykładowy launch [[pointcloud_to_laserscan]] remapuje na `/scan` — **niepewne**, jak to łączono).

## Konfiguracja (yaml)
Brak.

## Zależności
`rclcpp`, `sensor_msgs`.

## Interfejsy
Brak własnych.

## Różnice między branchami
Bez zmian.

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.
