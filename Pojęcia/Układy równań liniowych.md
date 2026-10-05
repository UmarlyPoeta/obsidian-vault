---
typ: pojęcie
status: nowe
przedmioty: ["[[AGH Metoda elementów skończonych]]", "[[AGH Programowanie równoległe]]", "[[AGH Optymalizacja]]"]
wymaga: ["[[Arytmetyka zmiennoprzecinkowa i błędy numeryczne]]"]
powiązane: ["[[Rozwiązywanie równań nieliniowych]]", "[[MES 1D — od różnic skończonych do macierzy sztywności]]", "[[Numeryczne rozwiązywanie RRZ]]", "[[Aproksymacja średniokwadratowa]]", "[[Interpolacja]]", "[[Trening — Programowanie równoległe w kodzie]]"]
tags: [pojęcie, metody-numeryczne]
utworzono: 2026-10-02
źródła: ["Fortuna, Macukow, Wąsowski — Metody numeryczne (WNT)", "Kincaid, Cheney — Analiza numeryczna (WNT)", "Burden, Faires — Numerical Analysis (Brooks/Cole), rozdz. 6 i 7", "Dokumentacja NumPy: numpy.linalg.solve, numpy.linalg.cond", "Dokumentacja SciPy: scipy.linalg.lu_factor, scipy.sparse.diags, scipy.sparse.linalg.spsolve", "Kurs MN w C++: Kod/metody_numeryczne (moduły, testy, rozwiązania wzorcowe)"]
---
# Układy równań liniowych

> [!skrot] W 30 sekund
> - **Co to:** rozwiązanie $A\mathbf x = \mathbf b$. Metody **dokładne** (Gauss, LU, Thomas) kończą się po skończonej liczbie działań, a **iteracyjne** (Jacobi, Gauss–Seidel) przybliżają $\mathbf x$ krok po kroku.
> - **Klucz:** Gauss/LU z wyborem elementu głównego kosztuje $O(n^3)$, Thomas dla trójdiagonalnej $O(n)$. Dokładność ogranicza $\operatorname{cond}(A)$: przy $\operatorname{cond} \approx 10^k$ tracisz ok. $k$ cyfr.
> - **Kiedy:** zawsze na końcu MES ($K\mathbf T = \mathbf F$, w nieustalonym w każdym kroku czasu), w Newtonie dla układów, w aproksymacji (równania normalne).

> [!intuicja] Po ludzku
> Eliminacja Gaussa to szkolne „odejmij wielokrotność pierwszego równania od pozostałych”, tylko zrobione systematycznie. LU to ta sama eliminacja zapisana na później: raz płacisz za rozkład, potem każde nowe $\mathbf b$ rozwiązujesz tanio. Thomas to Gauss, który wie, że poza trzema przekątnymi są same zera, więc ich nie rusza. Jacobi i Gauss–Seidel działają jak poprawianie wypracowania: każdy przebieg trochę lepszy, dopóki zmiany nie staną się pomijalne.

## Definicja i wzory
> [!wzor] Eliminacja Gaussa z częściowym wyborem elementu głównego
> Dla $k = 1, \dots, n-1$:
> 1. wybierz wiersz $p \ge k$ z największym $|a_{pk}|$ i zamień go z wierszem $k$,
> 2. dla $i > k$: $l_{ik} = a_{ik}/a_{kk}$, potem wiersz$_i$ $\leftarrow$ wiersz$_i - l_{ik}\cdot$wiersz$_k$ (także w $\mathbf b$).
>
> Na końcu podstawianie wstecz: $x_i = \frac{1}{u_{ii}}\left(y_i - \sum_{j>i} u_{ij}x_j\right)$. Koszt eliminacji $\approx \frac23 n^3$ działań.

> [!wzor] Rozkład LU
> $$PA = LU, \qquad A\mathbf x = \mathbf b \iff L\mathbf y = P\mathbf b,\quad U\mathbf x = \mathbf y$$
> $L$ to dolnotrójkątna z jedynkami na przekątnej i mnożnikami $l_{ik}$ pod nią, $U$ to wynik eliminacji. Rozkład kosztuje $O(n^3)$, a każde rozwiązanie z gotowym rozkładem tylko $O(n^2)$.

