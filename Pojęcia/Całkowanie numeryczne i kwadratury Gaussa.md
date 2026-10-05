---
typ: pojęcie
status: nowe
przedmioty: ["[[AGH Metoda elementów skończonych]]"]
wymaga: ["[[Całka oznaczona]]", "[[Interpolacja]]"]
powiązane: ["[[Jakobian]]", "[[Funkcje kształtu]]", "[[Macierz sztywności]]", "[[Aproksymacja średniokwadratowa]]", "[[Różniczkowanie numeryczne]]", "[[Numeryczne rozwiązywanie RRZ]]", "[[MES 1D — od różnic skończonych do macierzy sztywności]]"]
tags: [pojęcie, metody-numeryczne, mes]
utworzono: 2026-10-02
źródła: ["A. Milenin — Podstawy metody elementów skończonych (Wyd. AGH 2010)", "O.C. Zienkiewicz, R.L. Taylor, J.Z. Zhu — The Finite Element Method: Its Basis and Fundamentals", "Z. Fortuna, B. Macukow, J. Wąsowski — Metody numeryczne (WNT)", "R.L. Burden, J.D. Faires — Numerical Analysis", "Dokumentacja numpy.polynomial.legendre.leggauss i scipy.integrate.quad", "Instrukcja: Lab — kwadratury Gaussa-Legendre'a", "Kurs MN w C++: Kod/metody_numeryczne (moduły, testy, rozwiązania wzorcowe)"]
---
# Całkowanie numeryczne i kwadratury Gaussa

> [!skrot] W 30 sekund
> - **Co to:** zastępujesz całkę ważoną sumą wartości funkcji w wybranych punktach: $\int_a^b f\,dx \approx \sum_i w_i f(x_i)$.
> - **Klucz:** kwadratura Gaussa–Legendre'a z $n$ punktami jest **dokładna dla wielomianów stopnia $2n-1$**. W MES 2D całkujesz po kwadracie $[-1,1]^2$ i mnożysz przez $\det J$.
> - **Kiedy:** każda macierz w MES ($K$, $C$, wektor $F$) to całka po elemencie liczona właśnie tak: punkty Gaussa, wagi, $\det J$.

> [!intuicja] Po ludzku
> Trapezy i Simpson próbkują funkcję w **równych odstępach** i dopasowują do próbek prostą albo parabolę. Gauss jest sprytniejszy: sam wybiera, **gdzie** próbkować, i dzięki temu z dwóch punktów wyciska tyle, ile Simpson z trzech. To trochę jak ankieta: zamiast pytać co dziesiątego przechodnia, pytasz kilka starannie dobranych osób i masz wynik równie dobry przy mniejszym koszcie.

## Definicja i wzory
> [!wzor] Złożone trapezy i Simpson ($h = (b-a)/n$)
> $$T_n = h\left[\tfrac12 f_0 + f_1 + \dots + f_{n-1} + \tfrac12 f_n\right], \qquad E = O(h^2)$$
> $$S_n = \tfrac{h}{3}\left[f_0 + 4(f_1 + f_3 + \dots) + 2(f_2 + f_4 + \dots) + f_n\right], \qquad E = O(h^4),\ n \text{ parzyste}$$
> Simpson jest dokładny aż do sześcianów. **Richardson:** skoro $T_h \approx I + c h^2$, to $\frac{4T_{h/2} - T_h}{3}$ usuwa wyraz $h^2$ i daje dokładnie Simpsona. Powtarzanie tego kroku to **metoda Romberga**.

