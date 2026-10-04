---
typ: solar-pakiet
repo: "[[ros-sbt-interfaces]]"
build: ament_cmake
tags: [solar, pakiet]
zaktualizowano: 2026-10-04
---
# l1_interfaces

> [!skrot] W 30 sekund (co robi, wejście → wyjście)
> Interfejsy percepcji (L1, repo `l1od`): detekcje z kamery (`Detection`), klastry z lidaru (`Cluster`), detekcje po fuzji kamera+lidar (`FusedDetection`) i ich wektory, a do tego serwisy węzła `images_collector`. Główne topiki to `/l1/detection_*`, `/l1/obstacle_detection` i `/fused_detection`.

## Węzły
Brak.

## Launch
Brak.

## Konfiguracja (yaml)
Brak.

## Zależności
`std_msgs`, `std_srvs`, `sensor_msgs`, `geometry_msgs`. Maintainer w `package.xml`: julia.czekanska (adres w domenie `solarboat.agh.edu.com`).

## Interfejsy
**Stałe** (identyczne w `Detection.msg`, `FusedDetection.msg` i `l2_interfaces/TrackedDetection.msg`):
- `UNUSED_CLASS=0`, `UNUSED_TYPE=100`;
- klasy: `TALL_BUOY=1`, `SMALL_BUOY=2`, `OBSTACLE=3`, `DOCK=4`, `SHAPE=5`, `PANEL=6`, `LIGHT=7`, `NUMBER=8`;
- typy:
  - kolory 101–106 (RED, GREEN, BLUE, YELLOW, BLACK, WHITE);
  - łodzie 110–111 (TRIANGLE_BOAT, CROSS_BOAT);
  - FRONT/SIDE/EMPTY 120–122;
  - kształty 130–139 (CROSS, BLUE/GREEN/RED × CIRCLE/SQUARE/TRIANGLE);
  - STOP/GO 140–141, CASTLE/TAXI 150–151;
  - kierunki świata 152–155 (NORTH, EAST, SOUTH, WEST);
  - światła 156–157, cyfry 158–160;
  - DYNAMIC_BOAT 161, światła doku 162–163;
  - OBSTACLE 401.

| plik | pola |
|---|---|
| `msg/Detection.msg` | `int32 detection_class`, `int32 detection_type`, `string detection_string`, `float64 confidence`, `float64 bbox_x0`, `bbox_x1`, `bbox_y0`, `bbox_y1`, `float64 heading_diff`, `float64 distance`, `float64 area`, `float64 x`, `y`, `z` |
| `msg/VectorOfDetections.msg` | `std_msgs/Header header`, `l1_interfaces/Detection[] detections` |
| `msg/Cluster.msg` | `geometry_msgs/Point center`, `geometry_msgs/Point min`, `geometry_msgs/Point max` |
| `msg/VectorOfClusters.msg` | `std_msgs/Header header`, `l1_interfaces/Cluster[] clusters` |
| `msg/FusedDetection.msg` | `std_msgs/Header header`, `int32 id`, `bool is_fused_angle`, `bool is_fused`, `int32 detection_class`, `int32 detection_type`, `string detection_string`, `float64 confidence`, `geometry_msgs/Point position`, `geometry_msgs/Point[] yolo_bbox_corners` (dwa narożniki x,y), `geometry_msgs/Point[] cluster_bbox_corners` (dwa narożniki x,y,z), `geometry_msgs/Point[] spatial_points` |
| `msg/VectorOfFusedDetections.msg` | `std_msgs/Header header`, `l1_interfaces/FusedDetection[] fused_detection` |
| `msg/PlotPackage.msg` | `std_msgs/Header header`, `string model`, `string[] det_str`, `float32 duration`, `float32[] area`, `confidences`, `x_centres`, `y_centres`, `depths` |
| `srv/RequestCMD.srv` | `int32 requested_cmd` → `bool executed_cmd` |
| `srv/RequestString.srv` | `int32 requested_cmd`, `string requested_string` → `bool executed_cmd` |
| `srv/SaveImagesParams.srv` | `int32 requested_cmd`, `string save_path`, `bool save_all_img`, `bool resize_for_yolo`, `int32 frequency`, `int32 buffer` → `bool executed_cmd` |
| `srv/MakeVideoParams.srv` | `string input_directory`, `string film_filepath`, `int32 framerate`, `bool delete_images` → `bool executed_cmd` |
| `srv/MakeDetectionsParams.srv` | `string input_directory`, `string output_directory`, `bool delete_images` → `bool executed_cmd` |

Użycia: [[Interfejsy (msg, srv, action)#l1_interfaces]]. Konwencje osi pól `x/y/position` (FRU/RFU, różne między producentami) opisuje [[Drzewo TF i układy współrzędnych]].

## Różnice między branchami
Na `main` i w kopii `bash_do_uruchomienia` identyczne (diff `c7322b5..f1de70b` nie dotyka `l1_interfaces`).

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.
