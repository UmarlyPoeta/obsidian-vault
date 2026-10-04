---
typ: pojęcie
status: nowe
przedmioty: ["[[AGH Metoda elementów skończonych]]"]
wymaga: ["[[Funkcje kształtu]]", "[[Pochodna]]", "[[Całka oznaczona]]", "[[Układy równań liniowych]]"]
powiązane: ["[[MES — kurs dr. Kustry (mapa labów)]]", "[[MES 1D — od różnic skończonych do macierzy sztywności]]", "[[Macierz sztywności]]", "[[Nieustalony przepływ ciepła]]", "[[Funkcja celu i warunki optymalności]]"]
tags: [pojęcie, mes]
utworzono: 2026-10-05
źródła: ["P. Kustra — Wstęp: Symulacja ustalonych procesów cieplnych (rozdz. 5 z: A. Milenin, Podstawy MES, Wyd. AGH 2010)", "O.C. Zienkiewicz, R.L. Taylor, J.Z. Zhu — The Finite Element Method: Its Basis and Fundamentals", "H.P. Langtangen, K.-A. Mardal — Introduction to Numerical Methods for Variational Problems (Springer 2019, CC BY)"]
---
# Metoda Galerkina

> [!skrot] W 30 sekund
> - **Co to:** sposób zamiany równania różniczkowego (Fouriera) na układ równań liniowych $[H]\{t\} + \{P\} = 0$. W kursie robi się to przez **minimalizację funkcjonału** (metoda Ritza). Dla przewodzenia ciepła daje ona dokładnie to samo, co metoda Galerkina.
> - **Klucz:** wstawiasz $t = \{N\}^T\{t\}$ do funkcjonału $J$ i żądasz $\partial J/\partial \{t\} = 0$. Wychodzi $[H] = \int_V k\left(\frac{\partial N}{\partial x}\frac{\partial N}{\partial x}^T + \frac{\partial N}{\partial y}\frac{\partial N}{\partial y}^T\right)dV + \int_S \alpha N N^T dS$ oraz $\{P\} = -\int_S \alpha N t_\infty\,dS - \int_V Q N\,dV + \int_S q N\,dS$.
> - **Kiedy:** „Wstęp” do wydrukowania na lab 1, pytania egzaminacyjne o wyprowadzenie, a w kodzie każda całka $H$, $H_{bc}$, $P$.

> [!intuicja] Po ludzku
> Przyroda „wybiera” takie pole temperatury, przy którym pewna wielkość $J$ jest najmniejsza. To trochę jak sznurek, który sam układa się tak, żeby energia była minimalna. Nie znasz całej funkcji $t(x,y)$, ale zgadzasz się szukać jej tylko wśród „łamanych” zbudowanych z funkcji kształtu. Wtedy $J$ zależy tylko od kilku liczb (temperatur węzłowych), a minimum funkcji wielu zmiennych znajdziesz, przyrównując pochodne do zera. Tak jak w [[Funkcja celu i warunki optymalności]].

## Definicja i wzory
> [!definicja] Równanie Fouriera i warunki brzegowe
> Stan ustalony: $\operatorname{div}(k\,\operatorname{grad} t) + Q = 0$, czyli w 2D
> $$\frac{\partial}{\partial x}\left(k\frac{\partial t}{\partial x}\right) + \frac{\partial}{\partial y}\left(k\frac{\partial t}{\partial y}\right) + Q = 0$$
> Trzy typy warunków na brzegu:
> - zadana temperatura $t$,
> - **konwekcja:** $k\frac{\partial t}{\partial n} + \alpha(t - t_\infty) = 0$ (ten warunek jest w projekcie),
> - promieniowanie $\sigma(t^4 - t_\infty^4)$. Łączy się je z konwekcją w efektywne $\alpha = \alpha_{konw} + \sigma(t^2 + t_\infty^2)(t + t_\infty)$, liczone iteracyjnie.

> [!wzor] Funkcjonał (wzór 5.9 we „Wstępie”)
> $$J = \int_V \left[\frac{k}{2}\left(\left(\frac{\partial t}{\partial x}\right)^2 + \left(\frac{\partial t}{\partial y}\right)^2\right) - Qt\right]dV + \int_S \frac{\alpha}{2}(t - t_\infty)^2 dS + \int_S q\,t\,dS$$
> Równanie Fouriera jest **równaniem Eulera** tego funkcjonału, więc funkcja minimalizująca $J$ spełnia równanie Fouriera i warunki brzegowe.

> [!wzor] Dyskretyzacja → układ równań
> 1. $t = \sum_i N_i t_i = \{N\}^T\{t\}$.
> 2. $\frac{\partial J}{\partial \{t\}} = 0$ daje $[H]\{t\} + \{P\} = 0$, gdzie
> $$[H] = \int_V k\left(\frac{\partial\{N\}}{\partial x}\frac{\partial\{N\}^T}{\partial x} + \frac{\partial\{N\}}{\partial y}\frac{\partial\{N\}^T}{\partial y}\right)dV + \int_S \alpha\{N\}\{N\}^T dS$$
> $$\{P\} = -\int_S \alpha\{N\}t_\infty\,dS - \int_V Q\{N\}\,dV + \int_S q\{N\}\,dS$$
> 3. W kodzie rozbija się $[H]$ na część objętościową $[H]$ i brzegową $[H_{bc}]$, a znak przenosi się na drugą stronę: $[H + H_{bc}]\{t\} = \{P\}$ z $\{P\} = \int_S \alpha N t_\infty dS$. Pliki wyników prowadzącego używają tej drugiej wersji ($P = 12000 > 0$).

