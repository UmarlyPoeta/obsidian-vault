---
typ: pojęcie
status: nowe
aliases: ["Macierz H", "Macierz Hbc", "Wektor P", "Agregacja"]
przedmioty: ["[[AGH Metoda elementów skończonych]]"]
wymaga: ["[[Jakobian]]", "[[Funkcje kształtu]]", "[[Całkowanie numeryczne i kwadratury Gaussa]]", "[[Metoda Galerkina]]"]
powiązane: ["[[MES — kurs dr. Kustry (mapa labów)]]", "[[MES 1D — od różnic skończonych do macierzy sztywności]]", "[[Nieustalony przepływ ciepła]]", "[[Układy równań liniowych]]", "[[Siatka MES i wczytywanie danych]]"]
tags: [pojęcie, mes]
utworzono: 2026-10-05
źródła: ["P. Kustra — Całkowanie macierzy H; Agregacja 1D i 2D; Całkowanie macierzy Hbc; Całkowanie wektora obciążeń P (AGH WIMiIP)", "A. Milenin — Podstawy metody elementów skończonych (Wyd. AGH 2010)", "O.C. Zienkiewicz, R.L. Taylor, J.Z. Zhu — The Finite Element Method: Its Basis and Fundamentals"]
---
# Macierz sztywności

> [!skrot] W 30 sekund
> - **Co to:** w zadaniu cieplnym „macierz sztywności” nazywa się $[H]$ (macierz przewodności). Opisuje, jak łatwo ciepło płynie między węzłami. $[H_{bc}]$ i $\{P\}$ dokładają wymianę ciepła z otoczeniem na brzegu.
> - **Klucz:** $[H]_e = \sum_{p} k\left(\frac{\partial N}{\partial x}\frac{\partial N}{\partial x}^T + \frac{\partial N}{\partial y}\frac{\partial N}{\partial y}^T\right)_p w_p \det J_p$, $[H_{bc}]_e = \sum_{\text{boki BC}}\sum_p \alpha N N^T w_p \det J_{bok}$, $\{P\}_e = \sum_{\text{boki BC}}\sum_p \alpha N t_{ot}\,w_p \det J_{bok}$, gdzie $\det J_{bok} = L_{bok}/2$.
> - **Kiedy:** laby 4–8. Potem agregacja do macierzy globalnej i rozwiązanie $[H + H_{bc}]\{t\} = \{P\}$ (stan ustalony).

> [!intuicja] Po ludzku
> $H$ to sieć „rur” przewodzących ciepło między węzłami: wyraz $H_{ij} < 0$ mówi, jak mocno węzły $i$ i $j$ są połączone, a $H_{ii}$ to suma wszystkich połączeń węzła. $H_{bc}$ dokleja do węzłów brzegowych dodatkowe rury do otoczenia, a $P$ mówi, że po drugiej stronie tych rur jest piec o temperaturze $t_{ot}$. Agregacja to zlutowanie rur ze wszystkich elementów w jedną instalację.

## Definicja i wzory
> [!wzor] Macierz $[H]$ elementu (lab 4–5)
> $$[H] = \int_V k\left(\frac{\partial\{N\}}{\partial x}\frac{\partial\{N\}^T}{\partial x} + \frac{\partial\{N\}}{\partial y}\frac{\partial\{N\}^T}{\partial y}\right)dV \approx \sum_{p=1}^{n_{pc}} k\,\big(\ldots\big)_p\,w_p\,\det J_p$$
> Dla 2 punktów w 2D: $n_{pc} = 4$, $w_p = 1 \cdot 1$. Dla 3 punktów: 9 punktów, $w_p = w_i w_j$ z $\{5/9, 8/9, 5/9\}$.
>
> **Wynik kontrolny:** element kwadratowy (dowolny rozmiar!) ma
> $$[H] = \frac{k}{6}\begin{bmatrix} 4 & -1 & -2 & -1 \\ -1 & 4 & -1 & -2 \\ -2 & -1 & 4 & -1 \\ -1 & -2 & -1 & 4 \end{bmatrix}$$
> Dla $k = 25$: $16{,}6667$, $-4{,}16667$, $-8{,}33333$, tak jak w pliku prowadzącego.

