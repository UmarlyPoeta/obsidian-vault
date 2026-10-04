---
typ: pojęcie
status: nowe
przedmioty: ["[[AGH Metoda elementów skończonych]]"]
wymaga: ["[[Interpolacja]]", "[[Wielomiany]]"]
powiązane: ["[[MES — kurs dr. Kustry (mapa labów)]]", "[[Jakobian]]", "[[Całkowanie numeryczne i kwadratury Gaussa]]", "[[Macierz sztywności]]", "[[Siatka MES i wczytywanie danych]]", "[[MES 1D — od różnic skończonych do macierzy sztywności]]"]
tags: [pojęcie, mes]
utworzono: 2026-10-05
źródła: ["P. Kustra — MES zajęcia 1 (AGH WIMiIP)", "P. Kustra — Jakobian (AGH WIMiIP)", "A. Milenin — Podstawy metody elementów skończonych (Wyd. AGH 2010)", "O.C. Zienkiewicz, R.L. Taylor, J.Z. Zhu — The Finite Element Method: Its Basis and Fundamentals", "H.P. Langtangen, K.-A. Mardal — Introduction to Numerical Methods for Variational Problems (Springer 2019, CC BY)"]
---
# Funkcje kształtu

> [!skrot] W 30 sekund
> - **Co to:** funkcje $N_i$, które mówią, z jaką wagą wartość z węzła $i$ wchodzi do wartości w dowolnym punkcie elementu: $T_P = \sum_i N_i(P)\,T_i = \{N\}^T\{T\}$.
> - **Klucz:** $N_i = 1$ w swoim węźle i $0$ w pozostałych, a $\sum_i N_i = 1$ w każdym punkcie. Element 4-węzłowy: $N_1 = \tfrac14(1-\xi)(1-\eta)$, $N_2 = \tfrac14(1+\xi)(1-\eta)$, $N_3 = \tfrac14(1+\xi)(1+\eta)$, $N_4 = \tfrac14(1-\xi)(1+\eta)$.
> - **Kiedy:** pierwszy lab MES (interpolacja temperatury), a potem w każdej całce: $H$, $H_{bc}$, $P$, $C$ i jakobian.

> [!intuicja] Po ludzku
> Na końcach pręta masz 20 °C i 46 °C. Im bliżej lewego końca leży punkt, tym bardziej „słucha” lewego termometru. Funkcja kształtu to właśnie ten procent posłuszeństwa: przy lewym końcu 100%, przy prawym 0%, a pośrodku po 50%. Wagi zawsze sumują się do 100%, bo wynik jest średnią ważoną temperatur węzłowych.

## Definicja i wzory
> [!definicja] Własności (prowadzący wymienia je jako pierwszy punkt kursu)
> 1. **Interpolacyjność:** $N_i(x_j) = \delta_{ij}$, czyli 1 w swoim węźle i 0 w innych. Dzięki temu $T(x_j) = T_j$.
> 2. **Podział jedności:** $\sum_i N_i(x) = 1$ w każdym punkcie elementu. Stała temperatura jest więc odtwarzana dokładnie.
> 3. **Zakres:** dla elementów liniowych $N_i \in [0, 1]$ wewnątrz elementu.
> 4. **Ta sama interpolacja dla geometrii:** $x = \sum N_i x_i$, $y = \sum N_i y_i$ (element **izoparametryczny**, patrz [[Jakobian]]).

> [!wzor] 1D, element dwuwęzłowy
> We współrzędnych globalnych, dla węzłów $x_1$ i $x_2$:
> $$N_1(x) = \frac{x_2 - x}{x_2 - x_1}, \qquad N_2(x) = \frac{x - x_1}{x_2 - x_1}$$
> We współrzędnych lokalnych $\xi \in [-1, 1]$:
> $$N_1(\xi) = \tfrac12(1 - \xi), \qquad N_2(\xi) = \tfrac12(1 + \xi), \qquad \frac{dN_1}{d\xi} = -\tfrac12, \quad \frac{dN_2}{d\xi} = \tfrac12$$

> [!wzor] 2D, element czterowęzłowy (element uniwersalny)
> Węzły w układzie lokalnym: 1 $(-1,-1)$, 2 $(1,-1)$, 3 $(1,1)$, 4 $(-1,1)$, czyli **przeciwnie do ruchu wskazówek zegara**.
> $$N_1 = \tfrac14(1-\xi)(1-\eta) \quad N_2 = \tfrac14(1+\xi)(1-\eta) \quad N_3 = \tfrac14(1+\xi)(1+\eta) \quad N_4 = \tfrac14(1-\xi)(1+\eta)$$
> Pochodne (potrzebne od labu 3):
> $$\frac{\partial N}{\partial \xi} = \tfrac14\big[-(1-\eta),\ (1-\eta),\ (1+\eta),\ -(1+\eta)\big]$$
> $$\frac{\partial N}{\partial \eta} = \tfrac14\big[-(1-\xi),\ -(1+\xi),\ (1+\xi),\ (1-\xi)\big]$$
> To funkcje **biliniowe**: liniowe wzdłuż każdej krawędzi, ale z wyrazem $\xi\eta$ w środku.

