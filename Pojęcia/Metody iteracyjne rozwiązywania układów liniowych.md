---
typ: pojęcie
status: nowe
przedmioty: ["[[AGH Metody numeryczne]]", "[[AGH Metoda elementów skończonych]]", "[[AGH Programowanie równoległe]]"]
wymaga: ["[[Układy równań liniowych]]"]
powiązane: ["[[Uwarunkowanie zadania i stabilność algorytmów]]", "[[Macierz sztywności]]", "[[Metoda najszybszego spadku]]", "[[Metoda gradientów sprzężonych]]"]
tags: [pojęcie, numeryczne, wykład]
utworzono: 2026-10-05
źródła: ["D. Szeliga — Metody numeryczne, wykład 2: Układy równań liniowych (AGH 2025/26)", "Y. Saad — Iterative Methods for Sparse Linear Systems, 2nd ed. (SIAM 2003, darmowy PDF autora)"]
---
# Metody iteracyjne rozwiązywania układów liniowych

> [!skrot] W 30 sekund
> - **Co to:** zamiast dokładnie rozkładać $A$ (Gauss, LU: $O(n^3)$), poprawiasz przybliżenie $x^{(k)}$ krokami, w których najdroższe jest mnożenie przez $A$. Dla **macierzy rzadkich** (MES, różnice skończone) to często jedyna wykonalna droga.
> - **Klucz:** rozkład $A = M - Z$, iteracja $x^{(k+1)} = M^{-1}(Zx^{(k)} + b) = Bx^{(k)} + c$, gdzie $B = I - M^{-1}A$. **Zbieżna dla każdego startu ⇔ $\rho(B) < 1$**, a wystarczy $\|B\| < 1$.
> - **Metody:** Jacobi ($M = D$), Gauss–Seidel ($M = L + D$), SOR (z parametrem $\omega$), Richardson ($M = \frac1\tau I$), projekcyjne (najszybszy spadek, najmniejsze residuum), Kryłowa (CG, GMRES).

> [!intuicja] Po ludzku
> Gauss to rozebranie silnika na części i złożenie od nowa: dokładne, ale przy milionie części trwałoby 32 lata. Metoda iteracyjna to regulowanie gaźnika: kręcisz, słuchasz, kręcisz znowu. Każdy obrót jest tani. Pytanie brzmi tylko, czy i jak szybko dojdziesz do celu, a to mówi promień spektralny macierzy iteracji.

## Po co: macierze rzadkie (wykład)
> [!wzor] Liczby z wykładu
> Kilka milionów niewiadomych, macierz bez struktury: Gauss potrzebuje rzędu $10^{19}$ działań i $10^{13}$ B (10 000 GB) pamięci na czynniki rozkładu. Przy $10^{10}$ działań/s to $10^9$ s, czyli **ok. 32 lata**. Rozkład LU **wypełnia zera** (*fill-in*), więc traci rzadkość.
>
> Skąd rzadkie macierze: dyskretyzacja równań różniczkowych (MES, różnice skończone), łańcuchy Markowa. Typowo $O(N)$ niezerowych, np. ok. 10% albo mniej.

> [!przyklad] 1D laplasjan: $-u'' = f$ na $(0, 1)$, $u(0) = u(1) = 0$
> Różnice skończone $-\frac{u_{i-1} - 2u_i + u_{i+1}}{h^2} = f(x_i)$, $h = \frac{1}{N+1}$, dają $T_N U = h^2 F$ z macierzą trójdiagonalną
> $$T_N = \begin{bmatrix} 2 & -1 & & \\ -1 & 2 & \ddots & \\ & \ddots & \ddots & -1 \\ & & -1 & 2\end{bmatrix}$$
> Niecałe $3N$ niezerowych (wypełnienie $\approx 3/N$). Jest symetryczna i dodatnio określona, więc Gauss/LDLᵀ z uwzględnieniem pasma rozwiąże ją kosztem $O(N)$ (algorytm Thomasa).
>
> **2D laplasjan** na siatce $n \times n$ ($N = n^2$): schemat pięciopunktowy $\frac{4u_{ij} - u_{i\pm1,j} - u_{i,j\pm1}}{h^2} = f_{ij}$ daje macierz blokową pięciodiagonalną $P_N = I \otimes T_n + T_n \otimes I$ (iloczyn Kroneckera). Gauss z pasmem szerokości $2n$ kosztuje $O(n^4)$, co przy dużym $n$ jest za drogie. Tu wchodzą metody iteracyjne.

