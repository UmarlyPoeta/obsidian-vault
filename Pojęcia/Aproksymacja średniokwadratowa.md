---
typ: pojęcie
status: nowe
przedmioty: ["[[AGH Metody numeryczne]]", "[[AGH Metoda elementów skończonych]]", "[[AGH Optymalizacja]]"]
wymaga: ["[[Układy równań liniowych]]", "[[Interpolacja]]", "[[Arytmetyka zmiennoprzecinkowa i błędy numeryczne]]"]
powiązane: ["[[Metamodelowanie]]", "[[Funkcja celu i warunki optymalności]]", "[[Różniczkowanie numeryczne]]", "[[Wielomiany]]"]
tags: [pojęcie, metody-numeryczne]
utworzono: 2026-10-02
źródła: ["Fortuna, Macukow, Wąsowski — Metody numeryczne (WNT)", "Kincaid, Cheney — Analiza numeryczna", "Burden, Faires — Numerical Analysis", "Dokumentacja NumPy: numpy.linalg.lstsq, numpy.linalg.qr", "Instrukcja: Lab — aproksymacja średniokwadratowa", "Kurs MN w C++: Kod/metody_numeryczne (moduły, testy, rozwiązania wzorcowe)"]
---
# Aproksymacja średniokwadratowa

> [!skrot] W 30 sekund
> - **Co to:** dopasowanie funkcji z kilkoma parametrami do wielu (zaszumionych) punktów tak, żeby **suma kwadratów odchyleń** była najmniejsza. Funkcja nie musi przechodzić przez punkty.
> - **Klucz:** $\min_c \|A c - y\|_2^2$; rozwiązanie z układu normalnego $A^T A\, c = A^T y$, ale numerycznie lepiej przez QR: $R c = Q^T y$ (`numpy.linalg.lstsq`).
> - **Kiedy:** regresja z pomiarów, dobór parametrów modelu materiałowego, **metamodel** drogiej funkcji celu w optymalizacji.

> [!intuicja] Po ludzku
> Masz chmurę punktów z pomiaru i chcesz przez nią poprowadzić prostą „najlepiej jak się da”. Każdy punkt ciągnie prostą do siebie sprężynką o energii proporcjonalnej do kwadratu odległości w pionie. Prosta ustawia się tam, gdzie łączna energia sprężynek jest najmniejsza. Kwadraty sprawiają, że duże odchylenia kosztują dużo, a całe zadanie sprowadza się do układu liniowego.

## Definicja i wzory
> [!definicja] Liniowe zadanie najmniejszych kwadratów
> Model jest liniowy względem **parametrów**: $\hat y(x) = \sum_k c_k\, \varphi_k(x)$ (np. $\varphi_k = x^k$). Dla $m$ punktów i $n < m$ parametrów:
> $$A_{ik} = \varphi_k(x_i), \qquad S(c) = \|A c - y\|_2^2 = \sum_i \Big(y_i - \sum_k c_k \varphi_k(x_i)\Big)^2 \to \min$$

> [!wzor] Układ normalny
> Z warunku $\nabla S = 0$:
> $$A^T A\, c = A^T y$$
> Równoważnie: wektor reszt $r = y - Ac$ jest **prostopadły** do kolumn $A$, czyli $A^T r = 0$. Dla prostej $y \approx a_0 + a_1 x$:
> $$a_1 = \frac{\sum (x_i - \bar x)(y_i - \bar y)}{\sum (x_i - \bar x)^2}, \qquad a_0 = \bar y - a_1 \bar x$$

> [!wzor] Uwarunkowanie: dlaczego QR
> $$\mathrm{cond}(A^T A) = \mathrm{cond}(A)^2$$
> Rozkład $A = QR$ ($Q^T Q = I$, $R$ trójkątna górna) daje $\|Ac - y\| = \|Rc - Q^T y\|$ plus stała, więc wystarczy podstawianie wstecz w $R c = Q^T y$. Uwarunkowanie zostaje $\mathrm{cond}(A)$. `numpy.linalg.lstsq` używa SVD — jeszcze stabilniej, radzi sobie też z macierzą o niepełnym rzędzie.

> [!wzor] Linearyzacja modelu nieliniowego
> $$y = a e^{bx} \ \Rightarrow\ \ln y = \ln a + b x$$
> To zwykła prosta w zmiennych $(x, \ln y)$. Uwaga: minimalizujesz wtedy błąd **logarytmów** (względny), a nie $\sum (y_i - a e^{b x_i})^2$.

