---
typ: solar-pakiet
repo: "[[l0hwd]]"
build: ament_cmake
tags: [solar, pakiet]
zaktualizowano: 2026-10-04
---
# l0_launch

> [!skrot] W 30 sekund
> Pakiet z jednym launchem, który stawia całą warstwę L0 na łodzi: most SocketCAN (`can0`), sterownik lidaru Hesai, [[can_driver]], [[new_diff_thrust_driver]] i [[peripherals_handler]].

## Węzły
Brak własnych.

## Launch
`l0hwd/l0_launch/launch/l0hwd_launch.py`:

| element | szczegóły |
|---|---|
| `IncludeLaunchDescription(XML)` | `ros2_socketcan/launch/socket_can_bridge.launch.xml` z argumentami domyślnymi (`interface:=can0`, `receiver_interval_sec:=0.01`, `sender_timeout_sec:=0.01`, `enable_can_fd:=false`) → [[ros2_socketcan]] |
| `IncludeLaunchDescription(Python)` | `hesai_ros_driver/launch/start.py` (pakiet spoza `~/solar`) |
| `Node` | `can_driver` / `CanDriver` / `name="CanDriver"` |
| `Node` | `new_diff_thrust_driver` / `NewDiffThrustDriver` (komentarz `#changed to new`) |
| `Node` | `peripherals_handler` / `PeripheralsHandler` |

Brak argumentów, remapów i namespace. Uruchomienie: `ros2 launch l0_launch l0hwd_launch.py` (README podaje błędnie `l0hwd.py`).

W importach jest literówka `PythonLaunchArgumetsSource` (`:5`). Jeśli ta nazwa nie istnieje w `launch.launch_description_sources`, import się nie powiedzie (**niepewne**, nie uruchamiano).

## Konfiguracja (yaml)
Brak.

## Zależności
`package.xml`: `ros2_socketcan`, `can_driver`, `diff_thrust_driver`, `peripherals_handler`. Brakuje `new_diff_thrust_driver` i `hesai_ros_driver`, choć launch ich używa. CMake instaluje katalog `launch/`.

## Interfejsy
Brak.

## Różnice między branchami
Na `sbg-info` i `old_architecture` zamiast tego pakietu jest `l1_launch` (stara nazwa warstwy). W drzewie roboczym zmieniony jest tylko tryb pliku (755→644).

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.
