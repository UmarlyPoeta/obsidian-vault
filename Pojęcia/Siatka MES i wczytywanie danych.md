---
typ: pojęcie
status: nowe
przedmioty: ["[[AGH Metoda elementów skończonych]]"]
wymaga: ["[[Funkcje kształtu]]"]
powiązane: ["[[MES — kurs dr. Kustry (mapa labów)]]", "[[Macierz sztywności]]", "[[Jakobian]]", "[[Nieustalony przepływ ciepła]]"]
tags: [pojęcie, mes, kod]
utworzono: 2026-10-05
źródła: ["P. Kustra — MES zajęcia 1, praca domowa (AGH WIMiIP)", "Siatki testowe Test1_4_4, Test2_4_4_MixGrid, Test3_31_31_kwadrat (P. Kustra)", "cppreference — std::ifstream, std::istringstream"]
---
# Siatka MES i wczytywanie danych

> [!skrot] W 30 sekund
> - **Co to:** pierwszy klocek programu: struktury `GlobalData`, `node`, `element`, `grid` i odczyt pliku siatki prowadzącego.
> - **Klucz:** element trzyma **numery** swoich 4 węzłów (`ID = {1, 2, 6, 5}`), a nie ich współrzędne. Współrzędne są tylko w tablicy węzłów.
> - **Kiedy:** praca domowa po labie 1. Efekt: wypisanie w konsoli współrzędnych węzłów i ID węzłów każdego elementu.

> [!intuicja] Po ludzku
> Siatka to książka adresowa. Węzły to adresy (współrzędne $x, y$). Element to lista czterech numerów domów, które razem tworzą jedną działkę. Kiedy element potrzebuje współrzędnych, sprawdza je w książce pod swoimi numerami. Dzięki temu węzeł wspólny dla czterech elementów istnieje tylko raz, a jego temperatura jest jedną niewiadomą.

## Definicja i wzory
> [!definicja] Struktury ze slajdu „Praca domowa”
> ```
> struct GlobalData            struct node    { x, y; }
> {                            struct element { ID[4]; }   // np. {1, 2, 6, 5}
>   SimulationTime             struct grid
>   SimulationStepTime         {
>   Conductivity                 nN – liczba węzłów
>   Alfa                         nE – liczba elementów
>   Tot                          element[nE]
>   InitialTemp                  node[nN]
>   Density                    }
>   SpecificHeat
>   nN, nE
> }
> ```
> Do `node` warto od razu dodać flagę `BC` (węzeł ma kontakt z otoczeniem), a do `element` później macierze `H`, `Hbc`, `C` i wektor `P`.

> [!definicja] Format pliku siatki
> ```
> SimulationTime 500          ← czas symulacji [s]
> SimulationStepTime 50       ← krok czasowy Δτ [s]
> Conductivity 25             ← k [W/(m·K)]
> Alfa 300                    ← α [W/(m²·K)]
> Tot 1200                    ← temperatura otoczenia [°C]
> InitialTemp 100             ← temperatura początkowa [°C]
> Density 7800                ← ρ [kg/m³]
> SpecificHeat 700            ← c [J/(kg·K)]
> Nodes number 16             ← w niektórych wersjach: Nodes_number 16
> Elements number 9
> *Node
>       1,  0.100000001, 0.00499999989
>       ...
> *Element, type=DC2D4
>  1,  1,  2,  6,  5          ← nr elementu, potem 4 węzły przeciwnie do wskazówek zegara
>  ...
> *BC
> 1, 2, 3, 4, 5, 8, 9, 12, 13, 14, 15, 16   ← węzły z warunkiem brzegowym
> ```
> `*Node`, `*Element` i `*BC` to format Abaqusa (`DC2D4` = 4-węzłowy element dyfuzji ciepła 2D). Dlatego na jednym z labów będzie generowanie siatek w ABAQUS.

> [!wzor] Numeracja w siatce prostokątnej (pomocne przy własnych siatkach)
> Dla $n_B$ węzłów w poziomie i $n_H$ w pionie: $n_N = n_B n_H$, $n_E = (n_B - 1)(n_H - 1)$. Element w wierszu $r$ i kolumnie $s$ (od 0) ma w Test1 węzły
> $$ID = \{\,a,\ a+1,\ a+1+n_B,\ a+n_B\,\}, \qquad a = r\,n_B + s + 1.$$
> Sprawdzenie: element 1 to $a = 1$, czyli $\{1, 2, 6, 5\}$, a element 9 to $a = 11$, czyli $\{11, 12, 16, 15\}$.