> [!wzor] Metamodel kwadratowy 2D (powierzchnia odpowiedzi)
> $$\hat f(x_1, x_2) = c_0 + c_1 x_1 + c_2 x_2 + c_3 x_1^2 + c_4 x_1 x_2 + c_5 x_2^2$$
> Sześć parametrów, więc co najmniej 6 obliczeń prawdziwej funkcji (zwykle plan $3 \times 3$). Minimum modelu: $\nabla \hat f = 0$, czyli
> $$\begin{bmatrix} 2c_3 & c_4 \\ c_4 & 2c_5 \end{bmatrix} x = -\begin{bmatrix} c_1 \\ c_2 \end{bmatrix}$$

## Jak to zrobić — krok po kroku
1. Wybierz bazę $\varphi_k$ (wielomiany, wykładnicza po linearyzacji, kwadratowa 2D).
2. Zbuduj macierz $A$ (wiersz = punkt, kolumna = funkcja bazowa).
3. Rozwiąż `c = np.linalg.lstsq(A, y, rcond=None)[0]` albo przez QR. Układ normalny tylko dla małych, dobrze uwarunkowanych zadań i do obliczeń ręcznych.
4. Sprawdź reszty: powinny wyglądać jak szum, bez wyraźnego trendu.
5. Stopień/liczbę parametrów dobierz na **osobnym** zbiorze walidacyjnym, nie na danych uczących.

## Przykład
> [!przyklad] Prosta przez $(0,1), (1,3), (2,4), (3,4)$
> $$A = \begin{bmatrix} 1 & 0 \\ 1 & 1 \\ 1 & 2 \\ 1 & 3 \end{bmatrix}, \quad A^T A = \begin{bmatrix} 4 & 6 \\ 6 & 14 \end{bmatrix}, \quad A^T y = \begin{bmatrix} 12 \\ 23 \end{bmatrix}$$
> Z pierwszego równania $a_0 = 3 - 1{,}5\,a_1$. Wstawiamy do drugiego: $18 - 9a_1 + 14 a_1 = 23$, więc $a_1 = 1$, $a_0 = 1{,}5$.
>
> Wzór ze średnimi daje to samo: $\bar x = 1{,}5$, $\bar y = 3$, licznik $3 + 0 + 0{,}5 + 1{,}5 = 5$, mianownik $2{,}25 + 0{,}25 + 0{,}25 + 2{,}25 = 5$.
>
> Prosta $\hat y = 1{,}5 + x$. Reszty $(-0{,}5;\ 0{,}5;\ 0{,}5;\ -0{,}5)$ sumują się do zera, $S = 1$.

```cpp
#include "m04.h"   // Kod/metody_numeryczne, korzysta z gauss() z m02
Vec x = {0, 1, 2, 3}, y = {1, 3, 4, 4}, w(4, 1.0);
Vec a = aproksymacja(x, y, w, 1);                // a0 = 1.5, a1 = 1.0
double E = blad_aproksymacji(x, y, w, a);        // 1.0 (reszty -0.5, 0.5, 0.5, -0.5)
```

> [!kod]- To samo w Pythonie (do szybkiego sprawdzenia)
> ```python
> import numpy as np
> x = np.array([0, 1, 2, 3.]); y = np.array([1, 3, 4, 4.])
> A = np.column_stack([np.ones_like(x), x])
> print(np.linalg.lstsq(A, y, rcond=None)[0])   # [1.5 1. ]
> ```

> [!przyklad]- Przykład 2: uwarunkowanie w praktyce
> Wielomian stopnia 8 dopasowany do 60 punktów na $[0, 10]$, prawdziwe współczynniki same jedynki. $\mathrm{cond}(A) \approx 1{,}9 \cdot 10^9$, $\mathrm{cond}(A^T A) \approx 3{,}5 \cdot 10^{18}$. Błąd współczynników: układ normalny ok. $10^{-2}$, QR ok. $10^{-8}$. Przy stopniu 10 układ normalny daje błąd rzędu $10^2$ — wynik bezużyteczny.

## Lab: aproksymacja — co oddajesz i wyniki do sprawdzenia
Instrukcja: [[Lab — aproksymacja średniokwadratowa.pdf]]. Oddajesz na zajęciach (10 pkt), na UPEL `.cpp` i plik z rozwiązaniem zad. 2.