> [!wzor] Trójkąt i element 8-węzłowy (dla orientacji, są na slajdach)
> - **Trójkąt 3-węzłowy:** $N_1 = S_{P23}/S_{123}$, czyli stosunek pola trójkąta naprzeciw węzła 1 (z punktem $P$) do pola całego trójkąta. Analogicznie $N_2$, $N_3$.
> - **Element 8-węzłowy** (węzły w narożach i w środkach boków) ma funkcje kwadratowe i interpoluje $T_P = \sum_{i=1}^{8} N_i T_i$. W kursie używasz elementu 4-węzłowego.

## Jak to zrobić — krok po kroku
1. Ustal, w którym elemencie leży punkt i jakie są jego węzły (z tablicy `element.ID`).
2. Przelicz punkt na współrzędne lokalne. W 1D: $\xi = \frac{2x - (x_1 + x_2)}{x_2 - x_1}$. W 2D dla prostokąta tak samo osobno dla $x$ i $y$, a dla dowolnego czworokąta trzeba odwrócić przekształcenie (Newton).
3. Policz $N_1 \ldots N_n$ w tym punkcie i sprawdź, czy $\sum N_i = 1$.
4. $T_P = \sum N_i T_i$.

## Przykład
> [!przyklad] Interpolacja w 1D (ze slajdów prowadzącego)
> Węzły: $x_1 = -5$, $T_1 = 20$ °C; $x_2 = 4$, $T_2 = 46$ °C. Szukasz $T$ w $x = 1{,}55$.
>
> $$N_1 = \frac{4 - 1{,}55}{4 - (-5)} = \frac{2{,}45}{9} = 0{,}272222, \qquad N_2 = \frac{1{,}55 + 5}{9} = \frac{6{,}55}{9} = 0{,}727778$$
> Kontrola: $N_1 + N_2 = 1$.
> $$T(1{,}55) = 0{,}272222 \cdot 20 + 0{,}727778 \cdot 46 = 5{,}444 + 33{,}478 = 38{,}92\ ^\circ\text{C}$$
> Dla $x = -0{,}5$ (środek elementu) $N_1 = N_2 = 0{,}5$, więc $T = 33$ °C.

> [!przyklad]- Interpolacja w 2D, element prostokątny
> Węzły: 1 $(0,0)$, 2 $(4,0)$, 3 $(4,2)$, 4 $(0,2)$; temperatury $T = [20, 40, 80, 30]$. Punkt $P = (3;\ 0{,}5)$.
>
> 1. Lokalne współrzędne: $\xi = \frac{2 \cdot 3}{4} - 1 = 0{,}5$, $\eta = \frac{2 \cdot 0{,}5}{2} - 1 = -0{,}5$.
> 2. $N_1 = \tfrac14 \cdot 0{,}5 \cdot 1{,}5 = 0{,}1875$, $N_2 = \tfrac14 \cdot 1{,}5 \cdot 1{,}5 = 0{,}5625$, $N_3 = \tfrac14 \cdot 1{,}5 \cdot 0{,}5 = 0{,}1875$, $N_4 = \tfrac14 \cdot 0{,}5 \cdot 0{,}5 = 0{,}0625$. Suma: 1.
> 3. $T_P = 0{,}1875 \cdot 20 + 0{,}5625 \cdot 40 + 0{,}1875 \cdot 80 + 0{,}0625 \cdot 30 = 3{,}75 + 22{,}5 + 15 + 1{,}875 = 43{,}125$ °C.
> 4. Kontrola geometrii: $\sum N_i x_i = 0{,}5625 \cdot 4 + 0{,}1875 \cdot 4 = 3$ i $\sum N_i y_i = 0{,}1875 \cdot 2 + 0{,}0625 \cdot 2 = 0{,}5$. Wraca dokładnie punkt $P$.

```python
import numpy as np

def N4(ksi, eta):
    return 0.25 * np.array([(1 - ksi) * (1 - eta), (1 + ksi) * (1 - eta),
                            (1 + ksi) * (1 + eta), (1 - ksi) * (1 + eta)])

N = N4(0.5, -0.5)
print(N, N.sum(), N @ [20, 40, 80, 30])   # [0.1875 0.5625 0.1875 0.0625] 1.0 43.125
```

## Typowe błędy
> [!blad] Zła kolejność węzłów
> $N_1 \ldots N_4$ idą **przeciwnie do ruchu wskazówek zegara**, zaczynając od $(-1,-1)$. Plik siatki też podaje węzły elementu w tej kolejności (`1, 2, 6, 5`). Pomieszanie kolejności daje ujemny $\det J$ albo „wywinięty” element.

