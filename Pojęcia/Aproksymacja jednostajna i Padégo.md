---
typ: pojęcie
status: nowe
przedmioty: ["[[AGH Metody numeryczne]]"]
wymaga: ["[[Aproksymacja średniokwadratowa]]", "[[Interpolacja]]"]
powiązane: ["[[Wielomiany ortogonalne]]", "[[Funkcja wykładnicza]]", "[[Transmitancja]]"]
tags: [pojęcie, numeryczne, wykład]
utworzono: 2026-10-05
źródła: ["D. Szeliga — Metody numeryczne, wykład 4: Aproksymacja (AGH 2025/26)", "G.A. Baker, P. Graves-Morris — Padé Approximants (Cambridge UP)"]
---
# Aproksymacja jednostajna i Padégo

> [!skrot] W 30 sekund
> - **Aproksymacja jednostajna (minimaksowa):** szukasz $h^*$ z podprzestrzeni $U$, które minimalizuje **największy** błąd $\max_{x \in F}|f(x) - h(x)|$. W przeciwieństwie do średniokwadratowej pilnuje najgorszego punktu.
> - **Klucz:** jednoznaczność ⇔ **warunek Haara** (niezerowa funkcja z $U$ wymiaru $n$ ma najwyżej $n - 1$ zer). Charakteryzacja: **twierdzenie Czebyszewa o alternansie**: $h^*$ jest optymalne ⇔ błąd osiąga maksimum z **naprzemiennymi znakami** w co najmniej $n + 1$ punktach.
> - **Padé:** przybliżenie funkcją wymierną $r_{kl} = p_k/q_l$ tak, żeby rozwinięcie Maclaurina $f$ i $r_{kl}$ zgadzały się do wyrazu $x^{k+l}$. Często dużo lepsze niż wielomian Taylora tego samego „kosztu”.

> [!intuicja] Po ludzku
> Średniokwadratowa to „średnio dobrze”: może raz bardzo chybić, jeśli w innych miejscach trafia. Jednostajna to „nigdzie źle”: jak gwarancja tolerancji w specyfikacji technicznej, w której liczy się najgorszy przypadek. Alternans to podpis rozwiązania optymalnego: krzywa błędu odbija się na zmianę od górnej i dolnej granicy, jak piłka między ścianami. Gdyby gdzieś nie dobijała, dałoby się ją przesunąć i zmniejszyć maksimum.

## Definicja i wzory
> [!definicja] Zadanie aproksymacji (ogólnie, wykład 4)
> W przestrzeni unormowanej $V$ dla $f \in V$ szukamy $h^* \in U$ ($U$ skończenie wymiarowa) takiego, że $\|f - h^*\| \le \|f - h\|$ dla każdego $h \in U$. **Błąd aproksymacji** $\varepsilon_U(f) = \inf_{h \in U}\|f - h\|$. Element optymalny **zawsze istnieje** (dla $U$ skończenie wymiarowej). Jednoznaczność zależy od normy:
> - norma z iloczynu skalarnego (średniokwadratowa) → zawsze jednoznaczny, $f - h^* \perp U$: [[Aproksymacja średniokwadratowa]],
> - norma jednostajna $\|f\| = \max_{x \in F}|f(x)|$ → jednoznaczny tylko przy warunku Haara.

> [!definicja] Warunek Haara
> $n$-wymiarowa $U \subset C_F$ spełnia warunek Haara, gdy każda niezerowa funkcja z $U$ ma w $F$ **co najwyżej $n - 1$ zer**. Równoważnie: dla dowolnych różnych $x_1, \dots, x_n$ wyznacznik $\det[f_j(x_i)] \ne 0$ dla dowolnej bazy, albo: zadanie interpolacji w $U$ w dowolnych $n$ punktach ma jednoznaczne rozwiązanie.
> - Wielomiany stopnia $\le n-1$ (wymiar $n$) spełniają warunek Haara.
> - Wielomiany trygonometryczne stopnia $\le n$ (wymiar $2n+1$) spełniają go na $[0, 2\pi - \varepsilon]$ (dowód przez wzory Eulera: $t_n(x) = \frac12e^{-inx}\sum_{k=0}^{2n}c_k(e^{ix})^k$ ma najwyżej $2n$ zer).
>
> **Twierdzenie:** dla każdej $f \in C_F$ element optymalny jest jedyny ⇔ $U$ spełnia warunek Haara. Stąd aproksymacja jednostajna funkcji ciągłej wielomianami (algebraicznymi lub trygonometrycznymi) ma **jednoznaczne** rozwiązanie.