> [!wzor] Galerkin wprost (to samo inną drogą)
> Mnożysz równanie przez funkcję testową $N_j$, całkujesz po obszarze i przez części przerzucasz jedną pochodną na $N_j$:
> $$\int_V k\,\nabla N_j \cdot \nabla t\,dV + \int_S \alpha N_j (t - t_\infty)\,dS - \int_V Q N_j\,dV = 0 \quad \text{dla każdego } j$$
> Po wstawieniu $t = \sum N_i t_i$ wychodzi ten sam układ. Funkcjonału nie da się zbudować dla każdego równania (np. konwekcja–dyfuzja), a Galerkina da się zawsze. Dlatego w literaturze częściej spotkasz tę drugą drogę.

## Jak to zrobić — krok po kroku
1. Zapisz funkcjonał dla swojego problemu i warunków brzegowych.
2. Podziel obszar na elementy, w każdym $t = \{N\}^T\{t\}$.
3. Policz całki elementowe: część z $k$ daje $H$, część z $\alpha$ daje $H_{bc}$ i $P$.
4. Zsumuj wkłady elementów (agregacja) i rozwiąż układ.

## Przykład
> [!przyklad] Pręt na dwóch elementach (rozdział 5.2 „Wstępu”, liczby moje)
> Pręt $L = 0{,}1$ m, przekrój $S = 4\cdot10^{-4}$ m², $k = 50$ W/(m·K), dwa elementy po $L^{(e)} = 0{,}05$ m. Na lewym końcu **doprowadzasz** $10\,000$ W/m², czyli w konwencji „Wstępu” $q = -10\,000$ ($q > 0$ oznacza ciepło **odprowadzane**). Prawy koniec oddaje ciepło przez konwekcję: $\alpha = 100$ W/(m²·K), $t_\infty = 20$ °C.
>
> **1. Współczynniki** ze wzoru (5.30): $C^{(1)} = C^{(2)} = \frac{Sk}{L^{(e)}} = \frac{4\cdot10^{-4}\cdot50}{0{,}05} = 0{,}4$ W/K, a $\alpha S = 0{,}04$ W/K.
>
> **2. Układ (5.32):**
> $$\begin{bmatrix} 0{,}4 & -0{,}4 & 0 \\ -0{,}4 & 0{,}8 & -0{,}4 \\ 0 & -0{,}4 & 0{,}44 \end{bmatrix}\begin{Bmatrix} t_1 \\ t_2 \\ t_3 \end{Bmatrix} + \begin{Bmatrix} qS \\ 0 \\ -\alpha S t_\infty \end{Bmatrix} = 0, \qquad \{P\} = \begin{Bmatrix} -4 \\ 0 \\ -0{,}8 \end{Bmatrix}$$
>
> **3. Rozwiązanie:** z pierwszego równania $t_1 - t_2 = 10$, z drugiego $t_2 - t_3 = t_1 - t_2 = 10$, z trzeciego $-0{,}4t_2 + 0{,}44t_3 = 0{,}8$. Po podstawieniu $t_2 = t_3 + 10$ wychodzi $0{,}04t_3 = 4{,}8$, czyli $t_3 = 120$ °C, $t_2 = 130$ °C, $t_1 = 140$ °C.
>
> **4. Bilans:** wpływa $|q|S = 4$ W, a konwekcja odbiera $\alpha S(t_3 - t_\infty) = 0{,}04 \cdot 100 = 4$ W. Spadek temperatury na przewodzeniu $|q|L/k = 20$ K, zgodnie z $140 - 120$.

```python
import numpy as np
S, k, Le, alfa, t_inf, q = 4e-4, 50.0, 0.05, 100.0, 20.0, -1e4
C = S * k / Le
H = np.array([[C, -C, 0], [-C, 2 * C, -C], [0, -C, C + alfa * S]])
P = np.array([q * S, 0, -alfa * S * t_inf])
print(np.linalg.solve(H, -P))        # [140. 130. 120.]
```

## Typowe błędy
> [!blad] Znak strumienia $q$
> We „Wstępie” $q > 0$ oznacza ciepło **odprowadzane** z pręta. W notatce [[MES 1D — od różnic skończonych do macierzy sztywności]] przyjęta jest odwrotna konwencja ($q > 0$ to ciepło wpływające). Obie są poprawne, ale na kolokwium używaj konwencji prowadzącego i zawsze sprawdź wynik bilansem.

