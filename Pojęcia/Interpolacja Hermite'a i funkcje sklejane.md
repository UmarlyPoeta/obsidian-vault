---
typ: pojęcie
status: nowe
przedmioty: ["[[AGH Metody numeryczne]]", "[[AGH Metoda elementów skończonych]]"]
wymaga: ["[[Interpolacja]]"]
powiązane: ["[[Funkcje kształtu]]", "[[Całkowanie numeryczne i kwadratury Gaussa]]", "[[Wielomiany ortogonalne]]", "[[Transformata Fouriera]]"]
tags: [pojęcie, numeryczne, wykład]
utworzono: 2026-10-05
źródła: ["D. Szeliga — Metody numeryczne, wykład 3: Interpolacja (AGH 2025/26)", "R.L. Burden, J.D. Faires — Numerical Analysis, rozdz. 3.4–3.5"]
---
# Interpolacja Hermite'a i funkcje sklejane

> [!skrot] W 30 sekund
> - **Hermite:** wielomian $H_n$ zgadza się z $f$ nie tylko w węzłach, ale też w **pochodnych** do rzędu $m_i - 1$ w węźle $x_i$. Łączna liczba warunków $\sum m_i = n + 1$. Gdy wszystkie $m_i = 1$, to zwykły Lagrange.
> - **Ilorazy różnicowe z węzłami wielokrotnymi:** $f[x_l, i] = \frac{f^{(i-1)}(x_l)}{(i-1)!}$, czyli węzeł powtórzony $i$ razy daje pochodną. Reszta liczona jak w tablicy Newtona.
> - **Funkcje sklejane (spline):** na każdym podprzedziale wielomian niskiego stopnia $m$, całość klasy $C^{m-1}$. Kubiczne: 4 współczynniki na odcinek, warunki ciągłości $S, S', S''$, na brzegach np. $S'' = 0$ (spline naturalny). **Lekarstwo na efekt Rungego.**
> - Dodatkowo wykład: **interpolacja trygonometryczna** (współczynniki = dyskretna transformata Fouriera, FFT w $O(n\log n)$).

> [!intuicja] Po ludzku
> Lagrange przeciąga gumkę przez gwoździe. Hermite dodatkowo każe gumce przechodzić przez gwoździe **pod zadanym kątem** (pochodna), więc lepiej trzyma kształt. Spline to elastyczna listwa szkutnicza: między gwoździami wygina się łagodnie i nigdy nie robi dzikich fal, bo każdy kawałek ma niski stopień.

## Definicja i wzory
> [!definicja] Zadanie Hermite'a
> Dane $k+1$ różnych węzłów $x_0, \dots, x_k$ i krotności $m_0, \dots, m_k$, $\sum m_i = n + 1$. Szukamy $H_n$ stopnia $\le n$:
> $$H_n^{(j)}(x_i) = f^{(j)}(x_i), \qquad i = 0, \dots, k, \quad j = 0, \dots, m_i - 1$$
> Rozwiązanie istnieje i jest jednoznaczne.

> [!wzor] Postać Newtona i ilorazy różnicowe z węzłami wielokrotnymi
> $s(0) = 0$, $s(i) = m_0 + \dots + m_{i-1}$ (ile warunków zużyły poprzednie węzły). Baza:
> $$p_{s(i)+j}(x) = (x-x_0)^{m_0}\cdots(x-x_{i-1})^{m_{i-1}}(x - x_i)^j$$
> $$H_n(x) = \sum_{l=0}^n b_lp_l(x), \qquad b_{s(i)+j} = f[x_0, m_0;\ \dots;\ x_{i-1}, m_{i-1};\ x_i, j+1]$$
> Iloraz z węzłem $i$-krotnym: $f[x_l, i] = \dfrac{f^{(i-1)}(x_l)}{(i-1)!}$, w szczególności $f[x_i, x_i] = f'(x_i)$. Dla różnych węzłów zwykła rekurencja (różnica ilorazów niższego rzędu przez $x_{l+k} - x_l$).

