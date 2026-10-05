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

## Laby z układów równań — co oddajesz i wyniki do sprawdzenia
Instrukcje: [[Lab — eliminacja Gaussa.pdf]], [[Lab — Gauss z wyborem elementu (pivoting, Crout).pdf]], [[Lab — LU Doolittle'a i skalowanie.pdf]], [[Lab — metoda Jacobiego.pdf]]. Wszędzie dane czytasz **z pliku**, a `.cpp` wrzucasz na UPEL.

### Eliminacja Gaussa (6 + 4 pkt)
> [!przyklad] Przykład z instrukcji: $[A \mid b]$ i wynik postępowania prostego
> $$\left[\begin{array}{cccc|c} 2 & 4 & 2 & 1 & 10 \\ 2 & 2 & 3 & 3 & 6 \\ 4 & 2 & 2 & 1 & 6 \\ 0 & 2 & 1 & 1 & 4 \end{array}\right] \;\to\; \left[\begin{array}{cccc|c} 2 & 4 & 2 & 1 & 10 \\ 0 & -2 & 1 & 2 & -4 \\ 0 & 0 & -5 & -7 & -2 \\ 0 & 0 & 0 & 0{,}2 & -0{,}8 \end{array}\right]$$
> Mnożniki: $m_{10} = 1$, $m_{20} = 2$, $m_{30} = 0$, potem $m_{21} = 3$, $m_{31} = -1$, na końcu $m_{32} = -0{,}4$. Postępowanie odwrotne: $x_3 = -0{,}8/0{,}2 = -4$, $x_2 = \frac{-2 - (-7)(-4)}{-5} = 6$, $x_1 = \frac{-4 - (1 \cdot 6 + 2 \cdot (-4))}{-2} = 1$, $x_0 = \frac{10 - (4 + 12 - 4)}{2} = -1$. **Wynik: $x = (-1, 1, 6, -4)$.**

> [!zadanie] Zad. 2: dwa układy, na których metoda się psuje (każdy z innego powodu)
> 1. **Zero na przekątnej przy macierzy nieosobliwej:** $\begin{bmatrix} 0 & 1 \\ 1 & 1 \end{bmatrix} x = \begin{bmatrix} 1 \\ 2 \end{bmatrix}$ (rozwiązanie $x = (1, 1)$ istnieje, ale $m_{10} = 1/0$). Ten sam efekt w połowie eliminacji daje macierz z labu „pivoting” (po pierwszym kroku $a_{11} = 0$).
> 2. **Macierz osobliwa:** $\begin{bmatrix} 1 & 2 \\ 2 & 4 \end{bmatrix}$ — po eliminacji $a_{11} = 0$, bo rozwiązań nie ma albo jest ich nieskończenie wiele. Tego nie naprawi żaden wybór elementu.
> 3. (zapasowy) **Mały element główny, zły wynik bez błędu:** $\begin{bmatrix} 10^{-17} & 1 \\ 1 & 1 \end{bmatrix} x = \begin{bmatrix} 1 \\ 2 \end{bmatrix}$. Dokładnie $x \approx (1, 1)$, a Gauss bez wyboru daje $x_0 = 0$, bo $1 - 10^{17}$ zjada jedynkę.
> 4. (zapasowy) **Złe uwarunkowanie:** układ z labu LU niżej — metoda działa, ale wynik ma błąd kilku procent.

### Wybór elementu głównego (3 + 3 + 4 pkt)
> [!przyklad] Układ z instrukcji
> $$\left[\begin{array}{cccc|c} 2 & 4 & 2 & 1 & 10 \\ 1 & 2 & 3 & 3 & 6 \\ 4 & 5 & 2 & 1 & 6 \\ 0 & 1 & 2 & 9 & 1 \end{array}\right], \qquad x = (-4{,}011905;\ 4{,}023810;\ 1{,}273810;\ -0{,}619048)$$
> - **Bez wyboru** nie działa: po pierwszym kroku wiersz 1 to $(0, 0, 2, 2{,}5 \mid 1)$, czyli zero na przekątnej.
> - **Wybór w kolumnie (zamiana wierszy):** w kolumnie 0 największe jest 4, więc wiersz 2 idzie na górę. Dalej tak samo w każdej kolumnie.
> - **Gauss–Crout (zamiana kolumn):** w wierszu 0 największe jest 4 w kolumnie 1, więc wektor kolumn zaczyna się od $[1, 0, 2, 3]$. Kolumn nie przestawiasz w pamięci: odwołujesz się `A[i][kol[j]]`, a na końcu wynik z kolumny $j$ wpisujesz do `x[kol[j]]`.
> - **Zad. 3 (tabelka):** dla macierzy z załącznika prowadzącego zrób kolumny „bez wyboru / wiersze / kolumny” i wpisuj ✓ albo ✗ (zero na przekątnej, zły wynik). Jako „zły wynik” sprawdzaj residuum $\|Ax - b\|$ i porównanie z drugą metodą. Pliku z tymi macierzami nie mam, więc tabeli nie policzyłem.

### LU Doolittle'a i skalowanie
> [!przyklad] Przykład z instrukcji: $A = \begin{bmatrix} 1 & 2 & 3 \\ 4 & 5 & 6 \\ 7 & 8 & 10 \end{bmatrix}$, $b = (6, 8, 3)$
> $$L = \begin{bmatrix} 1 & 0 & 0 \\ 4 & 1 & 0 \\ 7 & 2 & 1 \end{bmatrix}, \quad U = \begin{bmatrix} 1 & 2 & 3 \\ 0 & -3 & -6 \\ 0 & 0 & 1 \end{bmatrix}, \quad y = (6, -16, -7), \quad x = (-11{,}667;\ 19{,}333;\ -7)$$
> Format pliku z instrukcji: najpierw $n$ wierszy macierzy, potem $n$ liczb wektora $b$. Rozmiar $n$ wyznaczysz z liczby liczb w pierwszym wierszu albo dopisz go na początku pliku.

> [!przyklad] Zad. 2: $A = \begin{bmatrix} 1 & 1 \\ 1 & 1{,}00000000000001 \end{bmatrix}$, $b = (2;\ 2{,}00000000000001)$, dokładnie $x = (1, 1)$
> | wariant | $u_{22}$ | $x$ (double, bez pivotingu) |
> |---|---|---|
> | oryginał | $9{,}992\cdot10^{-15}$ (zamiast $10^{-14}$) | (0,977778; 1,022222) |
> | ×$10^{14}$ mnożone w programie | 1 | (0,96875; 1,03125) |
> | ×$10^{14}$ wpisane do pliku (`1e14`, `100000000000001`, `200000000000001`) | 1 | **(1; 1) dokładnie** |
> | ×$10^{-14}$ | $9{,}94\cdot10^{-29}$ | (0,984127; 1,015873) |
> | dla porównania: `long double` | | (1,000000000000000; 1,000000000000000) |
>
> **Odpowiedzi na część 4:**
> 1. **Tak.** Pomnożenie obu stron równań przez tę samą liczbę nie zmienia rozwiązania: zawsze $x = (1, 1)$.
> 2. **Nie.** Każdy wariant daje inne $x$, błąd od 0 do ok. 3%.
> 3. Najdokładniej: skalowanie w górę **z danymi wpisanymi dokładnie**, bo wszystkie liczby są wtedy całkowite i dokładnie reprezentowalne. Najgorzej: skalowanie w górę przez mnożenie w programie (3,1%). Oryginał ma 2,2%, a skalowanie w dół 1,6%.
> 4. **Dlaczego skalowanie wpływa na wynik:** $1{,}00000000000001$ nie ma dokładnej reprezentacji binarnej (w double to $1{,}0000000000000099920$). Mnożenie przez $10^{\pm14}$, które nie jest potęgą dwójki, zaokrągla każdą liczbę od nowa, więc każdy wariant ma **inne błędy danych**. Układ jest prawie osobliwy: $\operatorname{cond}(A) \approx 4/10^{-14} = 4\cdot10^{14}$. Błąd względny danych ok. $10^{-16}$ rośnie więc do ok. $4\cdot10^{-2}$, czyli kilku procent. Skalowanie przez potęgę dwójki (np. $2^{47}$) daje wynik identyczny jak oryginał, co potwierdza, że winne są zaokrąglenia, a nie sam rozmiar liczb.
> 5. **Krok z problemem:** $u_{22} = a_{22} - l_{21}u_{12} = 1{,}00000000000001 - 1$ (i tak samo $y_2 = b_2 - l_{21}y_1$). To **odejmowanie prawie równych liczb**: z 16 cyfr zostają 1–2 poprawne. Potem $x_2 = y_2/u_{22}$ dzieli dwie zaszumione małe liczby.
> 6. **Jak temu zaradzić:** pivoting tu **nie pomaga** (element główny i tak wynosi 1, a $l_{21} = 1$), bo problemem jest złe uwarunkowanie, a nie mały dzielnik. Pomaga: większa precyzja (`long double`, `__float128`), podanie danych w postaci dokładnie reprezentowalnej, skalowanie przez potęgi dwójki zamiast $10^k$, iteracyjne poprawianie rozwiązania (residuum liczone w wyższej precyzji), a najlepiej przeformułowanie modelu, żeby macierz nie była prawie osobliwa.

### Metoda Jacobiego (4 + 2 + 4 pkt)
> [!definicja] Wzory z instrukcji
> $A = L + D + U$, a stąd $x^{(i+1)} = -D^{-1}(L + U)x^{(i)} + D^{-1}b$, start $x^{(0)} = 0$. W kodzie bez macierzy: $x_i^{nowe} = \frac{b_i - \sum_{j \ne i} a_{ij}x_j^{stare}}{a_{ii}}$. **Wszystkie** składowe liczysz ze starego wektora (w Gauss–Seidlu od razu z nowych). Zbieżna dla macierzy nieredukowalnej, diagonalnie słabo dominującej: $|a_{ii}| \ge \sum_{j \ne i}|a_{ij}|$ w każdym wierszu i $>$ w co najmniej jednym.

> [!przyklad] Mały test: $A = \begin{bmatrix} 4 & -1 & 1 \\ -1 & 4 & -2 \\ 1 & -2 & 4 \end{bmatrix}$, $b = (12, -1, 5)$, dokładnie $x = (3, 1, 1)$
> - Dominacja: $4 \ge 2$, $4 \ge 3$, $4 \ge 3$, wszędzie ostro, więc zbieżna.
> - Pierwsza iteracja z $x = 0$: $x^{(1)} = D^{-1}b = (3;\ -0{,}25;\ 1{,}25)$.
> - $\varepsilon = 10^{-3}$: **21 iteracji**, $x = (3{,}000211;\ 0{,}999711;\ 1{,}000288)$, błąd $\approx 3\cdot10^{-4}$.
> - $\varepsilon = 10^{-6}$: **39 iteracji**, błąd $\approx 3\cdot10^{-7}$.
> - Gauss–Seidel na tej samej macierzy zbiega w mniej więcej połowie iteracji.

> [!przyklad] Zad. 3: macierz 1000×1000 (pomiar na tym samym kodzie, g++ -O2)
> Macierz losowa: $a_{ij} \in [-1, 1]$, $a_{ii} = \sum_{j \ne i}|a_{ij}| + 1$ (mocna dominacja), $b$ losowe.
>
> | metoda | $\varepsilon$ | iteracje | czas | $\lVert Ax - b\rVert_\infty$ |
> |---|---|---|---|---|
> | Jacobi | $10^{-3}$ | 2 | 1,5 ms | $2{,}3\cdot10^{-3}$ |
> | Jacobi | $10^{-6}$ | 4 | 3,0 ms | $3{,}1\cdot10^{-6}$ |
> | Jacobi | $10^{-10}$ | 7 | 4,8 ms | $1{,}6\cdot10^{-10}$ |
> | Gauss z wyborem | — | — | 177 ms | $4{,}3\cdot10^{-15}$ |
>
> Komentarz: iteracja Jacobiego kosztuje $O(n^2)$, a Gauss $O(n^3)$, więc przy silnej dominacji Jacobi jest kilkadziesiąt razy szybszy, a błąd ustawiasz przez $\varepsilon$. Przy słabej dominacji iteracji jest dużo więcej. Czas mierz `std::chrono::steady_clock` (przykład w notatce [[Kompilacja i pomiar czasu wykonania]]). Twoje czasy będą inne, ale proporcje podobne.

> [!kod]- Wczytywanie macierzy rozszerzonej z pliku (wspólne dla wszystkich labów)
> ```cpp
> #include <fstream>
> // plik: n, potem n wierszy po n + 1 liczb (A i b)
> std::ifstream we("dane.txt");
> int n; we >> n;
> Mat A(n, Vec(n)); Vec b(n);
> for (int i = 0; i < n; ++i) {
>     for (int j = 0; j < n; ++j) we >> A[i][j];
>     we >> b[i];
> }
> void wypisz(const Mat& M) {
>     for (auto& w : M) { for (double v : w) std::printf("%10.4f ", v); std::printf("\n"); }
> }
> ```

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
> - **Ćwicz w C++:** `Kod/metody_numeryczne/zadania/m02.cpp` (Gauss bez wyboru: postępowanie proste i odwrotne; Gauss z wyborem w wierszach; Gauss–Crout z wektorem kolumn; LU Doolittle'a z L, U, y; Jacobi z warunkiem dominacji; Gauss–Seidel; Thomas; residuum), specyfikacja w `m02.h`, sprawdzasz `./sprawdz.sh 02`. Wzorzec: `rozwiazania/m02.cpp`, dopiero po własnej próbie. Ten moduł jest potrzebny w m04, m07, m08 i m10, więc zrób go wcześnie.

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

Rozwiązanie przykładu z labu „Eliminacja Gaussa” (2 4 2 1 | 10 …)::x = (−1, 1, 6, −4)
Dwa różne powody, dla których Gauss bez wyboru nie działa::Zero na przekątnej przy macierzy nieosobliwej (pomaga pivoting) albo macierz osobliwa (nic nie pomoże)
Czym różni się wybór częściowy po wierszach od metody Gaussa–Crouta?::Wiersze: największy |a_ik| w kolumnie, zamiana wierszy. Crout: największy |a_kj| w wierszu, zamiana kolumn zapamiętana w wektorze numerów kolumn
Dlaczego w Croucie trzeba pamiętać wektor kolumn?::Zamiana kolumn zmienia kolejność niewiadomych, więc przy podstawianiu wstecz x[kol[j]] wraca na właściwe miejsce
Który krok LU psuje wynik dla A = [[1, 1], [1, 1,00000000000001]]?::u22 = a22 − l21·u12: odejmowanie prawie równych liczb, cond(A) ≈ 4·10¹⁴
Czy pivoting naprawi układ [[1, 1], [1, 1 + 1e-14]]?::Nie, problemem jest złe uwarunkowanie; pomaga wyższa precyzja, dokładne dane, skalowanie potęgą 2
Wzór iteracji Jacobiego::x⁽ⁱ⁺¹⁾ = −D⁻¹(L + U)x⁽ⁱ⁾ + D⁻¹b; każda składowa ze starego wektora
Warunek zbieżności Jacobiego z instrukcji::Macierz nieredukowalna, diagonalnie słabo dominująca: |a_ii| ≥ Σ|a_ij| w każdym wierszu i > w co najmniej jednym
Koszt iteracji Jacobiego a eliminacji Gaussa::Iteracja O(n²), Gauss O(n³)

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.

## Źródła
- Z. Fortuna, B. Macukow, J. Wąsowski, *Metody numeryczne*, WNT — rozdz. o układach równań liniowych (metody dokładne i iteracyjne).
- D. Kincaid, W. Cheney, *Analiza numeryczna*, WNT — rozkład LU, wybór elementu głównego, metody iteracyjne, uwarunkowanie.
- R. L. Burden, J. D. Faires, *Numerical Analysis*, Brooks/Cole — rozdz. 6 (Direct Methods for Solving Linear Systems), rozdz. 7 (Iterative Techniques in Matrix Algebra; przykład 4×4 dla Jacobiego).
- Dokumentacja NumPy: `numpy.linalg.solve`, `numpy.linalg.cond`.
- Dokumentacja SciPy: `scipy.linalg.lu_factor`, `scipy.linalg.solve_banded`, `scipy.sparse.diags`, `scipy.sparse.linalg.spsolve`.
- Kurs MN w C++: Kod/metody_numeryczne (moduły, testy, rozwiązania wzorcowe)
- Instrukcje: Lab — eliminacja Gaussa; Lab — Gauss z wyborem elementu (pivoting, Crout); Lab — LU Doolittle'a i skalowanie; Lab — metoda Jacobiego