## Jak to zrobić — krok po kroku
1. Czytaj plik linia po linii. W nagłówku pierwsze słowo to klucz, ostatnie to wartość.
2. Po `*Node` czytasz $n_N$ linii: zamieniasz przecinki na spacje i czytasz `id x y`.
3. Po `*Element` czytasz $n_E$ linii: `id n1 n2 n3 n4`.
4. Po `*BC` czytasz jedną linię numerów i ustawiasz `node[n-1].BC = true`.
5. Wypisz wszystko na ekran i porównaj z plikiem *Wyniki dla pliku Test1_4_4* (węzły i `ID[1,2,6,5]` … `ID[11,12,16,15]`).

## Przykład
> [!przyklad] Kod w C++ (sprawdzony na wszystkich trzech siatkach, g++ 13, `-std=c++17 -Wall`)
> Pełny plik: `Załączniki/MES/kod/siatka.cpp`. Kompilacja i uruchomienie:
> `g++ -std=c++17 -Wall -O2 siatka.cpp -o siatka && ./siatka "Test1_4_4.txt"`

```cpp
struct Node    { double x, y; bool bc = false; };
struct Element { int id[4]; };                 // numery węzłów od 1, jak w pliku
struct GlobalData {
    double simulationTime, simulationStepTime, conductivity, alfa,
           tot, initialTemp, density, specificHeat;
    int nN, nE;
};
struct Grid { int nN, nE; std::vector<Node> nodes; std::vector<Element> elements; };

static std::string zamienPrzecinki(std::string s) {
    for (char& c : s) if (c == ',') c = ' ';
    return s;
}

bool wczytaj(const std::string& sciezka, GlobalData& gd, Grid& grid) {
    std::ifstream f(sciezka);
    if (!f) return false;
    std::string linia, klucz;
    while (std::getline(f, linia)) {
        if (linia.empty()) continue;
        std::istringstream ss(linia);
        ss >> klucz;
        if      (klucz == "SimulationTime")     ss >> gd.simulationTime;
        else if (klucz == "SimulationStepTime") ss >> gd.simulationStepTime;
        else if (klucz == "Conductivity")       ss >> gd.conductivity;
        else if (klucz == "Alfa")               ss >> gd.alfa;
        else if (klucz == "Tot")                ss >> gd.tot;
        else if (klucz == "InitialTemp")        ss >> gd.initialTemp;
        else if (klucz == "Density")            ss >> gd.density;
        else if (klucz == "SpecificHeat")       ss >> gd.specificHeat;
        else if (klucz == "Nodes")   { std::string n; ss >> n >> gd.nN; }   // "Nodes number 16"
        else if (klucz == "Elements"){ std::string n; ss >> n >> gd.nE; }
        else if (klucz == "Nodes_number")       ss >> gd.nN;                // wariant z podkreślnikiem
        else if (klucz == "Elements_number")    ss >> gd.nE;
        else if (klucz == "*Node") {
            grid.nodes.resize(gd.nN);
            for (int i = 0; i < gd.nN && std::getline(f, linia); ++i) {
                std::istringstream w(zamienPrzecinki(linia));
                int id; w >> id >> grid.nodes[i].x >> grid.nodes[i].y;
            }
        } else if (klucz.rfind("*Element", 0) == 0) {
            grid.elements.resize(gd.nE);
            for (int i = 0; i < gd.nE && std::getline(f, linia); ++i) {
                std::istringstream w(zamienPrzecinki(linia));
                int id; w >> id;
                for (int& n : grid.elements[i].id) w >> n;
            }
        } else if (klucz == "*BC") {
            std::getline(f, linia);
            std::istringstream w(zamienPrzecinki(linia));
            for (int n; w >> n;) grid.nodes[n - 1].bc = true;
        }
    }
    grid.nN = gd.nN; grid.nE = gd.nE;
    return true;
}
```

Wynik dla Test1 (początek i koniec):
```
nN = 16, nE = 9, k = 25, alfa = 300, tot = 1200
wezel 1: x = 0.1  y = 0.005  BC = 1
wezel 6: x = 0.0666667  y = -0.0283333  BC = 0
element 1: ID[1,2,6,5]
element 9: ID[11,12,16,15]
```

