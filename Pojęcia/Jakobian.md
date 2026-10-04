---
typ: pojęcie
status: nowe
aliases: ["Przekształcenie izoparametryczne", "Jakobian przekształcenia"]
przedmioty: ["[[AGH Metoda elementów skończonych]]"]
wymaga: ["[[Funkcje kształtu]]", "[[Pochodna]]", "[[Całkowanie numeryczne i kwadratury Gaussa]]"]
powiązane: ["[[MES — kurs dr. Kustry (mapa labów)]]", "[[Macierz sztywności]]", "[[Nieustalony przepływ ciepła]]", "[[Siatka MES i wczytywanie danych]]"]
tags: [pojęcie, mes]
utworzono: 2026-10-05
źródła: ["P. Kustra — Jakobian; Całkowanie numeryczne metodą Gaussa w przedziale A,B (AGH WIMiIP)", "A. Milenin — Podstawy metody elementów skończonych (Wyd. AGH 2010)", "O.C. Zienkiewicz, R.L. Taylor, J.Z. Zhu — The Finite Element Method: Its Basis and Fundamentals"]
---
# Jakobian

> [!skrot] W 30 sekund
> - **Co to:** macierz, która przelicza pochodne z układu lokalnego $(\xi, \eta)$ elementu uniwersalnego na układ globalny $(x, y)$ i mówi, ile razy element jest większy od kwadratu $[-1,1]^2$.
> - **Klucz:** $[J] = \begin{bmatrix} \frac{\partial x}{\partial \xi} & \frac{\partial y}{\partial \xi} \\ \frac{\partial x}{\partial \eta} & \frac{\partial y}{\partial \eta} \end{bmatrix}$, gdzie $\frac{\partial x}{\partial \xi} = \sum_i \frac{\partial N_i}{\partial \xi}x_i$. Dalej $\begin{Bmatrix} \partial N_i/\partial x \\ \partial N_i/\partial y \end{Bmatrix} = [J]^{-1}\begin{Bmatrix} \partial N_i/\partial \xi \\ \partial N_i/\partial \eta \end{Bmatrix}$ oraz $dV = dx\,dy = \det J\,d\xi\,d\eta$.
> - **Kiedy:** lab 3 i każda całka po elemencie ($H$, $C$). Liczysz go **osobno w każdym punkcie całkowania**.

> [!intuicja] Po ludzku
> Funkcje kształtu znasz tylko na „wzorcowej” kafelce o boku 2. Prawdziwy element jest przesuniętą, rozciągniętą, może trochę przekrzywioną kopią tej kafelki. Jakobian to lokalna „skala mapy”: mówi, ile metrów w rzeczywistości odpowiada jednej jednostce na wzorcu i w którą stronę. $\det J$ to współczynnik zmiany pola: w 1D element od $-5$ do $4$ ma długość 9, a wzorzec 2, więc $\det J = 4{,}5$.

## Definicja i wzory
> [!wzor] 1D
> $x(\xi) = N_1(\xi)x_1 + N_2(\xi)x_2$, więc
> $$\det J = \frac{dx}{d\xi} = -\tfrac12 x_1 + \tfrac12 x_2 = \frac{x_2 - x_1}{2} = \frac{L}{2}$$
> $$\int_{x_1}^{x_2} f(x)\,dx = \int_{-1}^{1} f(x(\xi))\,\det J\,d\xi \approx \sum_i w_i f(x(\xi_i))\,\det J$$

