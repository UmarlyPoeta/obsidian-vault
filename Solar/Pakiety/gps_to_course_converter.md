---
typ: solar-pakiet
repo: "[[l4sim]]"
build: ament_cmake
tags: [solar, pakiet]
zaktualizowano: 2026-10-04
---
# gps_to_course_converter

> [!skrot] W 30 sekund (co robi, wejście → wyjście)
> Liczy kurs z kolejnych pozycji GPS symulatora → `/course/data_sim` (`std_msgs/Float32`). Relikt.

Opis wg `~/solar/bash_do_uruchomienia/l4sim@main` (`7a789a2`), katalog `src/gps_to_course_converter`.

## Węzły
- `GpsToCourseConverter` (executable `GpsToCourseConverter`): subskrybuje `/wamv/sensors/gps/gps/fix` (`NavSatFix`), publikuje `/course/data_sim`.

## Launch
Brak.

## Konfiguracja (yaml)
Brak.

## Zależności
`rclcpp`, `sensor_msgs`, `std_msgs`. W `Dockerfile.sim` kopiowany do obrazu.

## Interfejsy
Brak własnych.

## Różnice między branchami
Bez zmian.

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.
