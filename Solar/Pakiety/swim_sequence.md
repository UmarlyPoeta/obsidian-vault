---
typ: solar-pakiet
repo: "[[l0hwd]]"
build: ament_cmake
tags: [solar, pakiet]
zaktualizowano: 2026-10-04
---
# swim_sequence

> [!skrot] W 30 sekund
> Testowa sekwencja „pływania”. Węzeł `SwimSequence` publikuje kolejne `SpeedVector` na `/l2/speed_vector`, czyli udaje L2. Po każdym kroku zeruje ciąg i czeka, aż prędkość z `/filter/velocity` spadnie poniżej 0.05 m/s, zanim przejdzie dalej. Odbiera to [[diff_thrust_driver]].

## Węzły
### SwimSequence
- Plik: `l0hwd/swim_sequence/src/swim_sequence.cpp`, `main` w `src/node.cpp`, węzeł `"SwimSequence"`, executable `SwimSequence`.
- **Parametry** (`:7-9`):
  - `linear_sequence` = `[0.5, 1.0, -0.5, -1.0]`
  - `angular_sequence` = `[0, 0, 0, 0]`
  - `durations` = `[10, 10, 10, 10]` s
  - Wartości przycinane do [−1, 1] (`:24-25`).
- **Publikuje:** `/l2/speed_vector` (`l0_interfaces/msg/SpeedVector`).
- **Subskrybuje:** `/filter/velocity` (`geometry_msgs/msg/Vector3Stamped`) → `velocity_ = √(x²+y²)`.
- **Timer** 100 ms, maszyna stanów `EXECUTING` → `WAITING_DRIFT` (`:46-105`). W `WAITING_DRIFT` wysyła zera, dopóki `v > 0.05`, i loguje przy tym „Tokyo Drift”.
- Działa tylko z [[diff_thrust_driver]], który z kolei nadaje dopiero po przełączeniu `/regulator_or_thrust_driver` na `diff_thrust_driver` (drzewo robocze).

## Launch
Brak.

## Konfiguracja (yaml)
Brak.

## Zależności
`rclcpp`, `geometry_msgs`, `l0_interfaces` → [[l0_interfaces]].

## Interfejsy
Brak.

## Różnice między branchami
`sim_controller` zmienia `swim_sequence.cpp` w 6 liniach. Pakiet dodano w `cc69320` (2025-11-30, „artificial swimming node”).

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.