> [!blad] $[H]\{t\} + \{P\} = 0$ a $[H]\{t\} = \{P\}$
> To ten sam układ z innym znakiem $P$. Błąd pojawia się, kiedy weźmiesz $P$ z jednej konwencji, a `solve` z drugiej. Objaw: temperatury wychodzą poniżej temperatury otoczenia, choć ciało powinno się nagrzewać.

> [!blad] Konwekcja tylko w $\{P\}$
> Człon $\frac{\alpha}{2}(t - t_\infty)^2$ zawiera niewiadomą $t$, więc daje wkład **i** do macierzy ($\alpha S$, w 2D $H_{bc}$), **i** do wektora ($\alpha S t_\infty$).

## Na zajęcia i egzamin
> [!zajecia]
> - **Lab 1:** prowadzący prosi, żeby **wydrukować „Wstęp”** i robić na nim notatki. To rozdział 5 z książki Milenina: funkcjonał, warunki brzegowe, pręt na dwóch elementach.
> - **Egzamin:** „wyprowadź $[H]$ i $\{P\}$ z funkcjonału”, „jakie są typy warunków brzegowych”, „dlaczego macierz jest symetryczna i pasmowa”.
> - **Kolokwium ustne:** „skąd się bierze $H_{bc}$” — z członu $\frac{\alpha}{2}(t - t_\infty)^2$ w funkcjonale.
> - Łączy się z: [[Macierz sztywności]], [[Nieustalony przepływ ciepła]], [[MES 1D — od różnic skończonych do macierzy sztywności]].

## Zadania
> [!zadanie] Zadanie 1
> W przykładzie z pręta zwiększ $\alpha$ do $1000$ W/(m²·K). Jak zmienią się temperatury?
> > [!odpowiedz]- Odpowiedź
> > Spadek na przewodzeniu się nie zmienia (20 K). Konwekcja: $t_3 - t_\infty = \frac{|q|}{\alpha} = 10$ K, czyli $t_3 = 30$, $t_2 = 40$, $t_1 = 50$ °C.

> [!zadanie] Zadanie 2
> Policz $\partial J/\partial t_1$ dla funkcjonału (5.29) i pokaż, że daje pierwszy wiersz układu.
> > [!odpowiedz]- Odpowiedź
> > W $J$ z $t_1$ są tylko $\frac{C^{(1)}}{2}(t_1^2 - 2t_1t_2 + t_2^2)$ i $qSt_1$. Pochodna: $C^{(1)}t_1 - C^{(1)}t_2 + qS = 0$, czyli wiersz $[C^{(1)}, -C^{(1)}, 0]$ i $P_1 = qS$.

> [!zadanie] Zadanie 3
> Dlaczego macierz $[H]$ jest symetryczna?
> > [!odpowiedz]- Odpowiedź
> > Bo powstaje z drugich pochodnych funkcjonału, który jest formą kwadratową: $H_{ij} = \partial^2 J / \partial t_i \partial t_j = H_{ji}$. Widać to też we wzorze: $\frac{\partial N}{\partial x}\frac{\partial N}{\partial x}^T$ i $NN^T$ są symetryczne.

## Fiszki #fiszki/mes
Funkcjonał ustalonego przewodzenia ciepła z konwekcją::J = ∫ [k/2·(∇t)² − Q·t] dV + ∫ α/2·(t − t∞)² dS + ∫ q·t dS
Co otrzymujesz z warunku ∂J/∂{t} = 0?::Układ [H]{t} + {P} = 0
Wzór na [H] w 2D::[H] = ∫ k (∂N/∂x·∂Nᵀ/∂x + ∂N/∂y·∂Nᵀ/∂y) dV + ∫ α N Nᵀ dS
Wzór na {P} przy samej konwekcji (konwencja [H]{t} + {P} = 0)::{P} = −∫ α {N} t∞ dS
Trzy typy warunków brzegowych w przewodzeniu ciepła::Zadana temperatura, konwekcja α(t − t∞), promieniowanie σ(t⁴ − t∞⁴)
Czym różni się metoda Ritza od Galerkina?::Ritz minimalizuje funkcjonał, Galerkin zeruje residuum ważone funkcjami kształtu; dla przewodzenia ciepła dają ten sam układ
Konwencja znaku q we „Wstępie” Kustry/Milenina::q > 0, gdy ciepło jest odprowadzane z ciała

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.

## Źródła
- P. Kustra / A. Milenin, *5. Symulacja ustalonych procesów cieplnych* („Wstęp — proszę wydrukować”): ![[01 Wstęp — ustalony przepływ ciepła (do druku).pdf]]
- A. Milenin, *Podstawy metody elementów skończonych*, Wydawnictwa AGH, Kraków 2010, rozdz. 5.
- O.C. Zienkiewicz, R.L. Taylor, J.Z. Zhu, *The Finite Element Method: Its Basis and Fundamentals*, rozdz. 3 (sformułowania słabe i wariacyjne).
- H.P. Langtangen, K.-A. Mardal, *Introduction to Numerical Methods for Variational Problems*, rozdz. 5 i 6 (Galerkin, warunki brzegowe z pochodną): [[Langtangen, Mardal — Introduction to Numerical Methods for Variational Problems (2019, CC BY).pdf]]