> [!wzor] Reszta interpolacji (Hermite i Lagrange)
> $$r(x) = f(x) - H_n(x) = \frac{f^{(n+1)}(\xi)}{(n+1)!}\,p_{n+1}(x), \qquad p_{n+1}(x) = \prod_{i=0}^k(x - x_i)^{m_i}$$
> Wniosek: jeśli $f$ jest wielomianem stopnia $\le n$, ilorazy różnicowe i różnice skończone rzędu $> n$ są **zerowe**.

> [!definicja] Funkcja sklejana stopnia $m$
> Węzły $a = x_0 < x_1 < \dots < x_N = b$. $S$ jest funkcją sklejaną stopnia $m$, gdy:
> 1. na każdym przedziale $(x_{i-1}, x_i)$ jest wielomianem stopnia $\le m$,
> 2. $S \in C^{m-1}$, czyli $S$ i pochodne do rzędu $m-1$ są ciągłe.
>
> $m = 1$: łamana, $S_i(x) = y_i + \frac{y_{i+1} - y_i}{x_{i+1} - x_i}(x - x_i)$. Każdy wielomian jest szczególnym przypadkiem spline'a.

> [!wzor] Spline kubiczny ($m = 3$)
> Na $n$ odcinkach $S_i(x) = a_{0i} + a_{1i}x + a_{2i}x^2 + a_{3i}x^3$, czyli $4n$ niewiadomych. Warunki:
> - interpolacja: $S_i(x_i) = y_i$, $S_i(x_{i+1}) = y_{i+1}$ → $2n$ równań,
> - ciągłość $S'$ i $S''$ w węzłach wewnętrznych → $2(n-1)$ równań,
> - 2 warunki brzegowe, np. **naturalne** $S''(x_0) = S''(x_n) = 0$.
>
> Razem $4n$ równań, macierz **pasmowa**, rozwiązywana eliminacją (w sformułowaniu przez momenty $M_i = S''(x_i)$ to układ trójdiagonalny, algorytm Thomasa w $O(n)$).

> [!definicja]- Interpolacja trygonometryczna i FFT (z wykładu)
> Funkcja okresowa (okres $2\pi$), węzły równoodległe $x_k = \frac{2k\pi}{n+1}$. Szukamy $t_n(x) = \sum_{j=0}^n c_je^{ijx}$, $t_n(x_k) = f(x_k)$. Funkcje $e^{ijx}$ są **ortogonalne** w dyskretnym iloczynie skalarnym, więc
> $$c_j = \frac{1}{n+1}\sum_{k=0}^n f(x_k)e^{-ijx_k}$$
> To **dyskretna analiza Fouriera**. Wersja rzeczywista: $t_n(x) = \frac{a_0}{2} + \sum_{j=1}^m(a_j\cos jx + b_j\sin jx)\ (+\ \text{człon } a_{m+1})$, $a_j = \frac{2}{n+1}\sum f(x_k)\cos jx_k$, $b_j = \frac{2}{n+1}\sum f(x_k)\sin jx_k$.
> - Bezpośrednio: ok. $(n+1)^2$ mnożeń zespolonych. **FFT (Cooley–Tukey):** przy $n+1 = r_1r_2\cdots r_p$ koszt $O((n+1)(r_1 + \dots + r_p))$, a dla $n+1 = 2^k$ rzędu $n\log_2n$.
> - Wartość wielomianu trygonometrycznego w punkcie: Horner na $z = e^{i\xi}$ ($4n-2$ mnożeń), **Goertzel** (taniej, ale niestabilny dla małych $|\xi|$, bo zadanie jest źle uwarunkowane względem $c = \cos\xi$), **Reinsch** (numerycznie poprawny, $n + 4$ mnożeń).
> Więcej: [[Transformata Fouriera]].