> [!wzor] Algorytm Thomasa (macierz trójdiagonalna)
> Wiersz $i$: $a_i x_{i-1} + b_i x_i + c_i x_{i+1} = d_i$.
> $$c'_1 = \frac{c_1}{b_1},\quad d'_1 = \frac{d_1}{b_1},\qquad m_i = b_i - a_i c'_{i-1},\quad c'_i = \frac{c_i}{m_i},\quad d'_i = \frac{d_i - a_i d'_{i-1}}{m_i}$$
> $$x_n = d'_n, \qquad x_i = d'_i - c'_i x_{i+1}$$
> Ok. $8n$ działań i $O(n)$ pamięci. Bezpieczny bez wyboru elementu, gdy macierz jest diagonalnie dominująca albo symetryczna dodatnio określona, a tak jest w 1D MES i różnicach skończonych.

> [!wzor] Metody iteracyjne, $A = D + L + U$ (przekątna, część pod i nad nią)
> - **Jacobi:** $\mathbf x^{(k+1)} = D^{-1}\left(\mathbf b - (L+U)\mathbf x^{(k)}\right)$. Każda składowa zależy tylko od starego wektora, więc wszystkie można liczyć **równolegle**.
> - **Gauss–Seidel:** $x_i^{(k+1)} = \frac{1}{a_{ii}}\left(b_i - \sum_{j<i} a_{ij}x_j^{(k+1)} - \sum_{j>i} a_{ij}x_j^{(k)}\right)$. Używa od razu nowych wartości, zwykle ok. 2× mniej iteracji, ale jest sekwencyjny.
> - Zbieżne, gdy $A$ jest **ściśle diagonalnie dominująca** ($|a_{ii}| > \sum_{j\ne i}|a_{ij}|$). Gauss–Seidel także dla symetrycznej dodatnio określonej. Warunek konieczny i wystarczający: promień spektralny macierzy iteracji $< 1$.

> [!definicja] Wskaźnik uwarunkowania
> $$\operatorname{cond}(A) = \|A\|\,\|A^{-1}\|, \qquad \frac{\|\delta\mathbf x\|}{\|\mathbf x\|} \le \operatorname{cond}(A)\,\frac{\|\delta\mathbf b\|}{\|\mathbf b\|}$$
> Norma wierszowa $\|A\|_\infty = \max_i \sum_j |a_{ij}|$. $\operatorname{cond} \ge 1$, dla macierzy jednostkowej $= 1$, dla macierzy Hilberta $10 \times 10$ ok. $3{,}5\cdot10^{13}$.

## Jak to zrobić — krok po kroku
1. **Rozpoznaj strukturę**: gęsta mała, wtedy `np.linalg.solve`; trójdiagonalna, wtedy Thomas albo `scipy.linalg.solve_banded`; duża rzadka (2D MES), wtedy `scipy.sparse` (format CSR/CSC) i `spsolve`.
2. **Wiele prawych stron z tą samą macierzą** (MES nieustalony ze stałym $\Delta t$): rozkład raz (`scipy.linalg.lu_factor`), potem `lu_solve` w każdym kroku.
3. **Nie odwracaj macierzy** (`inv(A) @ b`): wolniej, mniej dokładnie, a dla rzadkiej $A$ odwrotność jest gęsta.
4. **Sprawdź uwarunkowanie** (`np.linalg.cond`), gdy wynik wygląda podejrzanie.
5. **Metody iteracyjne** wybierz dla bardzo dużych rzadkich układów albo do zrównoleglenia. Sprawdź dominację przekątniową i ustaw stop na $\|\mathbf x^{(k+1)} - \mathbf x^{(k)}\|_\infty \le \text{tol}$.

## Przykład
> [!przyklad] Gauss z wyborem elementu głównego
> $$\begin{bmatrix} 2 & 1 & -1 \\ -3 & -1 & 2 \\ -2 & 1 & 2 \end{bmatrix}\mathbf x = \begin{bmatrix} 8 \\ -11 \\ -3 \end{bmatrix}$$
> **Krok 1.** Największe $|a_{i1}|$ to $3$ (wiersz 2), więc zamieniamy wiersze 1 i 2. Mnożniki $l_{21} = 2/(-3) = -\frac23$, $l_{31} = -2/(-3) = \frac23$:
> $$\left[\begin{array}{ccc|c} -3 & -1 & 2 & -11 \\ 0 & \frac13 & \frac13 & \frac23 \\ 0 & \frac53 & \frac23 & \frac{13}{3} \end{array}\right]$$
> **Krok 2.** W kolumnie 2 większe jest $\frac53$ (wiersz 3), więc zamieniamy wiersze 2 i 3. Mnożnik $l_{32} = \frac{1/3}{5/3} = \frac15$:
> $$\left[\begin{array}{ccc|c} -3 & -1 & 2 & -11 \\ 0 & \frac53 & \frac23 & \frac{13}{3} \\ 0 & 0 & \frac15 & -\frac15 \end{array}\right]$$
> **Wstecz:** $x_3 = -1$, $x_2 = \frac{13/3 + 2/3}{5/3} = 3$, $x_1 = \frac{-11 + 3 + 2}{-3} = 2$. Wynik $\mathbf x = (2;\ 3;\ -1)$.
>
> To jest od razu rozkład LU: $U$ to macierz końcowa, $L$ ma pod przekątną $\frac23, -\frac23, \frac15$ (w kolejności wierszy po zamianach), a permutacja to $p = (2, 3, 1)$.