> [!wzor] Gauss–Legendre na $[-1,1]$
> $$\int_{-1}^{1} f(\xi)\,d\xi \approx \sum_{i=1}^{n} w_i f(\xi_i)$$
>
> | $n$ | węzły $\xi_i$ | wagi $w_i$ | dokładna do stopnia |
> |---|---|---|---|
> | 1 | $0$ | $2$ | 1 |
> | 2 | $\pm 1/\sqrt3 \approx \pm 0{,}5774$ | $1,\ 1$ | 3 |
> | 3 | $0,\ \pm\sqrt{3/5} \approx \pm 0{,}7746$ | $8/9,\ 5/9,\ 5/9$ | 5 |
>
> Węzły to pierwiastki wielomianu Legendre'a $P_n$. Wagi zawsze sumują się do $2$ (długość przedziału).

> [!wzor] Zamiana przedziału $[a,b] \to [-1,1]$
> $$x = \frac{a+b}{2} + \frac{b-a}{2}\,\xi, \qquad dx = \frac{b-a}{2}\,d\xi, \qquad \int_a^b f\,dx \approx \frac{b-a}{2}\sum_i w_i f(x(\xi_i))$$
> Czynnik $\frac{b-a}{2}$ to jednowymiarowy **jakobian**. W elemencie 1D o długości $L_e$ wynosi $L_e/2$.

> [!wzor] Gauss 2D i całka po czworokącie (serce MES 2D)
> Na kwadracie stosujesz iloczyn tensorowy: $\int_{-1}^1\int_{-1}^1 f\,d\xi\,d\eta \approx \sum_i\sum_j w_i w_j f(\xi_i, \eta_j)$, czyli 2×2 = 4 albo 3×3 = 9 punktów.
> Czworokąt o węzłach $(x_k, y_k)$ opisujesz tymi samymi funkcjami kształtu co temperaturę („izo-parametryczny”):
> $$N_k = \tfrac14 (1 + \xi\xi_k)(1 + \eta\eta_k), \qquad x = \sum_k N_k x_k, \quad y = \sum_k N_k y_k$$
> $$J = \begin{bmatrix} \frac{\partial x}{\partial \xi} & \frac{\partial y}{\partial \xi} \\ \frac{\partial x}{\partial \eta} & \frac{\partial y}{\partial \eta} \end{bmatrix}, \qquad \iint_\Omega f\,dx\,dy \approx \sum_i\sum_j w_i w_j\, f(x_{ij}, y_{ij})\,\det J_{ij}$$

## Jak to zrobić — krok po kroku
1. Weź węzły i wagi dla $n$ punktów (do stopnia $2n-1$ wynik jest dokładny).
2. **1D:** przelicz węzły na $x_i$ i pomnóż sumę przez $(b-a)/2$.
3. **2D, element czworokątny:** w każdym punkcie Gaussa $(\xi_i, \eta_j)$ policz $N_k$ oraz $\partial N_k/\partial\xi$ i $\partial N_k/\partial\eta$.
4. Złóż $J$ ze współrzędnych węzłów, policz $\det J$. Jeśli $\det J \le 0$, masz złą numerację węzłów (muszą iść przeciwnie do ruchu wskazówek zegara) albo zdegenerowany element.
5. Policz $x, y$ punktu (potrzebne, gdy $f$ zależy od położenia), dodaj $w_i w_j f\,\det J$.
6. W MES zamiast $f$ podstawiasz np. $k\,(\nabla N)^T \nabla N$, a pochodne po $x, y$ dostajesz z $J^{-1}$, co będzie tematem kolejnego kroku projektu.

## Przykład
> [!przyklad] Wyprowadzenie Gaussa dla $n=2$
> Szukasz $w_1, w_2, \xi_1, \xi_2$ takich, że wzór jest dokładny dla $1, \xi, \xi^2, \xi^3$:
> $$w_1 + w_2 = 2, \quad w_1\xi_1 + w_2\xi_2 = 0, \quad w_1\xi_1^2 + w_2\xi_2^2 = \tfrac23, \quad w_1\xi_1^3 + w_2\xi_2^3 = 0$$
> Z symetrii $\xi_2 = -\xi_1$, $w_1 = w_2 = 1$, więc $2\xi_1^2 = 2/3$ i $\xi_1 = 1/\sqrt3$. Cztery niewiadome dają cztery warunki, stąd stopień $2n - 1 = 3$.
> Sprawdzenie: $\int_0^2 x^3 dx = 4$. Węzły $x = 1 \mp 1/\sqrt3 = 0{,}4226;\ 1{,}5774$, jakobian $1$: $0{,}4226^3 + 1{,}5774^3 = 0{,}0755 + 3{,}9245 = 4$ (dokładnie). Dla $x^4$ wychodzi $6{,}222$ zamiast $6{,}4$, bo stopień 4 jest już za wysoki.

