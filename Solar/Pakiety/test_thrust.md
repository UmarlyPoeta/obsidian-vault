---
typ: solar-pakiet
repo: "[[l4sim]]"
build: ament_cmake
tags: [solar, pakiet]
zaktualizowano: 2026-10-04
---
# test_thrust

> [!skrot] W 30 sekund (co robi, wejście → wyjście)
> Węzeł testowy pędników WAM-V: co 100 ms przechodzi sekwencję naprzód → wstecz → lewo → prawo → naprzód… publikując ciągi bezpośrednio na topiki VRX. README wspomina zapis metryk do CSV (`testThrust`) — **niepewne**, w przejrzanym fragmencie kodu nie widać zapisu CSV.

Opis wg `~/solar/bash_do_uruchomienia/l4sim@main` (`7a789a2`), katalog `src/testThrust`.

## Węzły
- `test_thrust_node` (`src/testThrust.cpp`, executable `thrust_node`): publikuje `/wamv/thrusters/left/thrust`, `/wamv/thrusters/right/thrust`; timer 100 ms.

## Launch
Brak.

## Konfiguracja (yaml)
Brak.

## Zależności
`rclcpp`, `std_msgs`. Katalog `src/testThrust`.

## Interfejsy
Brak własnych.

## Różnice między branchami
Dodany w `137feae` „controller handle and test scripts for controller” (2026-02-11) — **niepewne**.

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.