```cpp
#include "m02.h"   // Kod/metody_numeryczne
int n = 9;                                    // węzły wewnętrzne pręta -T'' = 1, T(0) = T(1) = 0
double h = 1.0 / (n + 1);
Vec a(n, -1), d(n, 2), c(n, -1), b(n, h * h);
Vec T = thomas(a, d, c, b);                   // T[4] = 0.125 = x(1-x)/2 w x = 0.5, O(n)
Vec x = gauss({{2, 1, -1}, {-3, -1, 2}, {-2, 1, 2}}, {8, -11, -3});   // (2, 3, -1)
```

> [!kod]- To samo w Pythonie (do szybkiego sprawdzenia)
> ```python
> import numpy as np, scipy.sparse as sp
> from scipy.sparse.linalg import spsolve
> n = 5; h = 1 / (n + 1)                       # -u'' = 1, u(0) = u(1) = 0
> K = sp.diags([-1.0, 2.0, -1.0], [-1, 0, 1], shape=(n, n), format="csc") / h**2
> print(spsolve(K, np.ones(n)))                # = x(1-x)/2 w węzłach (schemat dokładny dla paraboli)
> ```

> [!przyklad]- Thomas ręcznie: przekątna $2$, pozostałe $-1$, $\mathbf d = (1, 0, 1)$
> - $c'_1 = -\frac12$, $d'_1 = \frac12$.
> - $m_2 = 2 - (-1)(-\frac12) = \frac32$, $c'_2 = -\frac23$, $d'_2 = \frac{0 + \frac12}{3/2} = \frac13$.
> - $m_3 = 2 - (-1)(-\frac23) = \frac43$, $d'_3 = \frac{1 + \frac13}{4/3} = 1$.
> - Wstecz: $x_3 = 1$, $x_2 = \frac13 + \frac23 \cdot 1 = 1$, $x_1 = \frac12 + \frac12 \cdot 1 = 1$.

## W C++ — szkielet eliminacji Gaussa
> [!kod] Najważniejsze pętle (pełna wersja: `rozwiazania/m02.cpp`)
> ```cpp
> Vec gauss(Mat A, Vec b) {                       // kopie: możesz je niszczyć
>     int n = b.size();
>     for (int k = 0; k < n; ++k) {
>         int p = k;                              // 1. wybór elementu głównego w kolumnie k
>         for (int i = k + 1; i < n; ++i) if (fabs(A[i][k]) > fabs(A[p][k])) p = i;
>         swap(A[k], A[p]); swap(b[k], b[p]);
>         for (int i = k + 1; i < n; ++i) {       // 2. zerowanie pod przekątną
>             double m = A[i][k] / A[k][k];
>             for (int j = k; j < n; ++j) A[i][j] -= m * A[k][j];
>             b[i] -= m * b[k];
>         }
>     }
>     Vec x(n);                                   // 3. podstawianie wstecz
>     for (int i = n - 1; i >= 0; --i) {
>         double s = b[i];
>         for (int j = i + 1; j < n; ++j) s -= A[i][j] * x[j];
>         x[i] = s / A[i][i];
>     }
>     return x;
> }
> ```
> - `Mat` to `std::vector<std::vector<double>>`, dostęp `A[i][j]`. Dla dużych siatek MES (Test3: 961 węzłów, macierz 961×961 ≈ 7 MB) to jeszcze działa. Przy 100×100 elementach (10 201 węzłów, ok. 830 MB) potrzebna jest macierz pasmowa albo rzadka.
> - Gauss bez wyboru elementu głównego wywraca się na zerze na przekątnej. Macierz MES z $H_{bc}$ jest symetryczna i dodatnio określona, więc tam działa i bez wyboru, ale wybór niczego nie psuje.