> [!przyklad] Pole i moment czworokąta $(0,0), (4,0), (3,3), (0,2)$, Gauss 2×2
> Z funkcji kształtu: $x = \frac{(1+\xi)(7-\eta)}{4}$, $y = \frac{(1+\eta)(5+\xi)}{4}$, a stąd
> $$\det J = \frac{(7-\eta)(5+\xi) + (1+\xi)(1+\eta)}{16} = \frac{9 + 2\xi - \eta}{4}$$
>
> | $\xi$ | $\eta$ | $(x, y)$ | $\det J$ |
> |---|---|---|---|
> | −0,5774 | −0,5774 | (0,8006; 0,4673) | 2,1057 |
> | −0,5774 | +0,5774 | (0,6786; 1,7440) | 1,8170 |
> | +0,5774 | −0,5774 | (2,9880; 0,5893) | 2,6830 |
> | +0,5774 | +0,5774 | (2,5327; 2,1994) | 2,3943 |
>
> Wszystkie wagi są równe 1. **Pole** $= \sum \det J = 9{,}000$, zgodnie ze wzorem Gaussa na pole wielokąta. **Moment** $\iint x\,dA = \sum x \det J = 17{,}000$, więc środek ciężkości ma $\bar x = 17/9 \approx 1{,}889$. Wynik jest dokładny, bo $x\,\det J$ jest co najwyżej kwadratowy w każdej zmiennej.

```cpp
#include "m06.h"   // Kod/metody_numeryczne, calka_czworokat korzysta z N4 z m03
double x[4] = {0, 4, 3, 0}, y[4] = {0, 0, 3, 2};
double pole   = calka_czworokat([](double, double) { return 1.0; }, x, y, 2);   // 9
double moment = calka_czworokat([](double X, double) { return X; }, x, y, 2);   // 17
double I = gauss_legendre([](double t) { return std::sin(t); }, 0.5, 2.5, 2);  // 1.67163 (lab)
```

> [!kod]- To samo w Pythonie (do szybkiego sprawdzenia)
> ```python
> import numpy as np
> xi, w = np.polynomial.legendre.leggauss(2)
> wsp = np.array([(0, 0), (4, 0), (3, 3), (0, 2)], float)
> pole = moment = 0.0
> for a, wa in zip(xi, w):
>     for b, wb in zip(xi, w):
>         N = 0.25 * np.array([(1-a)*(1-b), (1+a)*(1-b), (1+a)*(1+b), (1-a)*(1+b)])
>         dNa = 0.25 * np.array([-(1-b), 1-b, 1+b, -(1+b)])
>         dNb = 0.25 * np.array([-(1-a), -(1+a), 1+a, 1-a])
>         J = np.array([dNa @ wsp, dNb @ wsp])     # wiersze: d/dξ, d/dη
>         detJ = np.linalg.det(J)
>         pole += wa * wb * detJ
>         moment += wa * wb * (N @ wsp[:, 0]) * detJ
> print(pole, moment)                              # 9.0 17.0
> ```

## Lab: metody geometryczne (prostokąty, trapezy, parabole) — wyniki do sprawdzenia
Instrukcja: [[Lab — całkowanie metodami prostokątów, trapezów i parabol.pdf]]. Oddajesz na zajęciach (10 pkt), na UPEL `.cpp` i sprawozdanie z wykresem.

