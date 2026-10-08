---
typ: solar-slam
tags: [solar, slam, l2]
zaktualizowano: 2026-10-08
---
# SLAM — TF map-odom-base

> [!skrot] W 30 sekund
> - **Na łodzi nikt nie publikuje TF `map→odom` ani `odom→base_link`.** Ani `odometry`, ani `pose_graph` nie używają `tf2` (grep po `pose_graph/src`, `pose_graph/include`, `odometry`, `planner`, `trajectory_planner`, `colision_detector`, `segment_scan` nic nie znalazł). To samo stwierdza [[Drzewo TF i układy współrzędnych]] dla całego stosu.
> - Rolę `map→odom` pełni zmienna `T_map_odom_` w `feature_extractor` (`feature_extractor.cpp:49`, `816-864`). Na zewnątrz wychodzi tylko jako **wiadomości**:
>   - `/slam/debug/map_to_odom` (`nav_msgs/Odometry`, poza dokładna, raz na skan);
>   - `/slam/odom_transform` (`TransformStamped` jako topik, a nie TF; efektywna korekta po wygładzeniu, z częstotliwością `/odom`);
>   - pośrednio `/slam/odom` = `map→base` po wygładzeniu.
> - `odom→base` to po prostu `/odom` z [[odometry]] (x = północ, y = wschód, kwaternion z Xsensa w NWU), bez TF.
> - Korekta `map→odom` jest więc niewidoczna dla każdego węzła, który czyta `/odom`. Węzły na `/slam/odom` widzą ją z rampą. Węzły trzymające punkty w `map` muszą je przeliczyć same (zob. [[L2 — konsumenci pozy i mapy (co się stanie po korekcie)]]).
> - Jeśli kiedyś włączymy TF, `map→odom` może już publikować ZED (`publish_map_tf: true`) i `sbt_tf_bridge` (identyczność). Trzeba je wyłączyć, zanim SLAM zacznie publikować własne.

Powiązane: [[Drzewo TF i układy współrzędnych]], [[sbt_transforms]], [[L2 — odometria, sensory i fuzja]], [[pose_graph]].

## Układy, które naprawdę istnieją w kodzie SLAM-u

| nazwa (w kodzie) | osie | kto ją definiuje | gdzie widać |
|---|---|---|---|
| „odom” (wire) | x = północ, y = wschód, z = 0; orientacja = kwaternion `/imu/data` (NWU) | `odometry/src/OdoCpp.cpp:203-207`, `229-236`, `270` | `/odom` |
| „odom” (wewnątrz SLAM) | x = wschód, y = północ (zamiana przy odbiorze), kwaternion **bez zmian** | `feature_extractor.cpp:1583-1594` | bufor deskewera, `T_odom_imu`, `T_odom_lidar` |
| `map` (wewnątrz SLAM) | x = wschód, y = północ, z do góry względem łodzi; początek = początek `odom`, bo `T_map_odom` startuje jako $I$ (`feature_extractor.cpp:49`) | scan matcher | `/slam/boat_pose`, `/slam/local_map_points`, `/slam/global_costmap`, `/global/clusters`, `/tracked_detections` |
| `map` (wire w `/slam/odom`) | x = północ, y = wschód: zamiana z powrotem dla zgodności z konsumentami `/odom` | `feature_extractor.cpp:1072-1085` | `/slam/odom`, `/slam/trajectory_extrapolation` |
| `slam_odom` ← `odom` | etykiety w `/slam/odom_transform`, w układzie wire | `feature_extractor.cpp:1406-1484` | L3 `GNSSToLocal` |
| `hesai_lidar` | ramka chmury (RFU wg `frames.yaml`) | sterownik Hesai | `/lidar_points` |
| IMU → LiDAR | `T_imu_lidar`: t = (0.40, 0.05, −0.40) m, q = identyczność | `slam.yaml:11-19` | deskewer (`deskewer.cpp:306`, `353`) |

> [!wazne] Ten sam kwaternion w dwóch układach osi
> W SLAM-ie pozycja jest zamieniona na (wschód, północ), a kwaternion przepisany bez zmian. Wewnętrzna poza `T_odom_imu` składa się więc z pozycji w jednej konwencji i orientacji w innej. Działa to dlatego, że zespół dobrał resztę (ekstrynsykę, `-atan2` w konsumentach) „pod to, co działało”: commity `2a0b6e0` „na naszych osiach…”, `2a06b84` „Osieet działa”, `2a4dbf6` „…NAPRAWILI SLAMA” oraz komentarz w `slam.yaml:106`. Szczegóły matematyki to front-end. Dla loop closure wynika z tego jedno: **grafu poz nie wolno budować na kwaternionach z `/odom` bez jawnej konwersji do jednego układu prawoskrętnego**. Inaczej obroty w czynnikach będą odbite.

## Kto publikuje `map→odom` i `odom→base` — porównanie z notatką o TF

