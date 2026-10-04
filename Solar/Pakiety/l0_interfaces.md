---
typ: solar-pakiet
repo: "[[ros-sbt-interfaces]]"
build: ament_cmake
tags: [solar, pakiet]
zaktualizowano: 2026-10-04
---
# l0_interfaces

> [!skrot] W 30 sekund (co robi, wejście → wyjście)
> Interfejsy warstwy sprzętowej, 12 msg i 2 srv. Najważniejsze w użyciu to `SpeedVector` (L2/L3 → [[diff_thrust_driver]]), `BmsData` i `BmsVoltages` ([[can_driver]] → dashboard) oraz serwisy `PumpControl` i `CannonControl` (L3 → [[peripherals_handler]]). Pakiet powstał przez zmianę nazwy dawnego `l1_interfaces` („l1 to l0”, `01b72cd`, 2025-10-15).

## Węzły
Brak (pakiet interfejsów).

## Launch
Brak.

## Konfiguracja (yaml)
Brak.

## Zależności
`package.xml`: `std_msgs`, `rosidl_default_generators` (buildtool), `rosidl_default_runtime`. Maintainer: treborsky (rob.koziarski2k@gmail.com).

## Interfejsy
| plik | pola |
|---|---|
| `msg/SpeedVector.msg` | `float64 linear`, `float64 angular` |
| `msg/Course.msg` | `std_msgs/Header header`, `float64 course` |
| `msg/Measurement.msg` | `std_msgs/Header header`, `float64 temperature`, `float64 current`, `float64 power`, `float64 soc` |
| `msg/VoltageMeasurement.msg` | `std_msgs/Header header`, `float64 voltage` |
| `msg/MeasureOld.msg` | `std_msgs/Header header`, `int16 source`, `float64 data` |
| `msg/GuiGraph.msg` | `float64 energy_consumption`, `float64 battery_percentage_left`, `float64 battery_percentage_right`, `float64 battery_percentage_central` |
| `msg/VesselState.msg` | `float64 latitude`, `float64 longitude`, `float64 heading`, `float64 speed`, `float64 distance_to_waypoint` |
| `msg/MissionState.msg` | `string mission`, `string state`, `string transition` |
| `msg/CellVoltages.msg` | `float32 cell1`, `float32 cell2`, `float32 cell3`, `float32 cell4` |
| `msg/BmsVoltages.msg` | `std_msgs/Header header`, `CellVoltages central`, `CellVoltages left`, `CellVoltages right` |
| `msg/BmsUnitData.msg` | `float32 temperature`, `float32 current`, `float32 power`, `float32 soc` |
| `msg/BmsData.msg` | `std_msgs/Header header`, `BmsUnitData central`, `BmsUnitData left`, `BmsUnitData right` |
| `srv/PumpControl.srv` | stałe `bool POMP_START = 1`, `bool POMP_STOP = 0`; `bool pump_req`, `int32 timeout` (komentarz: „timeout in seconds”) → `bool pomp_res` |
| `srv/CannonControl.srv` | `uint8 cannon_direction`, `uint8 cannon_command` (oba z komentarzem „# IDK ask Damian”) → `int32 cannon_res` |

Kto czego używa: [[Interfejsy (msg, srv, action)#l0_interfaces]].

## Różnice między branchami
- Na kopii `bash_do_uruchomienia/ros-sbt-interfaces` (`c7322b5`) pakiet jest taki sam jak na `main`, bo wiadomości BMS doszły właśnie w `c7322b5`.
- W [[l0hwd]] gałąź `sbg-info` używa jeszcze starej nazwy `l1_interfaces` dla tych typów.

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.
