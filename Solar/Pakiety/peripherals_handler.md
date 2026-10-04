---
typ: solar-pakiet
repo: "[[l0hwd]]"
build: ament_cmake
tags: [solar, pakiet]
zaktualizowano: 2026-10-04
---
# peripherals_handler

> [!skrot] W 30 sekund
> `PeripheralsHandler` wystawia dwa serwisy, `/srv/pump_control` i `/srv/cannon_control`. Wywołanie każdego z nich publikuje na `/to_can_bus` jedną ramkę: `PUMP_CTRL` (ID 7) albo `CANNON_CTRL` (ID 9). Klientami są węzły drzewa zachowań `l3nav`.

## Węzły
### PeripheralsHandler
- Plik: `l0hwd/peripherals_handler/src/peripherals_handler.cpp`, klasa `PeripheralsHandler`, węzeł `"PeripheralsHandler"`, executable `PeripheralsHandler`.
- **Publikuje:** `/to_can_bus` (`can_msgs/msg/Frame`, depth 10) (`:5`).
- **Serwisy (serwer):**
  - `/srv/pump_control` (`l0_interfaces/srv/PumpControl`) → `PompService` (`:16-23`): ramka `PUMP_CTRL` z `direction = HL2BB`, `command = ON`, `timeout = (uint16) request.timeout`. Pole `pump_req` jest ignorowane. Odpowiedź `pomp_res = true`.
  - `/srv/cannon_control` (`l0_interfaces/srv/CannonControl`) → `CannonService` (`:25-32`): ramka `CANNON_CTRL` z `direction = HL2BB`, `command = SHOOT`. Pola żądania są ignorowane. Odpowiedź `cannon_res = 202`.
- Ramki pakowane funkcjami `barka_pump_ctrl_pack` / `barka_cannon_ctrl_pack` z [[can-msgs]] (`:34-61`).
- W `PumpControl.srv` komentarz mówi „timeout in seconds”, a DBC opisuje `PUMP_CTRL.TIMEOUT` w `ms` (`can-msgs/barka.dbc:70`). Kod przepisuje liczbę bez przeliczenia.
- Klienci: `l3nav/waypoint_sequencer/src/waypoint_sequencer.cpp:36-37`, `l3nav/basic_controller/src/use_peripheral_node.cpp:6-7`.

## Launch
`l0hwd_launch.py:41-45` (`name="PeripheralsHandler"`) → [[l0_launch]].

## Konfiguracja (yaml)
Brak.

## Zależności
`can_msgs`, `rclcpp`, `l0_interfaces` → [[can_msgs]], [[l0_interfaces]].

## Interfejsy
Brak własnych (używa `PumpControl`, `CannonControl` z [[l0_interfaces]]).

## Różnice między branchami
Na `master` od `2d0ef53` (2025-02-20, „add cannon service, approved by me, works on jetson”) i `35a408f` („refactor peri handler”).

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.