| relacja | na łodzi (Njord, `0d444ff`) | `sbt_tf_bridge` (nieuruchamiany) | ZED (domyślnie) | `origin/lio_sam` |
|---|---|---|---|---|
| `map→odom` | **brak TF**; wartość tylko w `T_map_odom_`, a jako wiadomość w `/slam/debug/map_to_odom` (`feature_extractor.cpp:706-709`, `2500-2504`, `2635-2639`) | identyczność (`sbt_transforms/src/tf_bridge_node.cpp:71-252`) | `publish_map_tf: true` (`common_stereo.yaml:105`), własny VIO ZED | **statyczna identyczność** `static_transform_publisher 0 0 0 0 0 0 map odom` (`LIO-SAM-ros2/launch/run.launch.py:26-32`) |
| `odom→base` | **brak TF**; `/odom` (`OdoCpp.cpp:38`) | z `/odom`, NEU+NWU → ENU | `odom→zed_camera_link` (`publish_tf: true`, `common_stereo.yaml:104`) | `odom→base_link` z `imuPreintegration` (`imuPreintegration.cpp:142-148`), `odom→lidar_link` z `mapOptmization` (`mapOptmization.cpp:1680-1681`) |
| `base→lidar` | brak TF; tylko `T_imu_lidar` w parametrach | statyczne `base_link→hesai_lidar` (z = 0.78, yaw −90°) | — | `robot_state_publisher` z `config/robot.urdf.xacro` |

Rozbieżności względem [[Drzewo TF i układy współrzędnych]]:
- Notatka TF podaje dla lidaru wysokość z = 0.78 m nad `base_link` (bridge). SLAM ma `T_imu_lidar` z z = −0.40 (lidar 0.40 m *pod* IMU?). Pewnie to inna konwencja osi z (z w dół w ramce IMU albo odwrócona konwencja `T_a_b`), a nie fizyczna różnica. **Niepewne**, nie sprawdzano na łodzi. Zgodnie z ZASADAMI szczegóły ekstrynsyki należą do front-endu.
- LIO-SAM (porzucony) wkładał korekty pętli w `odom→lidar_link` (skacząca odometria, statyczne `map→odom`). To odwrotność konwencji REP-105, którą realizuje dzisiejszy `pose_graph` (gładkie `odom`, korekty w `map→odom`).

## Co oznacza korekta `map→odom` dla reszty systemu

> [!definicja] Konwencja REP-105 w wersji łodzi
> `T_map_boat = T_map_odom · T_odom_boat`. `T_odom_boat` (z `/odom`) jest ciągłe w krótkim czasie, choć skacze z GNSS-em, zob. [[L2 — odometria, sensory i fuzja]]. `T_map_odom` zmienia się skokowo przy każdej przyjętej korekcie scan matchera (dziś ≤ 0.35 m i ≤ 3° na skan, `slam.yaml:88-89`), a w przyszłości przy loop closure.

> [!wzor] Przeliczenie punktu po korekcie
> Punkt zapisany w `map` przed korektą: $p^{map}_{stary}$. Po zmianie $T_{map\_odom}: A \to A'$ ten sam punkt fizyczny (jeśli był „przyklejony” do odometrii) ma współrzędne
> $p^{map}_{nowy} = A'\,A^{-1}\,p^{map}_{stary}$.
> Punkty przyklejone do **mapy** (boje, ściany slipu wykryte LiDAR-em) przy samej korekcie `T_map_odom` się nie zmieniają. Przy loop closure, które przesuwa stare keyframe'y, trzeba je przeliczyć przez pozę ich keyframe'u: $p_{nowy} = T'_{map,kf}\,T^{-1}_{map,kf}\,p_{stary}$.
> Punkty **GNSS** (cele zadane współrzędnymi geograficznymi) są absolutne w `odom` (bo `odom` = metry od pierwszego fixa). Ich poprawne współrzędne w `map` to $A'\,p^{odom}$, czyli trzeba je przeliczać przy każdej korekcie. `GNSSToLocal` robi to raz (migawka `/slam/odom_transform`, `gnss_to_local.cpp:294-317`).

> [!przyklad] Trzy klasy punktów i co z nimi zrobić po korekcie
> | klasa | przykład | przelicz przez |
> |---|---|---|
> | związane z odometrią | waypoint z `MoveAlongHeading` (liczony z `/odom`) | $A'$ (z `odom`) |
> | związane z GNSS | boje z regulaminu podane jako lat/lon (`GNSSToLocal`) | $A'$ (z `odom`, bo `odom` = GNSS) |
> | związane z mapą | boja z `/tracked_detections`, wielokąt slipu, woksele mapy | nic (sama korekta `map→odom`) albo $T'_{map,kf}T^{-1}_{map,kf}$ (loop closure) |

## Rekomendacja na przyszłość (TF)
1. Gdy SLAM ma publikować TF, zachować obecny model: `odom→base_link` z węzła odometrii (po przejściu na ENU/FLU wg `sbt_transforms`), `map→odom` z SLAM-u w tempie skanów (10 Hz), z `T_map_odom` po korekcie. **Bez** rampy w TF; rampa zostaje tylko w `/slam/odom` dla starych konsumentów.
2. Przed tym wyłączyć w ZED `publish_tf` i `publish_map_tf` (argumenty `zed_camera.launch.py`) oraz nie uruchamiać `sbt_tf_bridge` z `map→odom`.
3. `/slam/odom_transform` zostawić jako zgodność dla L3. Dodać zdarzenie korekty (zob. [[L2 — konsumenci pozy i mapy (co się stanie po korekcie)]]).

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.
