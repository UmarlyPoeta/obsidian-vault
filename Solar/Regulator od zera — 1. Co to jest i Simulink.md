---
typ: solar-nauka
pakiet: "[[regulator]]"
tags: [solar, regulator, simulink, nauka]
zaktualizowano: 2026-10-05
---
# Regulator od zera — 1. Co to jest i jak działa Simulink

> [!skrot] W 30 sekund
> - **Regulator to automatyczny kierowca.** Co 20 ms patrzy, gdzie jest łódź i gdzie ma jechać, i decyduje, ile „gazu” dać lewemu i prawemu silnikowi.
> - **Mózg regulatora narysowano w Simulinku** (program z MATLAB-a, w którym rysuje się schemat z klocków), a potem Simulink sam wygenerował z rysunku kod w C. My mamy tylko ten wygenerowany kod.
> - **Kod C++ wokół to „listonosz”:** odbiera wiadomości ROS, wkłada liczby do skrzynki `rtU`, każe modelowi policzyć krok, wyjmuje wynik ze skrzynki `rtY` i wysyła go na magistralę CAN.
>
> Seria: **1. Co to jest i Simulink** → [[Regulator od zera — 2. Matematyka]] → [[Regulator od zera — 3. Logika rozmyta]] → [[Regulator od zera — 4. Dlaczego się buja]]

## 1. Regulator jako kierowca

Wyobraź sobie, że prowadzisz łódkę z dwoma silnikami i nie masz steru. Robisz w kółko to samo:

```mermaid
flowchart LR
  A["👀 Patrzę:<br/>gdzie jestem?<br/>w którą stronę płynę?<br/>jak szybko?"] --> B["🧠 Myślę:<br/>gdzie mam płynąć?<br/>o ile się mylę?"]
  B --> C["✋ Działam:<br/>ile gazu lewy,<br/>ile gazu prawy"]
  C --> D["🚤 Łódź się rusza"]
  D --> A
```

Regulator robi dokładnie to, tylko **50 razy na sekundę** (co 20 ms). Taka pętla „patrz → myśl → działaj → patrz” to **sprzężenie zwrotne**. Regulator nie musi wiedzieć, jak działa woda i wiatr: poprawia się na bieżąco na podstawie tego, co mierzy.

## 2. Gdzie regulator siedzi w całej łodzi

```mermaid
flowchart LR
  BT["L3: drzewo zachowań<br/>(basic_controller)<br/>„jedź do boi”"] -->|"/goal"| PL["L2: planner A*<br/>rysuje trasę<br/>omijającą przeszkody"]
  BT -->|"/regulator_or_thrust_driver<br/>/regulator_speed"| REG
  PL -->|"/path_points<br/>(lista punktów trasy)"| REG["L0: REGULATOR<br/>(ten pakiet)"]
  SLAM["SLAM: /slam/odom<br/>gdzie jestem"] --> REG
  XS["Xsens INS:<br/>/filter/euler (kurs)<br/>/filter/velocity (prędkość)<br/>/imu/data (obrót)"] --> REG
  REG -->|"/to_can_bus<br/>ramka 0x05"| CAN["ros2_socketcan<br/>→ magistrala CAN"]
  CAN --> MOT["⚙️ Sterowniki silników<br/>lewy / prawy"]
  MOT -.->|"łódź się porusza,<br/>sensory to widzą"| SLAM & XS
```

- **Planner** mówi *którędy* jechać (rysuje trasę jako listę punktów).
- **Regulator** mówi *jak* jechać tą trasą (ile gazu w każdym silniku).
- Przełącznik `/regulator_or_thrust_driver` decyduje, czy steruje regulator, czy [[diff_thrust_driver]]. Nigdy oba naraz.

## 3. Jak się skręca łodzią z dwoma silnikami

Łódź nie ma steru. Skręca się różnicą ciągu, jak czołg albo wózek inwalidzki:

