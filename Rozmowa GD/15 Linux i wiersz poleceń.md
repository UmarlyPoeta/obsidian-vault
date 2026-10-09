---
typ: pojęcie
status: nowe
przedmioty: ["[[PK Komputerowe systemy operacyjne]]"]
wymaga: []
powiązane: ["[[16 Sieci — IP, podsieci, łączenie urządzeń]]", "[[17 Docker dla robotyki]]", "[[19 Troubleshooting — jak diagnozować]]", "[[Proces i wątek]]"]
tags: [pojęcie, rozmowa-gd, linux]
utworzono: 2026-10-09
źródła: []
---
# 15 Linux i wiersz poleceń

> [!skrot] W 30 sekund
> - **Wszystko jest plikiem**, nawet urządzenia: kamera `/dev/video0`, port szeregowy `/dev/ttyUSB0` / `/dev/ttyACM0`, dysk `/dev/sda`.
> - **Uprawnienia:** `rwx` dla właściciela / grupy / reszty (`chmod 755`). Dostęp do portów szeregowych daje grupa **`dialout`** (`sudo usermod -aG dialout $USER` + **wyloguj się**).
> - **Diagnostyka sprzętu:** `lsusb`, `dmesg -w`, `ls /dev/tty* /dev/video*`, `udevadm info`. Procesy: `htop`, `ps aux | grep`, `kill`. Usługi: `systemctl status`, `journalctl -u nazwa -f`. Dysk: `df -h`, `du -sh`.
> - **Zdalnie:** `ssh user@ip`, `scp`, `rsync -avP`, sesje, które przeżyją rozłączenie: **`tmux`**.
> - **Pipe `|`** łączy komendy, `>` nadpisuje plik, `>>` dopisuje, `2>&1` łączy błędy z wyjściem.

> [!intuicja] Po ludzku
> Terminal to rozmowa z komputerem krótkimi zdaniami: „pokaż” (`ls`), „idź” (`cd`), „szukaj” (`grep`, `find`), „kto tu pracuje” (`ps`, `htop`), „co się stało” (`dmesg`, `journalctl`). Na rozmowie nie musisz znać wszystkich flag na pamięć — musisz wiedzieć, **jakiego narzędzia użyć do jakiego problemu**.

## Struktura katalogów
| Katalog | Co tam jest |
|---|---|
| `/` | korzeń |
| `/home/user` = `~` | twoje pliki |
| `/etc` | konfiguracja systemu (`/etc/netplan`, `/etc/udev/rules.d`, `/etc/hosts`) |
| `/dev` | urządzenia |
| `/var/log` | logi |
| `/opt` | dodatkowe programy (`/opt/ros/humble`) |
| `/tmp` | pliki tymczasowe (czyszczone przy restarcie) |
| `/usr/bin`, `/usr/lib` | programy i biblioteki |
| `/proc`, `/sys` | wirtualne pliki z informacjami o jądrze i sprzęcie |
| `/mnt`, `/media` | zamontowane dyski, pendrive'y |

## Ściąga komend — pogrupowana

### Pliki i nawigacja
```bash
pwd                    # gdzie jestem
ls -la                 # lista z ukrytymi i szczegółami (uprawnienia, rozmiar)
cd ~/ws ; cd .. ; cd - # idź do / w górę / wróć
mkdir -p a/b/c         # utwórz z podkatalogami
cp -r src dst ; mv a b ; rm plik ; rm -r katalog   # ⚠️ rm nie ma kosza!
cat plik ; less plik ; head -n 20 plik ; tail -f log.txt   # podgląd; tail -f = na żywo
find . -name "*.mcap" -mtime -1        # pliki .mcap zmienione w ostatniej dobie
grep -rn "serial_no" config/           # szukaj tekstu rekurencyjnie z numerami linii
wc -l plik ; sort ; uniq -c            # liczenie, sortowanie, zliczanie powtórzeń
tar czf backup.tar.gz katalog/ ; tar xzf backup.tar.gz     # spakuj / rozpakuj
ln -s /cel nazwa                       # dowiązanie symboliczne
```

### Uprawnienia i użytkownicy
```bash
ls -l /dev/ttyUSB0          # crw-rw---- 1 root dialout ... → potrzebna grupa dialout
groups                       # w jakich grupach jestem
sudo usermod -aG dialout,video,plugdev $USER   # dopisz do grup (wyloguj/zaloguj!)
chmod +x skrypt.sh          # nadaj prawo wykonania
chmod 644 plik ; chmod 755 katalog
sudo chown -R user:user katalog
```
> [!definicja] Uprawnienia `rwx` w liczbach
> r = 4, w = 2, x = 1. Trzy cyfry: **właściciel, grupa, inni**. `755` = rwx r-x r-x (programy, katalogi), `644` = rw- r-- r-- (zwykłe pliki), `600` = tylko właściciel (klucze SSH!).

