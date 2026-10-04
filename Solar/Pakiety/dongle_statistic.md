---
typ: solar-pakiet
repo: "[[dongle-service]]"
build: ament_cmake
tags: [solar, pakiet]
zaktualizowano: 2026-10-04
---
# dongle_statistic

> [!skrot] W 30 sekund (co robi, wejście → wyjście)
> Wrapper na `vnstat`. Węzeł co `1/publish_rate` s wywołuje `vnstat -i <interface> --json` i `who`, a potem publikuje dzienny i miesięczny transfer (GB) oraz liczbę sesji SSH jako `dongle_statistic/msg/TrafficStats`. Pakiet istnieje tylko na gałęzi `Dongle-Statistic` repo [[dongle-service]].

## Węzły
### dongle_statistic_node
- Plik: `dongle-service/dongle_statistic/dongle_statistic/dongle_statistic_node.py`, klasa `DongleStatisticNode`, węzeł `'dongle_statistic_node'`. Instalowany przez CMake `install(PROGRAMS … RENAME dongle_statistic_node)`.
- **Parametry:** `interface` = `'wlan0'`, `publish_rate` = `0.2` Hz (`:15-16`).
- **Publikuje:** `dongle_statistic/traffic_stats` (względny, czyli `/dongle_statistic/traffic_stats`), typ `dongle_statistic/msg/TrafficStats`, depth 10.
- **Logika** (`timer_callback`, `:94-135`):
  - `vnstat --json` → pierwszy interfejs;
  - `traffic.day[0]` i `traffic.month[0]` → `rx`/`tx`/suma w GB (zaokrąglone do 0.01);
  - `ssh_connections` = liczba unikalnych hostów w nawiasach z wyjścia `who`;
  - przy starcie ostrzega, jeśli `vnstat` nie jest zainstalowany.

## Launch
`dongle_statistic/launch/dongle_statistic_launch.py`: argumenty `interface` (`wlan0`) i `publish_rate` (`0.2`), `output='screen'`, `emulate_tty=True`. Uruchamiany przez `dongle-statistic.service` bez argumentów.

## Konfiguracja (yaml)
Brak.

## Zależności
`rclpy`, `std_msgs`, `rosidl_default_generators`, `ament_cmake_python`. Maintainerzy: Wiktor Szyszka, Bartosz Woźnica.

## Interfejsy
`msg/TrafficStats.msg`: `string timestamp`, `string interface_name`, `float64 today_received_gb`, `float64 today_transmited_gb`, `float64 today_total_gb`, `float64 month_received_gb`, `float64 month_transmited_gb`, `float64 month_total_gb`, `uint32 ssh_connections`.

## Różnice między branchami
Brak na `main` i `QoL/fixes_hotplug_watchdog_doctor`.

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.
