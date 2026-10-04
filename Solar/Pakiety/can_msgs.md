---
typ: solar-pakiet
repo: "[[l0hwd]]"
build: ament_cmake
tags: [solar, pakiet]
zaktualizowano: 2026-10-04
---
# can_msgs

> [!skrot] W 30 sekund
> Pakiet interfejsów z jedną wiadomością `can_msgs/msg/Frame`, czyli ramką CAN 2.0 w ROS. Leży w `l0hwd/can_msgs/` i nazywa się tak samo jak pakiet systemowy `ros-humble-can-msgs`, który README i CI każą doinstalować przez apt. Używają go wszystkie węzły L0 i [[ros2_socketcan]]. Nie mylić z repo [[can-msgs]] (DBC + kod C).

## Węzły
Brak (pakiet tylko z interfejsem).

## Launch
Brak.

## Konfiguracja (yaml)
Brak.

## Zależności
`package.xml`: `rosidl_default_generators` (build), `rosidl_default_runtime` (exec), `std_msgs`. `CMakeLists.txt`: `rosidl_generate_interfaces(can_msgs "msg/Frame.msg" DEPENDENCIES std_msgs)`.

## Interfejsy
`l0hwd/can_msgs/msg/Frame.msg`:

| pole | typ |
|---|---|
| `header` | `std_msgs/Header` |
| `id` | `uint32` |
| `is_rtr` | `bool` |
| `is_extended` | `bool` |
| `is_error` | `bool` |
| `dlc` | `uint8` |
| `data` | `uint8[8]` |

Topiki: `/to_can_bus` (ROS → CAN, publikują [[new_diff_thrust_driver]], [[diff_thrust_driver]], [[regulator]], [[peripherals_handler]], [[can_sequence]], w symulacji także `l4sim/sim_can_adapter` jako subskrybent) i `/from_can_bus` (CAN → ROS, subskrybuje [[can_driver]]).

> [!wazne] Dwa pakiety o tej samej nazwie
> README (`l0hwd/README.md`) i CI (`l0hwd/.gitlab-ci.yml`) instalują `ros-humble-can-msgs`, a workspace buduje lokalny `can_msgs`. Po `source install/setup.bash` lokalny przesłania systemowy. Definicja pól jest identyczna z upstreamową `can_msgs/Frame` (**niepewne**, upstreamu nie porównywano bajt po bajcie), więc oba są zgodne z `ros2_socketcan`.

## Różnice między branchami
Na wszystkich gałęziach [[l0hwd]] pakiet jest taki sam (nie porównywano plików `Frame.msg` między gałęziami, **niepewne**).

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.
