---
typ: solar-naprawa
repo: "[[l0hwd]]"
pakiet: "[[regulator]]"
tags: [solar, regulator, oscylacje, kod]
zaktualizowano: 2026-10-06
---
# Regulator: gotowe poprawki do wdrożenia (waypoint → skos prawo/lewo)

> [!skrot] W 30 sekund
> - Objaw: po podaniu waypointu łódź jedzie ~0.3 s wężykiem (skos prawo/lewo).
> - Przyczyna: 3 sprzężone rzeczy — schodkowa trasa z plannera, lookahead ≤ 1 m (a przy błędzie jeszcze krótszy), fuzzy ×15000 działa jak przekaźnik.
> - To, co da się wdrożyć **od razu bez modelu**: ogranicznik narastania ciągu, `use`, `end_heading` w radianach, stop przy nieświeżych danych, zagęszczanie ścieżki. Patche niżej.
> - To, co **usuwa przyczynę** (lookahead, wzmocnienie): siedzi w modelu Simulink → wymaga albo `.slx`, albo przepisania Pure Pursuit do C++ (patrz sekcja 3).
> - Uwaga: patche bazują na numerach linii z notatek (2026-10-05). Zweryfikuj linie w bieżącym drzewie przed aplikacją.

## 1. Poprawki w `src/regulator.cpp` (bez ruszania modelu — wdrażaj od razu)

### 1.1. `use` niezainicjalizowane → łódź może sterować "zanim powinna" (`:30`)
```cpp
// BYŁO (regulator.cpp:30): bool use = true;   // tworzy LOKALNĄ zmienną, pole klasy nieustalone
// MA BYĆ: inicjalizacja pola klasy w nagłówku regulator.hpp
bool use{false};   // albo true, zależnie od zamierzonego stanu startowego
```
Dopóki nie ma decyzji z `/regulator_or_thrust_driver`, zachowanie jest nieokreślone (UB). To jeden z pierwszych fixów.

### 1.2. `end_heading` — jednostki (stopnie vs radiany) (`PathCallback`)
Model odejmuje `end_heading` od ψ w **radianach** (`code_gen_1.c:991`) i nie zawija różnicy. `PathCallback` liczy yaw w **stopniach**. W strefie 0.3 m `e_end = 10·(end_heading − ψ)` jest nasycone → łódź kręci się na końcu.
```cpp
// w PathCallback, przy zapisie rtU.end_heading
// BYŁO: rtU.end_heading = QuaternionToYawDegrees(...);
// MA BYĆ (radiany, konwencja −yaw jak w [[Drzewo TF i układy współrzędnych]]):
double yaw_deg = QuaternionToYawDegrees(...);
rtU.end_heading = -yaw_deg * M_PI / 180.0;   // odwróć znak i przekonwertuj
```
Uwaga: ψ jest liczone jako `−heading·π/180` (`code_gen_1.c:568`), więc końcowy kurs musi być w tej samej konwencji (minus + radiany).

### 1.3. Nieświeże dane → stop (`ControlLoop`)
`CheckTopicsStatus` tylko loguje. Jeśli `/slam/odom` lub `/path_points` przestaną przychodzić, regulator dalej jedzie na ostatnich wartościach. Na timeout: wyślij `{0,0}` i pomiń `code_gen_1_step()`.
```cpp
// w ControlLoop, zamiast wywołania step+send gdy dane nieświeże:
if (dataStale()) {                       // np. now()−last_odom_time_ > 1.0 s
  RCLCPP_WARN(this->get_logger(), "Stale odom/path — stopping thrusters");
  // wyślij {0,0} bez wołania code_gen_1_step():
  MotorSetpoints sp{0, 0, 0};            // int16 left, int16 right, int32 padding
  publishToCan(sp);
  return;
}
```

