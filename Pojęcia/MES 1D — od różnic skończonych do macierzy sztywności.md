---
typ: pojęcie
status: nowe
przedmioty: ["[[AGH Metoda elementów skończonych]]"]
wymaga: ["[[Układy równań liniowych]]", "[[Różniczkowanie numeryczne]]", "[[Całkowanie numeryczne i kwadratury Gaussa]]", "[[Numeryczne rozwiązywanie RRZ]]"]
powiązane: ["[[Funkcje kształtu]]", "[[Macierz sztywności]]", "[[Metoda Galerkina]]", "[[Nieustalony przepływ ciepła]]", "[[Schematy całkowania w czasie]]", "[[Interpolacja]]", "[[Równanie różniczkowe zwyczajne]]"]
tags: [pojęcie, metody-numeryczne, mes]
utworzono: 2026-10-02
źródła: ["A. Milenin — Podstawy metody elementów skończonych (Wyd. AGH 2010)", "O.C. Zienkiewicz, R.L. Taylor, J.Z. Zhu — The Finite Element Method: Its Basis and Fundamentals", "Z. Fortuna, B. Macukow, J. Wąsowski — Metody numeryczne (WNT)", "R.L. Burden, J.D. Faires — Numerical Analysis", "Dokumentacja numpy.linalg.solve"]
---
# MES 1D — od różnic skończonych do macierzy sztywności

> [!skrot] W 30 sekund
> - **Co to:** dzielisz pręt na elementy, w każdym przybliżasz temperaturę linią prostą, liczysz małą macierz $2\times2$ i **składasz** je w jeden układ $KT = F$.
> - **Klucz:** $K_e = \frac{kA}{L_e}\begin{bmatrix} 1 & -1 \\ -1 & 1 \end{bmatrix}$, $F_e = \frac{QAL_e}{2}\begin{bmatrix} 1 \\ 1 \end{bmatrix}$, a konwekcja na brzegu dodaje $\alpha A$ do $K_{ii}$ i $\alpha A T_\infty$ do $F_i$.
> - **Kiedy:** pierwszy lab MES (pręt, ustalony przepływ ciepła) i fundament projektu 2D, gdzie zmienia się tylko element: 4 węzły, Gauss i jakobian.

> [!intuicja] Po ludzku
> Pręt to łańcuch rezystorów cieplnych. Każdy element przewodzi ciepło proporcjonalnie do różnicy temperatur na swoich końcach, ze „sprawnością” $kA/L_e$, jak przewodność elektryczna. Agregacja to prawo Kirchhoffa: w każdym węźle suma strumieni od sąsiednich elementów plus źródła musi dać zero. Konwekcja na końcu to dodatkowy opór $1/(\alpha A)$.

## Definicja i wzory
> [!definicja] Zadanie
> Ustalony przepływ ciepła w pręcie: $-\frac{d}{dx}\left(k\frac{dT}{dx}\right) = Q$ na $[0, L]$, przekrój $A$. Warunki brzegowe:
> - **Dirichlet:** $T = \bar T$ (zadana temperatura),
> - **Neumann:** zadany strumień $q$ (w kodzie kursu $q > 0$ oznacza ciepło **wpływające**),
> - **Robin (konwekcja):** strumień wpływający $= \alpha(T_\infty - T)$.

> [!wzor] (a) Różnice skończone
> W węźle wewnętrznym, przy kroku $h$: $-k\frac{T_{i-1} - 2T_i + T_{i+1}}{h^2} = Q$. Wychodzi układ **trójdiagonalny** (algorytm Thomasa, $O(n)$).

> [!wzor] (b) Element liniowy
> Funkcje kształtu $N_1 = 1 - \frac{s}{L_e}$, $N_2 = \frac{s}{L_e}$, gdzie $T(s) = N_1 T_1 + N_2 T_2$, a $B = \frac{dN}{ds} = \left[-\frac1{L_e}, \frac1{L_e}\right]$. Minimalizacja funkcjonału
> $$J = \int_0^L \left[\frac k2 \left(\frac{dT}{dx}\right)^2 - QT\right] A\,dx + \text{wyrazy brzegowe}$$
> (albo metoda Galerkina, która daje to samo) prowadzi do
> $$K_e = \int kA\,B^T B\,ds = \frac{kA}{L_e}\begin{bmatrix} 1 & -1 \\ -1 & 1 \end{bmatrix}, \qquad F_e = \int QA\,N^T ds = \frac{QAL_e}{2}\begin{bmatrix} 1 \\ 1 \end{bmatrix}$$
> **Agregacja:** element o węzłach $(i, j)$ dodaje $K_e$ w wiersze i kolumny $i, j$ macierzy globalnej.
> **Warunki brzegowe** w węźle brzegowym $i$:
> - strumień: $F_i \mathrel{+}= qA$,
> - konwekcja: $K_{ii} \mathrel{+}= \alpha A$ oraz $F_i \mathrel{+}= \alpha A T_\infty$,
> - Dirichlet: wiersz $i$ zastępujesz równaniem $T_i = \bar T$ (zera, $1$ na przekątnej, $F_i = \bar T$).
>
> Przed nałożeniem warunków $K$ jest **symetryczna, pasmowa i osobliwa**: suma każdego wiersza wynosi 0, bo stała temperatura nie daje żadnego przepływu.