> [!wzor] Macierz $[H_{bc}]$ — warunek konwekcji (lab 7)
> $$[H_{bc}] = \int_S \alpha\{N\}\{N\}^T dS \approx \sum_{\text{bok } b \text{ z BC}}\ \sum_{p=1}^{n_{pc}} \alpha\,\{N(\xi_p, \eta_p)\}\{N(\xi_p, \eta_p)\}^T\,w_p\,\det J_b, \qquad \det J_b = \frac{L_b}{2}$$
> - Bok liczysz tylko, gdy **oba** jego węzły mają flagę BC.
> - Punkty całkowania leżą **na boku**. Boki elementu uniwersalnego: bok 1 ($\eta = -1$, węzły 1–2), bok 2 ($\xi = 1$, węzły 2–3), bok 3 ($\eta = 1$, węzły 3–4), bok 4 ($\xi = -1$, węzły 4–1). Np. bok 1 przy 2 punktach: $(-1/\sqrt3, -1)$ i $(1/\sqrt3, -1)$.
> - Na boku niezerowe są tylko 2 funkcje kształtu, w punktach Gaussa równe $0{,}788675$ i $0{,}211325$.
>
> **Wynik kontrolny:** jeden bok długości $L$ daje na swoich dwóch węzłach $\frac{\alpha L}{6}\begin{bmatrix} 2 & 1 \\ 1 & 2 \end{bmatrix}$.

> [!wzor] Wektor $\{P\}$ (lab 8)
> $$\{P\} = \int_S \alpha\{N\}t_{ot}\,dS \approx \sum_{\text{bok } b \text{ z BC}}\ \sum_p \alpha\,\{N_p\}\,t_{ot}\,w_p\,\det J_b$$
> Jeden bok długości $L$ daje $\frac{\alpha t_{ot} L}{2}$ na każdy z dwóch węzłów. Znak: w programie rozwiązujesz $[H + H_{bc}]\{t\} = \{P\}$, więc $P > 0$ (zapis teoretyczny $[H]\{t\} + \{P\} = 0$ ma $P$ z minusem, patrz [[Metoda Galerkina]]).

> [!wzor] Agregacja (lab 6)
> Element ma węzły globalne $ID = \{ID_1, ID_2, ID_3, ID_4\}$. Wyraz lokalny $(i, j)$ **dodajesz** do globalnego $(ID_i, ID_j)$:
> $$H^{G}_{ID_i,\,ID_j} \mathrel{+}= H^{e}_{ij} + H^{e}_{bc,\,ij}, \qquad P^{G}_{ID_i} \mathrel{+}= P^{e}_{i}$$
> Przykład ze slajdu (1D): element o węzłach 3 i 8 wpisuje $H_{11} \to H[3][3]$, $H_{12} \to H[3][8]$, $H_{21} \to H[8][3]$, $H_{22} \to H[8][8]$.

## Jak to zrobić — krok po kroku
1. Element uniwersalny: $\partial N/\partial \xi$, $\partial N/\partial \eta$ w punktach wnętrza oraz $N$ w punktach na każdym z 4 boków.
2. Dla każdego elementu i każdego punktu wnętrza: [[Jakobian]], $\partial N/\partial x$, $\partial N/\partial y$, dodaj $k(\ldots)w_p\det J_p$ do $[H]_e$.
3. Dla każdego boku: sprawdź flagi BC obu węzłów. Jeśli tak, policz $L_b$ z współrzędnych, $\det J_b = L_b/2$ i dodaj wkłady do $[H_{bc}]_e$ i $\{P\}_e$.
4. Agreguj $[H]_e + [H_{bc}]_e$ i $\{P\}_e$ do globalnych.
5. Stan ustalony: rozwiąż $[H + H_{bc}]\{t\} = \{P\}$ (np. eliminacją Gaussa, patrz [[Układy równań liniowych]]).

