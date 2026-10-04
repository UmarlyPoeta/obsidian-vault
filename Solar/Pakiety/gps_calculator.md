---
typ: solar-pakiet
repo: "[[l2dp]]"
build: ament_cmake
tags: [solar, pakiet]
zaktualizowano: 2026-10-04
---
# gps_calculator

> [!skrot] W 30 sekund (co robi, wejście → wyjście)
> Przelicza fix GNSS `/gps/nav_sat_fix` na pozycję metryczną względem punktu „home” (`/gps/metric_position`, `l2_interfaces/MetricPosition`) i udostępnia serwisy ustawiania home. Przodek [[odometry]] — dziś relikt: w przejrzanych repo nikt nie publikuje `/gps/nav_sat_fix` (**niepewne** dla repo innych grup).

## Węzły
### `GpsConverter` (klasa `GpsConverter`)
- Źródło: `gps_calculator/src/gps_converter.cpp`, `src/node.cpp`, `include/gps_converter.hpp`. Executable `GpsConverter`.
- Subskrybuje `/gps/nav_sat_fix` (`sensor_msgs/NavSatFix`); publikuje `/gps/metric_position` (`l2_interfaces/MetricPosition`: `distance`, `x` = odległość haversine po długości (wschód), `y` = po szerokości (północ), znaki wg kierunku).
- Serwisy (serwer): `/srv/set_home_keyboard_input` (`l2_interfaces/srv/SetHome`), `/srv/set_home_boat_curr_position` (`std_srvs/Trigger`).
- Pierwszy fix staje się home (`gps_converter.cpp:33–40`).

## Launch
Brak.

## Konfiguracja (yaml)
Brak.

## Zależności
`l2_interfaces`, `rclcpp`, `sensor_msgs`, `std_srvs`.

## Interfejsy
Brak.

## Różnice między branchami
Brak istotnych w linii Njord.

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.
