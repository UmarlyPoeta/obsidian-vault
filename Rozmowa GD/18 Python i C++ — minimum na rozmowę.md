---
typ: pojęcie
status: nowe
przedmioty: ["[[PK Algorytmy i podstawy programowania]]"]
wymaga: []
powiązane: ["[[05 ROS 2 — podstawy]]", "[[04 Jakość danych, organizacja i upload]]", "[[Trening — Wzorce projektowe w kodzie]]"]
tags: [pojęcie, rozmowa-gd, programowanie]
utworzono: 2026-10-09
źródła: []
---
# 18 Python i C++ — minimum na rozmowę

> [!skrot] W 30 sekund
> - Ogłoszenie: „coursework level is fine”. Spodziewaj się: **prosty skrypt** (pliki, parsowanie, pętla, słownik), **NumPy** (macierz, transformacja punktu), pytania o **różnice Python vs C++**, ewentualnie „co robi ten kod / gdzie jest błąd”.
> - **Python:** listy, słowniki, comprehensions, funkcje, klasy, wyjątki, `with open`, `pathlib`, `json`, NumPy (`@`, `shape`, broadcasting), `venv` + `pip`.
> - **C++:** wskaźnik vs referencja, stos vs sterta, **RAII** i **smart pointery** (`unique_ptr`, `shared_ptr` — ROS 2 używa `SharedPtr` wszędzie), `const &` przy przekazywaniu, `vector` (z biblioteki std), klasy i `virtual`, kompilacja (CMake).
> - **Na rozmowie mów na głos, co robisz** — liczy się sposób myślenia bardziej niż perfekcyjna składnia.

> [!intuicja] Po ludzku
> Python to **szwajcarski scyzoryk** — szybko piszesz skrypt do sprawdzenia danych, sterowania, ML. C++ to **narzędzie precyzyjne** — szybkie, w czasie rzeczywistym (sterowniki, kontrolery 1 kHz), ale sam pilnujesz pamięci. W labie: skrypty QC i narzędzia w Pythonie, sterowniki i kontrolery w C++.

## Python vs C++ — tabela porównawcza
| | Python | C++ |
|---|---|---|
| Wykonanie | interpretowany (bytecode) | kompilowany do kodu maszynowego |
| Typy | dynamiczne (typ sprawdzany w trakcie) | statyczne (sprawdzane przy kompilacji) |
| Pamięć | automatyczna (zliczanie referencji + GC) | ręczna / RAII / smart pointery |
| Szybkość | wolniejszy (ale NumPy/PyTorch w C pod spodem) | bardzo szybki, przewidywalny |
| Równoległość | **GIL** ogranicza wątki CPU (multiprocessing, asyncio) | pełne wątki |
| W ROS 2 | `rclpy` — prototypy, narzędzia, ML | `rclcpp` — sterowniki, kontrolery, wydajne węzły |

## Python — co musisz umieć napisać

```python
# Struktury danych
xs = [3, 1, 2]; xs.append(5); xs.sort()          # lista (mutowalna)
t = (1.0, 2.0)                                   # krotka (niemutowalna)
d = {"cam_wrist": 598, "cam_scene": 601}         # słownik
for name, n in d.items(): print(name, n)
s = {"a", "b"}                                   # zbiór — szybkie „czy jest”
squares = [x**2 for x in range(10) if x % 2 == 0]           # list comprehension
by_cam = {k: v for k, v in d.items() if v < 600}            # dict comprehension

# Funkcje, wyjątki, pliki
from pathlib import Path
import json

def load_meta(path: Path) -> dict:
    try:
        with open(path) as f:              # with = plik zawsze zamknięty
            return json.load(f)
    except FileNotFoundError:
        print(f"brak {path}")
        return {}

# Klasa
class Episode:
    def __init__(self, idx: int, success: bool, duration_s: float):
        self.idx, self.success, self.duration_s = idx, success, duration_s
    def __repr__(self):
        return f"Episode({self.idx}, ok={self.success}, {self.duration_s:.1f}s)"
```

