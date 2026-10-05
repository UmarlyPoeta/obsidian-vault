---
typ: pojęcie
status: nowe
przedmioty: ["[[AGH Metody numeryczne]]", "[[AGH Metoda elementów skończonych]]"]
wymaga: ["[[Wielomiany]]", "[[Całka oznaczona]]"]
powiązane: ["[[Aproksymacja średniokwadratowa]]", "[[Całkowanie numeryczne i kwadratury Gaussa]]", "[[Interpolacja]]", "[[Aproksymacja jednostajna i Padégo]]"]
tags: [pojęcie, numeryczne, wykład]
utworzono: 2026-10-05
źródła: ["D. Szeliga — Metody numeryczne, wykład 4: Aproksymacja i wykład 5: Kwadratury (AGH 2025/26)", "A. Ralston — Wstęp do analizy numerycznej (PWN)"]
---
# Wielomiany ortogonalne

> [!skrot] W 30 sekund
> - **Co to:** ciąg wielomianów $P_0, P_1, \dots$ ($P_k$ stopnia dokładnie $k$), dla których iloczyn skalarny różnych jest zerowy: $(P_k, P_l) = \int_a^b P_kP_l\,p(x)\,dx = 0$ dla $k \ne l$ (albo suma po punktach w wersji dyskretnej).
> - **Klucz:** spełniają **regułę trójczłonową** $P_k = (\alpha_kx + \beta_k)P_{k-1} + \gamma_kP_{k-2}$. Ich pierwiastki są **rzeczywiste, jednokrotne i leżą wewnątrz** $(a, b)$.
> - **Po co:** (1) baza do aproksymacji średniokwadratowej **bez źle uwarunkowanego układu normalnego**; (2) pierwiastki $P_{n+1}$ to **węzły kwadratury Gaussa**; (3) węzły Czebyszewa leczą efekt Rungego.

> [!intuicja] Po ludzku
> Baza $1, x, x^2, \dots$ jest jak zestaw strzałek prawie równoległych: na $[0,1]$ funkcje $x^9$ i $x^{10}$ wyglądają niemal tak samo, więc trudno je rozróżnić i układ równań się sypie. Wielomiany ortogonalne to strzałki **prostopadłe**: każda „mierzy” inny kierunek, więc współczynnik przy każdej liczysz osobno, jednym iloczynem skalarnym.

## Definicja i wzory
> [!definicja] Przestrzenie unitarne z wykładu
> - $L^2_p[a,b]$: $(f, g) = \int_a^b f(x)g(x)p(x)\,dx$, funkcja wagowa $p \ge 0$, całkowalna.
> - $l^2_{p,N}$: $(f, g) = \sum_{i=1}^N f(x_i)g(x_i)p(x_i)$ (wersja dyskretna, punkty pomiarowe).
> - Norma $\|f\| = \sqrt{(f, f)}$. Układ ortogonalny: $(f_k, f_l) = 0$ dla $k \ne l$; ortonormalny, gdy dodatkowo $(f_k, f_k) = 1$. W przestrzeni unitarnej zachodzi twierdzenie Pitagorasa.
> - Przykład: $1, \cos x, \sin x, \cos 2x, \sin 2x, \dots$ w $L^2[0, 2\pi]$ z $p = 1$; po normowaniu $\frac{1}{\sqrt{2\pi}}, \frac{\cos x}{\sqrt\pi}, \frac{\sin x}{\sqrt\pi}, \dots$

> [!wzor] Ortogonalizacja Grama–Schmidta
> Z liniowo niezależnych $f_1, \dots, f_n$:
> $$g_1 = f_1, \qquad g_i = f_i - \sum_{j=1}^{i-1}\frac{(f_i, g_j)}{(g_j, g_j)}g_j$$
> Koszt: $\frac{(n+2)(n-1)}{2}$ iloczynów skalarnych. W arytmetyce zmiennoprzecinkowej wynik bywa daleki od ortogonalnego. Poprawki: **reortogonalizacja** (puścić jeszcze raz na wynikach) albo **zmodyfikowany G–S** (od razu odejmujesz rzut $g_i$ od wszystkich pozostałych $f_j$, $j > i$), przy tym samym koszcie.

> [!wzor] Reguła trójczłonowa (dla wielomianów unormowanych tak, że $a_k = 1$)
> $$P_k(x) = (x + \beta_k)P_{k-1}(x) + \gamma_kP_{k-2}(x), \qquad P_{-1} = 0,\ P_0 = 1$$
> $$\beta_k = -\frac{(xP_{k-1}, P_{k-1})}{(P_{k-1}, P_{k-1})}, \qquad \gamma_k = -\frac{(P_{k-1}, P_{k-1})}{(P_{k-2}, P_{k-2})}$$
> Koszt $n$ wielomianów: **$2n - 2$ iloczynów skalarnych** (zamiast $\frac{(n+2)(n-1)}{2}$ w G–S) i lepsze własności numeryczne. Ciąg jest wyznaczony jednoznacznie z dokładnością do stałych mnożników.