> [!definicja] Zapis z instrukcji (baza jednomianów, wagi $w_i$)
> $$g_{kj} = \sum_{i=0}^{m} w(x_i)\,x_i^{k}\,x_i^{j}, \qquad F_k = \sum_{i=0}^{m} w(x_i)\,x_i^{k}\,f(x_i), \qquad \sum_{j=0}^{n} g_{kj}\,a_j = F_k$$
> To jest dokładnie układ normalny $A^T W A\,a = A^T W y$ z notatki wyżej, tylko rozpisany na sumy. $E = \sum w_i [f(x_i) - F(x_i)]^2$.

> [!przyklad] Przykład z instrukcji: 8 punktów, prosta, wagi 1
> $(1,2), (2,4), (3,3), (4,5), (5,6), (6,9), (7,11), (8,11)$.
> - $g_{00} = \sum 1 = 8$, $g_{01} = g_{10} = \sum x_i = 36$, $g_{11} = \sum x_i^2 = 204$, $F_0 = \sum y_i = 51$, $F_1 = \sum x_i y_i = 288$.
> - $\begin{bmatrix} 8 & 36 \\ 36 & 204 \end{bmatrix}\begin{bmatrix} a_0 \\ a_1 \end{bmatrix} = \begin{bmatrix} 51 \\ 288 \end{bmatrix}$ daje $a_0 = 0{,}107$, $a_1 = 1{,}393$ (dokładnie $3/28$ i $39/28$).
> - Błąd $E = 6{,}393$.

> [!przyklad] Zad. 2: dobór stopnia dla $(1; 3{,}2), (2; 5{,}7), \ldots, (8; 65{,}6)$
> | stopień | współczynniki $a_0, a_1, \ldots$ | $E$ | $R^2$ |
> |---|---|---|---|
> | 1 | −12,889; 8,981 | 164,24 | 0,9538 |
> | **2** | **1,896; 0,110; 0,986** | **1,006** | **0,99972** |
> | 3 | 2,671; −0,697; 1,197; −0,0157 | 0,860 | 0,99976 |
> | 4 | 2,286; −0,130; 0,950; 0,0253; −0,0023 | 0,851 | 0,99976 |
>
> **Uzasadnienie stopnia 2:** przejście z 1 na 2 zmniejsza $E$ ponad 160 razy, a dalsze stopnie prawie nic nie dają (0,86 i 0,85), za to dokładają współczynniki bliskie zeru, czyli dopasowują szum. Dane wyglądają na $y \approx x^2$ (współczynnik przy $x^2$ ≈ 0,99). Przy 8 punktach wysoki stopień zaczyna oscylować między węzłami.

> [!kod]- Format pliku wejściowego (propozycja, wymaganie a) z instrukcji)
> ```
> 1          ← stopień wielomianu
> 8          ← liczba węzłów
> 1 2 1      ← x  y  waga
> 2 4 1
> ...
> ```
> Wczytujesz `std::ifstream plik("dane.txt"); plik >> n >> m; for (...) plik >> x[i] >> y[i] >> w[i];`, wołasz `aproksymacja(x, y, w, n)` i wypisujesz: liczbę węzłów, współczynniki, tabelę $x_i$, $y_i$, $F(x_i)$ i błąd $E$.