## Jak to zrobić — krok po kroku (Hermite przez tablicę ilorazów)
1. Wypisz węzły **z powtórzeniami** zgodnie z krotnościami (np. $0, 0, 2, 2, 2$).
2. Pierwsza kolumna: $f(x_i)$ przy każdym powtórzeniu.
3. Iloraz 1. rzędu dla dwóch takich samych węzłów to $f'(x_i)$, dla różnych zwykła różnica przez różnicę węzłów.
4. Iloraz 2. rzędu dla trzech takich samych węzłów to $\frac{f''(x_i)}{2!}$ itd.
5. Współczynniki $b_l$ to górna przekątna tablicy. Baza: $1, (x-x_0), (x-x_0)^2, \dots$ zgodnie z powtórzonymi węzłami.

## Przykład
> [!przyklad] Z wykładu: $W(0) = 1$, $W'(0) = 2$, $W(2) = 2$, $W'(2) = 1$, $W''(2) = 3$
> Węzły z krotnościami: $x_0 = 0$ ($m_0 = 2$), $x_1 = 2$ ($m_1 = 3$), $n = 4$. Ciąg węzłów: $0, 0, 2, 2, 2$. Baza: $1,\ x,\ x^2,\ x^2(x-2),\ x^2(x-2)^2$.
>
> | węzeł | $f$ | 1. rząd | 2. rząd | 3. rząd | 4. rząd |
> |---|---|---|---|---|---|
> | 0 | 1 | | | | |
> | 0 | 1 | $f'(0) = 2$ | | | |
> | 2 | 2 | $\frac{2-1}{2} = \frac12$ | $\frac{1/2 - 2}{2} = -\frac34$ | | |
> | 2 | 2 | $f'(2) = 1$ | $\frac{1 - 1/2}{2} = \frac14$ | $\frac{1/4 + 3/4}{2} = \frac12$ | |
> | 2 | 2 | $f'(2) = 1$ | $\frac{f''(2)}{2!} = \frac32$ | $\frac{3/2 - 1/4}{2} = \frac58$ | $\frac{5/8 - 1/2}{2} = \frac{1}{16}$ |
>
> $$W(x) = 1 + 2x - \tfrac34x^2 + \tfrac12x^2(x-2) + \tfrac{1}{16}x^2(x-2)^2$$
> Sprawdzenie: $W(2) = 1 + 4 - 3 = 2$ ✓, $W'(2) = 2 - 3 + \frac12(12 - 8) = 1$ ✓, $W''(2) = -\frac32 + \frac12\cdot8 + \frac{1}{16}\cdot8 = 3$ ✓.
>
> ![[Hermite — przykład z wykładu.png]]

> [!przyklad]- Spline kontra Runge
> Dla $f(x) = 1/(1+25x^2)$ na 20 równoodległych węzłach wielomian Lagrange'a ma błąd do 8,6 na brzegach (wykres w [[Interpolacja]]). Spline kubiczny (naturalny) na tych samych węzłach ma maksymalny błąd ok. 0,012 (policzone) i nie oscyluje, bo każdy kawałek ma stopień 3 bez względu na liczbę węzłów. Wykład pokazuje to samo dla spline'u 1. stopnia (łamana).

## Typowe błędy
> [!blad] Pułapki
> - W tablicy Hermite'a dzielenie przez $x_i - x_i = 0$: dla powtórzonego węzła wstawiasz pochodną (podzieloną przez silnię), a nie liczysz różnicy.
> - Zapomniane $\frac{1}{(i-1)!}$ przy wyższych pochodnych: $f[x, x, x] = \frac{f''(x)}{2}$, nie $f''(x)$.
> - Stopień Hermite'a to $\sum m_i - 1$, a nie liczba węzłów minus 1.
> - Spline kubiczny bez 2 warunków brzegowych: układ ma $4n - 2$ równań na $4n$ niewiadomych.