> [!wzor] Twierdzenie Czebyszewa o alternansie
> $U$ wymiaru $n$ z warunkiem Haara. $h^*$ jest optymalne dla $f$ ⇔ istnieje $n + 1$ punktów $x_1 < \dots < x_{n+1}$ w $F$ i $z = \pm1$, że
> $$f(x_k) - h^*(x_k) = (-1)^k\,z\,\|f - h^*\|, \qquad k = 1, \dots, n+1$$
> Te punkty to **alternans**.

> [!wzor] Konstrukcja na $n + 1$ punktach (przypadek najprostszy z wykładu)
> $F = \{x_1, \dots, x_{n+1}\}$, $U = W_{n-1}$ (wielomiany stopnia $\le n - 1$). Cały $F$ jest alternansem. Z liniowości ilorazu różnicowego i tego, że iloraz rzędu $n$ wielomianu stopnia $n - 1$ znika:
> $$z\,\varepsilon_U(f) = \frac{f[x_1, \dots, x_{n+1}]}{g[x_1, \dots, x_{n+1}]}, \qquad g(x_k) = (-1)^k$$
> Potem $h^*$ z $n$ warunków interpolacyjnych $h^*(x_k) = f(x_k) - (-1)^kz\varepsilon_U$ (np. tablicą ilorazów Newtona). Na całym przedziale ten krok iteruje się, przesuwając punkty alternansu (algorytm Remeza, poza zakresem wykładu).

> [!definicja] Aproksymacja Padégo
> $f(x) = \sum_{j\ge0}c_jx^j$ (Maclaurin, $c_j = \frac{f^{(j)}(0)}{j!}$). Szukamy
> $$r_{kl}(x) = \frac{p_k(x)}{q_l(x)} = \frac{a_kx^k + \dots + a_0}{b_lx^l + \dots + b_1x + 1}$$
> ($b_0 = 1$, bo $q_l(0) \ne 0$ i mnożnik wspólny jest dowolny), takiego, że $f$ i $r_{kl}$ mają równe wartości i pierwsze $N = k + l$ pochodne w zerze. Warunek: w iloczynie $f\cdot q_l - p_k = \sum_{i \ge k+l+1}d_ix^i$ znikają wszystkie współczynniki przy $x^0, \dots, x^{k+l}$:
> - $\sum_{j=0}^l c_{N-m-j}b_j = 0$ dla $m = 0, \dots, N - k - 1$ ($c_j = 0$ dla $j < 0$) → **najpierw** $b_j$ (układ liniowy, jednoznaczny gdy wyznacznik Hankla z $c_j$ $\ne 0$),
> - $a_j = \sum_{n=0}^{j}c_{j-n}b_n$ dla $j = 0, \dots, k$ → **potem** $a_j$.

## Jak to zrobić — krok po kroku (Padé)
1. Rozwiń $f$ w szereg Maclaurina do $x^{k+l}$.
2. Wymnóż $(\sum c_ix^i)(1 + b_1x + \dots + b_lx^l) - (a_0 + \dots + a_kx^k)$.
3. Przyrównaj do zera współczynniki przy $x^0, \dots, x^{k+l}$.
4. Rozwiąż: równania dla $x^{k+1}, \dots, x^{k+l}$ dają $b_j$, a pozostałe dają $a_j$.

## Przykład
> [!przyklad] Z wykładu: $r_{12}$ dla $e^x$
> $k = 1$, $l = 2$, $c_0 = 1$, $c_1 = 1$, $c_2 = \frac12$, $c_3 = \frac16$.
> $$\left(1 + x + \tfrac12x^2 + \tfrac16x^3\right)(1 + b_1x + b_2x^2) - (a_0 + a_1x)$$
> - $x^0$: $1 - a_0 = 0$ → $a_0 = 1$
> - $x^1$: $b_1 + 1 - a_1 = 0$
> - $x^2$: $b_2 + b_1 + \frac12 = 0$
> - $x^3$: $b_2 + \frac12b_1 + \frac16 = 0$
>
> Odejmując dwa ostatnie: $\frac12b_1 + \frac13 = 0$, więc $b_1 = -\frac23$, $b_2 = \frac16$, $a_1 = \frac13$.
> $$r_{12}(x) = \frac{1 + \frac13x}{1 - \frac23x + \frac16x^2}$$
> W $x = 1$: $r_{12}(1) = \frac{4/3}{1/2} = 2{,}6667$, a $e = 2{,}7183$. Taylor 3. stopnia (ten sam „koszt” 4 współczynników) daje $2{,}6667$ w $x = 1$, ale dla ujemnych $x$ Padé zachowuje się znacznie lepiej, np. w $x = -3$: $r_{12} = 0/(\dots) = 0$, Taylor $-2$, dokładnie $0{,}0498$.