## Wykład dr Szeligi (W04) — co dochodzi
> [!definicja] Ujęcie z wykładu: aproksymacja w przestrzeni unitarnej
> Dla $f \in V$ (przestrzeń unitarna) i skończenie wymiarowej podprzestrzeni $U$: element $h^* \in U$ jest optymalny **⇔ $f - h^*$ jest ortogonalny do $U$**. Błąd:
> $$\varepsilon_U = \|f - h^*\| = \left(\|f\|^2 - \|h^*\|^2\right)^{1/2} \qquad (\text{Pitagoras: } \|f\|^2 = \|f - h^*\|^2 + \|h^*\|^2)$$
> - Baza $f_1, \dots, f_n$ podprzestrzeni $U$: $h^* = \sum\alpha_jf_j$ spełnia **układ równań normalnych** $\sum_j\alpha_j(f_j, f_i) = (f, f_i)$ z **macierzą Grama** $[(f_j, f_i)]$. Koszt: $\frac{n(n+1)}{2} + n$ iloczynów skalarnych plus ok. $n^3$ na układ.
> - Baza **ortonormalna** $g_j$: układ staje się jednostkowy, $h^* = \sum_j(f, g_j)g_j$, $\varepsilon_U^2 = \|f\|^2 - \sum_j(f, g_j)^2$. Tylko $n$ iloczynów skalarnych i lepsza dokładność. Współczynniki $(f, g_j)$ to **współczynniki Fouriera**, a $h_n^*$ to sumy częściowe szeregu Fouriera. Zbieżność do $f$ dla każdego $\varepsilon$ ⇔ **równość Parsevala** $\|f\|^2 = \sum_{j=1}^\infty(f, g_j)^2$.
> - Rekurencja (gdy chcemy błąd $< \varepsilon$ możliwie niskim stopniem): $h_n^* = h_{n-1}^* + (f, g_n)g_n$, $\varepsilon_{U_n}^2 = \varepsilon_{U_{n-1}}^2 - (f, g_n)^2$, kolejne $g_n$ ze zmodyfikowanego Grama–Schmidta. Bazy i rekurencje: [[Wielomiany ortogonalne]].
>
> **Ciągła** (w $L^2_p[a,b]$) i **dyskretna** (w $l^2_{p,N}$, suma po punktach z wagami) aproksymacja średniokwadratowa wielomianami. W dyskretnej stopień $n \le N - 1$; dla $n = N - 1$ wynikiem jest wielomian interpolacyjny Lagrange'a i błąd 0. (Slajd mówi, że dla $n < N - 1$ zadanie „nie ma jednoznacznego rozwiązania”. To pomyłka: przy różnych punktach rozwiązanie dla $n < N - 1$ **jest** jednoznaczne, a niejednoznaczność pojawia się dopiero dla $n > N - 1$.)

> [!wzor] Baza $1, x, \dots, x^n$ na $[0,1]$ z wagą 1 → macierz Hilberta
> $(x^i, x^j) = \int_0^1x^{i+j}dx = \frac{1}{i+j+1}$, czyli macierz Grama to **macierz Hilberta**, skrajnie źle uwarunkowana:
>
> | $n$ | 3 | 5 | 8 | 10 |
> |---|---|---|---|---|
> | $\operatorname{cond}_2(H_n)$ | 524 | $4{,}8\cdot10^5$ | $1{,}5\cdot10^{10}$ | $1{,}6\cdot10^{13}$ |
>
> Dlatego baza potęgowa daje złe wyniki już dla umiarkowanych stopni, a lepsza jest baza ortogonalna z reguły trójczłonowej.

> [!przyklad] Z wykładu: $f(x) = \sqrt x$, waga $p(x) = x$ na $[0, 1]$, wielomian stopnia 2
> **Sposób I (układ normalny):** $(x^i, x^j) = \int_0^1x^{i+j+1}dx = \frac{1}{i+j+2}$, $(x^i, \sqrt x) = \frac{2}{2i+5}$:
> $$\begin{bmatrix}\frac12 & \frac13 & \frac14\\ \frac13 & \frac14 & \frac15\\ \frac14 & \frac15 & \frac16\end{bmatrix}\begin{bmatrix}\alpha_0\\\alpha_1\\\alpha_2\end{bmatrix} = \begin{bmatrix}\frac25\\ \frac27\\ \frac29\end{bmatrix} \ \Rightarrow\ w_2^*(x) = \frac{8}{35} + \frac87x - \frac{8}{21}x^2$$
> (sprawdzone numerycznie: $0{,}22857;\ 1{,}14286;\ -0{,}38095$).
> **Sposób II (Gram–Schmidt)** na $1, x, x^2$: $g_1 = 1$, $g_2 = x - \frac23$, $g_3 = x^2 - \frac65x + \frac{3}{10}$, potem normowanie i $w_2^* = \sum(\sqrt x, g_k)g_k$.
> **Sposób III (reguła trójczłonowa):** $\beta_1 = -\frac23$, $\beta_2 = -\frac{8}{15}$, $\gamma_2 = -\frac{1}{18}$, ten sam $P_2 = x^2 - \frac65x + \frac{3}{10}$. Szczegóły w [[Wielomiany ortogonalne]].
> Wszystkie trzy dają ten sam wielomian; różnią się kosztem i odpornością na błędy zaokrągleń.

> Aproksymacja jednostajna (minimaksowa, warunek Haara, alternans Czebyszewa) i aproksymacja Padégo: [[Aproksymacja jednostajna i Padégo]].

