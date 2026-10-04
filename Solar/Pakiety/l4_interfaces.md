---
typ: solar-pakiet
repo: "[[ros-sbt-interfaces]]"
build: ament_cmake
tags: [solar, pakiet]
zaktualizowano: 2026-10-04
---
# l4_interfaces

> [!skrot] W 30 sekund (co robi, wejście → wyjście)
> Pakiet z jedną wiadomością, `WorldObject`. Z nazwy przeznaczony dla warstwy L4 (symulacja, `l4sim`), ale grep po `~/solar`, także po `bash_do_uruchomienia/l4sim`, nie znajduje żadnego użycia.

## Węzły
Brak.

## Launch
Brak.

## Konfiguracja (yaml)
Brak.

## Zależności
`std_msgs`, `std_srvs`. Maintainer: MKalandykM (mateusz.kalandyk@solarboat.agh.edu.pl).

## Interfejsy
| plik | pola |
|---|---|
| `msg/WorldObject.msg` | `string name`, `uint64 id`, `float64 x`, `float64 y` |

## Różnice między branchami
Bez różnic między `main` a kopią `bash_do_uruchomienia`.

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.
