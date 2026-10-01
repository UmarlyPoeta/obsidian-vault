---
typ: pojęcie
status: nowe
przedmioty: ["[[AGH Metoda elementów skończonych]]", "[[AGH Optymalizacja]]"]
wymaga: ["[[Układy równań liniowych]]", "[[Interpolacja]]", "[[Arytmetyka zmiennoprzecinkowa i błędy numeryczne]]"]
powiązane: ["[[Metamodelowanie]]", "[[Funkcja celu i warunki optymalności]]", "[[Różniczkowanie numeryczne]]", "[[Wielomiany]]"]
tags: [pojęcie, metody-numeryczne]
utworzono: 2026-10-02
źródła: ["Fortuna, Macukow, Wąsowski — Metody numeryczne (WNT)", "Kincaid, Cheney — Analiza numeryczna", "Burden, Faires — Numerical Analysis", "Dokumentacja NumPy: numpy.linalg.lstsq, numpy.linalg.qr"]
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

```python
import numpy as np
x = np.array([0, 1, 2, 3.]); y = np.array([1, 3, 4, 4.])
A = np.column_stack([np.ones_like(x), x])
print(np.linalg.lstsq(A, y, rcond=None)[0])   # [1.5 1. ]
```

> [!przyklad]- Przykład 2: uwarunkowanie w praktyce
> Wielomian stopnia 8 dopasowany do 60 punktów na $[0, 10]$, prawdziwe współczynniki same jedynki. $\mathrm{cond}(A) \approx 1{,}9 \cdot 10^9$, $\mathrm{cond}(A^T A) \approx 3{,}5 \cdot 10^{18}$. Błąd współczynników: układ normalny ok. $10^{-2}$, QR ok. $10^{-8}$. Przy stopniu 10 układ normalny daje błąd rzędu $10^2$ — wynik bezużyteczny.

## Typowe błędy
> [!blad] Odwracanie $A^T A$ dla wielomianów wysokiego stopnia
> Kwadrat uwarunkowania zjada połowę cyfr. Używaj `lstsq` albo QR, a zmienną $x$ przeskaluj do $[-1, 1]$.

> [!blad] Dobór stopnia po błędzie na danych uczących
> Błąd uczący zawsze maleje ze stopniem — przy $n+1 = m$ parametrach spada do zera (interpolacja). Błąd walidacyjny najpierw maleje, potem rośnie: to przeuczenie, model uczy się szumu.

> [!blad] Linearyzacja traktowana jak „ten sam” MNK
> Po zlogarytmowaniu małe $y$ dostają dużą wagę. Do dokładnego dopasowania w oryginalnych zmiennych użyj wyniku jako punktu startowego dla `scipy.optimize.curve_fit`.

## Na zajęcia i egzamin
> [!zajecia]
> - **Optymalizacja:** metamodelowanie — zastępujesz drogą funkcję celu (np. symulację MES) tanią powierzchnią odpowiedzi dopasowaną MNK i optymalizujesz model. Ćwicz w `m04_aproksymacja.py` (`metamodel_kwadratowy`, `metamodel_minimum`).
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

## Fiszki #fiszki/numeryczne
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

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.

## Źródła
- Z. Fortuna, B. Macukow, J. Wąsowski, *Metody numeryczne*, WNT — aproksymacja średniokwadratowa.
- D. Kincaid, W. Cheney, *Analiza numeryczna*, WNT — zadanie najmniejszych kwadratów, rozkład QR.
- R. L. Burden, J. D. Faires, *Numerical Analysis*, Brooks/Cole — discrete least squares approximation.
- Dokumentacja NumPy: `numpy.linalg.lstsq`, `numpy.linalg.qr`; SciPy: `scipy.optimize.curve_fit`.