> [!blad] Mylenie $N(x)$ z $N(\xi)$
> W 2D funkcje kształtu znasz **tylko** w układzie lokalnym $(\xi, \eta)$. Pochodnych $\partial N/\partial x$ nie liczysz wprost, tylko przez [[Jakobian]].

> [!blad] Brak kontroli $\sum N_i = 1$
> To najszybszy test kodu: jeśli suma w dowolnym punkcie różni się od 1, masz literówkę w którejś funkcji. Podobnie suma pochodnych $\sum_i \partial N_i / \partial \xi = 0$.

## Na zajęcia i egzamin
> [!zajecia]
> - **Lab 1:** interpolacja temperatury w punkcie z funkcji kształtu (1D i 2D), własności $N_i$. Na kartkówce: „policz $T$ w punkcie $x$” albo „podaj $N_3$ elementu 4-węzłowego”.
> - **Lab 3:** tabela $\partial N/\partial \xi$ i $\partial N/\partial \eta$ w punktach Gaussa, czyli element uniwersalny. Dla 2 punktów musisz dostać $-0{,}394338$, $0{,}394338$, $0{,}105662$, $-0{,}105662$ (wiersz pc1 w pliku wyników Test1).
> - **Egzamin:** własności funkcji kształtu, zapis $t = \{N\}^T\{t\}$, wyprowadzenie pochodnych.
> - Łączy się z: [[Interpolacja]] (bazy Lagrange'a), [[Jakobian]], [[Macierz sztywności]].

## Zadania
> [!zadanie] Zadanie 1
> Węzły $x_1 = 2$ ($T = 100$) i $x_2 = 6$ ($T = 300$). Policz $T(3)$.
> > [!odpowiedz]- Odpowiedź
> > $N_1 = \frac{6 - 3}{4} = 0{,}75$, $N_2 = 0{,}25$, $T = 75 + 75 = 150$ °C.

> [!zadanie] Zadanie 2
> Policz $\partial N_i/\partial \xi$ w punkcie Gaussa $\xi = \eta = -1/\sqrt3$.
> > [!odpowiedz]- Odpowiedź
> > $1 - \eta = 1 + 0{,}57735 = 1{,}57735$ i $1 + \eta = 0{,}42265$. Stąd $\frac{\partial N}{\partial \xi} = \tfrac14[-1{,}57735;\ 1{,}57735;\ 0{,}42265;\ -0{,}42265] = [-0{,}394338;\ 0{,}394338;\ 0{,}105662;\ -0{,}105662]$. Tak samo jest w pliku prowadzącego.

> [!zadanie] Zadanie 3
> Ile wynosi $N_3$ w środku elementu ($\xi = \eta = 0$) i dlaczego?
> > [!odpowiedz]- Odpowiedź
> > $N_3 = \tfrac14 \cdot 1 \cdot 1 = 0{,}25$. Środek jest jednakowo daleko od wszystkich 4 węzłów, więc każdy dostaje 1/4.

## Fiszki #fiszki/mes
Dwie podstawowe własności funkcji kształtu::N_i = 1 w swoim węźle i 0 w pozostałych; suma N_i = 1 w każdym punkcie
N1 i N2 elementu 1D we współrzędnych globalnych::N1 = (x2 − x)/(x2 − x1), N2 = (x − x1)/(x2 − x1)
N1…N4 elementu 4-węzłowego::¼(1−ξ)(1−η), ¼(1+ξ)(1−η), ¼(1+ξ)(1+η), ¼(1−ξ)(1+η)
W jakiej kolejności numeruje się węzły elementu 4-węzłowego?::Przeciwnie do ruchu wskazówek zegara, od (−1, −1)
Co znaczy, że element jest izoparametryczny?::Geometrię (x, y) i temperaturę interpolujesz tymi samymi funkcjami kształtu
Interpolacja temperatury w punkcie z funkcji kształtu::T_P = Σ N_i(P)·T_i = {N}ᵀ{T}
Ile wynosi suma pochodnych ∂N_i/∂ξ po wszystkich węzłach?::Zero, bo suma N_i jest stała (równa 1)

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.

## Źródła
- P. Kustra, *MES wstęp* (slajdy z zajęć 1) i *Jakobian*, AGH WIMiIP: ![[01 MES zajęcia 1 — funkcje kształtu, interpolacja, siatka.pdf]]
- A. Milenin, *Podstawy metody elementów skończonych*, Wydawnictwa AGH 2010, rozdz. o funkcjach kształtu.
- O.C. Zienkiewicz, R.L. Taylor, J.Z. Zhu, *The Finite Element Method: Its Basis and Fundamentals*, rozdz. 6 (funkcje kształtu, rodzina Lagrange'a i serendipity).
- H.P. Langtangen, K.-A. Mardal, *Introduction to Numerical Methods for Variational Problems*, Springer 2019 (CC BY), rozdz. 4.1 (funkcje bazowe elementów) i 4.6 (elementy 2D i 3D): [[Langtangen, Mardal — Introduction to Numerical Methods for Variational Problems (2019, CC BY).pdf]]