### NumPy — transformacje (bardzo prawdopodobne!)
```python
import numpy as np

def rot_z(theta):
    c, s = np.cos(theta), np.sin(theta)
    return np.array([[c, -s, 0], [s, c, 0], [0, 0, 1]])

T = np.eye(4)                       # macierz jednorodna 4x4
T[:3, :3] = rot_z(np.pi)            # obrót 180° wokół z
T[:3, 3] = [1.0, 0.0, 0.0]          # przesunięcie
p_cam = np.array([0.4, 0.1, 0.0, 1.0])
p_base = T @ p_cam                  # @ = mnożenie macierzy → [0.6, -0.1, 0, 1]

pts = np.random.rand(1000, 3)       # chmura 1000 punktów (N×3)
pts_base = (T[:3, :3] @ pts.T).T + T[:3, 3]     # wszystkie naraz (bez pętli!)
dist = np.linalg.norm(pts, axis=1)  # odległość każdego punktu od zera
near = pts[dist < 0.5]              # maskowanie logiczne
```

### Zadania „na tablicę” z rozwiązaniami
> [!zadanie] Średnia krocząca z listy pomiarów (okno k)
> > [!odpowiedz]- Odpowiedź
> > ```python
> > def moving_average(xs, k):
> >     if k <= 0 or k > len(xs): return []
> >     out, s = [], sum(xs[:k])
> >     out.append(s / k)
> >     for i in range(k, len(xs)):
> >         s += xs[i] - xs[i - k]        # przesuwamy okno w O(1)
> >         out.append(s / k)
> >     return out                        # O(n) zamiast O(n·k)
> > # NumPy: np.convolve(xs, np.ones(k)/k, mode="valid")
> > ```

> [!zadanie] Z listy timestampów (w sekundach) znajdź „dziury” — przerwy większe niż 1,5× okres przy 30 FPS (dropped frames)
> > [!odpowiedz]- Odpowiedź
> > ```python
> > def find_gaps(ts, fps=30, factor=1.5):
> >     period = 1.0 / fps
> >     gaps = []
> >     for a, b in zip(ts, ts[1:]):      # pary sąsiednich timestampów
> >         if b - a > factor * period:
> >             gaps.append((a, b, round((b - a) / period) - 1))   # ile klatek zgubiono
> >     return gaps
> > ```

> [!zadanie] Policz, ile epizodów każdego zadania jest w katalogu `datasets/<zadanie>/<sesja>/episode_*`
> > [!odpowiedz]- Odpowiedź
> > ```python
> > from collections import Counter
> > from pathlib import Path
> > counts = Counter(p.parts[1] for p in Path("datasets").glob("*/*/episode_*"))
> > for task, n in counts.most_common():
> >     print(f"{task}: {n}")
> > ```

> [!zadanie] Odwróć słownik `{nazwa_kamery: numer_seryjny}` → `{numer: nazwa}` i sprawdź duplikaty
> > [!odpowiedz]- Odpowiedź
> > ```python
> > inv = {}
> > for name, sn in cams.items():
> >     if sn in inv:
> >         raise ValueError(f"{sn} przypisany do {inv[sn]} i {name}")
> >     inv[sn] = name
> > ```

> [!blad] Pułapki Pythona, o które lubią pytać
> - **Mutowalny argument domyślny:** `def f(x, acc=[])` — lista jest **wspólna** między wywołaniami. Używaj `acc=None` i `if acc is None: acc = []`.
> - `is` vs `==`: `is` — ten sam obiekt, `==` — równa wartość.
> - Kopia płytka vs głęboka: `b = a` to **ta sama** lista; `a.copy()` płytka; `copy.deepcopy(a)` głęboka.
> - Dzielenie: `7 / 2 = 3.5`, `7 // 2 = 3`.
> - Porównywanie floatów: `abs(a - b) < 1e-9` (albo `math.isclose`), nie `a == b`.