> [!przyklad]- To samo w Pythonie (gdyby prowadzący dopuścił inny język)
> ```python
> def wczytaj(sciezka):
>     gd, wezly, elementy, bc, sekcja = {}, [], [], set(), None
>     for s in (l.strip() for l in open(sciezka)):
>         if not s: continue
>         if s.startswith("*"):
>             sekcja = s.split(",")[0]; continue
>         if sekcja is None:
>             *klucz, w = s.split(); gd["_".join(klucz)] = float(w)
>         elif sekcja == "*Node":
>             _, x, y = map(float, s.split(",")); wezly.append((x, y))
>         elif sekcja == "*Element":
>             elementy.append([int(v) - 1 for v in s.split(",")[1:]])
>         elif sekcja == "*BC":
>             bc |= {int(v) - 1 for v in s.split(",")}
>     return gd, wezly, elementy, bc
> ```
> Ta wersja od razu przechodzi na numerację od 0. Pełny referencyjny program jest w `Załączniki/MES/kod/mes2d.py`.

## Typowe błędy
> [!blad] Element trzyma kopie węzłów zamiast numerów
> Jeśli w `element` zapiszesz cztery obiekty `node`, to węzeł wspólny dla kilku elementów będzie istniał w kilku kopiach. Przy agregacji nie będziesz wiedzieć, gdzie wpisać wynik. Trzymaj numery.

> [!blad] Przecinki w liniach węzłów i elementów
> `1,  0.100000001, 0.00499999989` nie wczyta się samym `>>` w C++. Zamień przecinki na spacje albo czytaj z `getline(..., ',')`.

> [!blad] Różne nagłówki w wersjach plików
> W jednych plikach jest `Nodes number 16`, w innych `Nodes_number 16`. Parser powinien obsłużyć oba warianty (jak wyżej) albo zawsze brać **ostatnie** słowo linii jako wartość.

> [!blad] Brak flagi BC
> Linia `*BC` wydaje się zbędna na labie 1, ale bez niej na labie 7 nie policzysz $H_{bc}$ i $P$. Wczytaj ją od razu.

## Na zajęcia i egzamin
> [!zajecia]
> - **Praca domowa po labie 1:** program wczytuje Test1_4_4 i wypisuje węzły oraz ID elementów. Przetestuj też Test2 (inne współrzędne, ta sama topologia) i Test3 (961 węzłów, czy nic się nie wysypuje).
> - **Kolokwium ustne:** „dlaczego element trzyma numery węzłów”, „jak znajdujesz węzły z warunkiem brzegowym”, „co znaczy DC2D4”.
> - Łączy się z: [[Macierz sztywności]] (agregacja korzysta z `ID`), [[Jakobian]] (współrzędne węzłów elementu).

## Zadania
> [!zadanie] Zadanie 1
> Siatka ma 5 węzłów w poziomie i 4 w pionie, numerowanych jak w Test1. Ile ma węzłów i elementów? Jakie ID ma element w drugim wierszu i trzeciej kolumnie?
> > [!odpowiedz]- Odpowiedź
> > $n_N = 20$, $n_E = 4 \cdot 3 = 12$. Wiersz $r = 1$, kolumna $s = 2$, więc $a = 1 \cdot 5 + 2 + 1 = 8$ i $ID = \{8, 9, 14, 13\}$.

> [!zadanie] Zadanie 2
> Które węzły Test1_4_4 **nie** mają warunku brzegowego?
> > [!odpowiedz]- Odpowiedź
> > 6, 7, 10 i 11, czyli 4 węzły wewnętrzne. Pozostałe 12 leży na obwodzie kwadratu i jest w linii `*BC`.

## Fiszki #fiszki/mes
Co przechowuje struktura element w programie MES?::Numery (ID) swoich 4 węzłów, nie ich współrzędne
Pola GlobalData w kursie Kustry::SimulationTime, SimulationStepTime, Conductivity, Alfa, Tot, InitialTemp, Density, SpecificHeat, nN, nE
Co oznacza sekcja *BC w pliku siatki?::Listę węzłów z warunkiem brzegowym (kontakt z otoczeniem, konwekcja)
Co znaczy typ elementu DC2D4?::Element Abaqusa: 4-węzłowy element dyfuzji ciepła w 2D
Liczba węzłów i elementów siatki n_B × n_H węzłów::n_N = n_B·n_H, n_E = (n_B − 1)(n_H − 1)

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.

## Źródła
- P. Kustra, *MES wstęp*, slajd „Praca domowa” (AGH WIMiIP).
- Siatki testowe prowadzącego: [[Test1_4_4.txt]], [[Test2_4_4_MixGrid.txt]], [[Test3_31_31_kwadrat.txt]].
- Wynik wzorcowy: `Załączniki/MES/Kustra — wyniki testowe/Test1_4_4 — jakobian, H, Hbc, P lokalne.txt` (pierwsze 40 linii to węzły i ID elementów).
- cppreference.com: `std::ifstream`, `std::getline`, `std::istringstream`.