> [!wzor] (c) Nieustalony: macierz pojemności
> $$\rho c\,\dot T - (kT')' = Q \;\Rightarrow\; C\dot T + KT = F$$
> $$C_e^{\text{spójna}} = \frac{\rho c A L_e}{6}\begin{bmatrix} 2 & 1 \\ 1 & 2 \end{bmatrix}, \qquad C_e^{\text{skupiona}} = \frac{\rho c A L_e}{2}\begin{bmatrix} 1 & 0 \\ 0 & 1 \end{bmatrix}$$
> W czasie stosujesz schemat θ z [[Numeryczne rozwiązywanie RRZ]]: $\left(\frac{C}{\Delta t} + \theta K\right) T_{k+1} = \left(\frac{C}{\Delta t} - (1-\theta)K\right) T_k + F$.

## Jak to zrobić — krok po kroku
1. **Dane globalne:** $L$, liczba elementów, $k$, $A$, $Q$, $\rho$, $c$, warunki na obu końcach.
2. **Siatka:** lista węzłów (współrzędne $x$) i lista elementów (pary numerów węzłów).
3. Dla każdego elementu policz $K_e$ i $F_e$, a potem **dodaj** je do $K$ i $F$ (nie przypisuj!).
4. Nanieś warunki naturalne (strumień, konwekcja), a na końcu Dirichleta.
5. Rozwiąż $KT = F$ i sprawdź wynik z rozwiązaniem analitycznym oraz bilansem ciepła.
6. **Nieustalony:** zbuduj $C$ tak samo jak $K$. W pętli czasowej rozwiązuj układ schematu θ, a Dirichleta nakładaj na macierz układu.
7. **Weryfikacja (jak w projekcie):** zagęść siatkę i skróć krok czasowy. Wynik nie może się istotnie zmieniać.

> [!wazne] Różnice skończone a MES
> Na siatce równomiernej równanie MES dla węzła wewnętrznego to $\frac{kA}{h}(-T_{i-1} + 2T_i - T_{i+1}) = QAh$, czyli **dokładnie** równanie różnic skończonych pomnożone przez $A/h$. MES wygrywa przy nierównych siatkach, w 2D na dowolnych kształtach i przy naturalnym wprowadzaniu warunków strumieniowych.

## Przykład
> [!przyklad] Pręt na 2 elementach, policzony ręcznie
> $L = 0{,}2$ m, 2 elementy po $L_e = 0{,}1$ m, $k = 40$ W/(m·K), $A = 0{,}01$ m², $Q = 0$. Lewy koniec ma $T = 100$ °C, prawy oddaje ciepło przez konwekcję: $\alpha = 200$ W/(m²·K), $T_\infty = 20$ °C.
>
> **1. Element:** $\frac{kA}{L_e} = \frac{40 \cdot 0{,}01}{0{,}1} = 4$ W/K, więc $K_e = 4\begin{bmatrix} 1 & -1 \\ -1 & 1 \end{bmatrix}$, a $F_e = 0$.
>
> **2. Agregacja** (element 1 to węzły 1–2, element 2 to węzły 2–3; w środku spotykają się dwa razy po 4):
> $$K = \begin{bmatrix} 4 & -4 & 0 \\ -4 & 8 & -4 \\ 0 & -4 & 4 \end{bmatrix}, \qquad \det K = 0$$
>
> **3. Konwekcja w węźle 3:** $\alpha A = 2$ W/K, $\alpha A T_\infty = 40$ W. **Dirichlet w węźle 1:** wiersz $[1, 0, 0]$ i $F_1 = 100$.
> $$\begin{bmatrix} 1 & 0 & 0 \\ -4 & 8 & -4 \\ 0 & -4 & 6 \end{bmatrix}\begin{bmatrix} T_1 \\ T_2 \\ T_3 \end{bmatrix} = \begin{bmatrix} 100 \\ 0 \\ 40 \end{bmatrix}$$
>
> **4. Rozwiązanie:** z trzeciego równania $T_2 = 1{,}5T_3 - 10$. Po wstawieniu do drugiego: $-400 + 8(1{,}5T_3 - 10) - 4T_3 = 0$, czyli $8T_3 = 480$. Wychodzi $T_3 = 60$ °C i $T_2 = 80$ °C.
>
> **5. Sprawdzenie analityczne:** szeregowo opór przewodzenia $L/k = 0{,}005$ i opór konwekcji $1/\alpha = 0{,}005$ (m²·K/W), więc $q = \frac{100 - 20}{0{,}01} = 8000$ W/m² i $T(x) = 100 - \frac{q}{k}x = 100 - 200x$. Daje to $T(0{,}1) = 80$ i $T(0{,}2) = 60$. MES trafia **dokładnie**, bo w 1D przy elementach liniowych wartości w węzłach są dokładne. Bilans: przewodzenie $q A = 80$ W, konwekcja $\alpha A (60 - 20) = 80$ W.

```python
import numpy as np
k, A, Le, alfa, Tinf = 40.0, 0.01, 0.1, 200.0, 20.0
elementy = [(0, 1), (1, 2)]
K, F = np.zeros((3, 3)), np.zeros(3)
for i, j in elementy:                                  # agregacja
    K[np.ix_([i, j], [i, j])] += k * A / Le * np.array([[1, -1], [-1, 1]])
K[2, 2] += alfa * A; F[2] += alfa * A * Tinf           # konwekcja (Robin)
K[0, :] = 0; K[0, 0] = 1; F[0] = 100.0                 # Dirichlet
print(np.linalg.solve(K, F))                           # [100.  80.  60.]
```

> [!przyklad]- To samo z macierzą pojemności (stal: $\rho = 7800$, $c = 460$)
> $\rho c A L_e = 7800 \cdot 460 \cdot 0{,}01 \cdot 0{,}1 = 3588$ J/K. Macierz spójna elementu to $598\begin{bmatrix} 2 & 1 \\ 1 & 2 \end{bmatrix}$, a skupiona $1794\,I$. Po agregacji (skupiona) $C = \mathrm{diag}(1794,\ 3588,\ 1794)$. Suma wszystkich wyrazów $C$ to zawsze $\rho c A L = 7176$ J/K (pojemność cieplna całego pręta), co jest dobrym testem kodu.

## Typowe błędy
> [!blad] Przypisanie zamiast dodawania przy agregacji
> `K[i, j] = Ke[...]` nadpisuje wkład sąsiedniego elementu. Musi być `+=`. Objaw: środkowy wyraz przekątnej równy 4 zamiast 8.

> [!blad] Rozwiązywanie bez warunku Dirichleta albo konwekcji
> Sama macierz $K$ jest osobliwa, więc `solve` zgłosi błąd albo zwróci śmieci. Temperatura musi być gdzieś „zakotwiczona”: zadana wartość albo wymiana z otoczeniem.

> [!blad] Konwekcja tylko w wektorze F
> Warunek $\alpha(T - T_\infty)$ zawiera **niewiadomą** $T$, więc część $\alpha A$ idzie do $K$, a do $F$ tylko $\alpha A T_\infty$.

> [!blad] Znak strumienia
> Ustal raz konwencję (tutaj $q > 0$ to ciepło wpływające) i trzymaj się jej. W podręczniku albo instrukcji prowadzącego znak może być odwrotny, więc sprawdź to na prostym bilansie.

## Na zajęcia i egzamin
> [!zajecia]
> - **Lab MES 1D:** dokładnie ten pręt: funkcjonał → $K_e$, $F_e$ → agregacja → warunki brzegowe → rozwiązanie → porównanie z analitycznym. Ćwicz na `m08_mes1d.py`, a funkcję `nieustalony` traktuj jako szkielet pętli czasowej projektu 2D.
> - **Projekt 2D:** struktury (węzły, elementy, dane globalne), agregacja i schemat θ zostają bez zmian. Zmienia się $K_e$ i $C_e$, które liczysz Gaussem 2×2 z $\det J$ ([[Całkowanie numeryczne i kwadratury Gaussa]]), oraz warunek konwekcji, który działa teraz na krawędzi elementu.
> - **Weryfikacja w sprawozdaniu:** zbieżność przy zagęszczaniu siatki (błąd między węzłami $\sim h^2$), wpływ kroku czasowego, porównanie spójnej i skupionej $C$, kontrola bilansu ciepła.
> - **Egzamin:** „wyprowadź macierz sztywności elementu 1D”, „jak uwzględnić konwekcję i strumień”, „dlaczego $K$ jest osobliwa przed warunkami brzegowymi”.
> - Łączy się z: [[Macierz sztywności]], [[Funkcje kształtu]], [[Metoda Galerkina]], [[Nieustalony przepływ ciepła]].

## Zadania
> [!zadanie] Zadanie 1
> Złóż globalną $K$ dla 3 równych elementów z $kA/L_e = 5$.
> > [!odpowiedz]- Odpowiedź
> > $K = 5\begin{bmatrix} 1 & -1 & 0 & 0 \\ -1 & 2 & -1 & 0 \\ 0 & -1 & 2 & -1 \\ 0 & 0 & -1 & 1 \end{bmatrix}$. Jest symetryczna, trójdiagonalna, a sumy wierszy wynoszą 0.

> [!zadanie] Zadanie 2
> Pręt $L = 1$, $k = 1$, $A = 1$, $Q = 2$, oba końce $T = 0$, 2 elementy. Policz $T$ w środku i porównaj z analitycznym.
> > [!odpowiedz]- Odpowiedź
> > $L_e = 0{,}5$, $kA/L_e = 2$, $F_e = Q A L_e/2 \cdot [1, 1] = [0{,}5;\ 0{,}5]$, a w środkowym węźle $F_2 = 1$. Równanie: $4T_2 = 1$, więc $T_2 = 0{,}25$. Analitycznie $T = x(1-x)$, czyli $T(0{,}5) = 0{,}25$: wynik dokładny w węźle (między węzłami już nie).

> [!zadanie] Zadanie 3
> W przykładzie z notatki zamień konwekcję na prawym końcu na zadany strumień wypływający $8000$ W/m². Jaki wynik?
> > [!odpowiedz]- Odpowiedź
> > $F_3 = qA = -8000 \cdot 0{,}01 = -80$, $K_{33} = 4$. Rozwiązanie: $T = [100, 80, 60]$, takie samo jak wcześniej, bo to ten sam strumień.

## Fiszki #fiszki/numeryczne
Lokalna macierz sztywności liniowego elementu 1D::kA/L_e razy macierz (1, −1; −1, 1)
Lokalny wektor obciążeń od źródła Q w elemencie 1D::Q·A·L_e/2 · [1, 1]
Jak konwekcja na brzegu zmienia K i F?::K_ii += α·A oraz F_i += α·A·T_∞
Jak wprowadzić warunek Dirichleta przez modyfikację wiersza?::Wiersz i zerujesz, K_ii = 1, F_i = zadana temperatura
Dlaczego K jest osobliwa przed warunkami brzegowymi?::Sumy wierszy są zerowe: stała temperatura nie daje przepływu, więc T jest określone z dokładnością do stałej
Własności globalnej macierzy sztywności::Symetryczna, pasmowa, dodatnio półokreślona (po warunkach brzegowych dodatnio określona)
Spójna macierz pojemności elementu 1D::ρcA·L_e/6 razy macierz (2, 1; 1, 2)
Skupiona macierz pojemności elementu 1D::ρcA·L_e/2 · I, czyli sumy wierszy spójnej na przekątnej
Kiedy MES 1D z elementami liniowymi daje dokładne wartości w węzłach?::Dla −(kT')' = Q przy stałym k, gdy wektor obciążeń jest policzony dokładnie
Jak maleje błąd MES liniowego między węzłami przy zagęszczaniu siatki?::Jak h² (podwojenie liczby elementów zmniejsza błąd 4 razy)
Czym różni się MES od różnic skończonych na równej siatce 1D?::Dla węzłów wewnętrznych niczym, równania są identyczne z dokładnością do czynnika A/h

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.

## Źródła
- A. Milenin, *Podstawy metody elementów skończonych*, Wydawnictwa AGH, Kraków 2010 (MES 1D dla przepływu ciepła, funkcjonał, procesy nieustalone).
- O.C. Zienkiewicz, R.L. Taylor, J.Z. Zhu, *The Finite Element Method: Its Basis and Fundamentals*, Elsevier.
- Z. Fortuna, B. Macukow, J. Wąsowski, *Metody numeryczne*, WNT (układy trójdiagonalne, różnice skończone).
- R.L. Burden, J.D. Faires, *Numerical Analysis*, Cengage (finite-difference methods for boundary-value problems).
- Dokumentacja: `numpy.linalg.solve`.