## Na zajęcia i egzamin
> [!zajecia]
> - **Egzamin (test):** definicja zadania Hermite'a, iloraz z węzłem wielokrotnym, reszta interpolacji, definicja funkcji sklejanej stopnia $m$ (klasa $C^{m-1}$), liczba warunków dla spline'u kubicznego, koszt FFT, twierdzenie Fabera.
> - **MES:** funkcje kształtu to lokalna interpolacja Lagrange'a. Elementy belkowe (ugięcie i kąt w węźle) używają właśnie **wielomianów Hermite'a**: [[Funkcje kształtu]].
> - Łączy się z: [[Interpolacja]], [[Wielomiany ortogonalne]], [[Transformata Fouriera]].

## Pytania testowe (egzamin UPeL)
> [!zadanie] 1. Zadanie interpolacji Hermite'a z węzłami o krotnościach $m_0 = 2$, $m_1 = 1$, $m_2 = 3$ wyznacza wielomian stopnia co najwyżej:
> a) 2 b) 3 c) 5 d) 6
> > [!odpowiedz]- Odpowiedź
> > **c.** $\sum m_i = 6 = n + 1$, więc $n = 5$.

> [!zadanie] 2. $f[x_0, x_0, x_0]$ dla $f(x) = x^3$ w $x_0 = 1$ wynosi:
> a) 6 b) 3 c) 1 d) 0
> > [!odpowiedz]- Odpowiedź
> > **b.** $f''(1)/2! = 6/2 = 3$.

> [!zadanie] 3. Funkcja sklejana stopnia 3 jest klasy:
> a) $C^0$ b) $C^1$ c) $C^2$ d) $C^3$
> > [!odpowiedz]- Odpowiedź
> > **c.** $C^{m-1}$.

> [!zadanie] 4. Które zdania są prawdziwe?
> a) Interpolacja Hermite'a z wszystkimi $m_i = 1$ to interpolacja Lagrange'a. b) FFT dla $n+1 = 2^k$ wymaga rzędu $n\log_2 n$ działań. c) Dla każdego układu węzłów istnieje funkcja ciągła, do której ciąg wielomianów interpolacyjnych nie jest jednostajnie zbieżny. d) Algorytm Goertzela jest numerycznie poprawny dla wszystkich $\xi$.
> > [!odpowiedz]- Odpowiedź
> > **a, b, c.** (c) to twierdzenie Fabera. (d) fałsz: Goertzel psuje się dla małych $|\xi|$, poprawny jest algorytm Reinscha.

## Fiszki #fiszki/numeryczne
Iloraz różnicowy z węzłem $i$-krotnym::$f[x_l, i] = f^{(i-1)}(x_l)/(i-1)!$
Stopień wielomianu Hermite'a przy krotnościach $m_i$::$\sum m_i - 1$
Reszta interpolacji Hermite'a::$\frac{f^{(n+1)}(\xi)}{(n+1)!}\prod(x - x_i)^{m_i}$
Definicja funkcji sklejanej stopnia $m$::Na każdym przedziale wielomian stopnia ≤ m, całość klasy $C^{m-1}$
Warunki spline'u naturalnego::$S''(x_0) = S''(x_n) = 0$
Koszt FFT dla $n+1 = 2^k$::Rzędu $n \log_2 n$
Twierdzenie Fabera::Dla dowolnych węzłów istnieje funkcja ciągła, dla której interpolacja wielomianowa nie jest jednostajnie zbieżna
Współczynniki trygonometrycznego wielomianu interpolacyjnego::$c_j = \frac{1}{n+1}\sum f(x_k)e^{-ijx_k}$ (dyskretna transformata Fouriera)

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.

## Źródła
- [[W03 Interpolacja.pdf]] (D. Szeliga, rozdz. 5–7 i przykłady).
- R.L. Burden, J.D. Faires, *Numerical Analysis*, rozdz. 3.4 (Hermite), 3.5 (spline kubiczny).