## Przykład
> [!przyklad] Narożny element Test1_4_4 ($k = 25$, $\alpha = 300$, $t_{ot} = 1200$, bok $L = 1/30$ m)
> **H:** element jest kwadratem, więc $[H] = \frac{25}{6}[\ldots]$: przekątna $16{,}6667$, sąsiedzi $-4{,}16667$, naprzeciw $-8{,}33333$. Suma każdego wiersza wynosi 0.
>
> **Hbc:** element 1 ma węzły 1, 2, 6, 5. Flagę BC mają 1, 2 i 5, więc liczą się boki 1–2 (bok 1) i 5–1 (bok 4). Bok 2–6 odpada, bo węzeł 6 jest wewnątrz. Każdy bok daje $\frac{\alpha L}{6}\begin{bmatrix} 2 & 1 \\ 1 & 2 \end{bmatrix} = \frac{300}{180}\begin{bmatrix} 2 & 1 \\ 1 & 2 \end{bmatrix} = \begin{bmatrix} 3{,}333 & 1{,}667 \\ 1{,}667 & 3{,}333 \end{bmatrix}$. Węzeł 1 należy do obu boków, więc:
> $$[H_{bc}]_1 = \begin{bmatrix} 6{,}667 & 1{,}667 & 0 & 1{,}667 \\ 1{,}667 & 3{,}333 & 0 & 0 \\ 0 & 0 & 0 & 0 \\ 1{,}667 & 0 & 0 & 3{,}333 \end{bmatrix}$$
>
> **P:** każdy bok daje $\frac{\alpha t_{ot} L}{2} = \frac{300 \cdot 1200}{60} = 6000$ na węzeł, więc $\{P\}_1 = [12000;\ 6000;\ 0;\ 6000]$.
>
> **Po agregacji:** w węźle globalnym 1 jest tylko element 1, więc $H^G_{11} = 16{,}667 + 6{,}667 = 23{,}333$. Wszystkie trzy wyniki zgadzają się z plikami prowadzącego.

> [!przyklad]- Slajd prowadzącego: bok $L = 0{,}025$ m, $\alpha = 25$, $t_{ot} = 1200$
> $\det J_b = 0{,}0125$. W punktach boku: $N = [0{,}7887;\ 0{,}2113]$ i $[0{,}2113;\ 0{,}7887]$.
> - $H_{bc}$: $25 \cdot 0{,}0125 \cdot (0{,}7887^2 + 0{,}2113^2) = 0{,}2083$ na przekątnej i $25 \cdot 0{,}0125 \cdot 2 \cdot 0{,}7887 \cdot 0{,}2113 = 0{,}1042$ poza nią. To samo daje $\frac{\alpha L}{6}[2, 1]$.
> - $P$: $25 \cdot 1200 \cdot 0{,}0125 \cdot (0{,}7887 + 0{,}2113) = 375$ na każdy z dwóch węzłów.

```python
import numpy as np
alfa, t_ot, L = 300.0, 1200.0, 1 / 30
detJ = L / 2
Hbc, P = np.zeros((2, 2)), np.zeros(2)
for s in (-1 / np.sqrt(3), 1 / np.sqrt(3)):          # 2 punkty na boku, wagi 1
    N = np.array([0.5 * (1 - s), 0.5 * (1 + s)])
    Hbc += alfa * np.outer(N, N) * detJ
    P += alfa * N * t_ot * detJ
print(Hbc.round(4), P)   # [[3.3333 1.6667] [1.6667 3.3333]] [6000. 6000.]
```

## Typowe błędy
> [!blad] `=` zamiast `+=` przy agregacji
> Węzeł wewnętrzny należy do 4 elementów. Przypisanie nadpisuje wkład poprzednich: zamiast $66{,}667$ na przekątnej zobaczysz $16{,}667$.

> [!blad] $\det J$ wnętrza zamiast $\det J$ boku
> W $H_{bc}$ i $P$ całkujesz po **linii**, więc mnożysz przez $L_b/2$, a nie przez $\det J$ z wnętrza elementu (które jest polem/4).

> [!blad] Bok z jednym węzłem BC
> Węzeł 5 w Test1 ma BC, węzeł 6 nie. Bok 5–6 nie wymienia ciepła z otoczeniem, mimo że węzeł 5 jest brzegowy.

> [!blad] Wiersze $H$ nie sumują się do zera
> Bez $H_{bc}$ suma każdego wiersza $[H]$ musi wynosić 0 (stała temperatura nie daje przepływu). To szybki test poprawności $\partial N/\partial x$.

## Na zajęcia i egzamin
> [!zajecia]
> - **Ćwicz w C++:** `Kod/metody_numeryczne/zadania/m09.cpp`, funkcje `macierz_H` i `brzeg` (Hbc i P), testy na liczbach prowadzącego. Agregację i układ ćwiczysz w 1D w m08 (`pret_HP`).
> - **Laby 4–8:** każdy etap porównujesz z plikami w `Załączniki/MES/Kustra — wyniki testowe/`: $H$ lokalne, $H$ globalne bez BC, z BC, wektor $P$. Najpierw Test1 (proste liczby), potem MixGrid (tu wychodzą błędy w jakobianie).
> - **Stan ustalony dla Test1:** wszystkie temperatury $= 1200$ °C (brak źródeł, cały brzeg w otoczeniu 1200 °C). To dobry test układu równań.
> - **Egzamin:** „wyprowadź $[H]$ elementu”, „jak działa agregacja”, „jak wprowadzić warunek konwekcji”, „własności macierzy globalnej: symetryczna, pasmowa, rzadka, dodatnio określona po dodaniu $H_{bc}$”.
> - Łączy się z: [[Nieustalony przepływ ciepła]] (ta sama agregacja dla $C$), [[MES 1D — od różnic skończonych do macierzy sztywności]] (to samo w 1D).