## C++ — co musisz umieć powiedzieć

```cpp
#include <cmath>
#include <memory>
#include <vector>
#include <string>
#include <iostream>

struct Pose { double x{}, y{}, z{}; };

// przekazanie przez const& — bez kopiowania, bez modyfikacji
double norm(const Pose& p) { return std::sqrt(p.x*p.x + p.y*p.y + p.z*p.z); }

class Sensor {                                  // klasa bazowa z polimorfizmem
public:
  virtual ~Sensor() = default;                  // wirtualny destruktor w klasie bazowej!
  virtual std::string name() const = 0;         // czysto wirtualna → klasa abstrakcyjna
};
class Camera : public Sensor {
public:
  std::string name() const override { return "camera"; }
};

int main() {
  std::vector<Pose> path{{0,0,0}, {1,0,0}};      // dynamiczna tablica
  path.push_back({1,1,0});
  for (const auto& p : path) std::cout << norm(p) << "\n";

  auto cam = std::make_unique<Camera>();        // jedyny właściciel, sam się usunie
  std::shared_ptr<Sensor> s = std::make_shared<Camera>();   // współdzielone (licznik)
  std::cout << cam->name() << " " << s->name() << "\n";
}   // tu wszystko zwolnione automatycznie (RAII)
```

| Pojęcie C++ | W jednym zdaniu |
|---|---|
| **Wskaźnik `T*`** | zmienna przechowująca **adres**; może być `nullptr`, można go przestawić |
| **Referencja `T&`** | **alias** istniejącego obiektu; nie może być pusta ani przestawiona |
| **Stos (stack) vs sterta (heap)** | stos: zmienne lokalne, automatycznie zwalniane; sterta: `new`/`make_unique`, żyje dłużej |
| **RAII** | zasób (pamięć, plik, mutex) zwalniany w **destruktorze** obiektu → nie da się zapomnieć |
| **`unique_ptr`** | jedyny właściciel; można przenieść (funkcją std&#58;&#58;move), nie skopiować |
| **`shared_ptr`** | wielu właścicieli, licznik referencji; ROS 2: typy Node&#58;&#58;SharedPtr, msg&#58;&#58;SharedPtr |
| **`const`** | „nie zmienię”; `const T&` — standard przy przekazywaniu dużych obiektów |
| **`virtual` / `override`** | polimorfizm: wywołanie metody klasy pochodnej przez wskaźnik do bazowej |
| **Wyciek pamięci** | `new` bez `delete` → używaj smart pointerów |
| **Undefined behavior** | np. odczyt poza tablicą, użycie zwolnionej pamięci, niezainicjalizowana zmienna — program może zrobić cokolwiek |
| **Kompilacja** | `g++ -O2 -std=c++17 main.cpp -o main`; w projektach **CMake** (`CMakeLists.txt`), w ROS 2 przez `ament_cmake` + colcon |
| **Header `.hpp` vs source `.cpp`** | deklaracje vs definicje |
| **Szablony (templates)** | kod generyczny, np. vector&lt;T&gt; z biblioteki std |

## Złożoność (Big-O) w minimum
| | Złożoność |
|---|---|
| dostęp do listy/wektora po indeksie | O(1) |
| szukanie w nieposortowanej liście | O(n) |
| `x in set` / `dict[key]` / `unordered_map` | O(1) średnio |
| sortowanie | O(n log n) |
| wyszukiwanie binarne w posortowanej | O(log n) |
| dwie zagnieżdżone pętle po n | O(n²) |

## Pytania rekrutacyjne
> [!zadanie] Czym się różni Python od C++? Kiedy którego użyjesz w robotyce?
> > [!odpowiedz]- Odpowiedź
> > Python: interpretowany, dynamicznie typowany, automatyczna pamięć, szybki w pisaniu — skrypty, narzędzia, analiza danych, ML, prototypy węzłów. C++: kompilowany, statycznie typowany, ręczna/RAII pamięć, szybki i przewidywalny — sterowniki, kontrolery czasu rzeczywistego, przetwarzanie chmur punktów.
> > **EN:** *"Python is interpreted and dynamically typed — great for scripts, tooling, data processing and ML. C++ is compiled and statically typed with deterministic memory management — I'd use it for drivers, real-time controllers and performance-critical perception."*

> [!zadanie] Wskaźnik vs referencja?
> > [!odpowiedz]- Odpowiedź
> > Wskaźnik przechowuje adres, może być nullptr i można go przestawić na inny obiekt; wymaga dereferencji `*`/`->`. Referencja jest aliasem istniejącego obiektu, musi być zainicjalizowana, nie może być pusta ani przestawiona.

> [!zadanie] Co to jest RAII i smart pointery?
> > [!odpowiedz]- Odpowiedź
> > Resource Acquisition Is Initialization — zasób jest pozyskiwany w konstruktorze i zwalniany w destruktorze, więc zwalnia się automatycznie przy wyjściu z zakresu (także przy wyjątku). Smart pointery to RAII dla pamięci: `unique_ptr` (jeden właściciel), `shared_ptr` (licznik referencji), `weak_ptr` (bez własności, przerywa cykle).

> [!zadanie] Jak w NumPy przekształcić 1000 punktów macierzą jednorodną bez pętli?
> > [!odpowiedz]- Odpowiedź
> > `pts_base = (R @ pts.T).T + t` dla `pts` o kształcie (N, 3), albo dopisać kolumnę jedynek i `(T @ pts_h.T).T`.

> [!zadanie] Co to jest GIL?
> > [!odpowiedz]- Odpowiedź
> > Global Interpreter Lock w CPythonie — tylko jeden wątek wykonuje bytecode naraz, więc wątki nie przyspieszają obliczeń CPU; do tego `multiprocessing` lub biblioteki w C (NumPy zwalnia GIL). Wątki dalej pomagają przy I/O. (Python 3.13+ ma eksperymentalny tryb bez GIL.)

## Fiszki #fiszki/rozmowa-gd/programowanie
Wskaźnik vs referencja w C++?::Wskaźnik — adres, może być nullptr i przestawiony; referencja — alias, zawsze zainicjalizowana, nieprzestawialna.
Co to jest RAII?::Zasób pozyskany w konstruktorze, zwolniony w destruktorze — automatyczne sprzątanie przy wyjściu z zakresu.
unique_ptr vs shared_ptr?::unique_ptr — jeden właściciel (tylko move); shared_ptr — wielu właścicieli, licznik referencji.
Dlaczego wirtualny destruktor w klasie bazowej?::Żeby delete przez wskaźnik do bazy wywołał destruktor klasy pochodnej.
Po co przekazywać przez const&?::Bez kopiowania dużych obiektów i bez możliwości ich zmiany.
Co to jest GIL?::Global Interpreter Lock — jeden wątek Pythona wykonuje bytecode naraz.
Pułapka mutowalnego argumentu domyślnego?::def f(x, acc=[]) — ta sama lista współdzielona między wywołaniami; użyj None.
is vs == w Pythonie?::is — ten sam obiekt; == — równa wartość.
Operator mnożenia macierzy w NumPy?::@ (np. T @ p).
Złożoność dict lookup / x in set?::O(1) średnio.
Złożoność sortowania?::O(n log n).
Jak budować projekt C++ w ROS 2?::CMakeLists.txt + ament_cmake, budowa przez colcon build.
Jak bezpiecznie porównać floaty?::abs(a − b) < eps (math.isclose), nie ==.
Jak utworzyć izolowane środowisko Pythona?::python3 -m venv .venv && source .venv/bin/activate

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.
