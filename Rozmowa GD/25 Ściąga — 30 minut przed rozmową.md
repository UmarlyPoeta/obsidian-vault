---
typ: ściąga
tags: [rozmowa-gd, sciaga]
utworzono: 2026-10-09
---
# ⚡ Ściąga — 30 minut przed rozmową

> [!wazne] Nastawienie
> Jesteś kandydatem na **stażystę**. Nikt nie oczekuje eksperta. Oczekują: **rzetelności, bezpieczeństwa, ciekawości, uczciwego „nie wiem, ale sprawdziłbym tak…”** i tego, że **umiesz opowiedzieć o swoich projektach**. Mów wolno, konkretnie, „ja zrobiłem”.

## 1. Twoje 60 sekund (EN) — powiedz na głos raz
AGH Computer Science (5th sem.) + Automation & Robotics at PK → **Solar Boat**: autonomous boat, ROS 2 Humble, lidar + ZED + INS + CAN, Jetson, my part: SLAM/localization layer, docking in costmap, timestamp matching, tooling; Njord 2026 Trondheim → **SNN_Agent**: glass-break detection with spiking NN, Arduino → RPi 5 → cloud, my part: streaming decoder + telemetry → *"I love where software meets hardware; robot learning is the most exciting direction."* Pełna wersja: [[22 Moje projekty — jak o nich opowiadać]].

## 2. Rola w jednym zdaniu
Zbierasz **wysokiej jakości dane z teleoperacji** do trenowania **VLA / imitation learning**, resetujesz sceny, robisz **QC i upload**, utrzymujesz ramiona, chwytaki, kamery, **diagnozujesz i eskalujesz**, prowadzisz **inwentarz i log**.

## 3. Definicje, które muszą wylecieć z ust
| Pojęcie | Jedno zdanie |
|---|---|
| **VLA** | Model wizja-język dotrenowany tak, by z obrazów + polecenia + stanu robota generować akcje (RT-2, OpenVLA, π0) |
| **Behavior cloning** | Uczenie nadzorowane na parach (obserwacja, akcja) z demonstracji; problem: **distribution shift** |
| **Action chunking** | Przewidywanie kawałka przyszłych akcji naraz (ACT, π0) — gładziej, mniej błędów |
| **Leader–follower** | Ramię-replika, robot powtarza kąty stawów 1:1 |
| **Topic / service / action** | Strumień pub/sub / zapytanie–odpowiedź / długie zadanie z feedbackiem i anulowaniem |
| **TF** | Drzewo układów współrzędnych w czasie; przeliczanie póz między ramkami |
| **URDF** | XML: linki + jointy robota |
| **FK / IK** | Stawy → poza (1 wynik) / poza → stawy (0, kilka, ∞) |
| **Osobliwość** | Jakobian traci rząd; robot traci kierunek ruchu, prędkości stawów → ∞ |
| **Intrinsics / extrinsics** | $K$ + dystorsja / poza kamery względem świata |
| **Stereo** | $Z = fB/d$, błąd ∝ $Z^2$ |
| **Hand-eye** | $AX = XB$; eye-in-hand (kołnierz→kamera) / eye-to-hand (baza→kamera) |
| **MoveIt** | IK + planowanie bezkolizyjne (OMPL RRTConnect) + parametryzacja czasowa → FollowJointTrajectory |
| **ros2_control** | hardware interface + controller manager + kontrolery |
| **PID** | $K_p e + K_i\int e + K_d \dot e$ |

## 4. Komendy — wymień bez myślenia
`ros2 topic list / echo / hz / info -v` · `ros2 node list` · `ros2 bag record -o x /t` · `ros2 bag info` · `colcon build --symlink-install` · `source install/setup.bash` · `lsusb -t` · `dmesg -w` · `ls /dev/ttyUSB* /dev/video*` · `usermod -aG dialout` · `htop` · `df -h` · `journalctl -u x -f` · `ip a` · `ping` · `nmcli` · `nmap -sn 192.168.1.0/24` · `ssh` / `rsync -avP` / `tmux` · `docker run --network host --device ...`

## 5. Procedury (powiedz kroki)
- **Sesja zbierania:** protokół → e-stop i strefa → sprzęt i kamery (pozycja!) → dysk → log → [scena → home → REC → zadanie → STOP → etykieta → reset] → QC → upload + weryfikacja → log.
- **QC epizodu:** kompletność strumieni, klatki vs czas (dropped frames), sync timestampów, długość, obraz, płynność, chwytak, etykieta, metadane.
- **Troubleshooting:** bezpiecznie → objaw + *co się zmieniło* → zasilanie → kabel → system widzi → uprawnienia/config → driver → ROS (QoS, TF, domain) → izolacja → jedna zmiana → weryfikacja → log → eskalacja z konkretami.
- **Sieć do robota:** IP robota → statyczne IP PC w tej samej podsieci, bez bramy → `ip link` UP → ping → porty/firewall → driver.
- **Kalibracja kamery:** szachownica/ChArUco, 20–50 ujęć (rogi! nachylenia!), błąd reprojekcji < 0,5 px, zapis + log.
- **Bezpieczeństwo:** e-stop w zasięgu, pusta strefa, nie sięgam do aktywnego robota, nowe ruchy wolno, pinch points, LOTO, zgłaszam near-missy.

## 6. Liczby, które robią wrażenie
- ACT: ~**50** demonstracji na zadanie · Open X-Embodiment: **>1 mln** trajektorii, **22** roboty · π0: akcje do **50 Hz**
- UR5e: **5 kg**, **850 mm**, powtarzalność **±0,03 mm** · Franka: 7 DOF, **1 kHz** FCI
- Kamera: 30 FPS × 20 s ≈ **600** klatek · USB 3 = **5 Gb/s** · `/24` = **254** hosty
- CAN: **60 Ω** między H a L · napięcie przemysłowe **24 V** · T1: **250 mm/s**
- Humble LTS do **2027** (Ubuntu 22.04) · Jazzy do **2029** (Ubuntu 24.04)

## 7. Gdy nie wiesz
*"I haven't worked with that directly. My understanding is … — and in practice I'd check … first, and if I couldn't solve it I'd escalate to an engineer with my notes."*

## 8. Twoje pytania na koniec (wybierz 2)
- *What teleoperation setups and arms do you use?*
- *How is data quality measured — do you have QC tooling?*
- *What models do you train on the data?*
- *What does a successful intern look like after six months?*

## 9. Logistyka
- [ ] Grafik dostępności (AGH + PK) — **konkretne dni i godziny**
- [ ] Laptop naładowany, kamera/mikrofon przetestowane, cisza, woda
- [ ] Otwarta notatka [[22 Moje projekty — jak o nich opowiadać]] i diagram łodzi (możesz pokazać ekran!)
- [ ] Polecenie od Szymona Nowaka — wspomnij, jeśli zapytają, skąd wiesz o ofercie
