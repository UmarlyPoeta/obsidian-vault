---
typ: pojęcie
status: nowe
przedmioty: ["[[AGH Metody numeryczne]]", "[[AGH Metoda elementów skończonych]]", "[[AGH Optymalizacja]]"]
wymaga: ["[[Pochodna]]", "[[Arytmetyka zmiennoprzecinkowa i błędy numeryczne]]"]
powiązane: ["[[Interpolacja]]", "[[Aproksymacja średniokwadratowa]]", "[[Funkcja celu i warunki optymalności]]", "[[Numeryczne rozwiązywanie RRZ]]", "[[MES 1D — od różnic skończonych do macierzy sztywności]]", "[[Rozwiązywanie równań nieliniowych]]"]
tags: [pojęcie, metody-numeryczne]
utworzono: 2026-10-02
źródła: ["Fortuna, Macukow, Wąsowski — Metody numeryczne (WNT)", "Kincaid, Cheney — Analiza numeryczna", "Burden, Faires — Numerical Analysis", "Dokumentacja SciPy: scipy.optimize.approx_fprime", "Kurs MN w C++: Kod/metody_numeryczne (moduły, testy, rozwiązania wzorcowe)"]
---
# Różniczkowanie numeryczne

> [!skrot] W 30 sekund
> - **Co to:** przybliżanie pochodnej z wartości funkcji w kilku pobliskich punktach (różnice skończone).
> - **Klucz:** centralna $f'(x) \approx \frac{f(x+h) - f(x-h)}{2h}$ z błędem $O(h^2)$; druga pochodna $\frac{f(x+h) - 2f(x) + f(x-h)}{h^2}$. Za małe $h$ psuje wynik — optimum to $h \sim \varepsilon^{1/3} \approx 6 \cdot 10^{-6}$.
> - **Kiedy:** gradient i hesjan funkcji celu bez wzoru analitycznego (optymalizacja), schematy różnicowe dla równań różniczkowych (wstęp do MES), sprawdzanie ręcznie wyprowadzonych pochodnych.

> [!intuicja] Po ludzku
> Pochodna to nachylenie. Mierzysz wysokość w dwóch bliskich punktach i dzielisz różnicę przez odległość. Zbyt duży rozstaw myli krzywiznę z nachyleniem (błąd obcięcia). Zbyt mały sprawia, że dwie prawie równe liczby zmiennoprzecinkowe odejmują się, a z różnicy zostaje głównie szum zaokrągleń. Najlepsze $h$ leży pośrodku.

## Definicja i wzory
> [!wzor] Schematy i ich rząd (z rozwinięcia Taylora)
> $$f(x \pm h) = f \pm h f' + \tfrac{h^2}{2} f'' \pm \tfrac{h^3}{6} f''' + \tfrac{h^4}{24} f'''' + \dots$$
> $$\text{w przód: } \frac{f(x+h) - f(x)}{h} = f' + \tfrac{h}{2} f'' + \dots \quad O(h)$$
> $$\text{w tył: } \frac{f(x) - f(x-h)}{h} = f' - \tfrac{h}{2} f'' + \dots \quad O(h)$$
> $$\text{centralna: } \frac{f(x+h) - f(x-h)}{2h} = f' + \tfrac{h^2}{6} f''' + \dots \quad O(h^2)$$
> $$\text{druga: } \frac{f(x+h) - 2f(x) + f(x-h)}{h^2} = f'' + \tfrac{h^2}{12} f'''' + \dots \quad O(h^2)$$
> Rząd $p$ oznacza: zmniejszasz $h$ dwa razy, błąd maleje $2^p$ razy.

> [!wzor] Kompromis: obcięcie kontra zaokrąglenie
> Wartości $f$ są obarczone błędem względnym rzędu $\varepsilon \approx 2{,}2 \cdot 10^{-16}$:
> $$E_{\text{przód}}(h) \approx \tfrac{h}{2}|f''| + \tfrac{2\varepsilon |f|}{h} \ \Rightarrow\ h^* \sim \sqrt{\varepsilon} \approx 1{,}5 \cdot 10^{-8}$$
> $$E_{\text{centr}}(h) \approx \tfrac{h^2}{6}|f'''| + \tfrac{\varepsilon |f|}{h} \ \Rightarrow\ h^* \sim \varepsilon^{1/3} \approx 6 \cdot 10^{-6}$$
> Dla drugiej pochodnej $h^* \sim \varepsilon^{1/4} \approx 10^{-4}$. Dla dużych $|x|$ mnożymy przez $|x|$.

