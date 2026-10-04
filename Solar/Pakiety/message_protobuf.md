---
typ: solar-pakiet
repo: "[[l2dp]]"
build: ament_python
tags: [solar, pakiet]
zaktualizowano: 2026-10-04
---
# message_protobuf

> [!skrot] W 30 sekund (co robi, wejście → wyjście)
> Agregator raportów dla organizatorów zawodów (RoboBoat / „Robocommand”): zbiera stan łodzi, pozycję GNSS, kurs, prędkość, zadanie, detekcje, wyniki dokowania i sygnałów dźwiękowych i wysyła je jako komunikaty protobuf (`robocommand.roboboat.v1.Report`) przez TCP do `127.0.0.1:50000`. Heartbeat co 1 s, budowanie wiadomości co 0,25 s.

## Węzły
### `protobuf` (klasa `Protobuf`, `message_protobuf/protobuf.py`, executable `protobuf`)
- Stałe: `team_id = "AGHK"`, `vehicle_id = "BARKA"`, `server_addr = ("127.0.0.1", 50000)` (`protobuf.py:23–49`).
- **Subskrybuje** (`:57–70`): `/gnss` (`NavSatFix`), `/fused_detection` (`l1_interfaces/VectorOfFusedDetections`), `/start_pose` (`Point`), `filter/euler` i `filter/velocity` (`Vector3Stamped`, nazwy względne), `/robot/state` (`String` — z `can_driver` w [[l0hwd]]: killed/manual/auto), `/robot/task` (`String` — z węzła BT `SetTask` w [[basic_controller]]), `/delivery` (`l3_interfaces/Delivery`), `/robot/speed_challenge` (`SpeedReport`), `/dock/side` (`String`), `/dock/slip` (`Float32`), `/sound_signal` (`String`).
- **Publikuje:** `/protobuf_message` (`std_msgs/UInt8MultiArray`, kopia testowa).
- Timery: 1.0 s → `send_heartbeat` (`:258`), 0.25 s → `build_message` (`:271`). Połączenie TCP z timeoutem 1 s (`connect_to_server`, `:342`).
- Mapowanie stanu na enum `RobotState` (`STATE_KILLED/MANUAL/AUTO`), zadania na `TaskType` (`ENTRY_EXIT`, `NAV_CHANNEL`, `SPEED_CHALLENGE`, `OBJECT_DELIVERY`, `DOCKING`, `SOUND_SIGNAL`).

## Launch
Brak; `ros2 run message_protobuf protobuf`.

## Konfiguracja (yaml)
Brak.

## Zależności
`protobuf`, `python3-protobuf`, `rclpy`, `sensor_msgs`, `geometry_msgs`, `nav_msgs`, `std_msgs`, `l2_interfaces` (w kodzie też `l1_interfaces`, `l3_interfaces`).

## Interfejsy
- `proto/report.proto` (package `robocommand.roboboat.v1`): enumy `RobotState`, `TaskType`, `ObjectType` (BOAT, LIGHT_BEACON, BUOY), `Color`, `GateType`, `DeliveryType`, `SignalType`; wiadomości `Report` (`team_id`, `vehicle_id`, `seq`, `sent_at`, `oneof body`), `LatLng`, `Heartbeat` (state, position, `spd_mps`, `heading_deg`, `current_task`), `ObjectDetected`, `GatePass`, `ObjectDelivery`, `Docking` (`dock`, `slip`), `SoundSignal` (`signal_type`, `frequency_hz`, `assigned_task`).
- `message_protobuf/report_pb2.py` — wygenerowany kod.
- `message_protobuf/test_server.py` — testowy serwer (`socketserver`, parsuje `Report`; komunikat błędu mówi o UDP, choć klient używa TCP).

## Różnice między branchami
Zmergowana `origin/communication_protobuf` (2026-02: heartbeat 1 Hz, raport 5 Hz → dziś 4 Hz timer, tematy delivery i speed challenge).

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.