## Definicja i wzory
> [!definicja] Stacjonarne metody iteracyjne
> $A = M - Z$, $M$ łatwo odwracalna i bliska $A$. Zadanie punktu stałego $x = M^{-1}(Zx + b)$ rozwiązujesz iteracją prostą Banacha:
> $$x^{(k+1)} = Bx^{(k)} + c, \qquad B = M^{-1}Z = I - M^{-1}A, \qquad c = M^{-1}b$$
> Błąd: $x^{(k+1)} - x^* = B(x^{(k)} - x^*) = B^{k+1}(x^{(0)} - x^*)$.
> - **Warunek wystarczający:** $\|B\| < 1$ (dowolna norma indukowana), wtedy zbieżność co najmniej liniowa: $\|x^{(k+1)} - x^*\| \le \|B\|\,\|x^{(k)} - x^*\|$.
> - **Warunek konieczny i wystarczający:** $\rho(B) = \max|\lambda_i(B)| < 1$ (promień spektralny).
> - Szybkość: liczba iteracji do zmniejszenia błędu 10× to ok. $\frac{\ln 0{,}1}{\ln\rho(B)}$.

> [!definicja] Rozkład $A = L + D + U$ (ściśle dolna, przekątna, ściśle górna)
> | metoda | $M$ | iteracja | uwagi |
> |---|---|---|---|
> | **Jacobi** | $D$ | $x^{(k+1)} = D^{-1}(b - (L+U)x^{(k)})$, czyli $x_i^{(k+1)} = \frac{1}{a_{ii}}\big(b_i - \sum_{j\ne i}a_{ij}x_j^{(k)}\big)$ | w pełni **równoległa** (składowe niezależne) |
> | **Gauss–Seidel** | $L + D$ | $x^{(k+1)} = (L+D)^{-1}(b - Ux^{(k)})$, czyli $x_i^{(k+1)} = \frac{1}{a_{ii}}\big(b_i - \sum_{j<i}a_{ij}x_j^{(k+1)} - \sum_{j>i}a_{ij}x_j^{(k)}\big)$ | używa od razu nowych wartości; wariant z górną trójkątną też istnieje |
> | **SOR** | $\frac1\omega D + L$ | $x_i^{(k+1)} = (1-\omega)x_i^{(k)} + \omega\,\tilde x_i^{GS}$ | $\omega = 1$: Gauss–Seidel; $\omega > 1$: **nadrelaksacja**, „najlepsza metoda stacjonarna” |
> | **klasyczna** | $I$ | $x^{(k+1)} = (I - A)x^{(k)} + b$ | |
> | **Richardson** | $\frac1\tau I$ | $x^{(k+1)} = x^{(k)} + \tau(b - Ax^{(k)})$ | parametr relaksacji $\tau$ |

> [!wzor] Twierdzenia o zbieżności (wykład)
> - **Jacobi:** $\|D^{-1}(L+U)\|_\infty < 1$, gdy $A$ ma **dominującą przekątną** ($|a_{ii}| > \sum_{j\ne i}|a_{ij}|$).
> - **Gauss–Seidel:** zbieżny dla dowolnego $x^{(0)}$, gdy $A$ jest **diagonalnie dominująca**.
> - **SOR (Ostrowski–Reich):** jeśli $A = A^T > 0$, SOR jest zbieżny dla **każdego $\omega \in (0, 2)$**.
> - **Richardson:** dla $A$ nieokreślonej nie jest zbieżny dla żadnego $\tau$ (dla dowolnego startu). Dla $A > 0$ zbieżny przy $0 < \tau < \frac{2}{\lambda_{max}}$, optymalnie $\tau_{opt} = \frac{2}{\lambda_{max} + \lambda_{min}}$.

> [!definicja] Metody projekcji
> Poprawka $x^{(k+1)} = x^{(k)} + \delta_k$. Idealna poprawka spełnia $A\delta^* = r_k$ (residuum $r_k = b - Ax^{(k)}$), ale to ten sam układ. Rozwiązujesz go więc w małej podprzestrzeni: $\delta_k = V_k a_k$, gdzie
> $$U_k^TAV_k\,a_k = U_k^Tr_k \qquad (\text{macierz zredukowana } d_k\times d_k)$$
> Residuum równania poprawki jest prostopadłe do kolumn $U_k$ (stąd „projekcja”). Wybór: $A = A^T > 0$ → $U_k = V_k$; $A$ tylko nieosobliwa → $U_k = AV_k$ (równania normalne $A^TA$). To są **metody minimalizacji** residuum.
>
> | metoda | założenie | wybór | krok |
> |---|---|---|---|
> | **najszybszy spadek** | $A = A^T > 0$ | $U_k = V_k = r_k$ | $a_k = \frac{r_k^Tr_k}{r_k^TAr_k}$, $x^{(k+1)} = x^{(k)} + a_kr_k$ |
> | **najmniejsze residuum** | $A$ nieosobliwa | $V_k = r_k$, $U_k = Ar_k$ | $a_k = \frac{r_k^TA^Tr_k}{r_k^TA^TAr_k}$ |
>
> Najszybszy spadek minimalizuje $\varphi(x) = \|x^* - x\|_A^2$, a $r_k$ to kierunek jego spadku. Zbieżność: $\|x^* - x^{(k+1)}\|_A \le \frac{\kappa - 1}{\kappa + 1}\|x^* - x^{(k)}\|_A$, $\kappa = \operatorname{cond}_2(A) = \lambda_{max}/\lambda_{min}$. Najmniejsze residuum: $\|r_{k+1}\|_2 \le \left(1 - \frac{\mu^2}{\|A\|_2^2}\right)^{1/2}\|r_k\|_2$, gdzie $\mu = \lambda_{min}\big(\frac{A + A^T}{2}\big) > 0$. Dalej: metody Kryłowa (CG to „najszybszy spadek z pamięcią”).

