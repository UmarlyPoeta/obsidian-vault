---
typ: pojęcie
status: nowe
przedmioty: ["[[AGH Optymalizacja]]"]
wymaga: ["[[Pochodna]]"]
powiązane: ["[[Metoda złotego podziału]]", "[[Metoda najszybszego spadku]]", "[[Mnożniki Lagrange'a]]"]
tags: [pojęcie, optymalizacja]
utworzono: 2026-10-01
źródła: ["Kusiak, Danielewska-Tułecka, Oprocha — Optymalizacja. Wybrane metody z przykładami zastosowań (PWN 2009), rozdz. 1–2", "Stachurski, Wierzbicki — Podstawy optymalizacji (OW PW 2001)"]
---
# Funkcja celu i warunki optymalności

> [!skrot] W 30 sekund
> - **Co to:** zadanie optymalizacji = znajdź $x^*$, które minimalizuje funkcję celu $f(x)$, przy ewentualnych ograniczeniach.
> - **Klucz:** minimum bez ograniczeń: $\nabla f(x^*) = 0$ (warunek konieczny) i hesjan $H(x^*)$ dodatnio określony (wystarczający).
> - **Kiedy:** zawsze na początku. Najpierw sformułuj zadanie (zmienne, cel, ograniczenia), dopiero potem wybierz metodę.

> [!intuicja] Po ludzku
> Stoisz w górach we mgle i szukasz najniższego punktu doliny. Funkcja celu to wysokość terenu, zmienne decyzyjne to twoja pozycja, a ograniczenia to płot, za który nie wolno wyjść. Na dnie doliny teren jest płaski (gradient = 0) i wszędzie dookoła idzie w górę (hesjan dodatnio określony). Płasko jest też na szczycie i na przełęczy, dlatego sam gradient nie wystarcza.

## Definicja i wzory
> [!definicja] Zadanie optymalizacji
> $$\min_{x \in \mathbb{R}^n} f(x) \quad \text{przy} \quad g_i(x) \le 0,\; h_j(x) = 0$$
> - $x = (x_1,\dots,x_n)$: **zmienne decyzyjne** (to, czym sterujesz).
> - $f$: **funkcja celu** (koszt, błąd, czas). Maksymalizację zamieniasz na minimalizację: $\max f = -\min(-f)$.
> - $g_i, h_j$: **ograniczenia** nierównościowe i równościowe. Punkty, które je spełniają, tworzą **zbiór dopuszczalny**.

> [!definicja] Minimum lokalne i globalne
> - **Globalne:** $f(x^*) \le f(x)$ dla każdego dopuszczalnego $x$.
> - **Lokalne:** $f(x^*) \le f(x)$ tylko w pewnym otoczeniu $x^*$.
> - Funkcja **unimodalna** na $[a,b]$ ma tam dokładnie jedno minimum: maleje do niego, potem rośnie. Na tym założeniu działają metody 1D (złoty podział, Fibonacci).
> - Funkcja **wypukła** ma każde minimum lokalne globalnym. To najwygodniejszy przypadek.

> [!wzor] Warunki optymalności (bez ograniczeń, $f$ gładka)
> **Konieczny (1. rzędu):** $\nabla f(x^*) = 0$ (punkt stacjonarny).
> **Wystarczający (2. rzędu):** $\nabla f(x^*) = 0$ i hesjan $H(x^*) = \left[\frac{\partial^2 f}{\partial x_i \partial x_j}\right]$ dodatnio określony, wtedy jest to minimum lokalne.
> - $H \succ 0$ (dodatnio określony): minimum. $H \prec 0$: maksimum. Wartości własne różnych znaków: **punkt siodłowy**.
> - Kryterium Sylvestera dla $2\times2$: $H=\begin{bmatrix}a & b\\ b & c\end{bmatrix} \succ 0 \iff a > 0 \text{ i } ac-b^2 > 0$.