```mermaid
flowchart TB
  subgraph A["Prosto"]
    a1["L: +500 ⬆️"] --- a2["P: +500 ⬆️"]
  end
  subgraph B["Łuk w lewo"]
    b1["L: +200 ⬆️"] --- b2["P: +700 ⬆️"]
  end
  subgraph C["Obrót w miejscu w lewo"]
    c1["L: −1000 ⬇️"] --- c2["P: +1000 ⬆️"]
  end
  subgraph D["Hamowanie / wstecz"]
    d1["L: −500 ⬇️"] --- d2["P: −500 ⬇️"]
  end
```

Każda komenda dla silnika to liczba od **−1000** (pełny wstecz) do **+1000** (pełny naprzód). Myśl o tym jak o dwóch składnikach:

| Składnik | Wzór intuicyjny | Co robi |
|---|---|---|
| **wspólny** (jazda) | L i P dostają to samo | do przodu / do tyłu |
| **różnicowy** (skręt) | L dostaje −x, P dostaje +x | obrót |

Regulator rozmyty (notatka 3) dokładnie tak myśli: osobno „za wolno/za szybko” (wspólny) i osobno „za mało/za dużo obrotu” (różnicowy).

## 4. Co to jest Simulink

**Simulink** to część MATLAB-a, w której zamiast pisać kod **rysujesz schemat z klocków** połączonych strzałkami. Strzałki to sygnały (liczby, które płyną), a klocki to operacje.

Najczęstsze klocki w naszym modelu:

| Klocek Simulinka | Symbol | Co robi | Przykład w regulatorze |
|---|---|---|---|
| **Inport / Outport** | `⟶▢` / `▢⟶` | wejście / wyjście modelu | `pos_x`, `Left Thruster` |
| **Gain** | `▷ k` | mnoży przez stałą | `×15`, `×1000`, `×2`, `×10` |
| **Sum** | `⊕` | dodaje / odejmuje | błąd = zadane − zmierzone |
| **Product** | `⊗` | mnoży dwa sygnały | `cos ψ · vel_y` |
| **Saturation** | `▢ ⟋` | obcina do zakresu | do ±1, do ±1000 |
| **Switch** | `⇄` | wybiera jeden z dwóch sygnałów wg warunku | „jeśli do końca > 1 m, to…” |
| **Delay** | `1/z` | oddaje wartość **z poprzedniego kroku** (pamięć) | lookahead z poprzedniego α |
| **Trigonometry** | `sin`, `cos`, `atan2`, `tanh` | funkcje matematyczne | kierunki, hamulce |
| **MATLAB System** | `▢ PP` | gotowy algorytm z biblioteki | **Pure Pursuit** |
| **Fuzzy Logic Controller** | `▢ FIS` | regulator rozmyty | 9 reguł → ciąg silników |

### Od rysunku do programu

```mermaid
flowchart LR
  SLX["📐 code_gen_1.slx<br/>schemat w Simulinku<br/>(NIE MA go w repo!)"] -->|"Simulink Coder<br/>R2024b, 10.05.2026"| GEN["📄 code_gen_1_ert_rtw/<br/>code_gen_1.c<br/>code_gen_1.h<br/>code_gen_1_data.c"]
  GEN --> CMAKE["CMakeLists.txt<br/>kompiluje razem z<br/>regulator.cpp, node.cpp"]
  CPP["regulator.cpp<br/>(ręcznie pisany)"] --> CMAKE
  CMAKE --> EXE["⚙️ program Regulator"]
```

> [!wazne] Ważne praktycznie
> W repo jest **tylko wygenerowany kod C**, nie ma pliku `.slx`. Żeby zmienić coś w środku regulatora (np. lookahead albo wzmocnienie), trzeba albo zdobyć model `.slx` od autora (`package.xml`: filip, `filip.nawalaniec@solarboat.agh.edu.pl`) i wygenerować kod na nowo, albo ręcznie edytować `code_gen_1.c` (da się, ale następna generacja to nadpisze), albo przepisać model do C++.