## Typowe błędy
> [!blad] Gauss bez wyboru elementu głównego
> Przy $a_{11} = 0$ dzielisz przez zero, a przy $a_{11} = 10^{-20}$ mnożnik $10^{20}$ zjada wszystkie cyfry. Dla układu $\begin{bmatrix}10^{-20} & 1\\ 1 & 1\end{bmatrix}\mathbf x = (1, 2)$ bez zamiany wychodzi $(0, 1)$ zamiast $(1, 1)$.

> [!blad] Zapominanie o zamianie mnożników w LU
> Gdy w kroku $k$ zamieniasz wiersze, zamień też **już policzone** kolumny $L$ (do $k-1$), inaczej $PA \ne LU$.

> [!blad] Gęsta macierz dla problemu 1D/2D
> `np.zeros((n, n))` dla $n = 10^5$ to 80 GB. Trójdiagonalną trzymaj jako trzy wektory, a macierz MES jako `scipy.sparse`.

> [!blad] Jacobi na macierzy bez dominacji przekątniowej
> Dla $\begin{bmatrix}1 & 2\\ 3 & 1\end{bmatrix}$ iteracje rosną wykładniczo. Zawsze dodaj `maxit` i wykrywanie rozbieżności.

## Na zajęcia i egzamin
> [!zajecia]
> - **MES (projekt 2D nieustalony):** po agregacji masz $\left(\frac{C}{\Delta t} + K\right)\mathbf T^{n+1} = \frac{C}{\Delta t}\mathbf T^{n} + \mathbf P$. Macierz po lewej jest stała, więc rozkład LU robisz raz, a w pętli po czasie tylko podstawienia. W 1D (pręt) macierz jest trójdiagonalna, więc Thomas. Macierz sztywności jest symetryczna dodatnio określona, a podwyższone uwarunkowanie przy bardzo drobnej siatce wyjaśnia, skąd biorą się ostatnie cyfry szumu.
> - **Programowanie równoległe:** Jacobi to klasyczny przykład problemu „embarrassingly parallel” w obrębie iteracji (każdy wątek liczy swoje składowe), a Gauss–Seidel wymaga kolorowania (red-black), żeby go zrównoleglić.
> - **Optymalizacja:** krok Newtona $H\Delta = -\nabla f$ to układ liniowy.
> - **Egzamin:** „po co wybór elementu głównego”, „koszt Gaussa i Thomasa”, „warunek zbieżności Jacobiego”, „co mówi wskaźnik uwarunkowania”.
> - **Ćwicz w C++:** `Kod/metody_numeryczne/zadania/m02.cpp` (Gauss z wyborem elementu głównego, LU, Thomas, Gauss–Seidel, residuum), specyfikacja w `m02.h`, sprawdzasz `./sprawdz.sh 02`. Wzorzec: `rozwiazania/m02.cpp`, dopiero po własnej próbie. Ten moduł jest potrzebny w m04, m07, m08 i m10, więc zrób go wcześnie.

## Zadania
> [!zadanie] Zadanie 1
> Ile razy Thomas jest tańszy od gęstego Gaussa dla $n = 10^4$?
> > [!odpowiedz]- Odpowiedź
> > Gauss: $\frac23 n^3 \approx 6{,}7\cdot10^{11}$ działań. Thomas: $8n = 8\cdot10^4$. Różnica ok. $10^7$ razy, a do tego gęsta macierz to 800 MB pamięci.

> [!zadanie] Zadanie 2
> Zrób pierwszy krok Jacobiego od $\mathbf x^{(0)} = \mathbf 0$ dla układu $10x_1 - x_2 + 2x_3 = 6$, $-x_1 + 11x_2 - x_3 + 3x_4 = 25$, $2x_1 - x_2 + 10x_3 - x_4 = -11$, $3x_2 - x_3 + 8x_4 = 15$.
> > [!odpowiedz]- Odpowiedź
> > $\mathbf x^{(1)} = \mathbf b / \operatorname{diag}(A) = (0{,}6;\ 2{,}2727;\ -1{,}1;\ 1{,}875)$. Macierz jest ściśle diagonalnie dominująca, więc metoda zbiega do $(1;\ 2;\ -1;\ 1)$: Jacobi w 29 iteracjach, Gauss–Seidel w 12 (tol $10^{-10}$).

