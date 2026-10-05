---
typ: pojęcie
status: nowe
przedmioty: ["[[AGH Metoda elementów skończonych]]", "[[AGH Optymalizacja]]"]
wymaga: ["[[Pochodna]]", "[[Arytmetyka zmiennoprzecinkowa i błędy numeryczne]]"]
powiązane: ["[[Układy równań liniowych]]", "[[Funkcja celu i warunki optymalności]]", "[[Wielomiany]]", "[[Numeryczne rozwiązywanie RRZ]]", "[[MES 1D — od różnic skończonych do macierzy sztywności]]"]
tags: [pojęcie, metody-numeryczne]
utworzono: 2026-10-02
źródła: ["Fortuna, Macukow, Wąsowski — Metody numeryczne (WNT)", "Kincaid, Cheney — Analiza numeryczna (WNT)", "Burden, Faires — Numerical Analysis (Brooks/Cole), rozdz. 2 i 10", "Dokumentacja SciPy: scipy.optimize.brentq, scipy.optimize.fsolve", "Instrukcje: Lab RN1 (metoda stycznych i siecznych), Lab RN2 (bisekcja i regula falsi)", "Kurs MN w C++: Kod/metody_numeryczne (moduły, testy, rozwiązania wzorcowe)"]
---
# Rozwiązywanie równań nieliniowych

