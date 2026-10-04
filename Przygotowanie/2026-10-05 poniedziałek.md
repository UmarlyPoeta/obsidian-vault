---
typ: przygotowanie
data: 2026-10-05
tags: [przygotowanie]
---
# Przygotowanie — poniedziałek 05.10

> [!skrot] Plan dnia
> - **Metoda elementów skończonych — lab 1/14** · dr inż. Piotr Kustra (B5, pok. 710) · [[AGH Metoda elementów skończonych]]
> - Godzinę i salę sprawdź w planie zajęć (nie mam dostępu do twojego planu z `~/studia`).

> [!wazne] Przed wyjściem (15 min)
> 1. **Wydrukuj „Wstęp”**: [[01 Wstęp — ustalony przepływ ciepła (do druku).pdf]]. Prowadzący prosi o to w samej nazwie pliku („proszę wydrukować, będzie łatwo robić notatki”). To 10 stron.
> 2. Przejrzyj zasady zaliczenia: [[00 Zasady zaliczenia 2025-26.pdf]] (1 strona, streszczenie niżej).
> 3. Laptop z działającym kompilatorem C++ (masz g++ 13.3 w WSL). Na kolejnych zajęciach będziesz pokazywać kod.

## Lab 1/14 — MES wstęp
[[MES — kurs dr. Kustry (mapa labów)]] — cały plan kursu, pliki testowe i wyniki do porównania.

> [!zajecia] Co będzie (z materiałów prowadzącego na lab 1)
> - **Analiza problemu:** płytka 300 °C w otoczeniu 20 °C z warunkiem $q = \alpha(t - t_{ot})$. Różnica między stanem **nieustalonym** (niestacjonarnym) a **ustalonym** (stacjonarnym).
> - **Temperatura w punkcie** między dwoma węzłami: funkcje kształtu $N_1 = \frac{x_2 - x}{x_2 - x_1}$, $N_2 = \frac{x - x_1}{x_2 - x_1}$, przykład $T(1{,}55) = 38{,}92$ °C.
> - **Interpolacja w 2D:** trójkąt (stosunki pól), element 4-węzłowy, element 8-węzłowy, zapis $T_P = \{N\}^T\{T\}$.
> - **Zasady kursu:** własny program MES do nieustalonego przepływu ciepła, kolokwium ustne na 3 ostatnich zajęciach, sprawozdanie na UPEL.
> - **Praca domowa:** struktury `GlobalData`, `node`, `element`, `grid`, odczyt pliku siatki i wypisanie węzłów oraz ID elementów.

> [!definicja]- Zasady zaliczenia w skrócie
> - Program piszesz **samodzielnie**. Kod i sprawozdanie idą na UPEL.
> - Sprawozdanie potwierdzające działanie programu = **3.0**. Problem rzeczywisty (np. ściana wielowarstwowa, hartowanie) = **3.5–5.0**.
> - Mogą być **kolokwia na UPEL albo niezapowiedziane kartkówki**. Średnia musi wynieść **≥ 50%**.
> - Ocena z labów = średnia z kolokwiów, sprawozdania i odpowiedzi ustnej. Przedmiot = 0,4·laby + 0,6·egzamin.
> - Dwa terminy poprawkowe.

### Minimum teorii (ok. 40 min)
1. [[Funkcje kształtu]] — własności ($N_i(x_j) = \delta_{ij}$, $\sum N_i = 1$), wzory 1D i 2D, przykład z 38,92 °C. **To jest najpewniejszy temat ewentualnej kartkówki.**
2. [[Metoda Galerkina]] — funkcjonał, skąd $[H]\{t\} + \{P\} = 0$, pręt na dwóch elementach. To treść „Wstępu”, który drukujesz.
3. [[Siatka MES i wczytywanie danych]] — struktury i format pliku. Gotowy szkielet C++ jest w notatce i w `Załączniki/MES/kod/siatka.cpp`.

Starsze notatki, które się przydadzą: [[Interpolacja]] (bazy Lagrange'a = funkcje kształtu) i [[MES 1D — od różnic skończonych do macierzy sztywności]] (pręt 1D; uwaga, tam $q > 0$ to ciepło wpływające, a u Kustry odprowadzane).

### Co przygotować
- **Na zajęcia:** wydrukowany „Wstęp”, długopis, laptop.
- **Po zajęciach (praca domowa, ok. 1 h):** napisz sam wczytywanie [[Test1_4_4.txt]] i wypisz węzły i elementy. Porównaj z pierwszymi 40 liniami pliku *Test1_4_4 — jakobian, H, Hbc, P lokalne.txt* w `Załączniki/MES/Kustra — wyniki testowe/`. Dopiero potem zajrzyj do `siatka.cpp`.
- **Na lab 2 (całkowanie Gaussa):** zerknij na [[Całkowanie numeryczne i kwadratury Gaussa]]. Praca domowa po labie 2 to całki $\int_{-1}^1 (5x^2 + 3x + 6)\,dx = 15{,}333$ i $\iint (5x^2y^2 + 3xy + 6) = 26{,}222$ schematem 2- i 3-punktowym.

### Pytania kontrolne
> [!zadanie]- 4 pytania na rozgrzewkę
> 1. Węzły $x_1 = -5$ ($T = 20$) i $x_2 = 4$ ($T = 46$). Ile wynosi $T(-0{,}5)$ i dlaczego tak łatwo?
> > [!odpowiedz]- Odpowiedź
> > $-0{,}5$ to środek odcinka, więc $N_1 = N_2 = 0{,}5$ i $T = 0{,}5 \cdot 20 + 0{,}5 \cdot 46 = 33$ °C.
> 2. Podaj dwie własności funkcji kształtu.
> > [!odpowiedz]- Odpowiedź
> > $N_i = 1$ w swoim węźle i $0$ w pozostałych; $\sum_i N_i = 1$ w każdym punkcie elementu. Dla elementów liniowych $N_i \in [0, 1]$.
> 3. Czym różni się stan ustalony od nieustalonego w zadaniu cieplnym?
> > [!odpowiedz]- Odpowiedź
> > W ustalonym temperatura nie zmienia się w czasie ($\partial t/\partial\tau = 0$), rozwiązujesz raz $[H]\{t\} + \{P\} = 0$. W nieustalonym dochodzi $[C]\,\partial\{t\}/\partial\tau$ i liczysz krok po kroku w czasie.
> 4. Dlaczego element w siatce trzyma numery węzłów, a nie ich współrzędne?
> > [!odpowiedz]- Odpowiedź
> > Węzeł jest wspólny dla kilku elementów i ma jedną temperaturę (jedną niewiadomą). Numer pozwala przy agregacji wpisać wkład każdego elementu w to samo miejsce macierzy globalnej.

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.