> [!wzor] Ekstrapolacja Richardsona
> Skoro $D(h) = f' + c\,h^2 + O(h^4)$, to
> $$R(h) = \frac{4\,D(h/2) - D(h)}{3} = f' + O(h^4)$$
> Dwie przybliżone wartości łączymy tak, żeby wiodący wyraz błędu się zniósł.

> [!wzor] Gradient i hesjan funkcji wielu zmiennych
> $$\frac{\partial f}{\partial x_j} \approx \frac{f(x + h e_j) - f(x - h e_j)}{2h}$$
> $$\frac{\partial^2 f}{\partial x_i \partial x_j} \approx \frac{f(x{+}he_i{+}he_j) - f(x{+}he_i{-}he_j) - f(x{-}he_i{+}he_j) + f(x{-}he_i{-}he_j)}{4h^2}$$
> Gradient kosztuje $2n$ obliczeń $f$, hesjan rzędu $2n^2$.

> [!wzor] Schemat różnicowy dla $u'' = f$
> Węzły $x_i = a + ih$, warunki $u(a) = u_a$, $u(b) = u_b$:
> $$\frac{u_{i-1} - 2u_i + u_{i+1}}{h^2} = f(x_i), \quad i = 1, \dots, n-1$$
> Wychodzi układ trójdiagonalny z „szablonem” $[1, -2, 1]$. Błąd globalny $O(h^2)$. MES 1D z elementami liniowymi daje macierz sztywności o tej samej strukturze.

## Jak to zrobić — krok po kroku
1. Wybierz schemat: centralny, jeśli możesz policzyć $f$ po obu stronach punktu; jednostronny przy brzegu dziedziny.
2. Dobierz $h$: centralna $\approx 10^{-5}$–$10^{-6}$ razy skala zmiennej, w przód $\approx 10^{-8}$, druga pochodna $\approx 10^{-4}$.
3. Sprawdź rząd: policz błąd dla $h, h/2, h/4$ i nachylenie prostej na wykresie log–log.
4. Potrzebujesz więcej dokładności → Richardson, a nie mniejsze $h$.
5. W kilku wymiarach: pętla po współrzędnych z wektorem bazowym $h e_j$.