## 5. Jak kod C++ rozmawia z kodem z Simulinka

Wygenerowany kod nie wie nic o ROS. Komunikacja idzie przez **trzy globalne „skrzynki”**:

```mermaid
flowchart LR
  subgraph ROS["regulator.cpp (C++, ROS 2)"]
    CB["callbacki<br/>(przychodzą wiadomości)"]
    TM["timer co 20 ms"]
    SC["SendControl()"]
  end
  subgraph BOX["Globalne zmienne (wspólna pamięć)"]
    U["📥 rtU<br/>wejścia:<br/>pos_x, pos_y, heading,<br/>vel_x, vel_y, angular_velocity,<br/>desired_vel, end_heading"]
    WP["📍 waypoint_x[100]<br/>waypoint_y[100]"]
    DW["🧠 rtDW<br/>pamięć między krokami:<br/>stan Pure Pursuit, Delay"]
    Y["📤 rtY<br/>wyjścia:<br/>LeftThruster, RightThruster,<br/>+ diagnostyka"]
  end
  subgraph MDL["code_gen_1.c (z Simulinka)"]
    ST["code_gen_1_step()"]
  end
  CB -->|"zapis"| U & WP
  TM -->|"woła"| ST
  U & WP --> ST
  ST <-->|"czyta i zapisuje"| DW
  ST -->|"zapis"| Y
  Y -->|"odczyt"| SC
  SC -->|"/to_can_bus"| OUT["CAN"]
```

A tak wygląda jedna sekunda pracy w czasie:

```mermaid
sequenceDiagram
  participant Odom as /slam/odom (~10 Hz?)
  participant Path as /path_points (~2 Hz)
  participant CPP as regulator.cpp
  participant U as rtU / waypoint_*
  participant M as code_gen_1_step()
  participant Y as rtY
  participant CAN as /to_can_bus
  Path->>CPP: nowa trasa (lista punktów)
  CPP->>U: waypoint_x/y[0..99] (dopełnione ostatnim punktem)
  Odom->>CPP: pozycja
  CPP->>U: pos_x, pos_y
  loop co 20 ms (50×/s)
    CPP->>M: code_gen_1_step()
    M->>U: czytaj wejścia
    M->>Y: zapisz ciąg L/P
    CPP->>Y: odczytaj
    CPP->>CAN: ramka 0x05 [L, P]
  end
```

Częstotliwości `/slam/odom` w diagramie to założenie, nie pomiar. `/path_points` przychodzi tak często, jak [[pose_graph]] publikuje costmapę (~2 Hz).

### Dlaczego „krok” (`step`)
Komputer nie liczy w sposób ciągły. Simulink zamienia schemat na funkcję, którą woła się co stały odstęp czasu (tu 20 ms). Każde wywołanie to jeden „kadr filmu”: weź aktualne wejścia, policz wyjścia, zapamiętaj co trzeba na następny kadr (`rtDW`).

## 6. Cały schemat modelu (odtworzony z kodu C)

To jest „rysunek Simulinka” odtworzony z `code_gen_1.c`. Szczegóły każdego klocka w [[Regulator od zera — 2. Matematyka]].

