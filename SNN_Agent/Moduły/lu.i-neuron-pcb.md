---
typ: snn-modul
sciezka: lu.i-neuron-pcb/
tags: [snn, modul, sprzet]
zaktualizowano: 2026-10-05
---
# lu.i-neuron-pcb

> [!skrot] W 30 sekund
> - **Co to:** submoduł gita z zewnętrznym projektem otwartego sprzętu **Lu.i** (giant-axon). To analogowa, edukacyjna płytka z neuronem leaky integrate-and-fire: 3 synapsy z regulowaną wagą i znakiem, regulacja potencjału spoczynkowego i stałych czasowych, pasek LED pokazujący potencjał, dioda spike, 3 terminale wyjścia (`docs/project/architecture/Architektura_SNN_Agent.md:232`).
> - **Wejście → wyjście:** impulsy na złącza J1–J3 → impuls wyjściowy (wg interfejsu źródłowego ok. 15 ms i 2,5 V, do sprawdzenia na posiadanych egzemplarzach, `Architektura_SNN_Agent.md:236`).
> - **Gdzie działa:** fizyczne płytki zasilane bateriami CR2032. W repo ten katalog jest **pusty**, bo submodułu nie zainicjowano. Projekt korzysta z Lu.i pośrednio, przez model i ograniczenia w kodzie.

## Struktura (drzewo z opisem plików)
- `.gitmodules`: `path = lu.i-neuron-pcb`, `url = https://github.com/giant-axon/lu.i-neuron-pcb.git`.
- Gitlink przypięty na commit **`0b2c0193595873fb41765e0a5293f15f4c8057b1`** (`git ls-tree`, `docs/project/W0_START_HERE.md:93-95`).
- `git submodule status` zwraca `-0b2c019… lu.i-neuron-pcb`. Prefiks `-` oznacza brak inicjalizacji, a katalog roboczy jest pusty.
- Licencja źródła: CERN-OHL-S v2. Jej kopia leży w `docs/project/architecture/assets/LICENSE-Lui.txt`, a zdjęcie płytki (`doc/figures/photograph-coin.jpg`) wykorzystano w architekturze (`ILUSTRACJE.md:12`).

**niepewne:** zawartość upstreamu na commicie 0b2c019 (schemat KiCad, BOM, `doc/electronic-interface.md`) nie została tu sprawdzona, bo katalog jest pusty, a repo jest tylko do odczytu. Dokumenty projektu powołują się na README i `doc/electronic-interface.md` [S1] oraz na publikację Stradmann i in., arXiv:2404.16664 [S2] (`Architektura_SNN_Agent.md:518-519`).

## Jak działa (płytka według dokumentów repo)
Trymery według `architecture_14_neurons_patryk_09_07/PRZEWODNIK_KOMPLETNY.md:95-102`:

| Trymer | Funkcja | Wartość |
|---|---|---|
| RV1, RV2, RV3 | waga wejść J1, J2, J3 | 22 kΩ |
| RV4 | stała czasowa (τ_syn albo τ_mem, nierozstrzygnięte) | 22 kΩ |
| RV5 | V_leak (spoczynek, potwierdzone ze schematu) | 100 kΩ |
| RV6 | stała czasowa (τ_syn albo τ_mem) | 50 kΩ |

Złącza: J1–J3 to wejścia synaptyczne, J4/terminale to wyjście (3 terminale z tym samym sygnałem, czyli fan-out do 3 płytek), GND musi iść na wspólną szynę. Model referencyjny: $\tau_{mem}\,dV/dt = -(V - V_{leak}) + I_{syn}/g_{leak}$ (`Architektura_SNN_Agent.md:234`).

### Jak repo z tego korzysta
- **Ograniczenia kontraktu:** `target_port ∈ {1,2,3}` i najwyżej jeden przewód na port (fan-in 3), czyli `PHYSICAL_PORTS = (1, 2, 3)` w `snn_runtime/manifest.py:45`. Limit 50 płytek występuje w `ModelManifest`, `NeuronFrame` i `TopologyDraft` (`snn_runtime/topology.py:30-31`).
- **Model dynamiki:** integrator `lui-order2-hard-reset` w [[snn_runtime]]. Potencjał wyrażony jest w ułamkach V_th (V_TH = 1,0 ≙ VDD/2, `snn_runtime/units.py:379-387`), czyli w `a.u.` aż do kalibracji (zadanie A2, Andrzej).
- **Sieć docelowa:** 7 kanałów → 4×H → 3×G → 1×D = **8 płytek** (`praca_wtorek_2026_08_04/README.md`). Nastawy trymerów i pasków LED pochodzą z `architecture_14_neurons_patryk_09_07/hw7_config.json`. Eksport `hw_*.json` niesie `led_bar_pct`, `scale_k`, `pot_pct`, `pulses_to_fire_100Hz`, które nie mieszczą się w `ModelManifest` (`snn_runtime/MAPPING.md` §3).
- **Dashboard** rysuje płytki Lu.i na podstawie `neuron.svg` i zdjęcia (`Architektura_SNN_Agent.md:258`).

## Jak uruchomić
```sh
git submodule update --init lu.i-neuron-pcb     # W0_START_HERE.md:97-98 (testy W0 tego nie potrzebują)
```

## Konfiguracja i parametry
Kalibracja dotyczy konkretnego egzemplarza: `board_id`, rewizja PCB, napięcie, temperatura, nastawy potencjometrów (`Architektura_SNN_Agent.md` §15). Protokół pomiarowy: [[praca_wtorek_2026_08_04]].

## Wyniki
W repo nie ma zapisanych pomiarów płytek. `praca_wtorek_2026_08_04/pomiary/` zawiera tylko README, a `tests/neuron_characterization_20260322.md` to niewypełniony szablon.

## Powiązania
[[snn_runtime]] (model i kontrakt), [[Architektury sieci]], [[Model SNN — od dźwięku do decyzji]], [[praca_wtorek_2026_08_04]], [[Sprzęt i wdrożenie]].

## Na gałęziach
- Gitlink dodał UmarlyPoeta w commicie 617cca52 (2026-07-16, „zmiany zmiany zmiany”), **bez** `.gitmodules`. Taki submoduł był zepsuty (`praca_wtorek_2026_08_04/00_REVIEW_PROJEKTU.md`, znalezisko P0-4).
- Wpis `.gitmodules` dodał W0, #52 (637989ee, 2026-09-24, Wiktor), żeby `actions/checkout` poprawnie obsługiwał submoduły. Commit pozostał ten sam.

## Znane problemy / TODO
- RV4 kontra RV6 (który odpowiada za τ_syn, a który za τ_mem) jest nierozstrzygnięte, bo w checkoutcie nie ma schematu (P0-4, zadanie B-4 w `praca_wtorek`).
- W dokumentach adres upstreamu występuje w dwóch pisowniach: `giant-axon` (`.gitmodules`) i `giantaxon` (`Architektura_SNN_Agent.md:518`). **niepewne:** która jest aktualna.
- Szerokość impulsu: 500 µs w jednym szkicu firmware'u kontra ok. 15 ms dla Lu.i (`Architektura_SNN_Agent.md:109`). W manifeście wpisano `pulse_width_us 500` jako wartość do potwierdzenia.

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.
