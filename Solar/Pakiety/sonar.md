---
typ: solar-pakiet
repo: "[[l0hwd]]"
build: ament_python
tags: [solar, pakiet]
zaktualizowano: 2026-10-04
---
# sonar

> [!skrot] W 30 sekund
> Dwa węzły Pythona do echosondy ECT400 (Echologger). `ect400_driver` czyta NMEA po porcie szeregowym i publikuje głębokość jako `sensor_msgs/msg/Range` na `/echologger/depth`. `depth_logger` co 0.2 s (5 Hz) zapisuje do CSV ostatnią pozycję GNSS z `/gnss` razem z głębokością. Opis z `setup.py`: „ROS2 package for ECT400 driver and GNSS-depth CSV logging”.

## Węzły
### echologger_range_publisher (executable `ect400_driver`)
- Plik: `l0hwd/sonar/sonar/ect400_driver.py`, klasa `EchologgerRangePublisher`, węzeł `'echologger_range_publisher'`.
- **Parametry** (`:16-23`): `port='/dev/ttyUSB5'`, `baud=115200`, `frame_id='echologger_link'`, `prefer_sentence='DPT'` (albo `DBT`), `poll_hz=20.0`, `min_range=0.15`, `max_range=100.0`, `field_of_view=0.0872665` rad (5°).
- **Publikuje:**
  - `/echologger/depth` (`sensor_msgs/msg/Range`, `radiation_type = ULTRASOUND`);
  - `/echologger/raw_echo` (`std_msgs/msg/String`) z każdą linią NMEA.
- **Logika:** timer `1/poll_hz` czyta do 512 B z `pyserial` i składa linie. Parsuje zdania `*DPT` (pole 1 = głębokość) i `*DBT` (pole 3 = metry), odrzuca wartości spoza `[min_range, max_range]` (`:53-138`). Nie używa submodułu `echosounderapi-python`.

### boat_echo_csv_logger (executable `depth_logger`)
- Plik: `l0hwd/sonar/sonar/depth_logger.py`, klasa `BoatEchoCsvLogger`, węzeł `'boat_echo_csv_logger'`.
- **Parametry:** `gnss_topic='/gnss'`, `depth_topic='/echologger/depth'`, `depth_msg_type='range'` (albo `'custom'` = `boat_msgs/msg/DepthSample`, pakietu nie ma w `~/solar`), `raw_echo_topic='/echologger/raw_echo'`, `sample_hz=5.0`, `max_stale_s=2.0`, `csv_dir=~/ros2_logs/boat_echo`, `csv_prefix='boat_echo'`, `rotate_max_mb=100.0`, `rotate_keep=10`.
- **Subskrybuje:** `/gnss` (`sensor_msgs/msg/NavSatFix`), `/echologger/depth` (`Range`), `/echologger/raw_echo` (`String`).
- **Wyjście:** pliki `<prefix>_<UTC>.csv` z kolumnami `timestamp, lat, lon, alt, x, y, z, depth, depth_units, raw_echo_payload`. Kolumny `x, y, z` są zawsze puste. Rotacja po przekroczeniu `rotate_max_mb`.

## Launch
`l0hwd/sonar/launch/sonar.launch.py` uruchamia tylko `depth_logger` (`name='depth_logger'`) z parametrami: `csv_dir='/tmp/boat_echo_logs'`, `csv_prefix='survey'`, reszta jak domyślne. Sterownika `ect400_driver` launch **nie** startuje.

## Konfiguracja (yaml)
Brak plików YAML.

## Zależności
`package.xml`: tylko zależności testowe (`ament_flake8` itd.). Kod importuje `rclpy`, `sensor_msgs`, `std_msgs`, `serial` (`pyserial`), ale `package.xml` tego nie deklaruje. Maintainer: julia.czekanska.

## Interfejsy
Brak.

## Różnice między branchami
Pakiet jest tylko na `master` (nie ma go na `music` ani `sim_controller`).

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.
