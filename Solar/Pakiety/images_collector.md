---
typ: solar-pakiet
repo: "[[l1od]]"
build: ament_python
tags: [solar, pakiet]
zaktualizowano: 2026-10-04
---
# images_collector

> [!skrot] W 30 sekund (co robi, wejście → wyjście)
> Narzędzie do zbierania zdjęć z kamery ZED pod datasety YOLO i do składania filmów. Sterowane serwisami: `/srv/save_images`, `/srv/make_detections`, `/srv/make_video`. Wejście: `/zed/zed_node/rgb/color/rect/image` → pliki PNG/MP4 na dysku.

## Węzły

### images_collector
`images_collector/images_collector/images_collector.py`, klasa `ImagesCollector`, executable `ImagesCollector` (`images_collector_main:main`).
- **Subskrybuje:** `/zed/zed_node/rgb/color/rect/image` (depth 1, bufor ostatniej klatki); w trybie „save all” dodatkowa subskrypcja tego samego topiku z RELIABLE i `depth = buffer`.
- **Serwisy (server):**
  - `/srv/save_images` — `l1_interfaces/srv/SaveImagesParams` (`requested_cmd`, `save_path`, `save_all_img`, `resize_for_yolo`, `frequency`, `buffer` → `executed_cmd`). `requested_cmd=1` start, `0` stop. `save_all_img=1` → wszystkie klatki w RAM, zapis przy stopie (opcjonalnie skalowane do 640×640, w nazwie pliku obliczone fps); inaczej timer co `frequency` s.
  - `/srv/make_detections` — `MakeDetectionsParams`. `preprocess()` używa `self.convNeuralNet` i `self.model_input_shape`, których nie inicjalizuje (`:162-170`) → serwis rzuci wyjątek.
  - `/srv/make_video` — `MakeVideoParams` (`input_directory`, `film_filepath`, `framerate`, `delete_images`); fourcc `mp4v`; framerate 0 → odczyt z nazwy pliku.
- Ścieżki: `resolve_save_path` — pusty → `dev_ws/saved_images_<timestamp>/` (README).
- README: na Jetsonie uruchamiać `/home/solar/collect_images.sh` (venv z NumPy 1.24.4).

## Launch
Brak; `ros2 run images_collector ImagesCollector`.

## Konfiguracja (yaml)
Brak.

## Zależności
`rclpy`, `std_msgs`, `sensor_msgs`, `cv_bridge`, `l1_interfaces` ([[ros-sbt-interfaces]]).

## Interfejsy
Brak własnych (serwisy z `l1_interfaces/srv`).

## Różnice między branchami
`dbscan_clustering`/`zed_exposure_controller`: 1 linia w `images_collector.py`.

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.