### 1.4. Ogranicznik narastania ciągu (slew) w `SendControl` — łagodzi szarpanie bez modelu
```cpp
// w SendControl, przed złożeniem MotorSetpoints:
static double prev_l = 0.0, prev_r = 0.0;
constexpr double kMaxStep = 50.0;        // na 20 ms → 0…1000 w 0.4 s
auto slew = [](double target, double& prev) {
  prev += std::clamp(target - prev, -kMaxStep, kMaxStep);
  return prev;
};
double left_thrust  = slew(rtY.LeftThruster,  prev_l);
double right_thrust = slew(rtY.RightThruster, prev_r);
```
To nie usuwa przyczyny (krótki lookahead), ale **od razu** gasi "skos prawo-lewo" na poziomie silników. Najtańsza ulga.

### 1.5. Zamiana lewy/prawy (`regulator.cpp:185`)
```cpp
// BYŁO: MotorSetpoints{right_thrust, left_thrust}   // prawy ląduje w polu left
// Jeżeli celowe (kolejność ramki/okablowanie) → dopisz komentarz.
// Jeżeli błąd → odwróć. Przed zmianą potwierdź na [[Magistrala CAN]] w jakiej kolejności w ramce 0x05.
```

### 1.6. Zagęszczanie ścieżki w PathCallback (ciągła krzywizna, stabilny rzut)
Po przycięciu przez planner jest mało punktów, a regulator ma na nie 100 miejsc. Zagęść co ~0.5 m + ewentualnie 1–2 iteracje Chaikina.
```cpp
// PathCallback: łamana → punkty co step metrów, maks. 100
std::vector<std::pair<double,double>> pts;
for (size_t k = 0; k + 1 < msg->poses.size(); ++k) {
  auto &a = msg->poses[k].position, &b = msg->poses[k+1].position;
  double len = std::hypot(b.x - a.x, b.y - a.y);
  int n = std::max(1, static_cast<int>(len / step));   // step ≈ 0.5 m
  for (int s = 0; s < n; ++s) pts.emplace_back(a.x + (b.x-a.x)*s/n, a.y + (b.y-a.y)*s/n);
}
pts.emplace_back(msg->poses.back().position.x, msg->poses.back().position.y);
```
Uwaga: 100 punktów co 0.5 m = zasięg 50 m; dla dłuższych ścieżek zwiększ `step` albo obetnij do najbliższych 50 m.

## 2. Co poprawić w plannerze (źródło schodków) — `planner/src/planner.cpp`
Łódź goni schodki, bo trasa z 8-sąsiedztwa ma załamania co 45°, a kara za skręt nie działa. Bez tego reszta pomaga mniej.

### 2.1. Kara za skręt w A* (`:135`, `:148`)
```cpp
// :135 — BYŁO (zawsze 0 — dzielenie całkowite int)
//   (std::abs(current.pastMoveIndex - i) % 5) / 5
//   MA BYĆ: kara za każdą zmianę kierunku
double turn = (i == current.pastMoveIndex) ? 0.0 : turn_penalty;  // np. 0.5
double newCost = gCost[oIndex] + moves[i].cost
               + costmap.data[nIndex] * slam_costmap + turn;
```
```cpp
// :148 — zapamiętać ruch, żeby turn w ogóle miał sens:
open.push({nx, ny, newCost + euclideanHeuristic(temp, end), i});
```