> [!definicja] Najważniejsze rodziny
> | rodzina | przedział, waga | rekurencja | uwagi |
> |---|---|---|---|
> | **Legendre** $P_k$ | $[-1, 1]$, $p = 1$ | $P_k = \frac{2k-1}{k}xP_{k-1} - \frac{k-1}{k}P_{k-2}$ | $P_2 = \frac32x^2 - \frac12$, $P_3 = \frac52x^3 - \frac32x$, $P_4 = \frac{35}{8}x^4 - \frac{30}{8}x^2 + \frac38$; $\|P_k\|^2 = \frac{2}{2k+1}$; wzór Rodriguesa $P_k = \frac{1}{2^kk!}\frac{d^k}{dx^k}(x^2-1)^k$ |
> | **Czebyszew** $T_k$ | $[-1, 1]$, $p = \frac{1}{\sqrt{1-x^2}}$ | $T_k = 2xT_{k-1} - T_{k-2}$, $T_0 = 1$, $T_1 = x$ | $T_2 = 2x^2 - 1$, $T_3 = 4x^3 - 3x$, $T_4 = 8x^4 - 8x^2 + 1$; $T_k(\cos\theta) = \cos k\theta$; parzystość $T_k(-x) = (-1)^kT_k(x)$ |
> | **Hermite** $H_k$ | $(-\infty, \infty)$, $p = e^{-x^2}$ | $H_k = 2xH_{k-1} - (2k-2)H_{k-2}$, $H_0 = 1$, $H_1 = 2x$ | $\|H_k\|^2 = \sqrt\pi\,2^kk!$ |
> | **Gram** $G_k$ | $N$ punktów równoodległych w $[-1,1]$, $p = 1$ | trójczłonowa | dla $k \ll \sqrt N$ bliskie Legendre'owi, dla dużych $k$ silnie oscylują między punktami |
>
> (Na wykładzie przy Czebyszewie napisano „dla $|x| > 1$ mają postać…”. Te jawne wzory $T_0 \dots T_4$ obowiązują dla każdego $x$, a reguła rekurencyjna również.)

## Jak to zrobić — krok po kroku (wielomiany ortogonalne z reguły trójczłonowej)
1. Ustal przedział, wagę $p$ i iloczyn skalarny.
2. $P_0 = 1$. Policz $\beta_1 = -\frac{(x, 1)}{(1, 1)}$, $P_1 = x + \beta_1$.
3. Dla $k \ge 2$: $\beta_k$ i $\gamma_k$ z wzorów, $P_k = (x + \beta_k)P_{k-1} + \gamma_kP_{k-2}$.
4. Do aproksymacji znormalizuj: $\tilde P_k = P_k/\|P_k\|$, wtedy współczynniki to $(f, \tilde P_k)$.

## Przykład
> [!przyklad] Z wykładu: ortogonalne na $[0, 1]$ z wagą $p(x) = x$
> Iloczyn $(x^i, x^j) = \int_0^1 x^{i+j}\cdot x\,dx = \frac{1}{i+j+2}$.
> - $k = 1$: $\beta_1 = -\frac{(x, 1)}{(1, 1)} = -\frac{1/3}{1/2} = -\frac23$, więc $P_1 = x - \frac23$.
> - $k = 2$: $\beta_2 = -\frac{(xP_1, P_1)}{(P_1, P_1)} = -\frac{8}{15}$, $\gamma_2 = -\frac{(P_1, P_1)}{(P_0, P_0)} = -\frac{1}{18}$, więc
> $$P_2 = \left(x - \tfrac{8}{15}\right)\left(x - \tfrac23\right) - \tfrac{1}{18} = x^2 - \tfrac65x + \tfrac{3}{10}$$
> Ten sam wynik daje Gram–Schmidt na $1, x, x^2$ ($g_3 = x^2 - \frac65x + \frac{3}{10}$). Sprawdzone numerycznie: $(P_2, P_0) = (P_2, P_1) = 0$.
> Do czego: aproksymacja $\sqrt x$ w [[Aproksymacja średniokwadratowa]] (ten sam przykład z wykładu).