## Typowe błędy
> [!blad] Odwracanie $A^T A$ dla wielomianów wysokiego stopnia
> Kwadrat uwarunkowania zjada połowę cyfr. Używaj `lstsq` albo QR, a zmienną $x$ przeskaluj do $[-1, 1]$.

> [!blad] Dobór stopnia po błędzie na danych uczących
> Błąd uczący zawsze maleje ze stopniem — przy $n+1 = m$ parametrach spada do zera (interpolacja). Błąd walidacyjny najpierw maleje, potem rośnie: to przeuczenie, model uczy się szumu.

> [!blad] Linearyzacja traktowana jak „ten sam” MNK
> Po zlogarytmowaniu małe $y$ dostają dużą wagę. Do dokładnego dopasowania w oryginalnych zmiennych użyj wyniku jako punktu startowego dla `scipy.optimize.curve_fit`.

## Na zajęcia i egzamin
> [!zajecia]
> - **Optymalizacja:** metamodelowanie — zastępujesz drogą funkcję celu (np. symulację MES) tanią powierzchnią odpowiedzi dopasowaną MNK i optymalizujesz model. Ćwicz: **Ćwicz w C++:** `Kod/metody_numeryczne/zadania/m04.cpp` (układ normalny, aproksymacja dowolnego stopnia, wielomian, błąd E), specyfikacja w `m04.h`, sprawdzasz `./sprawdz.sh 04`. Wzorzec: `rozwiazania/m04.cpp`, dopiero po własnej próbie.
> - **MES:** identyfikacja parametrów (np. współczynnika wymiany ciepła) przez dopasowanie wyników symulacji do pomiarów; walidacja modelu.
> - **Typowe pytanie:** „wyprowadź układ normalny”, „czym różni się interpolacja od aproksymacji”, „jak dobrać stopień wielomianu”.
> - Łączy się z: [[Metamodelowanie]], [[Funkcja celu i warunki optymalności]], [[Interpolacja]].

## Zadania
> [!zadanie] Zadanie 1
> Dla punktów $(1, 2), (2, 3), (3, 7)$ policz prostą MNK.
> > [!odpowiedz]- Odpowiedź
> > $\bar x = 2$, $\bar y = 4$. Licznik $(-1)(-2) + 0 + 1 \cdot 3 = 5$, mianownik $2$. $a_1 = 2{,}5$, $a_0 = 4 - 5 = -1$. Prosta $\hat y = -1 + 2{,}5x$.

> [!zadanie] Zadanie 2
> Jak dopasować $y = a x^b$ metodą liniową?
> > [!odpowiedz]- Odpowiedź
> > $\ln y = \ln a + b \ln x$ — prosta w zmiennych $(\ln x, \ln y)$. Wymaga $x > 0$, $y > 0$.

> [!zadanie] Zadanie 3
> Ile co najmniej obliczeń funkcji celu potrzeba do metamodelu kwadratowego w 3 zmiennych?
> > [!odpowiedz]- Odpowiedź
> > $1 + 3 + 3 + 3 = 10$ (stała, 3 liniowe, 3 kwadraty, 3 iloczyny mieszane). W praktyce więcej, żeby MNK uśredniało szum.

> [!zadanie] Zadanie 4
> $\mathrm{cond}(A) = 10^7$. Ile cyfr znaczących możesz stracić w układzie normalnym, a ile przez QR?
> > [!odpowiedz]- Odpowiedź
> > Układ normalny: $\mathrm{cond} = 10^{14}$, czyli do 14 z ok. 16 cyfr. QR: do 7 cyfr.

## Pytania testowe (egzamin UPeL)
> [!zadanie] 1. W przestrzeni unitarnej $h^* \in U$ jest elementem optymalnym dla $f$ wtedy i tylko wtedy, gdy:
> a) $h^*$ jest ortogonalny do $f$ b) $f - h^*$ jest ortogonalny do $U$ c) $\|h^*\| = \|f\|$ d) $h^*$ interpoluje $f$
> > [!odpowiedz]- Odpowiedź
> > **b.**

> [!zadanie] 2. Macierz układu równań normalnych dla bazy $1, x, \dots, x^n$ na $[0, 1]$ z wagą 1 to:
> a) macierz jednostkowa b) macierz Vandermonde'a c) macierz Hilberta d) macierz trójdiagonalna
> > [!odpowiedz]- Odpowiedź
> > **c.** I jest bardzo źle uwarunkowana.