## Przykład
> [!przyklad] $f(x) = \sin x$, $x = 1$, dokładnie $f'(1) = \cos 1 = 0{,}540302$
> Dla $h = 0{,}1$: $\sin 1{,}1 = 0{,}891207$, $\sin 1 = 0{,}841471$, $\sin 0{,}9 = 0{,}783327$.
>
> | schemat | wartość | błąd |
> |---|---|---|
> | w przód | $(0{,}891207 - 0{,}841471)/0{,}1 = 0{,}497364$ | $-4{,}3 \cdot 10^{-2}$ |
> | w tył | $(0{,}841471 - 0{,}783327)/0{,}1 = 0{,}581441$ | $+4{,}1 \cdot 10^{-2}$ |
> | centralna | $(0{,}891207 - 0{,}783327)/0{,}2 = 0{,}539402$ | $-9{,}0 \cdot 10^{-4}$ |
> | centralna, $h = 0{,}05$ | $0{,}540077$ | $-2{,}25 \cdot 10^{-4}$ |
> | Richardson | $(4 \cdot 0{,}540077 - 0{,}539402)/3 = 0{,}5403022$ | $-1{,}1 \cdot 10^{-7}$ |
>
> Połowienie $h$ zmniejszyło błąd centralnej dokładnie 4 razy (rząd 2). Błąd $-9 \cdot 10^{-4}$ zgadza się z przewidywaniem $\frac{h^2}{6} f'''(1) = -\frac{0{,}01}{6} \cos 1 = -9{,}0 \cdot 10^{-4}$. Druga pochodna z $h = 0{,}1$: $-0{,}840770$ wobec $-\sin 1 = -0{,}841471$.

```cpp
#include "m05.h"   // Kod/metody_numeryczne
#include <cstdio>
for (double h : {1e-1, 1e-3, 1e-5, 1e-8, 1e-11})
    std::printf("h = %g  błąd = %.3e\n", h,
                roznica_centralna([](double x) { return std::sin(x); }, 1.0, h) - std::cos(1.0));
// 1e-1: -9.0e-4   1e-3: -9.0e-8   1e-5: -1.1e-11   1e-8: 2.6e-9   1e-11: -1.2e-6  <- za małe h psuje wynik
Vec g = gradient(rosenbrock, {-1, 2});   // (396, 200)
Mat H = hesjan(rosenbrock, {1, 1});      // [[802, -400], [-400, 200]]
```

> [!kod]- To samo w Pythonie (do szybkiego sprawdzenia)
> ```python
> import numpy as np
> f, x = np.sin, 1.0
> for h in (1e-1, 1e-5, 1e-12):
>     print(h, (f(x + h) - f(x - h)) / (2 * h) - np.cos(x))   # małe h ≠ dokładnie!
> ```

> [!przyklad]- Przykład 2: za małe h
> Dla $f = e^x$, $x = 1$, różnica w przód: przy $h = 10^{-1}$ błąd $0{,}14$, przy $h = 10^{-8}$ błąd $6{,}6 \cdot 10^{-9}$ (najlepszy), przy $h = 10^{-12}$ błąd znowu rośnie do $4{,}3 \cdot 10^{-4}$. Na wykresie log–log widać literę V: lewa gałąź to zaokrąglenia, prawa — obcięcie.

## Typowe błędy
> [!blad] „Im mniejsze h, tym lepiej”
> Poniżej $h^*$ błąd **rośnie** jak $\varepsilon/h$. $h = 10^{-12}$ daje gorszy wynik niż $h = 10^{-5}$.

> [!blad] Ten sam krok dla gradientu i hesjanu
> W hesjanie dzielisz przez $h^2$, więc zaokrąglenia rosną jak $\varepsilon/h^2$. Krok $10^{-6}$ dobry dla gradientu daje w hesjanie śmieci; bierz około $10^{-4}$.

> [!blad] Nieprzeskalowane zmienne
> Jeśli $x_1 \sim 10^{-3}$, a $x_2 \sim 10^{4}$, jedno bezwzględne $h$ nie pasuje do obu. Skaluj krok względem $|x_j|$.

## Na zajęcia i egzamin
> [!zajecia]
> - **Optymalizacja:** gradient numeryczny w metodzie najszybszego spadku i metodach quasi-Newtona, hesjan do sprawdzania warunku dostatecznego minimum (dodatnia określoność). **Ćwicz w C++:** `Kod/metody_numeryczne/zadania/m05.cpp` (różnice przednia, centralna, druga pochodna, gradient, hesjan, krok optymalny), specyfikacja w `m05.h`, sprawdzasz `./sprawdz.sh 05`. Wzorzec: `rozwiazania/m05.cpp`, dopiero po własnej próbie. Dalej m10 składa z tego metodę Newtona i najszybszego spadku.
> - **MES:** schemat $[1, -2, 1]/h^2$ dla $u'' = f$ to punkt wyjścia do MES 1D; pochodne funkcji kształtu po $\xi$ budują jakobian. Ćwicz `uklad_fd` i sprawdź, że błąd maleje jak $h^2$.
> - **Typowe pytanie:** „wyprowadź z Taylora rząd różnicy centralnej”, „dlaczego nie można brać dowolnie małego $h$”, „na czym polega ekstrapolacja Richardsona”.
> - Łączy się z: [[Funkcja celu i warunki optymalności]], [[MES 1D — od różnic skończonych do macierzy sztywności]], [[Numeryczne rozwiązywanie RRZ]].

## Zadania
> [!zadanie] Zadanie 1
> Policz różnicą centralną z $h = 0{,}5$ pochodną $f(x) = x^3$ w $x = 2$. Jaki jest błąd i czy zgadza się z teorią?
> > [!odpowiedz]- Odpowiedź
> > $(2{,}5^3 - 1{,}5^3)/1 = 15{,}625 - 3{,}375 = 12{,}25$. Dokładnie $12$, błąd $0{,}25 = \frac{h^2}{6} f''' = \frac{0{,}25}{6} \cdot 6$. Zgadza się co do joty, bo $f''''= 0$.

> [!zadanie] Zadanie 2
> Błąd różnicy w przód przy $h = 0{,}01$ wynosi $3 \cdot 10^{-3}$. Ile wyniesie przy $h = 0{,}001$? A różnicy centralnej o błędzie $2 \cdot 10^{-4}$ przy $h = 0{,}01$?
> > [!odpowiedz]- Odpowiedź
> > W przód (rząd 1): ok. $3 \cdot 10^{-4}$. Centralna (rząd 2): ok. $2 \cdot 10^{-6}$.

> [!zadanie] Zadanie 3
> Policz numerycznie gradient i hesjan $f(x, y) = x^2 y + e^y$ w $(1, 0)$ i porównaj z dokładnymi.
> > [!odpowiedz]- Odpowiedź
> > Dokładnie: $\nabla f = (2xy,\ x^2 + e^y) = (0, 2)$, $H = \begin{bmatrix} 2y & 2x \\ 2x & e^y \end{bmatrix} = \begin{bmatrix} 0 & 2 \\ 2 & 1 \end{bmatrix}$. Numerycznie (h = $10^{-6}$ i $10^{-4}$) wychodzi to samo z dokładnością ok. $10^{-8}$.

> [!zadanie] Zadanie 4
> Zapisz układ różnicowy dla $u'' = 2$ na $[0, 1]$, $u(0) = u(1) = 0$, z $h = 1/3$.
> > [!odpowiedz]- Odpowiedź
> > Dwie niewiadome $u_1, u_2$: $9(-2u_1 + u_2) = 2$, $9(u_1 - 2u_2) = 2$. Stąd $u_1 = u_2 = -2/9$, co równa się dokładnemu $x^2 - x$ w $x = 1/3$ i $2/3$ (schemat jest dokładny dla wielomianów stopnia do 3).

## Fiszki #fiszki/numeryczne
Wzór na różnicę centralną pierwszej pochodnej::(f(x + h) − f(x − h)) / (2h), błąd O(h²)
Jaki rząd błędu ma różnica w przód?::Pierwszy: błąd ≈ (h/2)·f''(x), czyli O(h)
Wzór na drugą pochodną różnicami::(f(x + h) − 2f(x) + f(x − h)) / h², błąd O(h²)
Optymalne h dla różnicy w przód w float64::Około √ε ≈ 1,5·10⁻⁸ (razy skala zmiennej)
Optymalne h dla różnicy centralnej w float64::Około ε^(1/3) ≈ 6·10⁻⁶ (razy skala zmiennej)
Dlaczego za małe h psuje pochodną numeryczną?::Odejmowanie prawie równych liczb — błąd zaokrągleń rośnie jak ε/h
Wzór Richardsona dla różnicy centralnej::(4·D(h/2) − D(h)) / 3, błąd O(h⁴)
Ile obliczeń f kosztuje gradient centralny w n wymiarach?::2n
Skąd wziąć rząd schematu z eksperymentu?::Nachylenie prostej log(błąd) od log(h)
Jak wygląda wiersz macierzy schematu różnicowego dla u'' = f?::[1, −2, 1] / h² — macierz trójdiagonalna

Gradient i hesjan funkcji Rosenbrocka 100(y − x²)² + (1 − x)²::∇f = (−400x(y − x²) − 2(1 − x), 200(y − x²)); w (1, 1) hesjan [[802, −400], [−400, 200]]

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.

## Źródła
- Z. Fortuna, B. Macukow, J. Wąsowski, *Metody numeryczne*, WNT — różniczkowanie numeryczne.
- D. Kincaid, W. Cheney, *Analiza numeryczna*, WNT — różniczkowanie numeryczne i ekstrapolacja Richardsona.
- R. L. Burden, J. D. Faires, *Numerical Analysis*, Brooks/Cole — numerical differentiation, Richardson's extrapolation, finite-difference methods for boundary-value problems.
- Dokumentacja SciPy: `scipy.optimize.approx_fprime`, `scipy.optimize.rosen_der`.
- Kurs MN w C++: Kod/metody_numeryczne (moduły, testy, rozwiązania wzorcowe)