> [!przyklad] Alternans: $x^2$ na $F = \{0;\ 0{,}5;\ 1\}$ prostą $h(x) = a + bx$
> $n = 2$, $f[0;\ 0{,}5;\ 1] = 1$ (drugi iloraz $x^2$). $g = (-1, 1, -1)$: $g[0;\ 0{,}5] = \frac{2}{0{,}5} = 4$, $g[0{,}5;\ 1] = -4$, $g[0;\ 0{,}5;\ 1] = -8$. Stąd $z\varepsilon = -\frac18$.
> Warunki: $0 - h(0) = (-1)(-\frac18)$ → $h(0) = -\frac18$; $0{,}25 - h(0{,}5) = (+1)(-\frac18)$ → $h(0{,}5) = \frac38$. Więc $h^*(x) = x - \frac18$, błąd maksymalny $\frac18$ w punktach $0;\ 0{,}5;\ 1$ ze znakami $+, -, +$. To jest też najlepsza prosta dla $x^2$ na całym $[0, 1]$.

## Typowe błędy
> [!blad] Pułapki
> - Mylenie norm: średniokwadratowa minimalizuje $\sqrt{\sum(\cdot)^2}$ i ma rozwiązanie z układu normalnego. Jednostajna minimalizuje $\max|\cdot|$ i nie ma takiego prostego układu.
> - Alternans potrzebuje $n + 1$ punktów, gdzie $n$ to **wymiar** $U$ (dla prostej $n = 2$, czyli 3 punkty).
> - Padé: zapomniane $b_0 = 1$ albo liczenie $a_j$ przed $b_j$.

## Na zajęcia i egzamin
> [!zajecia]
> - **Egzamin (test):** definicja elementu optymalnego i błędu aproksymacji, istnienie (zawsze) a jednoznaczność (Haar), twierdzenie o alternansie, liczba punktów alternansu, idea Padégo (zgodność $k + l$ pochodnych w zerze, $b_0 = 1$).
> - Łączy się z: [[Aproksymacja średniokwadratowa]], [[Wielomiany ortogonalne]] (wielomiany Czebyszewa mają najmniejszą normę jednostajną wśród wielomianów monicznych na $[-1, 1]$), [[Transmitancja]] (przybliżenie Padégo opóźnienia $e^{-sT}$ w automatyce).

## Pytania testowe (egzamin UPeL)
> [!zadanie] 1. Element optymalny w aproksymacji jednostajnej względem skończenie wymiarowej $U$:
> a) zawsze istnieje b) zawsze jest jednoznaczny c) jest jednoznaczny dla każdej $f$ ⇔ $U$ spełnia warunek Haara d) wyznacza się z układu równań normalnych
> > [!odpowiedz]- Odpowiedź
> > **a, c.**

> [!zadanie] 2. Ile punktów musi zawierać alternans dla najlepszej aproksymacji jednostajnej wielomianem stopnia $\le 3$?
> a) 3 b) 4 c) 5 d) 6
> > [!odpowiedz]- Odpowiedź
> > **c.** Wielomiany stopnia $\le 3$ to przestrzeń wymiaru $n = 4$, alternans ma $n + 1 = 5$ punktów.

> [!zadanie] 3. W aproksymacji Padégo $r_{kl}$:
> a) $f$ i $r_{kl}$ mają równe wartości i pierwsze $k + l$ pochodne w $x = 0$ b) przyjmuje się $b_0 = 1$ c) $r_{kl}$ jest wielomianem d) współczynniki mianownika wyznacza się z układu liniowego
> > [!odpowiedz]- Odpowiedź
> > **a, b, d.**

## Fiszki #fiszki/numeryczne
Warunek Haara::Każda niezerowa funkcja z $U$ (wymiar $n$) ma w $F$ co najwyżej $n-1$ zer
Twierdzenie o alternansie::$h^*$ optymalne ⇔ błąd osiąga $\pm\|f - h^*\|$ naprzemiennie w ≥ $n+1$ punktach
Najlepsza jednostajnie prosta dla $x^2$ na $[0,1]$::$x - 1/8$, błąd 1/8
Aproksymacja Padégo $r_{12}$ dla $e^x$::$(1 + x/3)/(1 - 2x/3 + x^2/6)$
Warunek Padégo::W $f\,q_l - p_k$ znikają współczynniki przy $x^0, \dots, x^{k+l}$
Czy element optymalny w normie jednostajnej zawsze istnieje?::Tak (dla skończenie wymiarowej $U$); jednoznaczność tylko przy warunku Haara

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.

## Źródła
- [[W04 Aproksymacja.pdf]] (D. Szeliga, rozdz. „Aproksymacja jednostajna”, „Aproksymacja Padego”).
- G.A. Baker, P. Graves-Morris, *Padé Approximants*, Cambridge University Press.