### 2.2. Przycinanie po linii widzenia (po A*) — największy zysk
```cpp
bool LineFree(const nav_msgs::msg::OccupancyGrid& map, Cell a, Cell b, int8_t max_cost) {
  // Bresenham: każda komórka na odcinku a→b musi mieć koszt < max_cost
  int dx = std::abs(b.x - a.x), sx = a.x < b.x ? 1 : -1;
  int dy = -std::abs(b.y - a.y), sy = a.y < b.y ? 1 : -1;
  int err = dx + dy;
  while (true) {
    if (map.data[a.y * map.info.width + a.x] >= max_cost) return false;
    if (a.x == b.x && a.y == b.y) return true;
    int e2 = 2 * err;
    if (e2 >= dy) { err += dy; a.x += sx; }
    if (e2 <= dx) { err += dx; a.y += sy; }
  }
}

std::vector<Cell> Prune(const std::vector<Cell>& cells,
                        const nav_msgs::msg::OccupancyGrid& map) {
  std::vector<Cell> out{cells.front()};
  size_t i = 0;
  while (i + 1 < cells.size()) {
    size_t j = cells.size() - 1;
    while (j > i + 1 && !LineFree(map, cells[i], cells[j], /*max_cost=*/50)) --j;
    out.push_back(cells[j]);
    i = j;
  }
  return out;  // na otwartej wodzie: {start, cel}
}
```
Efekt: na otwartej wodzie trasa = jeden odcinek łódź→cel; przy przeszkodach tylko punkty na narożnikach. Znika cała kategoria oscylacji "od schodka do schodka". Próg `max_cost` dobrać do inflacji z [[pose_graph]].

## 3. Prawdziwa przyczyna: model Simulink (wymaga `.slx` albo przepisania)
Następujących wartości **nie zmienisz niezawodnie w wygenerowanym `code_gen_1.c`** (następna generacja je nadpisze). Zrób to jedną z dwóch dróg — patrz sekcja 4.

| Zmiana | Gdzie | Dlaczego |
|---|---|---|
| Lookahead **stały**, niezależny od α, np. `L = clamp(k·v, 3 m, 6 m)` | Pure Pursuit (S2) | Obecnie `L = v·(1−\|sat(0.7·atan(2α))\|)` maleje z błędem (dodatnie sprzężenie). Stały L = płynny powrót, koniec skosu. Największy pojedynczy zysk. |
| Prędkość nadal może maleć z α, ale **osobnym sygnałem** | S2/S4 | Zwalnianie w zakręcie bez skracania L |
| Wzmocnienie wyjścia **15 → ~5** | S6 Gain1 | FIS realnie daje max ~±0.17; ×15000 = przekaźnik przy 0.2 rad/s. Przy ×5 nadal prawie pełny zakres na max, bez zero-jedynki |
| `MaxAngularVelocity` 1.0 → ~0.4 rad/s | S2 | mniejsze ω_ref, mniej nasyceń → mniej przestrzeliwania |
| `end_heading` wzmocnienie 10 → ~2 | S6 | razem z 1.2: koniec kręcenia w kółko na końcu |

## 4. Dwie drogi do zmian w modelu
**A. Zdobądź `.slx`** od autora (package.xml: `filip`, `filip.nawalaniec@solarboat.agh.edu.pl`), zmień wartości w Simulinku, zregeneruj `code_gen_1`. Najczystsze, ale zależne od autora.

**B. Przepisz Pure Pursuit i wyjście do C++ z parametrami ROS** (lookahead `L`, wzmocnienia, `MaxAngularVelocity`, progi jako parametry). Model jest mały (~150 linii właściwej logiki) — realne. Plus: strojenie bez MATLAB-a, bez regeneracji, można w runtime `ros2 param set`. To w praktyce najsensowniejsza opcja dla zespołu.

## 5. Plan działania (proponowana kolejność)
1. **Dziś, bez modelu:** 1.1–1.5 w `regulator.cpp` (najpierw 1.4 slew — natychmiast gasi skos), zagęszczenie ścieżki 1.6.
2. **Planner:** 2.1 kara za skręt + 2.2 przycinanie po linii widzenia. Sprawdzić na bagu `~/solar/rosbagi/docking_regulator_2` (porównać `/path_points` przed/po).
3. **Model:** decyzja A vs B, potem lookahead stały + wzmocnienie ~5 + `MaxAngularVelocity` ↓.
4. Strojenie w symulacji ([[l4sim]], `sim:=true`) i dopiero na wodzie.

## Powiązane
[[regulator]] · [[Regulator — hierarchia i działanie]] · [[Regulator — TODO, błędy i oscylacje]] · [[planner]] · [[Drzewo TF i układy współrzędnych]] · [[l4sim]]

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.