## Zadania
> [!zadanie] Zadanie 1
> Ile wynosi $H^G_{6,6}$ dla Test1_4_4 i dlaczego?
> > [!odpowiedz]- Odpowiedź
> > Węzeł 6 należy do 4 elementów i w każdym jest na przekątnej z wartością $16{,}667$. Nie ma BC. Razem $4 \cdot 16{,}667 = 66{,}667$.

> [!zadanie] Zadanie 2
> Element prostokątny ma bok z BC o długości $0{,}05$ m, $\alpha = 50$, $t_{ot} = 20$. Podaj wkład do $H_{bc}$ i $P$ na tym boku.
> > [!odpowiedz]- Odpowiedź
> > $\frac{\alpha L}{6} = \frac{2{,}5}{6} = 0{,}4167$, więc $H_{bc} = \begin{bmatrix} 0{,}833 & 0{,}417 \\ 0{,}417 & 0{,}833 \end{bmatrix}$. $P = \frac{\alpha t_{ot} L}{2} = 25$ na każdy węzeł.

> [!zadanie] Zadanie 3
> Dlaczego stan ustalony Test1 to równe 1200 °C wszędzie? Pokaż to na równaniach.
> > [!odpowiedz]- Odpowiedź
> > Wstaw $t = 1200$ wszędzie. $[H]\{1200\} = 0$ (sumy wierszy $H$ są zerowe). $[H_{bc}]\{1200\} = 1200 \cdot$ (sumy wierszy $H_{bc}$) $= 1200 \cdot \sum \frac{\alpha L}{2} = P$. Równanie jest spełnione, a rozwiązanie jest jedyne.

## Fiszki #fiszki/mes
Wzór na lokalną macierz H w punktach Gaussa::H = Σ_p k·(∂N/∂x·∂Nᵀ/∂x + ∂N/∂y·∂Nᵀ/∂y)·w_p·det J_p
Macierz H elementu kwadratowego::k/6 · [[4,−1,−2,−1], [−1,4,−1,−2], [−2,−1,4,−1], [−1,−2,−1,4]]
Wzór na Hbc::Hbc = Σ_boki BC Σ_p α·N·Nᵀ·w_p·det J_boku, det J_boku = L/2
Wkład jednego boku długości L do Hbc::αL/6 · [[2, 1], [1, 2]] na jego dwóch węzłach
Wkład jednego boku długości L do wektora P::α·t_ot·L/2 na każdy z dwóch węzłów
Kiedy bok elementu ma warunek brzegowy?::Gdy oba jego węzły mają flagę BC
Na czym polega agregacja?::Wyraz lokalny H_ij dodajesz do globalnego H[ID_i][ID_j]
Własności globalnej macierzy H::Symetryczna, pasmowa, rzadka; z Hbc dodatnio określona
Szybki test poprawności lokalnej H (bez BC)::Suma każdego wiersza = 0
Wartości N na boku w 2-punktowym schemacie::0,788675 i 0,211325

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.

## Źródła
- P. Kustra, *Całkowanie macierzy H*: [[05 Całkowanie macierzy H.pdf]]
- P. Kustra, *Agregacja 1D oraz 2D*: [[06 Agregacja H.pdf]]
- P. Kustra, *Całkowanie macierzy [Hbc] metodą Gaussa*: [[07 Macierz Hbc.pdf]]
- P. Kustra, *Całkowanie wektora obciążeń {P}*: [[08 Wektor P.pdf]]
- Wyniki prowadzącego: `Załączniki/MES/Kustra — wyniki testowe/`.
- A. Milenin, *Podstawy metody elementów skończonych*, Wydawnictwa AGH 2010, rozdz. 5 i 9.
- O.C. Zienkiewicz, R.L. Taylor, J.Z. Zhu, *The Finite Element Method: Its Basis and Fundamentals*, rozdz. o przewodzeniu ciepła i agregacji.