### Procesy
```bash
htop                        # interaktywnie: CPU, RAM, procesy (F9 kill, F6 sort)
ps aux | grep ros2          # znajdź procesy
kill PID ; kill -9 PID      # grzecznie (SIGTERM) / siłowo (SIGKILL — ostateczność)
pkill -f camera_node        # zabij po nazwie
Ctrl+C                      # przerwij (SIGINT)   |  Ctrl+Z → zatrzymaj; bg / fg
komenda &                   # w tle  |  nohup komenda &  → przeżyje zamknięcie terminala
nvidia-smi                  # GPU: użycie, pamięć, procesy
free -h ; uptime            # RAM ; obciążenie
```

### Sprzęt i urządzenia
```bash
lsusb ; lsusb -t            # urządzenia USB / drzewo i prędkości
dmesg -w                    # komunikaty jądra na żywo (podłącz urządzenie i patrz)
sudo dmesg | tail -30       # ostatnie komunikaty
ls /dev/ttyUSB* /dev/ttyACM* /dev/video*    # porty szeregowe, kamery
ls -l /dev/serial/by-id/    # STAŁE nazwy portów szeregowych (po numerze seryjnym!)
udevadm info -a -n /dev/ttyUSB0 | grep -E "idVendor|idProduct|serial"   # atrybuty do reguły udev
lsblk ; df -h ; du -sh *    # dyski, wolne miejsce, rozmiary katalogów
lscpu ; lspci               # procesor ; karty PCI (GPU, sieć)
sensors                     # temperatury (lm-sensors)
```

### Usługi i logi (systemd)
```bash
systemctl status nazwa              # czy działa, ostatnie logi
sudo systemctl restart nazwa        # restart
sudo systemctl enable --now nazwa   # włącz przy starcie i uruchom
journalctl -u nazwa -f              # logi usługi na żywo
journalctl -b -p err                # błędy od ostatniego startu
```

### Pakiety i środowisko
```bash
sudo apt update && sudo apt install ros-humble-realsense2-camera
apt list --installed | grep ros
python3 -m venv .venv && source .venv/bin/activate && pip install -r requirements.txt
echo $PATH ; printenv | grep ROS ; export ROS_DOMAIN_ID=7
echo 'source /opt/ros/humble/setup.bash' >> ~/.bashrc   # na stałe dla nowych terminali
which python3 ; type ros2
```

### Przekierowania i potoki
```bash
ros2 topic list | grep camera            # | — wyjście jednej komendy na wejście drugiej
ros2 bag info ep > info.txt              # > — zapisz (nadpisz)
echo "notatka" >> log.txt                # >> — dopisz
komenda 2>&1 | tee wynik.log             # błędy + wyjście na ekran i do pliku
komenda1 && komenda2                     # druga tylko gdy pierwsza się udała
komenda1 || echo "nie wyszło"            # gdy pierwsza się nie udała
```

### Zdalna praca
```bash
ssh user@192.168.1.50                       # logowanie
ssh-keygen -t ed25519 ; ssh-copy-id user@ip # klucz zamiast hasła
scp plik user@ip:/sciezka/ ; scp -r katalog user@ip:~
rsync -avP --checksum dane/ user@ip:/data/  # synchronizacja z postępem i wznawianiem
ssh -X user@ip                              # przekazanie okien graficznych (rviz zdalnie, wolne)
tmux new -s zbieranie ; Ctrl+b d (odłącz) ; tmux attach -t zbieranie
```
> [!wazne] Dlaczego tmux?
> Uruchamiasz nagrywanie przez SSH, Wi-Fi się rwie → bez tmuxa proces **ginie** razem z sesją. W tmuxie dalej działa, a ty się podłączasz z powrotem (`tmux attach`).

## Reguła udev — stała nazwa urządzenia (często się przydaje!)
Problem: po restarcie dwa adaptery szeregowe zamieniają się `ttyUSB0` ↔ `ttyUSB1`. Rozwiązanie — plik `/etc/udev/rules.d/99-robot.rules`:
```
SUBSYSTEM=="tty", ATTRS{idVendor}=="0403", ATTRS{idProduct}=="6014", ATTRS{serial}=="FT6S4DSP", SYMLINK+="leader_arm", MODE="0666"
SUBSYSTEM=="video4linux", ATTRS{serial}=="ABC123", ATTR{index}=="0", SYMLINK+="cam_wrist"
```
Potem: `sudo udevadm control --reload-rules && sudo udevadm trigger` → masz `/dev/leader_arm` i `/dev/cam_wrist`. (Prostsza alternatywa dla portów szeregowych: `/dev/serial/by-id/...`.)

## Prosty skrypt bash (np. przed sesją)
```bash
#!/usr/bin/env bash
set -euo pipefail                         # przerwij przy błędzie / nieustawionej zmiennej
echo "== Kontrola przed sesją $(date -Iseconds) =="
df -h /data | tail -1                     # miejsce na dysku
for dev in /dev/cam_wrist /dev/cam_scene /dev/leader_arm; do
  [[ -e $dev ]] && echo "OK  $dev" || echo "BRAK $dev"
done
ping -c1 -W1 192.168.1.102 >/dev/null && echo "OK  robot pinguje" || echo "BRAK robota w sieci"
```

