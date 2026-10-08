---
typ: solar-slam
tags: [solar, slam, l2]
zaktualizowano: 2026-10-08
---
# SLAM — deskewing

> [!skrot] W 30 sekund
> - Lidar obraca się ~0,1 s, łódź w tym czasie płynie i się obraca. `Deskewer::deskew` (`pose_graph/src/deskewer.cpp:178-380`) przenosi każdy punkt do **układu lidaru w chwili początku skanu** $t_0$.
> - Czas punktu = pole `HesaiPoint::timestamp` (double, sekundy, `hesai_point.hpp:15`) przesunięte tak, by **max** czasu punktów pokrył się ze stemplem nagłówka (`deskew.header_stamp_reference: end`).
> - Poza w chwili punktu = **interpolacja liniowa pozycji + slerp kwaternionu** między dwiema sąsiednimi próbkami `/odom` (bufor 5 s); bez IMU, bez prędkości kątowych, bez ekstrapolacji.
> - Wzór: $p_{l0} = (T_{odom\_imu}(t_0) T_{imu\_lidar})^{-1}\, T_{odom\_imu}(t_i) T_{imu\_lidar}\, p_{li}$.
> - Jeśli `/odom` nie pokrywa całego skanu (najnowsza próbka starsza niż koniec skanu) → skan **odrzucany** (`SCAN_NOT_COVERED`), bez czekania.

Kontekst: [[SLAM — architektura węzła i przepływ danych]]. Konwencje osi: [[Drzewo TF i układy współrzędnych]].

## Bufor ruchu

- Źródło: `odomCallback` (`feature_extractor.cpp:1574-1604`), `MotionState{timestamp = header.stamp [s], position = (pos.y, pos.x, pos.z), orientation = q z /odom}` (`deskewer.hpp:33-40`). Interpretacja: $T_{odom\_imu}(t)$ — ale „odom” tutaj to wewnętrzny układ (x = wschód, y = północ, z = góra), a kwaternion jest przepisany bez zmian (patrz niżej, „niepewne”).
- `addMotionState` (`deskewer.cpp:61-108`): odrzuca NaN i $\lVert q\rVert^2 < 10^{-12}$, normalizuje $q$, wstawia **posortowane** (`lower_bound` po czasie), duplikat ($|\Delta t| < 10^{-9}$ s) nadpisuje, usuwa z przodu próbki starsze niż `newest − motion_buffer_length` (5,0 s; minimum wymuszone 0,5 s w konstruktorze `:54-55`).
- Przy ~400 Hz bufor ma ~2000 próbek; `deskew` kopiuje go w całości pod mutexem (`:245-250`) — O(N) na skan, tanie, ale blokuje na chwilę `odomCallback`.

## Czas punktu i okno skanu

`deskew(cloud, message_timestamp)` (`:178-263`):

$$t_{min} = \min_i t_i,\quad t_{max} = \max_i t_i \quad(\text{tylko skończone } t_i)$$

$$\delta = \begin{cases} t_{hdr} - t_{min} & \text{reference = start} \\ t_{hdr} - t_{max} & \text{reference = end (yaml)} \end{cases}$$

$$t_0 = t_{min} + \delta\ (\texttt{scan\_start}),\quad t_1 = t_{max} + \delta\ (\texttt{scan\_end}),\quad \texttt{reference\_timestamp} = t_0$$

- `scan_duration` $= t_{max} - t_{min}$; gdy > `maximum_scan_duration` (0,50 s) → `SCAN_DURATION_OUT_OF_RANGE` (`:215-223`). Chroni przed nanosekundami/milisekundami pomylonymi z sekundami tylko częściowo: czasy w ns dałyby duration ≫ 0,5 → odrzucenie (dobrze), w ms — też.
- `δ` (`timestamp_offset`) pozwala, by czasy punktów były względne (0…0,1 s) albo w innym zegarze — cały skan jest „przyklejany” do nagłówka. Konsekwencja: **dokładność deskew zależy od tego, czy nagłówek drivera Hesai to naprawdę koniec skanu** (niepewne: kontrakt `hesai_ros_driver` nie jest w repo, [[Sensory i sprzęt]]). Błąd tej kotwicy = stałe przesunięcie czasu lidar↔odom.
- Pokrycie: `local_buffer.front().timestamp ≤ t_0` i `t_1 ≤ local_buffer.back().timestamp`, inaczej `SCAN_NOT_COVERED` (`:258-263`). Przy `end` oznacza to, że **musi już przyjść próbka `/odom` o stemplu ≥ nagłówek lidaru**. Lidar przychodzi z opóźnieniem drivera, więc zwykle OK; ale jeśli `/odom` ma opóźnienie > opóźnienie lidaru, skan jest gubiony (QoS `KeepLast(1)`, brak ponownej próby).

## Interpolacja pozy

`interpolatePoseFromBuffer` (`:110-176`): `upper = lower_bound(t)`, `lower = upper − 1`,

$$\alpha = \mathrm{clamp}\left(\frac{t - t_{lower}}{t_{upper} - t_{lower}}, 0, 1\right)$$

$$p(t) = (1-\alpha)\,p_{lower} + \alpha\,p_{upper},\qquad q(t) = \mathrm{slerp}(q_{lower}, \pm q_{upper}, \alpha)$$