## Jak to zrobić — krok po kroku
1. Sprawdź strukturę: dominacja przekątnej? symetria i dodatnia określoność? rzadkość?
2. Dominująca przekątna → Jacobi (równolegle) albo Gauss–Seidel (mniej iteracji). Symetryczna dodatnio określona → SOR z $\omega \in (1, 2)$ albo CG.
3. Kryterium stopu: $\|x^{(k+1)} - x^{(k)}\| < \varepsilon$ **i** $\|b - Ax^{(k+1)}\| / \|b\| < \varepsilon$ plus limit iteracji.
4. Mała gęsta macierz ($n \lesssim$ kilkaset): metoda bezpośrednia jest prostsza i szybsza.

## Przykład
> [!przyklad] Jacobi na 1D laplasjanie $T_n$ (wolna zbieżność)
> $M = 2I$, $Z = 2I - T_n$, macierz iteracji $B = \frac12 Z = I - \frac12 T_n$. $\|B\|_\infty = 1$, więc warunek wystarczający **nie rozstrzyga**. Wartości własne $T_n$: $\lambda_j = 2\big(1 - \cos\frac{j\pi}{n+1}\big) = 4\sin^2\frac{j\pi}{2(n+1)} \in (0, 4)$, więc $\mu_j = 1 - \frac{\lambda_j}{2} \in (-1, 1)$ i Jacobi jest zbieżny. Ale
> $$\rho(B) = \cos\frac{\pi}{n+1} = 1 - \frac{\pi^2}{2(n+1)^2} + O(n^{-4})$$
> Dla $n = 100$: $\rho \approx 0{,}99952$, a redukcja błędu tylko 10× wymaga $\frac{\ln 0{,}1}{\ln\rho} \approx$ **4600–4800 iteracji** (wykład podaje ok. 4600 z zaokrąglonego $\rho \approx 0{,}9995$). Metoda bezpośrednia (Thomas) zrobi to samo w $O(n)$ działań. Gauss–Seidel jest tu ok. 2× szybszy, ale to nadal za wolno.

> [!przyklad]- Zadanie 2 z wykładu: $A = \begin{bmatrix}1 & \alpha & \alpha\\ \alpha & 1 & \alpha \\ \alpha & \alpha & 1\end{bmatrix}$
> (Na slajdzie w prawym dolnym rogu jest $\alpha$, co jest literówką: wynik z treści zadania wymaga jedynki.)
> - Wartości własne: $1 + 2\alpha$ (wektor $(1,1,1)$) i $1 - \alpha$ (podwójna). $A > 0 \Leftrightarrow 1 + 2\alpha > 0$ i $1 - \alpha > 0 \Leftrightarrow -1 < 2\alpha < 2$.
> - Macierz iteracji Jacobiego: $B = -\begin{bmatrix}0 & \alpha & \alpha\\ \alpha & 0 & \alpha \\ \alpha & \alpha & 0\end{bmatrix}$, wartości własne $-2\alpha$ i $\alpha$ (podwójna). $\rho(B) < 1 \Leftrightarrow |2\alpha| < 1 \Leftrightarrow -1 < 2\alpha < 1$.
> - Wniosek: dla $\frac12 \le \alpha < 1$ macierz jest dodatnio określona (Gauss–Seidel i SOR działają), a **Jacobi nie jest zbieżny**.
> - Uogólnienie na $n\times n$ (jedynki na przekątnej, reszta $\alpha$): wartości własne $B$ to $-(n-1)\alpha$ i $\alpha$, więc Jacobi zbieżny $\Leftrightarrow |\alpha| < \frac{1}{n-1}$, a $A > 0 \Leftrightarrow -\frac{1}{n-1} < \alpha < 1$.