> [!wzor] 2D (z instrukcji prowadzącego)
> Reguła łańcuchowa:
> $$\begin{Bmatrix} \frac{\partial N_i}{\partial \xi} \\ \frac{\partial N_i}{\partial \eta} \end{Bmatrix} = \underbrace{\begin{bmatrix} \frac{\partial x}{\partial \xi} & \frac{\partial y}{\partial \xi} \\ \frac{\partial x}{\partial \eta} & \frac{\partial y}{\partial \eta} \end{bmatrix}}_{[J]}\begin{Bmatrix} \frac{\partial N_i}{\partial x} \\ \frac{\partial N_i}{\partial y} \end{Bmatrix} \;\Rightarrow\; \begin{Bmatrix} \frac{\partial N_i}{\partial x} \\ \frac{\partial N_i}{\partial y} \end{Bmatrix} = \frac{1}{\det J}\begin{bmatrix} \frac{\partial y}{\partial \eta} & -\frac{\partial y}{\partial \xi} \\ -\frac{\partial x}{\partial \eta} & \frac{\partial x}{\partial \xi} \end{bmatrix}\begin{Bmatrix} \frac{\partial N_i}{\partial \xi} \\ \frac{\partial N_i}{\partial \eta} \end{Bmatrix}$$
> Składniki jakobianu:
> $$\frac{\partial x}{\partial \xi} = \sum_{i=1}^4 \frac{\partial N_i}{\partial \xi}x_i, \quad \frac{\partial y}{\partial \xi} = \sum_{i=1}^4 \frac{\partial N_i}{\partial \xi}y_i, \quad \frac{\partial x}{\partial \eta} = \sum_{i=1}^4 \frac{\partial N_i}{\partial \eta}x_i, \quad \frac{\partial y}{\partial \eta} = \sum_{i=1}^4 \frac{\partial N_i}{\partial \eta}y_i$$
> Dla prostokąta $a \times b$ ustawionego wzdłuż osi: $[J] = \mathrm{diag}(a/2,\ b/2)$ i $\det J = ab/4$ (czyli pole/4) w każdym punkcie.

## Jak to zrobić — krok po kroku
1. **Element uniwersalny** (raz na cały program): w każdym punkcie Gaussa $(\xi_p, \eta_p)$ policz tablice $\partial N_i/\partial \xi$ i $\partial N_i/\partial \eta$ (np. 4×4 dla 2 punktów).
2. Dla elementu weź współrzędne jego 4 węzłów z `grid.node[ID[i]]`.
3. W każdym punkcie $p$: policz 4 składniki $[J]$ jako sumy iloczynów, potem $\det J$.
4. Odwróć $[J]$ (wzór 2×2 wyżej) i policz $\partial N_i/\partial x$, $\partial N_i/\partial y$ dla $i = 1..4$.
5. Kontrola: $\sum_p w_p \det J_p$ = pole elementu.

## Przykład
> [!przyklad] Element z Test1_4_4 (porównaj z plikiem wyników prowadzącego)
> Element 1: węzły $1 (0{,}1;\ 0{,}005)$, $2 (0{,}0667;\ 0{,}005)$, $6 (0{,}0667;\ -0{,}0283)$, $5 (0{,}1;\ -0{,}0283)$. Bok ma $h = 1/30 \approx 0{,}0333$ m, ale $x$ **maleje** od węzła 1 do 2, a $y$ maleje od węzła 2 do 6.
>
> 1. $\frac{\partial x}{\partial \xi} = \frac{x_2 - x_1}{2}$ (dla prostokąta) $= -\frac{0{,}0333}{2} = -0{,}016667$, tak samo $\frac{\partial y}{\partial \eta} = -0{,}016667$, a składniki mieszane są równe 0.
> 2. $[J] = \begin{bmatrix} -0{,}016667 & 0 \\ 0 & -0{,}016667 \end{bmatrix}$, $\det J = 0{,}000277778$ (dodatni, bo oba znaki minus się znoszą). Pole: $4 \cdot 0{,}000277778 = 0{,}001111 = h^2$.
> 3. W pc1 ($\xi = \eta = -0{,}57735$): $\frac{\partial N}{\partial \xi} = [-0{,}394338;\ 0{,}394338;\ 0{,}105662;\ -0{,}105662]$, więc
> $$\frac{\partial N}{\partial x} = \frac{1}{-0{,}016667}\frac{\partial N}{\partial \xi} = [23{,}6603;\ -23{,}6603;\ -6{,}33975;\ 6{,}33975]$$
> Dokładnie tak jest w pliku *Wyniki dla pliku Test1_4_4* prowadzącego.