(znak $q_{upper}$ odwracany, gdy $q_{lower}\cdot q_{upper} < 0$, `:169-170`). Odstęp < 1 ns → porażka. Czas poza buforem → porażka (bez ekstrapolacji). Koszt: O(log N) na punkt.

> [!intuicja] Co to oznacza fizycznie
> Między próbkami `/odom` (co ~2,5 ms) ruch jest odcinkiem prostej i jednostajnym obrotem. Przy 400 Hz to praktycznie dokładne. Ale `/odom` z [[odometry]] to filtr Kalmana z korekcją GNSS — skok pozycji przy korekcji GNSS w trakcie skanu „zagnie” chmurę (część punktów przed skokiem, część po nim).

## Transformacja punktu

`:297-372`:

$$T_{odom\_lidar}(t) = T_{odom\_imu}(t)\cdot T_{imu\_lidar}$$

$$p_{l0} = T_{odom\_lidar}(t_0)^{-1}\cdot T_{odom\_lidar}(t_i)\cdot p_{li}$$

- $T_{imu\_lidar}$ z parametrów `extrinsic.imu_lidar.{x,y,z,qw,qx,qy,qz}` (`feature_extractor.cpp:165-204`; yaml: t = (0,40; 0,05; −0,40), q = jednostkowy). Ta sama macierz w $t_0$ i $t_i$.
- Pominięte (`skipped_points`, `:316-326`): NaN w xyz, NaN w czasie, punkt dokładnie (0,0,0), czas poza buforem. **Uwaga:** pominięte punkty ze skończonym xyz zostają w chmurze **nieskorygowane** (nie są usuwane) — patrz „Podejrzane”.
- Wynik zwraca `T_odom_imu = T_odom_imu(t_0)` i `T_odom_lidar = T_odom_lidar(t_0)` (`:311-312`) — to są **predykcje** dla matchera i pozy łodzi. `translation_m`, `rotation_deg` = ruch między $t_0$ a $t_1$ (tylko log, `:285-295`).
- `success = deskewed_points > 0` (`:374`).

Poprawność algebry sprawdza `pose_graph/math_checks.py` (test „deskew identity”, wg `REVIEW_AND_TEST_PLAN.md` §„Deskew audit”).

> [!wazne] Niepewne: semantyka orientacji (wniosek z połączenia kodu i notatki o TF)
> Pozycja jest zamieniana na (wschód, północ, góra), ale kwaternion Xsensa (względem NWU wg [[Drzewo TF i układy współrzędnych]]) jest użyty tak, jakby był wyrażony w tym samym układzie. Dla samego kursu: oś $R(q)\,\hat x$ w układzie (E, N) wskazuje fizycznie **w prawo** od dziobu, nie do przodu (stąd marker łodzi ma dodatkowe $+90^\circ$, `feature_extractor.cpp:1954-1961`). Hesai jest zamontowany w RFU (x w prawo, y do przodu), więc przy jednostkowej rotacji `extrinsic` te dwa przesunięcia o 90° się znoszą i chmura trafia do mapy z poprawnym kursem — to jest „the coordinate setup that worked” (`slam.yaml:106-107`). Skutek uboczny: wektor translacji `extrinsic` jest w osiach tej „obróconej” ramy (x ≈ w prawo, y ≈ do przodu), a nie w osiach FRU kadłuba; roll/pitch też mogą być zamienione miejscami. Wymaga testu z celem (Test 2 w `REVIEW_AND_TEST_PLAN.md`).

## Wydajność

Na punkt: `lower_bound` + slerp + `toRotationMatrix` + 2 złożenia izometrii (`:334-365`). Dla ~kilkudziesięciu tys. punktów (niepewne: model Hesai nieustalony) to pojedyncze ms. Jednowątkowo, w `lidar_group_`.

## Podejrzane / do naprawy

1. **Punkty bez skończonego czasu zostają nieskorygowane** (`deskewer.cpp:316-326` tylko `continue`), a potem przechodzą do cech i mapy. Propozycja: ustawić xyz na NaN (zostaną usunięte w `removeInvalidPoints`, `feature_extractor.cpp:1642-1668`) albo usunąć w miejscu.
2. **Brak czekania na odometrię**: `SCAN_NOT_COVERED` od razu odrzuca skan. Propozycja: krótka kolejka skanów (np. 50 ms) albo ekstrapolacja do `t_1` z ostatniej prędkości.
3. **Kotwica czasu zależna od drivera**: `header_stamp_reference: end` przyjęte bez potwierdzenia kontraktu drivera (komentarz `baseline_params.yaml:40-41`). Zmierzyć: przy szybkim obrocie w miejscu prosta ściana powinna być prosta w `/slam/debug/deskewed_scan` (`debug.publish_clouds: true`).
4. **Skoki GNSS w `/odom`** przenoszą się na deskew i predykcję; rozważyć deskew z czystej integracji IMU (prędkość kątowa + prędkość Xsensa) zamiast pozycji z KF.
5. Kopia całego bufora (~2000 próbek) na skan — wystarczy kopia wycinka $[t_0 - \epsilon, t_1 + \epsilon]$.

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.