> [!zadanie] Zadanie 3
> $A = \begin{bmatrix}1 & 1\\ 1 & 1{,}0001\end{bmatrix}$, $\mathbf b = (2;\ 2{,}0001)$ daje $\mathbf x = (1;\ 1)$. Co się stanie, gdy $b_2 = 2{,}0002$? Policz $\operatorname{cond}_\infty$.
> > [!odpowiedz]- Odpowiedź
> > $\mathbf x = (0;\ 2)$: zmiana danych o $5\cdot10^{-5}$ (względnie) zmienia wynik o 100%. $\operatorname{cond}_\infty(A) \approx 40\,004$, więc wzmocnienie rzędu $10^4$ jest zgodne z oszacowaniem.

> [!zadanie] Zadanie 4
> Dlaczego w MES nieustalonym opłaca się LU zamiast `solve` w każdym kroku czasu?
> > [!odpowiedz]- Odpowiedź
> > Przy stałym $\Delta t$ i liniowym problemie macierz układu się nie zmienia. `solve` za każdym razem robi rozkład $O(n^3)$, a z gotowym LU każdy krok to dwa podstawienia $O(n^2)$ (dla macierzy rzadkich jeszcze mniej).

## Fiszki #fiszki/numeryczne
Na czym polega częściowy wybór elementu głównego?::W kolumnie k zamieniasz wiersz k z wierszem o największym |a_ik| (i ≥ k), wtedy mnożniki mają moduł ≤ 1
Jaki jest koszt eliminacji Gaussa dla macierzy n×n?::Około 2n³/3 działań, czyli O(n³)
Co daje rozkład PA = LU?::Rozkład liczysz raz w O(n³), potem każde nowe b to Ly = Pb i Ux = y w O(n²)
Jaki jest koszt algorytmu Thomasa?::O(n) działań i pamięci (ok. 8n), dla macierzy trójdiagonalnej
Kiedy Thomas działa bez wyboru elementu głównego?::Gdy macierz jest diagonalnie dominująca albo symetryczna dodatnio określona (typowe w 1D MES)
Wzór iteracji Jacobiego::x^(k+1) = D⁻¹(b − (L + U)x^(k)), każda składowa liczona ze starego wektora
Czym Gauss–Seidel różni się od Jacobiego?::Od razu używa nowych składowych x_j dla j < i: zwykle szybszy, ale sekwencyjny
Wystarczający warunek zbieżności Jacobiego i Gaussa–Seidla?::Ścisła dominacja przekątniowa: |a_ii| > suma |a_ij| po j ≠ i w każdym wierszu
Definicja wskaźnika uwarunkowania macierzy::cond(A) = ‖A‖·‖A⁻¹‖, ograniczenie wzmocnienia błędu względnego danych w rozwiązaniu
Ile cyfr tracisz przy cond(A) ≈ 10^k?::Około k cyfr z mniej więcej 16 dostępnych w double
Dlaczego nie liczyć x = inv(A) @ b?::Wolniej i mniej dokładnie niż solve, a odwrotność macierzy rzadkiej jest gęsta
Czym rozwiązać duży rzadki układ w Pythonie?::scipy.sparse (CSR/CSC, np. sp.diags) i scipy.sparse.linalg.spsolve

Trzy etapy eliminacji Gaussa w kodzie::Wybór elementu głównego (zamiana wierszy), zerowanie pod przekątną, podstawianie wstecz
Złożoność eliminacji Gaussa i algorytmu Thomasa::Gauss O(n³), Thomas O(n) dla trójdiagonalnej

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.

## Źródła
- Z. Fortuna, B. Macukow, J. Wąsowski, *Metody numeryczne*, WNT — rozdz. o układach równań liniowych (metody dokładne i iteracyjne).
- D. Kincaid, W. Cheney, *Analiza numeryczna*, WNT — rozkład LU, wybór elementu głównego, metody iteracyjne, uwarunkowanie.
- R. L. Burden, J. D. Faires, *Numerical Analysis*, Brooks/Cole — rozdz. 6 (Direct Methods for Solving Linear Systems), rozdz. 7 (Iterative Techniques in Matrix Algebra; przykład 4×4 dla Jacobiego).
- Dokumentacja NumPy: `numpy.linalg.solve`, `numpy.linalg.cond`.
- Dokumentacja SciPy: `scipy.linalg.lu_factor`, `scipy.linalg.solve_banded`, `scipy.sparse.diags`, `scipy.sparse.linalg.spsolve`.
- Kurs MN w C++: Kod/metody_numeryczne (moduły, testy, rozwiązania wzorcowe)
