---
typ: solar-pakiet
repo: "[[l4sim]]"
build: ament_cmake
tags: [solar, pakiet]
zaktualizowano: 2026-10-04
---
# wamv_description

> [!skrot] W 30 sekund (co robi, wejście → wyjście)
> Opis URDF/xacro łodzi WAM-V używanej w symulacji jako model „barki”.

Opis wg `~/solar/bash_do_uruchomienia/l4sim@main` (`7a789a2`), katalog `src/vrx/vrx_urdf/wamv_description`.

## Węzły
Brak węzłów.

## Launch
Upstream.

## Konfiguracja (yaml)
URDF w `urdf/`.

## Zależności
`xacro`.

## Interfejsy
Brak własnych.

## Różnice między branchami
`origin/small-sim` (2025-11-23, Kacper Kustra, „make barka smaller”; niezmergowana) zmniejsza model w `wamv_description/urdf` i `wamv_gazebo/urdf`. Na `main` `137feae` „correct center of gravity” i `8df0c4d` „re scaled simulation” (filnaw).

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.