> [!skrot] W 30 sekund
> - **Co to:** szukanie $x^*$ takiego, że $f(x^*) = 0$ (albo wektora z $F(\mathbf x^*) = \mathbf 0$), kolejnymi przybliżeniami.
> - **Klucz:** Newton $x_{k+1} = x_k - \frac{f(x_k)}{f'(x_k)}$, w wersji wielowymiarowej $J(\mathbf x_k)\,\Delta = -F(\mathbf x_k)$. Bisekcja jest pewna, ale wolna (liniowa), Newton szybki (kwadratowy), ale tylko blisko pierwiastka.
> - **Kiedy:** minimum funkcji ($\nabla f = 0$ w optymalizacji), nieliniowy MES (przewodność zależna od temperatury), każde równanie bez wzoru jawnego.

> [!intuicja] Po ludzku
> Bisekcja to zgadywanie liczby metodą „więcej/mniej”: zawsze trafisz, ale każdy krok daje tylko jeden bit informacji. Newton patrzy na nachylenie i skacze tam, gdzie styczna przecina oś. Blisko celu podwaja liczbę dobrych cyfr na krok, ale z daleka potrafi wystrzelić w kosmos. Praktyka łączy oba podejścia: przedział dla bezpieczeństwa, styczne lub sieczne dla szybkości (tak działa `brentq`).

## Definicja i wzory
> [!wzor] Metody przedziałowe (wymagają $f(a)\,f(b) < 0$)
> - **Bisekcja:** $c = \frac{a+b}{2}$, zostaw połowę ze zmianą znaku. Po $n$ krokach błąd $\le \frac{b-a}{2^{n}}$, więc
> $$n \ge \log_2\frac{b-a}{\text{tol}}$$
> - **Regula falsi:** zamiast środka zero cięciwy $c = b - f(b)\frac{b-a}{f(b)-f(a)}$. Zawsze zbieżna, ale liniowo, bo jeden koniec często „przykleja się” na stałe.

> [!wzor] Metody otwarte (punkt startowy, bez gwarancji)
> - **Newton (stycznych):** $x_{k+1} = x_k - \frac{f(x_k)}{f'(x_k)}$, rząd $p = 2$ dla pierwiastka jednokrotnego. Dla pierwiastka wielokrotnego tylko $p = 1$.
> - **Sieczne:** $x_{k+1} = x_k - f(x_k)\frac{x_k - x_{k-1}}{f(x_k) - f(x_{k-1})}$, rząd $p = \frac{1+\sqrt5}{2} \approx 1{,}618$. Jedno nowe $f$ na krok, bez pochodnej.
> - **Iteracja prosta (punkt stały):** zapisz równanie jako $x = g(x)$ i licz $x_{k+1} = g(x_k)$. Zbieżna lokalnie, gdy $|g'(x^*)| < 1$, i wtedy $e_{k+1} \approx |g'(x^*)|\, e_k$ (liniowo). Newton to iteracja prosta z $g(x) = x - f/f'$, dla której $g'(x^*) = 0$, stąd rząd 2.

> [!wzor] Newton dla układu $F(\mathbf x) = \mathbf 0$
> $$J(\mathbf x_k)\,\Delta_k = -F(\mathbf x_k), \qquad \mathbf x_{k+1} = \mathbf x_k + \Delta_k, \qquad J_{ij} = \frac{\partial F_i}{\partial x_j}$$
> Każda iteracja to **rozwiązanie układu liniowego** (patrz [[Układy równań liniowych]]), nigdy odwracanie $J$.

> [!definicja] Kryteria stopu
> - Krok: $|x_{k+1} - x_k| \le \text{tol}\cdot\max(1, |x_{k+1}|)$ (mieszane: bezwzględne przy małych $x$, względne przy dużych).
> - Residuum: $|f(x_k)| \le \text{tol}_f$. Samo w sobie zawodne, bo przy płaskiej $f$ residuum jest małe daleko od pierwiastka.
> - Zawsze limit iteracji `maxit` i błąd, gdy go przekroczysz.

## Jak to zrobić — krok po kroku
1. **Narysuj $f$** albo policz znaki na siatce punktów, żeby znaleźć przedział $[a,b]$ ze zmianą znaku.
2. Jeśli masz przedział i chcesz pewności: **bisekcja** (w Pythonie do sprawdzenia `scipy.optimize.brentq`).
3. Jeśli masz pochodną i dobry start: **Newton**. Bez pochodnej: **sieczne**.
4. Dla układu: wypisz $F$ i jakobian $J$ ręcznie, sprawdź $J$ różnicami skończonymi, potem iteruj: rozwiąż $J\Delta = -F$ (`gauss` z m02, dla 2×2 Cramer) i dodaj $\Delta$.
5. **Zweryfikuj**: `brentq(f, a, b)` dla 1D, `fsolve(F, x0)` dla układów, i zmierz rząd zbieżności z historii iteracji.

## Przykład
> [!przyklad] $f(x) = x^2 - 2$, $x^* = \sqrt2 \approx 1{,}41421356237310$
> **Newton**, $x_0 = 1$, $x_{k+1} = x_k - \frac{x_k^2 - 2}{2x_k}$:
>
> | $k$ | $x_k$ | $e_k = \lvert x_k - \sqrt2\rvert$ |
> |---|---|---|
> | 1 | 1,5 | $8{,}6\cdot10^{-2}$ |
> | 2 | 1,416666666666667 | $2{,}5\cdot10^{-3}$ |
> | 3 | 1,414215686274510 | $2{,}1\cdot10^{-6}$ |
> | 4 | 1,414213562374690 | $1{,}6\cdot10^{-12}$ |
> | 5 | 1,414213562373095 | $0$ (w double) |
>
> Liczba poprawnych cyfr: 1, 2, 5, 11, 16, czyli **podwaja się**. Rząd z trzech błędów $k = 2, 3, 4$:
> $$p \approx \frac{\ln(1{,}6\cdot10^{-12} / 2{,}1\cdot10^{-6})}{\ln(2{,}1\cdot10^{-6} / 2{,}5\cdot10^{-3})} \approx 2{,}00$$
>
> **Bisekcja** na $[1, 2]$: $1{,}5 \to 1{,}25 \to 1{,}375 \to 1{,}4375 \to \dots$. Do tolerancji $10^{-6}$ potrzeba $\lceil\log_2 10^6\rceil = 20$ kroków (Newton: 4).

```cpp
#include "m01.h"   // Kod/metody_numeryczne
auto f  = [](double x) { return x * x - 2; };
auto df = [](double x) { return 2 * x; };
Wynik n = styczne(f, df, 1.0, 1e-12);       // x = 1.414213562373095, 5 iteracji
Wynik b = bisekcja(f, 1, 2, 1e-6);           // x = 1.41421366, 21 iteracji (stop |f(x)| < 1e-6)
double p = rzad_zbieznosci(n.historia, std::sqrt(2.0));   // ~2 (bez ostatniej iteracji, gdzie błąd = 0)
```

> [!kod]- To samo w Pythonie (do szybkiego sprawdzenia)
> ```python
> from scipy.optimize import brentq, fsolve
> brentq(lambda x: x**2 - 2, 1, 2)                               # 1.41421356237313 (domyślne xtol=2e-12)
> fsolve(lambda v: [v[0]**2 + v[1]**2 - 4, v[0]*v[1] - 1], [2, 0.5])  # [1.93185, 0.51764]
> ```

> [!przyklad]- Newton 2×2: jeden krok ręcznie
> $F(x,y) = \begin{bmatrix} x^2 + y^2 - 4 \\ xy - 1 \end{bmatrix}$, $J = \begin{bmatrix} 2x & 2y \\ y & x \end{bmatrix}$, start $(2;\ 0{,}5)$.
> - $F = (0{,}25;\ 0)$, $J = \begin{bmatrix} 4 & 1 \\ 0{,}5 & 2 \end{bmatrix}$, $\det J = 7{,}5$.
> - $J\Delta = -F$ daje $\Delta = (-0{,}0667;\ 0{,}0167)$.
> - $\mathbf x_1 = (1{,}9333;\ 0{,}5167)$. Dokładnie $(1{,}93185;\ 0{,}51764)$, a po 4 iteracjach mamy pełną precyzję.

## Lab RN1 i RN2 — co oddajesz i wyniki do sprawdzenia
Instrukcje: [[Lab RN1 — metoda stycznych i siecznych.pdf]], [[Lab RN2 — bisekcja i regula falsi.pdf]]. Oddajesz na zajęciach (10 pkt), plik `.cpp` (w RN1 także sprawozdanie) na UPEL.

> [!zajecia] Wymagania programu (z instrukcji)
> - $f(x)$ i $f'(x)$ jako **osobne funkcje**, pochodna analitycznie. Program wypisuje równanie, a w każdej iteracji $x$ i $f(x)$.
> - RN1: start $x_0$ podaje użytkownik, dla siecznych $x_{-1} = x_0 - 0{,}1$. Stop po zadanej liczbie iteracji daje **7 pkt**, stop $|f(x)| < \varepsilon$ z wypisaniem liczby iteracji daje **10 pkt**.
> - RN2: bisekcja wypisuje tabelę kroków ($a$, $b$, $x_0$, $f(x_0)$, znak $f(a)f(x_0)$), falsi wypisuje $x$ i $f(x)$ w każdej iteracji.

> [!przyklad] RN1, przykład: $f(x) = -x^3 + 10x + 5$, $x_0 = 6$, $\varepsilon = 10^{-6}$
> | $k$ | styczne $x_k$ | $f(x_k)$ | sieczne ($x_{-1} = 5{,}9$) $x_k$ | $f(x_k)$ |
> |---|---|---|---|---|
> | 1 | 4,459184 | −39,08 | 4,430517 | −37,66 |
> | 2 | 3,672201 | −7,798 | 3,908952 | −15,64 |
> | 3 | 3,416158 | −0,7054 | 3,538608 | −3,923 |
> | 4 | 3,387952 | $-8{,}1\cdot10^{-3}$ | 3,414581 | −0,6660 |
> | 5 | 3,387619 | $-1{,}1\cdot10^{-6}$ | 3,389222 | $-3{,}9\cdot10^{-2}$ |
> | 6 | 3,387619 | $-7\cdot10^{-15}$ | 3,387637 | $-4{,}4\cdot10^{-4}$ |
> | 7 | | | 3,387619 | $-2{,}9\cdot10^{-7}$ |
>
> Pierwiastek $x^* = 3{,}3876191$: styczne 6 iteracji, sieczne 7. Pierwsze trzy kroki stycznych zgadzają się z instrukcją (4,459; 3,672; 3,416).

> [!przyklad] RN1, zadanie inżynierskie: zbiornik $V(h) = \pi h^2 (R - h/3) = 8$ m³, $R = 2$ m
> - $f(h) = \pi h^2 (2 - h/3) - 8$, $f'(h) = \pi(4h - h^2)$.
> - $h_0 = 1{,}5$, $|f| < 10^{-6}$: **styczne** 1,279061 → 1,271015 → 1,271002 (**3 iteracje**), **sieczne** ($h_{-1} = 1{,}4$) 1,275874 → 1,271202 → 1,271002 → 1,271002 (**4 iteracje**).
> - Wynik: $h \approx 1{,}2710$ m. Sprawdzenie: $\pi \cdot 1{,}2710^2 \cdot (2 - 0{,}4237) = 8{,}000$.

> [!przyklad] RN2: bisekcja $\cos(x^3 - 2x)$ na $[0, 2]$, $\varepsilon = 0{,}01$
> Kroki $x_0$: 1 → 1,5 → 1,75 → 1,625 → 1,6875 → 1,71875 → 1,703125 → 1,710938 → 1,707031 → **1,708984** ($f = -0{,}0025$, 10 iteracji). Pierwsze trzy jak w instrukcji: $f(1) = 0{,}5403$, $f(1{,}5) = 0{,}9305$, $f(1{,}75) = -0{,}2846$.

> [!przyklad] RN2 zad 3 i 4: porównanie bisekcji i reguły falsi ($|f| < 10^{-6}$)
> | Zadanie | bisekcja | regula falsi | wynik |
> |---|---|---|---|
> | $e^x - 100$ na $[0, 10]$ | 25 iteracji | **865 iteracji** | $x = \ln 100 = 4{,}6051702$ |
> | boja: $\rho_w(\pi r^2 h + 0{,}15h^3) = m$ na $[0, 2]$ | 29 iteracji | 20 iteracji | $h = 0{,}364917$ m |
>
> - **Dlaczego falsi tak wolno dla $e^x$?** Funkcja jest mocno wypukła, $f(10) \approx 22\,000$. Prawy koniec $b = 10$ nie rusza się ani razu, a cięciwa za każdym razem przecina oś tuż obok lewego końca. Zbieżność jest liniowa z ilorazem bliskim 1. To klasyczny przykład „przyklejonego” końca.
> - **Boja:** $g$ się skraca, $\rho_w = 1000$ kg/m³, $r = 0{,}6$ m, $m = 420$ kg, czyli $f(h) = 1000(\pi \cdot 0{,}36\,h + 0{,}15h^3) - 420$. Tu funkcja jest prawie liniowa i falsi wygrywa.

> [!kod]- Szkielet programu na lab (C++), uzupełnij funkcje z m01
> ```cpp
> #include <cstdio>
> #include <cmath>
> #include "m01.h"
>
> double f(double x)  { return -x * x * x + 10 * x + 5; }   // równanie
> double df(double x) { return -3 * x * x + 10; }           // pochodna analitycznie
>
> int main() {
>     double x0, eps;
>     std::printf("f(x) = -x^3 + 10x + 5\nx0 = "); std::scanf("%lf", &x0);
>     std::printf("eps = ");                       std::scanf("%lf", &eps);
>     Wynik n = styczne(f, df, x0, eps);
>     for (size_t k = 0; k < n.historia.size(); ++k)
>         std::printf("%2zu  x = %.8f  f(x) = %.3e\n", k + 1, n.historia[k], f(n.historia[k]));
>     std::printf("styczne: %d iteracji\n", n.iteracje);
>     Wynik s = sieczne(f, x0, x0 - 0.1, eps);
>     // ... ta sama tabela dla siecznych
> }
> ```
> Zamiast własnych funkcji w `main` przekazujesz `f` i `df` jako argument typu `F1` (`std::function<double(double)>`), więc ta sama metoda działa dla każdego równania.

## Typowe błędy
> [!blad] Newton z daleka
> Dla $f(x) = \operatorname{arctg} x$ i $x_0 = 2$ kolejne punkty skaczą coraz dalej (rozbieżność). Przy $f'(x_k) \approx 0$ krok jest ogromny. Lekarstwo: lepszy start (np. kilka kroków bisekcji) albo tłumienie kroku.

> [!blad] Odwracanie jakobianu
> Liczenie $J^{-1}$ i mnożenie przez $F$ jest wolniejsze i mniej dokładne niż rozwiązanie układu $J\Delta = -F$ eliminacją Gaussa. W MES $J$ to duża macierz rzadka, której odwrotność jest **gęsta**.

> [!blad] Zła postać $x = g(x)$
> Dla $x^2 = 2$ postać $g(x) = 2/x$ ma $|g'(\sqrt2)| = 1$ i oscyluje ($1 \to 2 \to 1 \to \dots$). Postać $g(x) = x - (x^2-2)/4$ ma $|g'| \approx 0{,}29$ i zbiega.

> [!blad] Pierwiastek wielokrotny
> Dla $(x-1)^2 = 0$ Newton zbiega tylko liniowo (błąd maleje o połowę na krok), a bisekcja w ogóle nie zadziała, bo brak zmiany znaku.

## Na zajęcia i egzamin
> [!zajecia]
> - **Optymalizacja:** metoda Newtona minimalizacji to Newton zastosowany do $\nabla f(\mathbf x) = 0$, a jakobianem jest hesjan: $H\Delta = -\nabla f$. Stąd szybka zbieżność blisko minimum i kłopoty daleko (zob. [[Funkcja celu i warunki optymalności]]).
> - **MES:** przy przewodności $k(T)$ zależnej od temperatury układ $K(T)\,T = F$ jest nieliniowy, więc w każdym kroku czasu robisz iteracje Newtona albo prostą (Picarda) z kryterium stopu na $\|\Delta T\|$.
> - **Egzamin:** „porównaj bisekcję i Newtona (zbieżność, założenia)”, „ile kroków bisekcji do dokładności ε”, „warunek zbieżności iteracji prostej”, „napisz krok Newtona dla układu”.
> - **Ćwicz w C++:** `Kod/metody_numeryczne/zadania/m01.cpp` (bisekcja, regula falsi, styczne, sieczne, Newton dla układu 2×2, rząd zbieżności), specyfikacja w `m01.h`, sprawdzasz `./sprawdz.sh 01`. Wzorzec: `rozwiazania/m01.cpp`, dopiero po własnej próbie.

## Zadania
> [!zadanie] Zadanie 1
> Ile kroków bisekcji potrzeba na $[0, 1]$ do tolerancji $10^{-6}$?
> > [!odpowiedz]- Odpowiedź
> > $n \ge \log_2(1/10^{-6}) = 19{,}93$, czyli $n = 20$.

> [!zadanie] Zadanie 2
> Zrób dwa kroki Newtona dla $f(x) = x^3 - 2x - 5$, $x_0 = 2$.
> > [!odpowiedz]- Odpowiedź
> > $f(2) = -1$, $f'(2) = 10$, więc $x_1 = 2{,}1$. Dalej $f(2{,}1) = 0{,}061$, $f'(2{,}1) = 11{,}23$, więc $x_2 \approx 2{,}0945681$. Dokładnie $x^* = 2{,}0945514815$.

> [!zadanie] Zadanie 3
> Iteracja $x_{k+1} = \cos x_k$ zbiega do $x^* \approx 0{,}7391$. Z jakim rzędem i jaką stałą?
> > [!odpowiedz]- Odpowiedź
> > Liniowo, $e_{k+1} \approx |g'(x^*)|\,e_k = \sin(0{,}7391)\,e_k \approx 0{,}674\,e_k$. Dla błędu $10^{-10}$ trzeba ok. 58 kroków.

> [!zadanie] Zadanie 4
> Dlaczego sieczne mogą być w praktyce szybsze od Newtona, choć mają niższy rząd?
> > [!odpowiedz]- Odpowiedź
> > Krok Newtona kosztuje $f$ i $f'$, krok siecznych tylko jedno $f$. Na jedno obliczenie funkcji sieczne dają rząd $1{,}618$, a Newton $\sqrt2 \approx 1{,}414$ (gdy $f'$ jest tak drogie jak $f$).

## Fiszki #fiszki/numeryczne
Wzór na krok metody Newtona w 1D::x_{k+1} = x_k − f(x_k) / f'(x_k)
Jaki jest rząd zbieżności Newtona dla pierwiastka jednokrotnego?::2 (kwadratowa), dla wielokrotnego spada do 1
Jaki jest rząd zbieżności metody siecznych?::(1 + √5)/2 ≈ 1,618
Ile kroków bisekcji do tolerancji tol na [a, b]?::n ≥ log2((b − a)/tol), błąd maleje dwukrotnie na krok
Warunek konieczny startu bisekcji i reguły falsi?::f(a)·f(b) < 0, czyli zmiana znaku na końcach przedziału
Kiedy iteracja prosta x = g(x) jest zbieżna?::Gdy |g'(x*)| < 1 w otoczeniu punktu stałego; zbieżność liniowa ze stałą |g'(x*)|
Krok Newtona dla układu F(x) = 0::Rozwiąż J(x_k)Δ = −F(x_k), potem x_{k+1} = x_k + Δ
Czym jest jakobian?::Macierz pochodnych cząstkowych J_ij = ∂F_i/∂x_j
Dlaczego regula falsi bywa wolna?::Jeden koniec przedziału się nie rusza (przykleja), więc zbieżność jest tylko liniowa
Jak Newton w optymalizacji wiąże się z Newtonem dla równań?::To Newton dla ∇f = 0: jakobianem gradientu jest hesjan, krok H·Δ = −∇f
Którą funkcją scipy szukać pierwiastka 1D w przedziale?::scipy.optimize.brentq(f, a, b) (Brent: bisekcja + sieczne + interpolacja odwrotna)
Jakie kryterium stopu stosować dla kroku iteracji?::Mieszane |x_{k+1} − x_k| ≤ tol·max(1, |x|) plus limit maxit

Jak w RN1 wybierasz drugi punkt startowy siecznych?::x₋₁ = x₀ − 0,1
Pierwiastek −x³ + 10x + 5 z x₀ = 6 i liczba iteracji (ε = 1e-6)::x ≈ 3,387619; styczne 6 iteracji, sieczne 7
Dlaczego regula falsi dla eˣ − 100 na [0, 10] potrzebuje 865 iteracji?::Prawy koniec przykleja się (f(10) ≈ 22 000), cięciwa przesuwa lewy koniec o bardzo mało
Wysokość napełnienia zbiornika πh²(2 − h/3) = 8::h ≈ 1,2710 m (styczne 3 iteracje, sieczne 4)
Jak przekazać funkcję jako argument w C++?::std::function<double(double)> (alias F1) albo wskaźnik double (*f)(double); można podać nazwę funkcji albo lambdę

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.

## Źródła
- Z. Fortuna, B. Macukow, J. Wąsowski, *Metody numeryczne*, WNT — rozdz. o rozwiązywaniu równań nieliniowych.
- D. Kincaid, W. Cheney, *Analiza numeryczna*, WNT — równania nieliniowe (bisekcja, Newton, sieczne, punkt stały).
- R. L. Burden, J. D. Faires, *Numerical Analysis*, Brooks/Cole — rozdz. 2 (Solutions of Equations in One Variable), rozdz. 10 (Numerical Solutions of Nonlinear Systems of Equations).
- Dokumentacja SciPy: `scipy.optimize.brentq`, `scipy.optimize.fsolve`.
- Instrukcje: Lab RN1 (metoda stycznych i siecznych), Lab RN2 (bisekcja i regula falsi)
- Kurs MN w C++: Kod/metody_numeryczne (moduły, testy, rozwiązania wzorcowe)