> [!definicja] Wzory z instrukcji ($s = h = \frac{b-a}{n}$)
> - **Prostokąty:** $\int_a^b f \approx s\sum_{i=0}^{n-1} f\left(x_i + \frac s2\right)$ — wartość w **środku** podprzedziału.
> - **Trapezy:** $\sum_{i=0}^{n-1} \frac{x_{i+1} - x_i}{2}\left(f(x_i) + f(x_{i+1})\right)$.
> - **Parabole:** wzór $\frac{b-a}{6}\left(f(a) + 4f\left(\frac{a+b}{2}\right) + f(b)\right)$ stosujesz na **każdym** z $n$ podprzedziałów. Działa dla dowolnego $n$ i daje to samo, co klasyczny złożony Simpson na $2n$ podziałach (w kodzie: `parabole` obok `simpson`).

> [!przyklad] Zad. 1: $n = 4$
> | całka | dokładnie | prostokąty | trapezy | parabole |
> |---|---|---|---|---|
> | $\int_{0{,}5}^{2{,}5}\sin x$ | 1,6787262 | **1,6963413** | **1,6436061** | 1,6787629 |
> | $\int_{0{,}5}^{5}(x^2 + 2x + 5)$ | 88,875 | 88,4003906 | 89,8242188 | **88,875** (dokładnie) |
> | $\int_{0{,}5}^{5} e^x$ | 146,7644378 | 139,3013721 | 161,9265256 | 146,8430899 |
>
> Pogrubione wyniki dla $\sin$ zgadzają się z instrukcją (1,6963 i 1,6436). Prostokąty kroki: $h_0 = \sin 0{,}75 = 0{,}682$, $h_1 = \sin 1{,}25 = 0{,}949$, $h_2 = \sin 1{,}75 = 0{,}984$, $h_3 = \sin 2{,}25 = 0{,}778$, suma razy $0{,}5$.

> [!przyklad] Zad. 2: zbieżność (błąd $|I_n - I|$ dla $\int_{0{,}5}^{2{,}5}\sin x$)
> | $n$ | prostokąty | trapezy | parabole |
> |---|---|---|---|
> | 1 | $3{,}2\cdot10^{-1}$ | $6{,}0\cdot10^{-1}$ | $1{,}1\cdot10^{-2}$ |
> | 2 | $7{,}2\cdot10^{-2}$ | $1{,}4\cdot10^{-1}$ | $6{,}0\cdot10^{-4}$ |
> | 4 | $1{,}8\cdot10^{-2}$ | $3{,}5\cdot10^{-2}$ | $3{,}7\cdot10^{-5}$ |
> | 8 | $4{,}4\cdot10^{-3}$ | $8{,}8\cdot10^{-3}$ | $2{,}3\cdot10^{-6}$ |
> | 16 | $1{,}1\cdot10^{-3}$ | $2{,}2\cdot10^{-3}$ | $1{,}4\cdot10^{-7}$ |
> | 32 | $2{,}7\cdot10^{-4}$ | $5{,}5\cdot10^{-4}$ | $8{,}9\cdot10^{-9}$ |
> | 64 | $6{,}8\cdot10^{-5}$ | $1{,}4\cdot10^{-4}$ | $5{,}6\cdot10^{-10}$ |
>
> **Komentarz do sprawozdania:**
> - Podwojenie $n$ zmniejsza błąd prostokątów i trapezów **4 razy** (rząd $h^2$), a parabol **16 razy** (rząd $h^4$).
> - Prostokąty mają mniej więcej **połowę** błędu trapezów i **przeciwny znak** (prostokąty zawyżają, trapezy zaniżają dla funkcji wklęsłej jak $\sin$ na tym przedziale). Dlatego $\frac{2P + T}{3}$ to właśnie wzór parabol.
> - Dla wielomianu 2. stopnia parabole są dokładne już przy $n = 1$. Dla $e^x$ na długim przedziale błędy są większe (przy $n = 4$: 7,5; 15,2; 0,079), ale rzędy są te same.
> - Wykres: oś $x$ to $n$, oś $y$ to wynik (albo lepiej $\log$ błędu). Na skali log-log widać proste o nachyleniach −2, −2, −4.