```mermaid
flowchart LR
  %% wejścia
  hd(["heading [°]"]) --> G1["×(−π/180)<br/>S1: deg→rad"] --> WR["atan2(sin,cos)<br/>zawinięcie"] --> PSI(("ψ"))
  px(["pos_x, pos_y"]) --> PP
  wp(["waypoint_x/y"]) --> PP
  vd(["desired_vel"]) --> SUB1["⊕ vd − Delay"] -->|"L i v"| PP["MATLAB System<br/>PURE PURSUIT<br/>(S2)"]
  PSI --> PP
  PP -->|"α"| AT["0.7·atan(2α)<br/>sat ±1, abs, ×vd"] --> DEL["Delay 1/z"] --> SUB1
  wp -->|"ostatni punkt"| DIST["S5: odległość<br/>do końca d"]
  px --> DIST
  PSI --> LOC["S8: końcowy punkt<br/>w osiach łodzi<br/>x_loc, y_loc"]
  DIST --> SWV{"Switch<br/>d > 1 m ?"}
  PP -->|"v_pp"| SWV
  LOC -->|"tanh(0.5·x_loc)·v"| SWV
  PP -->|"ω_pp"| SWW{"Switch<br/>d > 1 m ?"}
  LOC -->|"sat₀.₅ tanh(y_loc)"| SWW
  vel(["vel_x, vel_y"]) --> VACT["v_act = cosψ·vel_y + sinψ·vel_x"]
  PSI --> VACT
  SWV --> E1["⊕ v_ref − v_act<br/>sat ±1"]
  VACT --> E1
  om(["angular_velocity"]) --> E2
  SWW --> E2["⊕ ω_ref + ω_imu<br/>×2, sat ±1"]
  eh(["end_heading"]) --> E3["d ≤ 0.3 ? (end_heading − ψ) : 0<br/>×10, sat ±1"]
  PSI --> E3
  E1 & E2 & E3 --> FIS["FUZZY LOGIC<br/>CONTROLLER<br/>(S9, 9 reguł)"]
  FIS -->|"lewy"| GL["×15 ×1000<br/>sat ±1000"] --> LT(["Left Thruster"])
  FIS -->|"prawy"| GR["×15 ×1000<br/>sat ±1000"] --> RT(["Right Thruster"])
  classDef pp fill:#e6dcf5,stroke:#9467bd,color:#000
  classDef brk fill:#dcf2dc,stroke:#2ca02c,color:#000
  classDef fz fill:#f8dcdc,stroke:#d62728,color:#000
  class PP,SUB1,AT,DEL pp
  class DIST,LOC,SWV,SWW brk
  class E1,E2,E3,FIS,GL,GR fz
```

Kolory w głowie: **fioletowe** to „gdzie jechać” (Pure Pursuit), **zielone** to „hamuj przy końcu”, **czerwone** to „ile gazu” (fuzzy).

```mermaid
flowchart LR
  A["1️⃣ GDZIE JECHAĆ<br/>Pure Pursuit:<br/>kierunek α, ω_ref, v_ref"] --> B["2️⃣ CZY JUŻ KONIEC?<br/>d ≤ 1 m → hamulce tanh"]
  B --> C["3️⃣ O ILE SIĘ MYLĘ?<br/>e_v, e_ω, e_end<br/>(obcięte do ±1)"]
  C --> D["4️⃣ ILE GAZU?<br/>fuzzy: 9 reguł<br/>→ ×15000 → ±1000"]
```

## 7. Słowniczek
| Termin | Po ludzku |
|---|---|
| **sprzężenie zwrotne** | mierzysz skutek swojej decyzji i poprawiasz następną |
| **wartość zadana** (ref) | to, co *chcę* (np. 1 m/s) |
| **wartość zmierzona** (act) | to, co *jest* (np. 0.7 m/s) |
| **błąd** (e) | zadana − zmierzona (tu: 0.3 m/s „za wolno”) |
| **wzmocnienie** (gain) | mnożnik: jak mocno reagować na błąd |
| **saturacja** | obcięcie do zakresu, „więcej się nie da” |
| **lookahead** | jak daleko przed siebie na trasie patrzy regulator |
| **kurs** (heading, ψ) | w którą stronę patrzy dziób |
| **fuzzy** (logika rozmyta) | reguły słowne typu „jeśli trochę za wolno, to trochę gazu” |

## Powiązane
[[regulator]] · [[Regulator — hierarchia i działanie]] (wersja techniczna) · [[Regulator — TODO, błędy i oscylacje]]

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.
