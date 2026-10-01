---
typ: pojęcie
status: nowe
przedmioty: ["[[AGH Metoda elementów skończonych]]", "[[AGH Optymalizacja]]"]
wymaga: ["[[Pochodna]]", "[[Arytmetyka zmiennoprzecinkowa i błędy numeryczne]]"]
powiązane: ["[[Układy równań liniowych]]", "[[Funkcja celu i warunki optymalności]]", "[[Wielomiany]]", "[[Numeryczne rozwiązywanie RRZ]]", "[[MES 1D — od różnic skończonych do macierzy sztywności]]"]
tags: [pojęcie, metody-numeryczne]
utworzono: 2026-10-02
źródła: ["Fortuna, Macukow, Wąsowski — Metody numeryczne (WNT)", "Kincaid, Cheney — Analiza numeryczna (WNT)", "Burden, Faires — Numerical Analysis (Brooks/Cole), rozdz. 2 i 10", "Dokumentacja SciPy: scipy.optimize.brentq, scipy.optimize.fsolve"]
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
2. Jeśli masz przedział i chcesz pewności: **bisekcja** (albo `scipy.optimize.brentq`).
3. Jeśli masz pochodną i dobry start: **Newton**. Bez pochodnej: **sieczne**.
4. Dla układu: wypisz $F$ i jakobian $J$ ręcznie, sprawdź $J$ różnicami skończonymi, potem iteruj `np.linalg.solve(J(x), -F(x))`.
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

```python
from scipy.optimize import brentq, fsolve
brentq(lambda x: x**2 - 2, 1, 2)                               # 1.41421356237313 (domyślne xtol=2e-12)
fsolve(lambda v: [v[0]**2 + v[1]**2 - 4, v[0]*v[1] - 1], [2, 0.5])  # [1.93185, 0.51764]
```

> [!przyklad]- Newton 2×2: jeden krok ręcznie
> $F(x,y) = \begin{bmatrix} x^2 + y^2 - 4 \\ xy - 1 \end{bmatrix}$, $J = \begin{bmatrix} 2x & 2y \\ y & x \end{bmatrix}$, start $(2;\ 0{,}5)$.
> - $F = (0{,}25;\ 0)$, $J = \begin{bmatrix} 4 & 1 \\ 0{,}5 & 2 \end{bmatrix}$, $\det J = 7{,}5$.
> - $J\Delta = -F$ daje $\Delta = (-0{,}0667;\ 0{,}0167)$.
> - $\mathbf x_1 = (1{,}9333;\ 0{,}5167)$. Dokładnie $(1{,}93185;\ 0{,}51764)$, a po 4 iteracjach mamy pełną precyzję.

## Typowe błędy
> [!blad] Newton z daleka
> Dla $f(x) = \operatorname{arctg} x$ i $x_0 = 2$ kolejne punkty skaczą coraz dalej (rozbieżność). Przy $f'(x_k) \approx 0$ krok jest ogromny. Lekarstwo: lepszy start (np. kilka kroków bisekcji) albo tłumienie kroku.

> [!blad] Odwracanie jakobianu
> `np.linalg.inv(J) @ F` jest wolniejsze i mniej dokładne niż `np.linalg.solve(J, F)`. W MES $J$ to duża macierz rzadka, której odwrotność jest **gęsta**.

> [!blad] Zła postać $x = g(x)$
> Dla $x^2 = 2$ postać $g(x) = 2/x$ ma $|g'(\sqrt2)| = 1$ i oscyluje ($1 \to 2 \to 1 \to \dots$). Postać $g(x) = x - (x^2-2)/4$ ma $|g'| \approx 0{,}29$ i zbiega.

> [!blad] Pierwiastek wielokrotny
> Dla $(x-1)^2 = 0$ Newton zbiega tylko liniowo (błąd maleje o połowę na krok), a bisekcja w ogóle nie zadziała, bo brak zmiany znaku.

## Na zajęcia i egzamin
> [!zajecia]
> - **Optymalizacja:** metoda Newtona minimalizacji to Newton zastosowany do $\nabla f(\mathbf x) = 0$, a jakobianem jest hesjan: $H\Delta = -\nabla f$. Stąd szybka zbieżność blisko minimum i kłopoty daleko (zob. [[Funkcja celu i warunki optymalności]]).
> - **MES:** przy przewodności $k(T)$ zależnej od temperatury układ $K(T)\,T = F$ jest nieliniowy, więc w każdym kroku czasu robisz iteracje Newtona albo prostą (Picarda) z kryterium stopu na $\|\Delta T\|$.
> - **Egzamin:** „porównaj bisekcję i Newtona (zbieżność, założenia)”, „ile kroków bisekcji do dokładności ε”, „warunek zbieżności iteracji prostej”, „napisz krok Newtona dla układu”.
> - **Ćwicz:** `kurs/m01_rownania_nieliniowe.py` (6 funkcji: bisekcja, regula falsi, Newton, sieczne, punkt stały, Newton dla układu), test `tests/test_m01.py`.

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

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.

## Źródła
- Z. Fortuna, B. Macukow, J. Wąsowski, *Metody numeryczne*, WNT — rozdz. o rozwiązywaniu równań nieliniowych.
- D. Kincaid, W. Cheney, *Analiza numeryczna*, WNT — równania nieliniowe (bisekcja, Newton, sieczne, punkt stały).
- R. L. Burden, J. D. Faires, *Numerical Analysis*, Brooks/Cole — rozdz. 2 (Solutions of Equations in One Variable), rozdz. 10 (Numerical Solutions of Nonlinear Systems of Equations).
- Dokumentacja SciPy: `scipy.optimize.brentq`, `scipy.optimize.fsolve`.