> [!przyklad]- Element nieprostokątny (trapez), punkt pc1
> Węzły: $(0, 0)$, $(0{,}04;\ 0)$, $(0{,}03;\ 0{,}02)$, $(0;\ 0{,}02)$. W pc1: $1 - \eta = 1{,}57735$, $1 + \eta = 0{,}42265$, $1 + \xi = 0{,}42265$, $1 - \xi = 1{,}57735$.
> - $\frac{\partial x}{\partial \xi} = \frac14[1{,}57735 \cdot 0{,}04 + 0{,}42265 \cdot 0{,}03] = 0{,}018943$
> - $\frac{\partial y}{\partial \xi} = \frac14[0{,}42265 \cdot 0{,}02 - 0{,}42265 \cdot 0{,}02] = 0$
> - $\frac{\partial x}{\partial \eta} = \frac14[-0{,}42265 \cdot 0{,}04 + 0{,}42265 \cdot 0{,}03] = -0{,}0010566$
> - $\frac{\partial y}{\partial \eta} = \frac14[0{,}42265 \cdot 0{,}02 + 1{,}57735 \cdot 0{,}02] = 0{,}01$
>
> $\det J = 0{,}018943 \cdot 0{,}01 - 0 \cdot (-0{,}0010566) = 0{,}00018943$. W tym elemencie $\det J$ **zmienia się** od punktu do punktu, a suma po 4 punktach (wagi 1) daje pole trapezu $0{,}0007$ m². Tak wyglądają elementy siatki MixGrid.

> [!przyklad]- Całka 1D na przedziale $[-5, 4]$ (ze slajdów)
> $\int_{-5}^{4}(0{,}5x^2 + 2x + 3)\,dx$, $\det J = 4{,}5$.
> - Punkty 2-punktowe: $x_{pc1} = 0{,}7887 \cdot (-5) + 0{,}2113 \cdot 4 = -3{,}098$, $x_{pc2} = 2{,}098$ (to funkcje kształtu w punktach Gaussa!).
> - $f(-3{,}098) = 1{,}603$, $f(2{,}098) = 9{,}397$.
> - Wynik: $(1 \cdot 1{,}603 + 1 \cdot 9{,}397) \cdot 4{,}5 = 49{,}5$. Analitycznie: $\frac{0{,}5}{3}(64 + 125) + (16 - 25) + 27 = 31{,}5 - 9 + 27 = 49{,}5$. Schemat 3-punktowy daje to samo, bo wielomian 2. stopnia całkuje się dokładnie już 2 punktami.

```python
import numpy as np

def jakobian(xy, ksi, eta):
    dksi = 0.25 * np.array([-(1 - eta), (1 - eta), (1 + eta), -(1 + eta)])
    deta = 0.25 * np.array([-(1 - ksi), -(1 + ksi), (1 + ksi), (1 - ksi)])
    J = np.array([[dksi @ xy[:, 0], dksi @ xy[:, 1]],
                  [deta @ xy[:, 0], deta @ xy[:, 1]]])
    dNdx, dNdy = np.linalg.inv(J) @ np.vstack([dksi, deta])
    return J, np.linalg.det(J), dNdx, dNdy

h = 1 / 30
xy = np.array([[0.1, 0.005], [0.1 - h, 0.005], [0.1 - h, 0.005 - h], [0.1, 0.005 - h]])
J, detJ, dNdx, dNdy = jakobian(xy, -1 / np.sqrt(3), -1 / np.sqrt(3))
print(J.round(6), detJ, dNdx.round(4))   # diag(-0.016667), 0.000277778, [23.6603 -23.6603 -6.3397 6.3397]
```

## Typowe błędy
> [!blad] Jeden jakobian na cały element
> Dla prostokąta to działa, dla MixGrid już nie. Licz $[J]$, $\det J$ i $\partial N/\partial x$ **w każdym punkcie całkowania** i trzymaj je w tablicach `[pc][i]`.

> [!blad] Transpozycja jakobianu
> Prowadzący zapisuje $[J]$ z $\partial x/\partial \xi$ i $\partial y/\partial \xi$ w **pierwszym wierszu**. Jeśli zbudujesz macierz transponowaną, a potem pomnożysz ją tak samo, pomieszasz $\partial N/\partial x$ z $\partial N/\partial y$. Dla prostokąta tego nie zauważysz, dla MixGrid wyniki się rozjadą.

> [!blad] Ujemny $\det J$
> Oznacza złą kolejność węzłów w elemencie (zgodnie z ruchem wskazówek zegara) albo wywinięty element. Element „odwrócony” w obu kierunkach naraz (jak w Test1) ma $\det J > 0$ i jest w porządku.

> [!blad] Zapomniany $\det J$ w całce
> $\int_V f\,dV = \sum_p w_p f(p)\det J_p$. Bez $\det J$ całkujesz po kwadracie $[-1,1]^2$, a nie po elemencie.