> [!zadanie] 3. Przy bazie ortonormalnej $g_j$ błąd aproksymacji wynosi:
> a) $\|f\|^2 - \sum(f, g_j)^2$ b) $\left(\|f\|^2 - \sum(f, g_j)^2\right)^{1/2}$ c) $\sum(f, g_j)^2$ d) 0
> > [!odpowiedz]- Odpowiedź
> > **b.** (a) to jego kwadrat.

> [!zadanie] 4. Dyskretna aproksymacja średniokwadratowa $N$ punktów wielomianem stopnia $N - 1$ daje:
> a) prostą regresji b) wielomian interpolacyjny Lagrange'a c) błąd zerowy d) układ sprzeczny
> > [!odpowiedz]- Odpowiedź
> > **b, c.**

## Fiszki #fiszki/numeryczne
Warunek optymalności w przestrzeni unitarnej::$f - h^* \perp U$
Błąd aproksymacji w przestrzeni unitarnej::$\varepsilon_U = (\|f\|^2 - \|h^*\|^2)^{1/2}$
Macierz Grama::Macierz iloczynów skalarnych $(f_j, f_i)$ funkcji bazowych, macierz układu normalnego
Równość Parsevala::$\|f\|^2 = \sum_j (f, g_j)^2$; równoważna zbieżności szeregu Fouriera do $f$
Aproksymacja $\sqrt x$ z wagą $x$ na [0,1], stopień 2 (wykład)::$w_2^* = 8/35 + 8x/7 - 8x^2/21$
cond macierzy Hilberta 10×10::ok. $1{,}6\cdot10^{13}$ (norma 2)
Czym różni się aproksymacja od interpolacji?::Aproksymacja nie przechodzi przez punkty, tylko minimalizuje sumę kwadratów odchyleń
Układ normalny MNK::AᵀA c = Aᵀy
Geometryczny sens rozwiązania MNK::Wektor reszt jest prostopadły do kolumn A (Aᵀr = 0)
Jak uwarunkowanie AᵀA ma się do uwarunkowania A?::cond(AᵀA) = cond(A)²
Jak rozwiązać MNK przez QR?::A = QR, potem podstawianie wstecz w R c = Qᵀy
Czego używa numpy.linalg.lstsq?::Rozkładu SVD — stabilne także dla macierzy o niepełnym rzędzie
Jak zlinearyzować y = a·e^(bx)?::ln y = ln a + b x, zwykła prosta w zmiennych (x, ln y)
Jak rozpoznać przeuczenie?::Błąd uczący maleje, a walidacyjny rośnie wraz ze stopniem modelu
Ile parametrów ma kwadratowa powierzchnia odpowiedzi w 2D?::Sześć: 1, x1, x2, x1², x1x2, x2²
Po co metamodel w optymalizacji?::Zastępuje drogą funkcję celu tanim modelem, który można szybko optymalizować

Wzory na g_kj i F_k w aproksymacji z instrukcji::g_kj = Σ w_i x_i^k x_i^j, F_k = Σ w_i x_i^k y_i; rozwiązujesz Σ_j g_kj a_j = F_k
Prosta dla 8 punktów z przykładu w instrukcji::a₀ = 0,107, a₁ = 1,393 (układ 8, 36; 36, 204 = 51, 288)
Jaki stopień dla danych 3,2; 5,7; …; 65,6 z zad. 2 i dlaczego?::2, bo E spada ze 164 do 1,0, a stopnie 3 i 4 prawie nic nie poprawiają

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.

## Źródła
- Z. Fortuna, B. Macukow, J. Wąsowski, *Metody numeryczne*, WNT — aproksymacja średniokwadratowa.
- D. Kincaid, W. Cheney, *Analiza numeryczna*, WNT — zadanie najmniejszych kwadratów, rozkład QR.
- R. L. Burden, J. D. Faires, *Numerical Analysis*, Brooks/Cole — discrete least squares approximation.
- Dokumentacja NumPy: `numpy.linalg.lstsq`, `numpy.linalg.qr`; SciPy: `scipy.optimize.curve_fit`.
- Instrukcja: Lab — aproksymacja średniokwadratowa
- Kurs MN w C++: Kod/metody_numeryczne (moduły, testy, rozwiązania wzorcowe)