## Lab: kwadratury Gaussa–Legendre'a — co oddajesz i wyniki do sprawdzenia
Instrukcja: [[Lab — kwadratury Gaussa-Legendre'a.pdf]]. Oddajesz na zajęciach (10 pkt), na UPEL `.cpp` i plik z odpowiedziami.

> [!definicja] Tablica z instrukcji i zamiana przedziału
> | $n$ | węzły $x_i$ | wagi $A_i$ |
> |---|---|---|
> | 2 | $\pm\frac{\sqrt3}{3} = \pm0{,}57735$ | 1, 1 |
> | 3 | $\pm\sqrt{3/5} = \pm0{,}774597$; 0 | $\frac59, \frac89, \frac59$ |
> | 4 | $\pm\frac{1}{35}\sqrt{525 - 70\sqrt{30}} = \pm0{,}339981$; $\pm\frac{1}{35}\sqrt{525 + 70\sqrt{30}} = \pm0{,}861136$ | $\frac{18 + \sqrt{30}}{36} = 0{,}652145$; $\frac{18 - \sqrt{30}}{36} = 0{,}347855$ |
>
> $$\int_a^b f(x)\,dx \approx \frac{b - a}{2}\sum_{i=1}^{n} A_i f(t_i), \qquad t_i = \frac{a + b}{2} + \frac{b - a}{2}x_i$$
> Czynnik $\frac{b-a}{2}$ to jakobian 1D z MES.

> [!przyklad] Zad. 1: trzy całki, Gauss 2/3/4 i Newton–Cotes dla $n = 20$
> | całka | dokładnie | G2 | G3 | G4 | prostokąty | trapezy | parabole (Simpson) |
> |---|---|---|---|---|---|---|---|
> | $\int_{0{,}5}^{2{,}5}\sin x$ | 1,6787262 | 1,6716257 | 1,6787876 | 1,6787259 | 1,6794259 | 1,6773270 | 1,6787271 |
> | $\int_{0{,}5}^{5}(x^2 + 2x + 5)$ | 88,875 | 88,875 | 88,875 | 88,875 | 88,856016 | 88,912969 | 88,875 |
> | $\int_{0{,}5}^{5} e^x$ | 146,764438 | 138,621354 | 146,426442 | 146,756901 | 146,455313 | 147,383079 | 146,766515 |
>
> Wnioski: dla wielomianu 2. stopnia Gauss od 2 węzłów i Simpson są dokładne. Dla $e^x$ na długim przedziale 4 węzły Gaussa (4 wywołania $f$) dają błąd $7{,}5\cdot10^{-3}$, czyli lepiej niż prostokąty i trapezy z 20 podziałami, ale gorzej niż Simpson z 21 wywołaniami. Prostokąty mają mniej więcej połowę błędu trapezów i przeciwny znak.

> [!przyklad] Zad. 2: $I_1 \ldots I_8$ schematem 2, 3 i 4-węzłowym
> | całka | dokładnie | G2 | G3 | G4 |
> |---|---|---|---|---|
> | $I_1 = \int_{-1}^1 (3x^2 + 2x + 1)$ | 4 | **4** | **4** | **4** |
> | $I_2 = \int_{-1}^1 (x^3 - 4x + 2)$ | 4 | **4** | **4** | **4** |
> | $I_3 = \int_{-1}^1 (x^4 + x^2 + 1)$ | 3,0666667 | 2,8888889 | **3,0666667** | **3,0666667** |
> | $I_4 = \int_{-1}^1 (x^5 - 2x^3 + x)$ | 0 | **0** | **0** | **0** |
> | $I_5 = \int_{-1}^1 (x^6 + x^2)$ | 0,9523810 | 0,7407407 | 0,9066667 | **0,9523810** |
> | $I_6 = \int_0^2 (x^4 - 3x^2 + 2)$ | 2,4 | 2,2222222 | **2,4** | **2,4** |
> | $I_7 = \int_0^1 e^x$ | 1,7182818285 | 1,7178963780 | 1,7182810044 | 1,7182818275 |
> | $I_8 = \int_0^\pi \sin x$ | 2 | 1,9358196 | 2,0013889 | 1,9999842 |
>
> **Odpowiedzi na pytania:**
> 1. **2 węzły** są dokładne dla wielomianów stopnia $\le 3$: $I_1$, $I_2$ i $I_4$. $I_4$ ma stopień 5, ale jest funkcją nieparzystą na przedziale symetrycznym, więc każda symetryczna kwadratura daje 0.
> 2. **3 węzły** są dokładne do stopnia $\le 5$: $I_1$–$I_4$ i $I_6$ (stopień 4 na $[0, 2]$).
> 3. **4 węzły** są dokładne do stopnia $\le 7$: wszystkie wielomiany, w tym $I_5$ (stopień 6).
> 4. $e^x$ i $\sin x$ nie są wielomianami: ich szereg Taylora ma nieskończenie wiele wyrazów, a $n$-węzłowy Gauss całkuje dokładnie tylko część do $x^{2n-1}$.
> 5. Błąd maleje bardzo szybko (dla funkcji gładkich wykładniczo z $n$): $I_7$ ma błędy $3{,}9\cdot10^{-4}$, $8{,}2\cdot10^{-7}$, $1{,}0\cdot10^{-9}$, czyli ok. 3 cyfry więcej na każdy węzeł.

> [!kod]- Funkcja podcałkowa jako argument (wymaganie z zad. 1)
> ```cpp
> #include <functional>
> double gauss_legendre(std::function<double(double)> f, double a, double b, int n);
>
> double moja(double x) { return x * x + 2 * x + 5; }
> gauss_legendre(moja, 0.5, 5, 3);                                   // nazwa funkcji
> gauss_legendre([](double x) { return std::exp(x); }, 0.5, 5, 4);   // lambda
> ```
> Bez `<functional>` działa też wskaźnik: `double gauss_legendre(double (*f)(double), ...)`, ale wtedy nie przekażesz lambdy z przechwytywaniem.

## Typowe błędy
> [!blad] Zapominasz o jakobianie
> $\sum w_i f(x_i)$ bez czynnika $(b-a)/2$ (albo bez $\det J$ w 2D) daje wynik przeskalowany. Kontrola: całka z $1$ musi dać długość odcinka albo pole elementu.

> [!blad] Węzły ponumerowane zgodnie z ruchem wskazówek zegara
> Wtedy $\det J < 0$ i macierz elementu wychodzi z przeciwnym znakiem. Numeruj przeciwnie do ruchu wskazówek, jak w podręczniku Milenina, i dodaj w kodzie kontrolę $\det J > 0$.

> [!blad] Za mało punktów Gaussa
> Dla elementu prostokątnego $\det J$ jest stałe, a $\partial N/\partial x$ jest liniowe, więc 2×2 punkty całkują $K$ dokładnie. Dla elementu krzywoliniowego albo przy elementach 9-węzłowych potrzebujesz 3×3. Sprawdź, czy wynik zmienia się po zwiększeniu liczby punktów.

> [!blad] Simpson z nieparzystym $n$
> Simpson łączy przedziały **parami**, więc $n$ musi być parzyste.

## Na zajęcia i egzamin
> [!zajecia]
> - **Projekt MES 2D:** napisz raz porządną strukturę „schemat całkowania” (tablice punktów i wag dla 2×2 i 3×3) oraz funkcję liczącą $J$, $\det J$ i $J^{-1}$ w każdym punkcie. **Ćwicz w C++:** `Kod/metody_numeryczne/zadania/m06.cpp` (prostokąty, trapezy, Simpson, parabole z instrukcji, węzły Gaussa 1–4, Gauss na [a, b], Gauss 2D, całka po czworokącie), specyfikacja w `m06.h`, sprawdzasz `./sprawdz.sh 06`. Wzorzec: `rozwiazania/m06.cpp`, dopiero po własnej próbie. Jakobian, $H$, $C$ i $H_{bc}$ elementu MES są w m09. Test kontrolny: suma $\det J \cdot w_i w_j$ po punktach musi równać się polu elementu.
> - **Egzamin:** „wyprowadź węzły i wagi dla dwóch punktów”, „do jakiego stopnia dokładna jest kwadratura $n$-punktowa”, „co to jest jakobian przekształcenia i po co $\det J$ w całce”, „ile punktów całkowania potrzeba dla elementu 4- i 9-węzłowego”.
> - Łączy się z: [[Jakobian|przekształcenie izoparametryczne]], [[Jakobian]], [[Funkcje kształtu]], [[Macierz sztywności]].

## Zadania
> [!zadanie] Zadanie 1
> Policz $\int_0^2 e^x dx$ kwadraturą Gaussa z 2 punktami i porównaj z dokładnym $e^2 - 1 = 6{,}3891$.
> > [!odpowiedz]- Odpowiedź
> > Węzły $x = 1 \mp 0{,}5774$, jakobian $1$: $e^{0{,}4226} + e^{1{,}5774} = 1{,}5260 + 4{,}8421 = 6{,}3681$. Błąd wynosi $0{,}021$. Trapezy z 4 przedziałami dają $6{,}522$, a 3 punkty Gaussa $6{,}38888$.

> [!zadanie] Zadanie 2
> Ile punktów Gaussa trzeba, żeby dokładnie scałkować wielomian stopnia 6 w 1D?
> > [!odpowiedz]- Odpowiedź
> > Potrzebujesz $2n - 1 \ge 6$, czyli $n = 4$.

> [!zadanie] Zadanie 3
> Prostokąt $[0,4] \times [0,2]$ jako element 4-węzłowy. Ile wynosi $J$ i $\det J$?
> > [!odpowiedz]- Odpowiedź
> > $x = 2(1+\xi)$, $y = 1 + \eta$, więc $J = \mathrm{diag}(2, 1)$ i $\det J = 2$ w każdym punkcie. Kontrola: $\det J \cdot 4 = 8$, czyli pole prostokąta. Ogólnie dla prostokąta $a \times b$ masz $\det J = ab/4$.

> [!zadanie] Zadanie 4
> Trapezy z $h = 0{,}1$ dały $T_h = 1{,}71971$, a z $h = 0{,}05$ dały $T_{h/2} = 1{,}71864$ (całka $\int_0^1 e^x dx$). Popraw wynik Richardsonem.
> > [!odpowiedz]- Odpowiedź
> > $I \approx \frac{4 \cdot 1{,}71864 - 1{,}71971}{3} = 1{,}718283$, a dokładnie $e - 1 = 1{,}718282$. Błąd spadł z $10^{-3}$ do $10^{-6}$.

## Fiszki #fiszki/numeryczne
Rząd błędu złożonej metody trapezów::O(h²) — połowa kroku zmniejsza błąd 4 razy
Rząd błędu złożonej metody Simpsona::O(h⁴), dokładna dla wielomianów do stopnia 3; n musi być parzyste
Do jakiego stopnia dokładna jest n-punktowa kwadratura Gaussa–Legendre'a?::Do stopnia 2n − 1
Węzły i wagi Gaussa dla 2 punktów na [−1, 1]::ξ = ±1/√3 ≈ ±0,5774, wagi 1 i 1
Węzły i wagi Gaussa dla 3 punktów na [−1, 1]::ξ = 0 i ±√(3/5) ≈ ±0,7746, wagi 8/9 oraz 5/9, 5/9
Ile wynosi suma wag Gaussa na [−1, 1]?::2, czyli długość przedziału
Jakobian zamiany przedziału [a, b] na [−1, 1]::(b − a)/2; w elemencie 1D to L_e/2
Jak działa Gauss 2D na kwadracie [−1, 1]²?::Iloczyn tensorowy: suma po i, j z w_i·w_j·f(ξ_i, η_j); 2×2 = 4 punkty, 3×3 = 9 punktów
Wzór na całkę po czworokącie w MES::Suma po punktach Gaussa z w_i·w_j·f(x(ξ,η), y(ξ,η))·det J
Co oznacza det J ≤ 0 w elemencie?::Złą numerację węzłów (zgodnie z ruchem wskazówek) albo zdegenerowany, wklęsły element
Ile wynosi suma det J razy wagi po punktach Gaussa?::Pole elementu, co jest dobrym testem kodu
Co daje ekstrapolacja Richardsona dla trapezów?::(4T(h/2) − T(h))/3 usuwa błąd h² i daje Simpsona; powtarzana to metoda Romberga

Węzły i wagi Gaussa 3-węzłowego::±√(3/5) ≈ ±0,7746 i 0; wagi 5/9, 8/9, 5/9
Węzły i wagi Gaussa 4-węzłowego::±0,339981 (waga 0,652145) i ±0,861136 (waga 0,347855)
Zamiana przedziału [a, b] na [−1, 1] w kwadraturze Gaussa::t = (a+b)/2 + (b−a)/2·x, całka = (b−a)/2·Σ A_i f(t_i)
Dla jakich wielomianów kwadratura n-węzłowa Gaussa jest dokładna?::Stopnia ≤ 2n − 1 (2 węzły: ≤ 3, 3: ≤ 5, 4: ≤ 7)
Dlaczego ∫(x⁵ − 2x³ + x) na [−1, 1] wychodzi dokładnie już z 2 węzłów?::Funkcja nieparzysta na symetrycznym przedziale: węzły symetryczne dają 0
∫ sin x na [0,5; 2,5] Gaussem 2-węzłowym::1,67163 (dokładnie 1,67873)

Wartości z labu „metody geometryczne”: ∫ sin x na [0,5; 2,5], n = 4::Prostokąty 1,6963, trapezy 1,6436, parabole 1,67876 (dokładnie 1,67873)
Rzędy złożonych metod prostokątów, trapezów i parabol::h², h² i h⁴: podwojenie n daje błąd 4, 4 i 16 razy mniejszy
Związek prostokątów, trapezów i parabol::Błąd prostokątów ≈ −½ błędu trapezów, więc (2P + T)/3 to Simpson (parabole)

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.

## Źródła
- A. Milenin, *Podstawy metody elementów skończonych*, Wydawnictwa AGH, Kraków 2010 (całkowanie Gaussa, przekształcenie izoparametryczne).
- O.C. Zienkiewicz, R.L. Taylor, J.Z. Zhu, *The Finite Element Method: Its Basis and Fundamentals*, Elsevier (rozdz. o elementach izoparametrycznych i całkowaniu numerycznym).
- Z. Fortuna, B. Macukow, J. Wąsowski, *Metody numeryczne*, WNT (rozdz. o całkowaniu numerycznym).
- R.L. Burden, J.D. Faires, *Numerical Analysis*, Cengage (Newton–Cotes, Romberg, Gaussian quadrature).
- Dokumentacja: `numpy.polynomial.legendre.leggauss`, `scipy.integrate.quad`.
- Instrukcja: Lab — kwadratury Gaussa-Legendre'a
- Kurs MN w C++: Kod/metody_numeryczne (moduły, testy, rozwiązania wzorcowe)
- Instrukcja: Lab — całkowanie metodami prostokątów, trapezów i parabol