## Na zajęcia i egzamin
> [!zajecia]
> - **Lab 3:** struktura `ElementUniwersalny` (tablice $\partial N/\partial\xi$, $\partial N/\partial\eta$ dla 2, 3 i 4 punktów) oraz struktura `Jakobian` ($J$, $J^{-1}$, $\det J$). Wyniki porównujesz z plikami prowadzącego dla Test1 i MixGrid.
> - **Egzamin:** „wyprowadź $\partial N/\partial x$ przez jakobian”, „co oznacza $\det J$”, „dlaczego jakobian liczy się w każdym punkcie całkowania”.
> - Łączy się z: [[Funkcje kształtu]], [[Całkowanie numeryczne i kwadratury Gaussa]], [[Macierz sztywności]].

## Zadania
> [!zadanie] Zadanie 1
> Element 1D od $x_1 = 2$ do $x_2 = 8$. Ile wynosi $\det J$ i $dN_2/dx$?
> > [!odpowiedz]- Odpowiedź
> > $\det J = 3$, $dN_2/dx = \frac{dN_2/d\xi}{\det J} = \frac{0{,}5}{3} = \frac16$, czyli $1/L$.

> [!zadanie] Zadanie 2
> Prostokąt $0{,}025 \times 0{,}025$ m wzdłuż osi. Podaj $[J]$, $\det J$ i $\partial N_1/\partial x$ w pc1.
> > [!odpowiedz]- Odpowiedź
> > $[J] = \mathrm{diag}(0{,}0125;\ 0{,}0125)$, $\det J = 0{,}00015625$ (jak na slajdzie z macierzą $C$). $\partial N_1/\partial x = -0{,}394338 / 0{,}0125 = -31{,}547$.

> [!zadanie] Zadanie 3
> Policz $\int_{-1}^1\int_{-1}^1 (5x^2y^2 + 3xy + 6)\,dx\,dy$ Gaussem 2-punktowym i analitycznie (praca domowa z labu 2).
> > [!odpowiedz]- Odpowiedź
> > W każdym z 4 punktów $x^2 = y^2 = 1/3$, a $xy = \pm 1/3$ i te wyrazy się znoszą. $f = 5/9 + 6$ w każdym punkcie, suma z wagami 1: $4 \cdot (5/9 + 6) = 26{,}222$. Analitycznie: $5 \cdot \frac23 \cdot \frac23 + 0 + 6 \cdot 4 = 26{,}222$.

## Fiszki #fiszki/mes
Jakobian przekształcenia 2D w zapisie Kustry::[J] = [[∂x/∂ξ, ∂y/∂ξ], [∂x/∂η, ∂y/∂η]]
Jak policzyć ∂x/∂ξ w elemencie 4-węzłowym?::∂x/∂ξ = Σ (∂N_i/∂ξ)·x_i
Jak z jakobianu dostać ∂N/∂x i ∂N/∂y?::{∂N/∂x; ∂N/∂y} = [J]⁻¹ · {∂N/∂ξ; ∂N/∂η}
Odwrotność macierzy 2×2 [[a, b], [c, d]]::1/(ad − bc) · [[d, −b], [−c, a]]
det J w 1D::(x2 − x1)/2 = L/2
det J prostokąta a × b::a·b/4, czyli pole elementu / 4
Gdzie liczy się jakobian?::W każdym punkcie całkowania osobno
Jak zmienia się dV przy przejściu do układu lokalnego?::dV = det J · dξ dη
det J elementu Test1_4_4::0,000277778 (bok 1/30 m)

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.

## Źródła
- P. Kustra, *Rozwiązanie problemu stacjonarnego oraz niestacjonarnego 2D — jakobian*: ![[03 Jakobian.pdf]]
- P. Kustra, *Całkowanie numeryczne metodą Gaussa w przedziale A,B; całkowanie macierzy H*: [[03.1 Całkowanie z jakobianem przekształcenia 1D-2D.pdf]]
- Wyniki prowadzącego: `Załączniki/MES/Kustra — wyniki testowe/Test1_4_4 — jakobian, H, Hbc, P lokalne.txt` i `Test2_4_4_MixGrid — jakobian, H, P lokalne.txt`.
- A. Milenin, *Podstawy metody elementów skończonych*, Wydawnictwa AGH 2010.
- O.C. Zienkiewicz, R.L. Taylor, J.Z. Zhu, *The Finite Element Method: Its Basis and Fundamentals*, rozdz. 6 (mapowanie izoparametryczne).