> [!przyklad]- Węzły Gaussa–Legendre'a z $P_4$
> Pierwiastki $P_4 = \frac{35}{8}x^4 - \frac{30}{8}x^2 + \frac38$: $x^2 = \frac{15 \pm 2\sqrt{30}}{35}$, czyli $\pm0{,}86114$ i $\pm0{,}33998$. Wagi z wzoru $A_i = \frac{a_{n+1}\|P_n\|^2}{a_nP'_{n+1}(x_i)P_n(x_i)}$: $0{,}34785$ i $0{,}65215$. Szczegóły: [[Całkowanie numeryczne i kwadratury Gaussa]].

## Typowe błędy
> [!blad] Pułapki
> - Ortogonalność zależy od **przedziału i wagi**: Legendre nie jest ortogonalny na $[0, 1]$, Czebyszew nie z wagą 1.
> - Współczynniki $P_k$ w bazie $1, x, \dots$ rosną szybko (13. wielomian Czebyszewa ma już współczynniki pięciocyfrowe), więc nie przechodź do postaci naturalnej, tylko licz wartości rekurencją.
> - Klasyczny Gram–Schmidt w arytmetyce zmiennoprzecinkowej traci ortogonalność. Używaj zmodyfikowanego albo reortogonalizacji.

## Na zajęcia i egzamin
> [!zajecia]
> - **Egzamin (test):** definicja ciągu wielomianów ortogonalnych, reguła trójczłonowa i jej koszt ($2n-2$ iloczynów), własność pierwiastków (rzeczywiste, jednokrotne, wewnątrz przedziału), rodziny i ich wagi, Gram–Schmidt i jego wada numeryczna.
> - **MES:** punkty Gaussa w elemencie to pierwiastki wielomianów Legendre'a: [[Całkowanie numeryczne i kwadratury Gaussa]].
> - Łączy się z: [[Aproksymacja średniokwadratowa]], [[Aproksymacja jednostajna i Padégo]] (Czebyszew), [[Interpolacja]] (węzły Czebyszewa).

## Pytania testowe (egzamin UPeL)
> [!zadanie] 1. Pierwiastki wielomianów ortogonalnych w $L^2_p[a, b]$ są:
> a) zespolone b) rzeczywiste c) jednokrotne d) leżą wewnątrz $[a, b]$
> > [!odpowiedz]- Odpowiedź
> > **b, c, d.**

> [!zadanie] 2. Wielomiany Czebyszewa pierwszego rodzaju spełniają:
> a) $T_k = 2xT_{k-1} - T_{k-2}$ b) $T_k = \frac{2k-1}{k}xT_{k-1} - \frac{k-1}{k}T_{k-2}$ c) $T_k(-x) = (-1)^kT_k(x)$ d) $T_2 = 2x^2 - 1$
> > [!odpowiedz]- Odpowiedź
> > **a, c, d.** (b) to rekurencja Legendre'a.

> [!zadanie] 3. Ile iloczynów skalarnych wymaga wyznaczenie $n$ wielomianów ortogonalnych regułą trójczłonową?
> a) $n$ b) $2n - 2$ c) $\frac{(n+2)(n-1)}{2}$ d) $n^3$
> > [!odpowiedz]- Odpowiedź
> > **b.** (c) to koszt Grama–Schmidta.

> [!zadanie] 4. Wielomiany Hermite'a $H_k$ są ortogonalne:
> a) na $[-1, 1]$ z wagą 1 b) na $(-\infty, \infty)$ z wagą $e^{-x^2}$ c) na $[0, \infty)$ z wagą $e^{-x}$ d) na zbiorze dyskretnym
> > [!odpowiedz]- Odpowiedź
> > **b.** (c) to Laguerre, (d) to Gram.

## Fiszki #fiszki/numeryczne
Reguła trójczłonowa (monic)::$P_k = (x+\beta_k)P_{k-1} + \gamma_kP_{k-2}$, $\beta_k = -\frac{(xP_{k-1},P_{k-1})}{(P_{k-1},P_{k-1})}$, $\gamma_k = -\frac{(P_{k-1},P_{k-1})}{(P_{k-2},P_{k-2})}$
Legendre: przedział i waga::$[-1, 1]$, $p = 1$
Czebyszew: przedział, waga, rekurencja::$[-1,1]$, $1/\sqrt{1-x^2}$, $T_k = 2xT_{k-1} - T_{k-2}$
Hermite: przedział i waga::$(-\infty, \infty)$, $e^{-x^2}$
$\|P_k\|^2$ dla Legendre'a::$2/(2k+1)$
Gram–Schmidt: wzór na $g_i$::$f_i - \sum_{j<i}\frac{(f_i, g_j)}{(g_j, g_j)}g_j$
Pierwiastki $P_4$ (Legendre)::$\pm0{,}86114$, $\pm0{,}33998$

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.

## Źródła
- [[W04 Aproksymacja.pdf]] (D. Szeliga: przestrzenie unitarne, G–S, wielomiany ortogonalne, przykłady).
- [[W05 Kwadratury.pdf]] (wielomiany Legendre'a w kwadraturze Gaussa).
- A. Ralston, *Wstęp do analizy numerycznej*, PWN, rozdz. o wielomianach ortogonalnych.