## Jak to zrobić — krok po kroku
1. **Sformułuj:** co jest zmienną, co minimalizujesz, jakie są ograniczenia (zakresy, równania bilansu).
2. **Policz gradient** i przyrównaj do zera, żeby dostać punkty stacjonarne.
3. **Policz hesjan** w każdym punkcie stacjonarnym i sprawdź jego określoność (Sylvester albo wartości własne).
4. Gdy $\nabla f = 0$ nie da się rozwiązać analitycznie (typowe w praktyce), sięgasz po **metody numeryczne**: 1D [[Metoda złotego podziału|złoty podział]], wielowymiarowe bezgradientowe (Hooke–Jeeves, Nelder–Mead), gradientowe ([[Metoda najszybszego spadku|najszybszy spadek]], Newton).
5. Z ograniczeniami: [[Mnożniki Lagrange'a]] albo funkcje kary.

## Przykład
> [!przyklad] $f(x,y) = x^2 + xy + y^2 - 3x$
> **Gradient:** $\nabla f = (2x + y - 3,\; x + 2y)$. Przyrównujemy do zera:
> $$\begin{cases} 2x + y = 3 \\ x + 2y = 0 \end{cases} \Rightarrow x = 2,\; y = -1$$
> **Hesjan:** $H = \begin{bmatrix}2 & 1\\ 1 & 2\end{bmatrix}$, więc $a = 2 > 0$ i $\det H = 4 - 1 = 3 > 0$, czyli $H \succ 0$.
> **Wniosek:** minimum w $(2,-1)$, $f(2,-1) = 4 - 2 + 1 - 6 = -3$. Hesjan jest stały i dodatnio określony, więc $f$ jest wypukła i to minimum jest **globalne**.

> [!przyklad]- Przykład 2: punkt siodłowy $f(x,y) = x^2 - y^2$
> $\nabla f = (2x, -2y) = 0$ tylko w $(0,0)$. Hesjan $\begin{bmatrix}2 & 0\\ 0 & -2\end{bmatrix}$ ma wartości własne $2$ i $-2$ (różne znaki), więc to **siodło**: wzdłuż osi $x$ widać minimum, wzdłuż $y$ maksimum. Gradient zerowy, a minimum nie ma.

> [!przyklad]- Przykład 3: sformułowanie zadania inżynierskiego
> Puszka o objętości $V = 0{,}5\,\text{l}$ ma zużyć jak najmniej blachy. Zmienne: promień $r$ i wysokość $h$. Cel: $f = 2\pi r^2 + 2\pi r h$ (pole powierzchni). Ograniczenie: $\pi r^2 h = V$, $r, h > 0$. Po wyrugowaniu $h = V/(\pi r^2)$ dostajesz zadanie 1D: $\min_r\; 2\pi r^2 + 2V/r$. Z warunku $f'(r) = 4\pi r - 2V/r^2 = 0$ wychodzi $r = \sqrt[3]{V/(2\pi)}$ i $h = 2r$ (wysokość równa średnicy).

## Typowe błędy
> [!blad] „Gradient zero, czyli minimum”
> To tylko warunek **konieczny**. Ten sam warunek spełniają maksimum i siodło, więc zawsze sprawdź hesjan albo zachowanie funkcji wokół punktu.

> [!blad] Minimum lokalne wzięte za globalne
> Metody numeryczne (gradientowe, Hooke–Jeeves) znajdują minimum **najbliższe punktowi startowemu**. Przy funkcji wielomodalnej odpal je z kilku punktów startowych albo użyj metod niedeterministycznych (algorytm genetyczny, PSO, wyżarzanie).

> [!blad] Zapomniane ograniczenia
> Optimum może leżeć **na brzegu** zbioru dopuszczalnego, gdzie $\nabla f \neq 0$. Przy ograniczeniach $\nabla f = 0$ nie jest warunkiem koniecznym.

## Na zajęcia i egzamin
> [!zajecia]
> - **Projekt:** pierwsze, co oceni prowadząca, to **poprawne sformułowanie zadania**: zmienne, funkcja celu, ograniczenia, dlaczego taka metoda. Zapisz to wprost w sprawozdaniu.
> - **Egzamin:** „podaj warunki konieczne i wystarczające istnienia minimum”, „zbadaj punkt stacjonarny funkcji dwóch zmiennych”, „co to jest funkcja unimodalna i po co jest metodom 1D”.
> - Łączy się z: [[Metoda złotego podziału]] (1D, wymaga unimodalności), [[Metoda najszybszego spadku]], [[Mnożniki Lagrange'a]].

## Zadania
> [!zadanie] Zadanie 1
> Znajdź i sklasyfikuj punkty stacjonarne $f(x,y) = x^2 + 4y^2 - 2x + 8y$.
> > [!odpowiedz]- Odpowiedź
> > $\nabla f = (2x - 2,\; 8y + 8) = 0 \Rightarrow (1,-1)$. Hesjan $\operatorname{diag}(2, 8) \succ 0$, więc minimum globalne, $f(1,-1) = 1 + 4 - 2 - 8 = -5$.

> [!zadanie] Zadanie 2
> Czy $f(x) = x^3$ ma minimum w $x = 0$?
> > [!odpowiedz]- Odpowiedź
> > Nie. $f'(0) = 0$ i $f''(0) = 0$ (test nierozstrzygający), ale $f$ rośnie po obu stronach zera: to **punkt przegięcia**, a nie ekstremum.

> [!zadanie] Zadanie 3
> Zamień $\max\,(10x - x^2)$ na zadanie minimalizacji i rozwiąż.
> > [!odpowiedz]- Odpowiedź
> > $\min\,(x^2 - 10x)$: $2x - 10 = 0 \Rightarrow x = 5$, druga pochodna $2 > 0$. Maksimum pierwotnej funkcji wynosi $25$.

> [!zadanie] Zadanie 4
> Hesjan w punkcie stacjonarnym to $\begin{bmatrix}1 & 3\\ 3 & 1\end{bmatrix}$. Co to za punkt?
> > [!odpowiedz]- Odpowiedź
> > $\det = 1 - 9 = -8 < 0$, więc wartości własne mają różne znaki ($4$ i $-2$): **punkt siodłowy**.

## Fiszki #fiszki/optymalizacja
Z czego składa się zadanie optymalizacji?::Zmienne decyzyjne, funkcja celu do minimalizacji i ograniczenia (zbiór dopuszczalny)
Jak zamienić maksymalizację na minimalizację?::max f = −min(−f)
Warunek konieczny minimum funkcji gładkiej bez ograniczeń::Gradient równy zero (punkt stacjonarny)
Warunek wystarczający minimum lokalnego::Gradient zero i hesjan dodatnio określony
Hesjan o wartościach własnych różnych znaków oznacza…::Punkt siodłowy
Kryterium Sylvestera dla hesjanu 2×2 (a b; b c)::Dodatnio określony ⇔ a > 0 i ac − b² > 0
Co to jest funkcja unimodalna?::Funkcja z jednym minimum na przedziale: maleje do niego, potem rośnie
Dlaczego funkcja wypukła jest wygodna?::Każde jej minimum lokalne jest globalne
Dlaczego metoda gradientowa może dać zły wynik?::Znajduje minimum lokalne najbliższe startowi, a nie globalne
Czy przy ograniczeniach optimum musi mieć gradient zero?::Nie, może leżeć na brzegu zbioru dopuszczalnego

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.

## Źródła
- J. Kusiak, A. Danielewska-Tułecka, P. Oprocha, *Optymalizacja. Wybrane metody z przykładami zastosowań*, PWN 2009 (podręcznik prowadzącego wykład).
- A. Stachurski, A. Wierzbicki, *Podstawy optymalizacji*, OW PW 2001.