## Typowe błędy
> [!blad] `Permission denied` na `/dev/ttyUSB0`
> Brak grupy `dialout`. `sudo usermod -aG dialout $USER` i **wyloguj się/zrestartuj** (grupy ładują się przy logowaniu). `sudo chmod 666` to doraźna łatka, znika po odłączeniu.

> [!blad] `sudo` do wszystkiego
> Uruchamianie ROS-a/pip przez sudo tworzy pliki roota w twoim katalogu → później błędy uprawnień. Sudo tylko gdy trzeba.

> [!blad] `rm -rf` bez sprawdzenia ścieżki
> Szczególnie na danych. Najpierw `ls` tej samej ścieżki.

## Pytania rekrutacyjne
> [!zadanie] Podłączyłeś urządzenie USB, a program go nie widzi. Co robisz?
> > [!odpowiedz]- Odpowiedź
> > `lsusb` (czy system je widzi), `dmesg -w` podczas podłączania (czy jądro rozpoznało, jaki sterownik, jakie `/dev/...`, błędy typu „device descriptor read error” = kabel/zasilanie), `ls /dev/ttyUSB* /dev/video*`, uprawnienia (`ls -l`, grupa `dialout`/`video`), czy inny proces nie trzyma urządzenia (`lsof /dev/ttyUSB0`), inny port/kabel, czy program wskazuje dobrą ścieżkę.
> > **EN:** *"I'd check lsusb to see if the OS detects it, watch dmesg while plugging it in to see the driver and device node or errors, check the /dev entry and its permissions — e.g. dialout group for serial — make sure no other process holds it, try another cable or port, and verify the program points to the right device path."*

> [!zadanie] Jak sprawdzisz, co obciąża komputer i zabijesz zawieszony proces?
> > [!odpowiedz]- Odpowiedź
> > `htop` (sortowanie po CPU/RAM), `nvidia-smi` dla GPU, `df -h` dla dysku; proces: `ps aux | grep nazwa` → `kill PID` (SIGTERM), a gdy nie reaguje `kill -9 PID`. Przy ROS — `pkill -f nazwa_węzła`.

> [!zadanie] Jak uruchomić długie nagrywanie na zdalnej maszynie, żeby nie przerwało się po rozłączeniu?
> > [!odpowiedz]- Odpowiedź
> > W `tmux` (albo `screen`) — po rozłączeniu sesja żyje, wracam `tmux attach`. Alternatywnie `nohup ... &` albo usługa systemd.

> [!zadanie] Co oznacza `chmod 755`?
> > [!odpowiedz]- Odpowiedź
> > Właściciel rwx (7), grupa r-x (5), inni r-x (5) — typowe dla skryptów i katalogów.

> [!zadanie] Jak zobaczyć logi usługi systemowej?
> > [!odpowiedz]- Odpowiedź
> > `systemctl status nazwa` (stan + ostatnie linie) i `journalctl -u nazwa -f` (na żywo), `journalctl -b -p err` — błędy od startu.

## Fiszki #fiszki/rozmowa-gd/linux
Grupa dająca dostęp do portów szeregowych?::dialout (sudo usermod -aG dialout $USER, potem ponowne logowanie).
Komenda: komunikaty jądra na żywo?::dmesg -w (lub sudo dmesg | tail).
Komenda: urządzenia USB i ich prędkość?::lsusb ; lsusb -t
Gdzie są stałe nazwy portów szeregowych?::/dev/serial/by-id/
Po co reguły udev?::Stałe nazwy (SYMLINK) i uprawnienia dla urządzeń na podstawie VID/PID/numeru seryjnego.
Komenda: przeładowanie reguł udev?::sudo udevadm control --reload-rules && sudo udevadm trigger
chmod 755 / 644 / 600?::rwxr-xr-x / rw-r--r-- / rw------- (tylko właściciel).
r, w, x w liczbach?::r=4, w=2, x=1.
kill vs kill -9?::SIGTERM (grzecznie, proces może posprzątać) vs SIGKILL (natychmiast, ostateczność).
Komenda: logi usługi systemd na żywo?::journalctl -u nazwa -f
Komenda: włącz usługę przy starcie i uruchom?::sudo systemctl enable --now nazwa
Komenda: wolne miejsce na dysku / rozmiar katalogu?::df -h / du -sh katalog
Po co tmux?::Sesje terminala, które przeżywają rozłączenie SSH (tmux attach).
Komenda: synchronizacja plików z postępem i wznawianiem?::rsync -avP źródło/ user@host:cel/
Różnica > i >>?::> nadpisuje plik, >> dopisuje na końcu.
Co robi 2>&1?::Przekierowuje strumień błędów (stderr) do standardowego wyjścia (stdout).
Komenda: kto trzyma urządzenie/plik?::lsof /dev/ttyUSB0 (lub fuser).
Gdzie trzymać konfigurację środowiska dla nowych terminali?::~/.bashrc (np. source /opt/ros/humble/setup.bash).

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.
