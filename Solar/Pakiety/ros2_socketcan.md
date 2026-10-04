---
typ: solar-pakiet
repo: "[[l0hwd]]"
build: ament_cmake
tags: [solar, pakiet]
zaktualizowano: 2026-10-04
---
# ros2_socketcan

> [!skrot] W 30 sekund
> Zewnętrzny pakiet Autoware Foundation (`https://github.com/autowarefoundation/ros2_socketcan`), dołączony do [[l0hwd]] jako submoduł przypięty na tagu **1.2.0** (`8996cdb`, 2023-03-04). Łączy linuksowy SocketCAN (`can0`) z ROS 2. Odbiornik publikuje ramki z magistrali na `/from_can_bus`, nadajnik wysyła na magistralę wszystko z `/to_can_bus`. Cała komunikacja L0 z płytkami idzie przez te dwa topiki.

## Węzły
Oba są węzłami lifecycle, aktywowanymi automatycznie przez launch (`auto_configure:=true`, `auto_activate:=true`).

### socket_can_receiver (executable `socket_can_receiver_node_exe`)
- `ros2_socketcan/src/socket_can_receiver_node.cpp`.
- Parametry: `interface="can0"`, `use_bus_time=false`, `enable_can_fd=false`, `interval_sec=0.01`, `filters="0:0"` (`:38-42`).
- Publikuje: `from_can_bus` (`can_msgs/msg/Frame`, depth 500), przy FD `from_can_bus_fd` (`ros2_socketcan_msgs/msg/FdFrame`) (`:72-75`). Nazwy są względne, a namespace pusty, więc w praktyce `/from_can_bus`.

### socket_can_sender (executable `socket_can_sender_node_exe`)
- `ros2_socketcan/src/socket_can_sender_node.cpp`.
- Parametry: `interface="can0"`, `enable_can_fd=false`, `timeout_sec=0.01` (`:36-38`).
- Subskrybuje: `to_can_bus` (`can_msgs/msg/Frame`, depth 500), `to_can_bus_fd` (`:63-67`).

## Launch
- `ros2_socketcan/launch/socket_can_bridge.launch.xml`: argumenty `interface=can0`, `receiver_interval_sec=0.01`, `sender_timeout_sec=0.01`, `enable_can_fd=false`. Dołącza `socket_can_receiver.launch.py` i `socket_can_sender.launch.py`.
- Dołączany z [[l0_launch]] bez nadpisywania argumentów.
- Bitrate magistrali **nie** jest ustawiany w ROS. Interfejs `can0` trzeba podnieść w systemie (`ip link set can0 up type can bitrate …`). Polecenia ani bitrate dla łodzi autonomicznej nie znaleziono w `~/solar` (patrz [[Magistrala CAN]]).

## Konfiguracja (yaml)
Brak plików w użyciu (tylko argumenty launch).

## Zależności
`rclcpp`, `rclcpp_components`, `rclcpp_lifecycle`, `lifecycle_msgs`, `can_msgs`, `ros2_socketcan_msgs` (drugi pakiet z submodułu, wiadomość `FdFrame`). `can_msgs` dostarcza lokalny [[can_msgs]] albo apt `ros-humble-can-msgs`.

## Interfejsy
`ros2_socketcan_msgs/msg/FdFrame` (nieużywany w łodzi, bo FD jest wyłączone).

## Różnice między branchami
Submoduł jest przypięty do `8996cdb` na `master`. Zdalne gałęzie samego submodułu: `main`, `humble`, `iron`, `jazzy`, `add-destructor`, `add-nodes`, `reorg-for-canfd-support`.

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.
