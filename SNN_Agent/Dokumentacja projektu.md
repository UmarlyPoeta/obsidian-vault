---
typ: snn
tags: [snn, dokumentacja, plan, adr]
zaktualizowano: 2026-10-05
---
# Dokumentacja projektu

> [!skrot] W 30 sekund
> - **Źródło prawdy:** `docs/project/CURRENT_ASSUMPTIONS.md` (stan 24.09.2026). Rozstrzyga starsze opisy (`docs/project/README.md:5`). README w roocie jest w dużej części **historycznym szkicem**.
> - **Pytanie badawcze:** jak liczba symulowanych neuronów Lu.i wpływa na jakość ciągłej detekcji stłuczenia szkła, opóźnienie i koszt, oraz jak SNN wypada wobec detektora **FFT na Arduino Uno**. Przewagi SNN nie zakładamy z góry (`CURRENT_ASSUMPTIONS.md:5`).
> - **Demonstrator E2E:** MAX4466 → Uno → USB → Raspberry Pi 5 → symulacja SNN w Azure → zdjęcie i analiza w Azure AI Foundry → LED/buzzer + e-mail; dashboard po angielsku (`CURRENT_ASSUMPTIONS.md:11`).
> - **Zespół:** Wiktor, Patryk, Karolina, Marcel, Kacper, Andrzej. Etapy W/P/C/M/K/A opisuje [[Branche — szczegóły#Skróty etapów]].

## README — co jest historyczne
`README.md:1-3` linkuje aktualne dokumenty i zaznacza: „Opis warstw poniżej jest historycznym szkicem koncepcji. Obowiązująca implementacja POC używa Arduino Uno, Raspberry Pi 5 i symulacji Lu.i w Azure”.

Historyczne są:
- katalogi `hardware/`, `neuromorphic_core/`, `firmware/` (`README.md:11-30`), których w repo nie ma;
- frameworki Rockpool/Sinabs/Nengo;
- MCU STM32/ESP32 i FPGA. ESP32 jest wprost wykluczony (`Architektura_SNN_Agent.md:34`);
- host Jetson Orin z LLM/VLM (Llama 3) i RAG.

Z opisu README faktycznie istnieje tylko `software/` (host_agent/rag, web_dashboard, infra) oraz `docs/`, patrz [[software]].

## Cel badawczy i hipotezy
- **Minimalny wynik** (`CURRENT_ASSUMPTIONS.md:7`): wersjonowany zbiór ciągły i generator, zamrożony champion SNN, działający baseline FFT na Uno, wspólny test i protokół zdarzeniowy, raport jakości, opóźnień i jawnie oznaczonych pomiarów/estymacji kosztu. Przy kilku liczbach neuronów pokazujemy **krzywą kompromisu**, a nie „optymalną” liczbę.
- **Hipotezy wymagające osobnych danych** (`CURRENT_ASSUMPTIONS.md:5`): większa odporność na zakłócenia, lepszy recall, mniejsze FA/h, oszczędność energii, zgodność symulatora z elektroniką.
- **Pytanie w wersji energetycznej** (`Architektura_SNN_Agent.md:493-496`): przy jakiej jakości detekcji, liczbie aktywacji i koszcie stałym bramka SNN zmniejsza energię całego układu względem FFT? „Odpowiedź może być negatywna.”
- **Rozszerzenia:** pomiary na fizycznych płytkach Lu.i, jeśli starczy sprawnych egzemplarzy (`CURRENT_ASSUMPTIONS.md:13`). Wersja Pi Zero zależy od sprzętu. Identyfikacja domowników to późniejsza faza.
- „Cyfrowy bliźniak 1:1”, przewaga energetyczna i końcowy recall przy budżecie FA/h to nadal **hipotezy** (`Architektura_SNN_Agent.md:55-56`).

## Zakres POC
- Uno + RPi 5 + MAX4466 + kamera USB/CSI. SNN i vision w Azure, Pi jako lekki klient stale włączony. „Wybudzenie” oznacza uruchomienie ścieżki zdjęcia, nie boot po halt (`Architektura_SNN_Agent.md:20, 41-47`).
- Dashboard: ciemny, po angielsku, 0–50 płytek Lu.i, telemetria. **Nie jest** pomiarem hardware.
- Poza zakresem v1: ESP32, identyfikacja domowników, automatyczna regulacja potencjometrów Lu.i (`Architektura_SNN_Agent.md:34`).
- Szczegóły sprzętu i wdrożenia: [[Sprzęt i wdrożenie]]; tor modelu: [[Model SNN — od dźwięku do decyzji]].

## Protokół badania
Pięć punktów z `CURRENT_ASSUMPTIONS.md:17-25`:
1. **Zamrożenie** wersji audio, grup źródłowych, generatora strumieni, seedów, podziału train/val/test, etykiet i manifestu enkodera. Kontrola przecieku. Syntetyczny strumień musi być raportowany jako syntetyczny.
2. **Te same wejścia** dla obu detektorów. Różnice toru wejściowego (mikrofon, ADC, głośnik) są dokumentowane. Baseline FFT ma jawne okno, krok, pasma, próg i cooldown.
3. **Parametry tylko na walidacji**, test uruchamiany raz. Dopasowanie trigger↔zdarzenie 1:1. Raport obejmuje recall zdarzeniowy, **FA/h** (mianownik = ekspozycja negatywna po odjęciu zdarzeń i tolerancji), opóźnienie p50/p95, duplikaty i aktywacje po cooldown.
4. Porównanie **detektorów** kończy się na „glass / no glass”. Tor wdrożony (Uno, Pi, łącze, Azure) raportujemy osobno: opóźnienie sieci, cold start, błędy, moc.
5. **Energia:** punkt pomiaru, czas, stany i powtórzenia. Koszt Azure ani czas CPU nie są pomiarem energii. Estymacja Lu.i ma postać $P_N \approx P_{base} + N \cdot P_{increment}$ z podanymi założeniami.

Metryki i liczby: [[Wyniki i metryki]]. Kontrakt zbioru (`docs/DATASET_CONTRACT.md`, wersje v1.0.0/v2.0.0, kontrole K1–K9, O1–O5, próg akceptacji): [[Dane i zbiory]].

> [!wazne] Uczciwość wyniku
> „Jeśli FFT wygra, raportujemy to wprost” (`CURRENT_ASSUMPTIONS.md:29`). Wczesne liczby recall, „optymalna liczba neuronów” i pobór prądu **nie są wynikami końcowymi**.

## Podział ról w zespole
Źródła: `CURRENT_ASSUMPTIONS.md:31`, `W0_START_HERE.md:24-33`, `Plan_zespolu_SNN.md:71-80, 144-155`.

| Osoba | Zakres | Gałąź (plan) | Pliki |
|---|---|---|---|
| **Wiktor** | Edge, API, Foundry, Azure, auth, integracja; właściciel kontraktów i CI | `feat/wiktor-edge-cloud(-NN)` | `rpi_agents/agent`, `cloud/app` backend, `cloud/infra`, `contracts`, `.github/workflows` |
| **Patryk** | Runtime Lu.i (symulator), stan, telemetria, eksport championa | `feat/patryk-lui-runtime` → `-state-stream-decoder` → `-telemetry-p3` | `snn_runtime`, `tests/runtime` |
| **Karolina** | Angielski dashboard i formularz logowania | `feat/karolina-dashboard` | `rpi_agents/cloud/app/templates`, `static`, `routes_dashboard`, `tests/dashboard` |
| **Marcel** | Trening, ocena, niezmienny champion; z Kacprem continuous dataset | `feat/master-pipeline` | `master_pipeline`, `ga_neuron_search` |
| **Kacper** | Enkoder i builder continuous, baseline FFT | `feat/encoder-features`, `feat/continuous-dataset` | `encoder/features-improvement`, `dataset/continuous` |
| **Andrzej** | Uno, stanowisko, kalibracja, energia, opcjonalne płytki Lu.i | `feat/andrzej-hardware-energy` (nie istnieje) | `hardware/calibration`, measurements (planowane) |

Reviewerzy (`Plan_zespolu_SNN.md:160-163`): Wiktor ↔ Patryk, Karolina ↔ Wiktor, Marcel ↔ Patryk, Kacper ↔ Marcel, Kacper ↔ Wiktor/Andrzej, Andrzej ↔ Patryk. Osobne listy zadań są w `docs/project/team-plan/taski/T01_Wiktor.md` … `T06_Andrzej.md` (6 tasków, 32 kroki, `team-plan/README.md:7`).

## Harmonogram i plan dostaw
- **Plan na 3 dni** (`Plan_zespolu_SNN.md:134-142`). D1: kontrakty, wybrany enkoder, inwentaryzacja, mock UI, testowy runtime, eksport bez ponownego treningu. D2: pierwszy prawdziwy E2E na Pi 5, UI live, ciągły dataset, pierwsze pomiary. D3: poprawki, zamrożona ocena, porównanie hardware, demo, manifest release.
- **Kolejność PR** (`Plan_zespolu_SNN.md:84-90`): W0 → K1/K2, K3 → P1/P2, W1/W2, C1/C2 → M2–M5, P3–P5, C3/C4 → W3/W4, C5 → W5/A5.
- **Proces:** tylko gałęzie `feat/`, PR do `master`, zielony `pr-gate`, squash. Wymóg akceptacji review tymczasowo wyłączony na polecenie Wiktora (`W0_START_HERE.md:17-20`, `docs/project/README.md:18`). Po squashu kolejny przyrost na nowej gałęzi (np. `feat/wiktor-edge-cloud-02`).
- **Trening** projektowany na MacBooka M5 Max 128 GB, przepustowość do ustalenia benchmarkiem M0 (`Plan_zespolu_SNN.md:656-698`).
- **Punkty decyzyjne** D1–D3 i definicja zakończenia: `Plan_zespolu_SNN.md:708-720`.
- Faktyczny przebieg (daty PR): [[Branche — szczegóły#Kamienie milowe (chronologicznie)]].

## Decyzje architektoniczne (ADR)

**W `master`: `architecture_10_neurons/adr/`** (szczegóły w [[Architektury sieci]])
- **ADR-0001** (accepted, 2026-07-09): sieć 10 płytek Lu.i z 7 neuronami cech L0, 2 kombinatorami L1 i 1 neuronem decyzji L2, **bez fan-outu J4**. Trzecia kopia Peak zastąpiona kanałem `ZCR/HF proxy` (`0001-lui-10-neuron-no-fanout.md:12-17`).

**Tylko na gałęzi `feat/dashboard`: `rpi_agents/docs/adr/`** (stary agent; do W0 nie przeniesione jako obowiązujące, `rpi_agents/MIGRATION.md`)
- 0001: Pi wypycha zdarzenia, dashboard nie odpytuje Pi.
- 0006: jeden POST (JSON + obraz base64) zamiast dwuetapowego uploadu SAS.
- 0007: Azure Container Apps (Consumption, scale-to-zero) + jeden Storage Account (Blob+Table).
- 0008: jedna aplikacja FastAPI serwuje API i UI.
- 0009: stałe HTTP Basic Auth zamiast Entra ID.
- 0010: sekrety przez GitHub Actions secrets + zmienne Terraform.
- 0011: Terraform zamiast Bicep.
- 0012: GHCR zamiast ACR.
- 0013: CD przez GitHub Actions (PR `feat/dashboard` → `feat/azure-cd`).
- 0014: `event_id` (ULID) generuje Pi, ingest to idempotentny upsert.
- 0015: ograniczona lokalna kolejka synchronizacji opróżniana przy każdym wybudzeniu.
- 0016: endpoint `GET /api/metrics` z agregatami.
- 0017: Gemini zwraca `window_broken` jako osobny osąd, który nie steruje alarmem.
- 0018: ręczny przegląd zdarzeń (`PATCH /api/events/{id}`) jako ground truth.
- ADR 0002–0005 (Functions, Static Web Apps, Entra ID, App Settings) usunięto jako zastąpione (README ADR na `feat/dashboard`).

> [!wazne] Nowsze decyzje zastępują stare ADR
> Plan zespołu wprowadza jeden wspólny login z sesją cookie `__Host-snn_session`, Argon2id, CSRF i limitem prób (`Plan_zespolu_SNN.md:112-130`). Zastępuje to ADR-0009 (Basic Auth z domyślnymi `ids/ids`). Pi jest stale włączone, co zastępuje plan halt/boot (`Architektura_SNN_Agent.md:36`).

## Pozostałe dokumenty w `docs/`
- `docs/project/architecture/Architektura_SNN_Agent.md` (v1.1, 546 linii, 32 rozdziały): pełna architektura demonstratora (enkoder i ramka USB, serwis Pi, sesja symulacji, API i telemetria, kontrakty, niezawodność, reguła alarmu, champion, model Lu.i, spec. dashboardu, metryki ciągłe, model energii, Azure, testy odbioru, ryzyka). Wersja `.docx` 1.0 jest starsza. `ILUSTRACJE.md` + `assets/` (diagramy .dot/.svg/.png, zdjęcie Lu.i z licencją).
- `docs/project/team-plan/`: `Plan_zespolu_SNN.md/.docx`, `Mapa_projektu.html` (interaktywna mapa), `zadania.json`, `analiza_branchy.json` (snapshot gałęzi), hooki blokujące commit/push na master + `install-hooks.sh`.
- `docs/project/RASPBERRY_PI_DEVELOPMENT.md`: kod i AI na Macu, Pi 5 jako zdalne stanowisko testowe przez SSH (`Host snn-pi`, klucze, bez sekretów w repo). Kolejność testów na Pi: serial → kamera → GPIO → mock API → sesja Azure → E2E.
- `docs/project/W0_START_HERE.md`: start zespołu po W0. Komendy: venv `.venv-w0`, `pip install --require-hashes -r requirements-w0.lock`, `ruff`, `pytest tests/w0`, `python -m rpi_agents.cloud.app.mock_api`. Granice własności plików i submoduł Lu.i (commit `0b2c0193…`). Szczegóły w [[contracts]] i [[tests]].
- `docs/DATASET_CONTRACT.md`: kontrakt danych SNN, opisany w [[Dane i zbiory]].
- Starsze (2026-03-23, Patryk, `6caf6689`): `BOM.md` (zakupy faz 1–4), `backplane_design.md` (backplane 3–5 neuronów analogowych), `spi_protocol.md` (SPI Arduino Mega ↔ RPi), `synapse_design.md` (synapsa jako dzielnik napięcia). Dotyczą wcześniejszej koncepcji własnych neuronów analogowych. **niepewne:** czy są jeszcze aktualne; CURRENT_ASSUMPTIONS ich nie wymienia.
- `docs/Projekt Neuromorficzny Wake-Up AI.pdf` (361 KB): pierwotna propozycja architektury (`README.md:41`).

## Materiały konferencyjne (`docs/konferencja_2026/`)
Wg `docs/konferencja_2026/README.md` to komplet decyzyjny przed wyborem tematu artykułu (wrzesień 2026). Docelowa konferencja w architekturze to ICTMOD, przedłużony termin 30.09.2026 (`Architektura_SNN_Agent.md:498-501`).

| PDF | Zawartość |
|---|---|
| `05_Mindmapa_jedna_strona.pdf` | 1 strona: pytanie, teza, model, co mamy i czego brakuje, terminy |
| `01_Propozycja_kiedy_SNN_sie_oplaca.pdf` | Pytania PB1–PB4, model energetyczny (bilans godzinowy, próg FA/h), literatura, eksperymenty E1–E8, baseline, abstrakty PL/EN, konspekt, ryzyka |
| `02_Roadmap_do_wysylki.pdf` | Harmonogram 23–30.09, ścieżka krytyczna, wariant zapasowy |
| `03_System_od_zera.pdf` | Nauka systemu: warstwy, przepływ, mapa repo, słownik, pułapki |
| `04_Metryki_i_pomiary.pdf` | Dlaczego ramkowe F1 myli, recall przy budżecie FA/h, pomiar energii |

- Główna teza (`_build/doc1.py:86-106`): bilans energii zdominowany przez fałszywe wybudzenia $N_{FA}$. Próg opłacalności względem stale aktywnego hosta: $N_{FA} < \frac{3600}{T_{wake}}\left(1 - \frac{P_{gate}}{P_{host,active}}\right)$. Przykładowe ~120 FA/h policzono z założonych, **niezmierzonych** wartości.
- Eksperymenty E1–E4 (pobór Lu.i, Arduino, hosta, bilans) to ścieżka krytyczna. E5 to baseline FFT, E6 SNN na zbiorze ciągłym, E7 płytki (opcjonalnie), E8 kwerenda Scopus/IEEE (`doc1.py:236-272`).
- Liczby `[X]` czekają na pomiar. Literatura pochodzi z otwartego internetu, przed wysyłką potrzebna jest kwerenda w Scopus/IEEE. PDF-y są generowane skryptami `_build/docN.py` (`uv run --with reportlab python docN.py`).

> [!wazne] Model energetyczny w PDF-ach zakłada usypianie hosta
> `doc1.py` liczy z hostem w uśpieniu i kosztem bootu. Nowsze CURRENT_ASSUMPTIONS i Architektura (24.09) przyjmują **stale włączone Pi** (rozdz. 26 „Model energetyczny przy stale włączonym Pi”). **niepewne:** która wersja modelu trafiła do zgłoszenia.

## Niezacommitowane zmiany
`git status --short` (2026-10-05) pokazuje wyłącznie pliki **nieśledzone** (`??`), bez modyfikacji śledzonych:
- `docs/konferencja_2026/` (12 plików): 5 PDF-ów, README i skrypty `_build/common.py`, `doc1–5.py`. To materiały konferencyjne opisane wyżej, lokalnie od 2026-09-23.
- `praca_wtorek_2026_08_04/` (24 pliki): review projektu, decyzje, zadania A–D, FAQ, literatura, protokół pomiarowy, narzędzia (`capture_serial.py`, `mic_check.ino`), pakiet `software/`. Opis w [[praca_wtorek_2026_08_04]].
- `uv.lock` w roocie.

## Powiązania
[[Struktura repozytorium]] · [[Branche — szczegóły]] · [[Model SNN — od dźwięku do decyzji]] · [[Dane i zbiory]] · [[Wyniki i metryki]] · [[Sprzęt i wdrożenie]] · [[Architektury sieci]]

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.