## Typowe błędy
> [!blad] Pułapki
> - „$\|B\| \ge 1$, więc rozbieżna”: nie. $\|B\| < 1$ to warunek **wystarczający**, rozstrzyga dopiero $\rho(B)$.
> - Brak dominacji przekątnej nie oznacza automatycznie rozbieżności (np. $T_n$: dominacja tylko słaba, a Jacobi i tak zbiega).
> - W Gauss–Seidlu liczenie z kopii starego wektora: to wtedy jest Jacobi.
> - SOR z $\omega \ge 2$ albo $\omega \le 0$: dla macierzy symetrycznych dodatnio określonych zbieżność gwarantuje tylko $\omega \in (0, 2)$.

## Na zajęcia i egzamin
> [!zajecia]
> - **Egzamin (test):** postać ogólna $x^{(k+1)} = Bx^{(k)} + c$; warunek konieczny i wystarczający $\rho(B) < 1$; $M$ dla Jacobiego, Gauss–Seidla, SOR; twierdzenie Ostrowskiego–Reicha; $\tau_{opt}$ Richardsona; wzór na krok najszybszego spadku; czemu metody iteracyjne dla macierzy rzadkich.
> - **Lab Jacobiego:** [[Układy równań liniowych#Metoda Jacobiego (4 + 2 + 4 pkt)]].
> - **MES:** układ $([H] + [C]/\Delta\tau)\{t\} = \dots$ jest rzadki i symetryczny dodatnio określony, więc w dużych siatkach rozwiązuje się go CG z prekondycjonowaniem: [[Macierz sztywności]].
> - **Optymalizacja:** najszybszy spadek dla $f(x) = \frac12x^TAx - b^Tx$ to ta sama metoda: [[Metoda najszybszego spadku]].

## Pytania testowe (egzamin UPeL)
> [!zadanie] 1. Stacjonarna metoda $x^{(k+1)} = Bx^{(k)} + c$ jest zbieżna dla dowolnego $x^{(0)}$ wtedy i tylko wtedy, gdy:
> a) $\|B\|_\infty < 1$ b) $\rho(B) < 1$ c) $A$ jest symetryczna d) $\det B \ne 0$
> > [!odpowiedz]- Odpowiedź
> > **b.** (a) jest tylko wystarczające.

> [!zadanie] 2. W metodzie Gaussa–Seidla macierz $M$ to:
> a) $D$ b) $L + D$ c) $\frac1\omega D + L$ d) $I$
> > [!odpowiedz]- Odpowiedź
> > **b.** (a) Jacobi, (c) SOR, (d) metoda klasyczna.

> [!zadanie] 3. Dla $A = A^T > 0$ metoda SOR jest zbieżna dla:
> a) $\omega \in (0, 1)$ b) $\omega \in (0, 2)$ c) $\omega > 1$ d) tylko $\omega = 1$
> > [!odpowiedz]- Odpowiedź
> > **b** (twierdzenie Ostrowskiego–Reicha).

> [!zadanie] 4. Które metody można w pełni zrównoleglić w obrębie jednej iteracji?
> a) Jacobi b) Gauss–Seidel w naturalnej kolejności c) Richardson d) SOR w naturalnej kolejności
> > [!odpowiedz]- Odpowiedź
> > **a, c.** Każda składowa nowego wektora zależy tylko od starego wektora.

> [!zadanie] 5. Krok metody najszybszego spadku dla $A = A^T > 0$:
> > [!odpowiedz]- Odpowiedź
> > $a_k = \frac{r_k^Tr_k}{r_k^TAr_k}$, $x^{(k+1)} = x^{(k)} + a_kr_k$, $r_k = b - Ax^{(k)}$.

## Fiszki #fiszki/numeryczne
Macierz iteracji metody stacjonarnej::$B = M^{-1}Z = I - M^{-1}A$
Warunek konieczny i wystarczający zbieżności metody stacjonarnej::$\rho(B) < 1$
$M$ w metodzie Jacobiego, Gaussa–Seidla, SOR::$D$; $L + D$; $\frac1\omega D + L$
Twierdzenie Ostrowskiego–Reicha::$A = A^T > 0$ ⇒ SOR zbieżny dla każdego $\omega \in (0, 2)$
Optymalne $\tau$ w metodzie Richardsona::$2/(\lambda_{max} + \lambda_{min})$
$\rho$ macierzy Jacobiego dla 1D laplasjanu $T_n$::$\cos(\pi/(n+1)) \approx 1 - \pi^2/(2(n+1)^2)$
Krok najszybszego spadku::$a_k = r_k^Tr_k / (r_k^TAr_k)$
Skąd nazwa „metoda projekcji”?::Residuum równania poprawki jest prostopadłe do podprzestrzeni rozpiętej przez kolumny $U_k$

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.

## Źródła
- [[W02 Układy równań liniowych.pdf]] (D. Szeliga, rozdz. „Metody przybliżone”, „Przykłady”).
- Y. Saad, *Iterative Methods for Sparse Linear Systems*, 2nd ed., SIAM 2003, rozdz. 4–5 (metody stacjonarne i projekcji), wersja PDF udostępniana przez autora.
