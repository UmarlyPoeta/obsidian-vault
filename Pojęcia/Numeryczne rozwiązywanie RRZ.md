---
typ: pojęcie
status: nowe
przedmioty: ["[[AGH Metody numeryczne]]", "[[AGH Metoda elementów skończonych]]", "[[PK Matematyka 2]]"]
wymaga: ["[[Równanie różniczkowe zwyczajne]]", "[[Układy równań liniowych]]"]
powiązane: ["[[Schematy całkowania w czasie]]", "[[Nieustalony przepływ ciepła]]", "[[Transformata Laplace'a]]", "[[Arytmetyka zmiennoprzecinkowa i błędy numeryczne]]", "[[Rozwiązywanie równań nieliniowych]]", "[[Różniczkowanie numeryczne]]", "[[Całkowanie numeryczne i kwadratury Gaussa]]", "[[MES 1D — od różnic skończonych do macierzy sztywności]]"]
tags: [pojęcie, metody-numeryczne, mes]
utworzono: 2026-10-02
źródła: ["A. Milenin — Podstawy metody elementów skończonych (Wyd. AGH 2010)", "O.C. Zienkiewicz, R.L. Taylor, J.Z. Zhu — The Finite Element Method: Its Basis and Fundamentals", "Z. Fortuna, B. Macukow, J. Wąsowski — Metody numeryczne (WNT)", "R.L. Burden, J.D. Faires — Numerical Analysis", "Dokumentacja scipy.integrate.solve_ivp", "Instrukcja: Lab RR — metody rozwiązywania równań różniczkowych (Euler, RK2, RK4)", "Kurs MN w C++: Kod/metody_numeryczne (moduły, testy, rozwiązania wzorcowe)", "H.P. Langtangen, S. Linge — Finite Difference Computing with PDEs (Springer 2017, CC BY), rozdz. 3 i dodatek B (pobrane: Załączniki/MES/Podręczniki)"]
---
# Numeryczne rozwiązywanie RRZ

> [!skrot] W 30 sekund
> - **Co to:** zamiast wzoru $y(t)$ liczysz wartości $y_k \approx y(t_k)$ krok po kroku, z krokiem $h = \Delta t$.
> - **Klucz:** schemat θ $(C/\Delta t + \theta K)\,T_{k+1} = (C/\Delta t - (1-\theta)K)\,T_k + F$, gdzie θ = 0 to Euler jawny, θ = ½ to Crank–Nicolson, a θ = 1 to Euler niejawny. Euler jawny jest stabilny dla $y' = \lambda y$ tylko gdy $h < 2/|\lambda|$.
> - **Kiedy:** wszędzie, gdzie jest czas. W MES nieustalonym po dyskretyzacji w przestrzeni dostajesz układ $C\dot T + KT = F$ i tutaj go rozwiązujesz.

> [!intuicja] Po ludzku
> Znasz położenie i prędkość, więc robisz mały krok do przodu, potem znów sprawdzasz prędkość i robisz kolejny krok. Euler jawny patrzy na prędkość **na początku** kroku, a niejawny **na końcu** (dlatego musi rozwiązać równanie). Gdy krok jest za długi w stosunku do tego, jak szybko coś się zmienia, jawny „przeskakuje” rozwiązanie i zaczyna się rozkołysywać coraz mocniej.

## Definicja i wzory
> [!wzor] Metody jednokrokowe dla $y' = f(t, y)$
> $$\text{Euler jawny:}\quad y_{k+1} = y_k + h\,f(t_k, y_k) \qquad (\text{rząd } 1)$$
> $$\text{Euler niejawny:}\quad y_{k+1} = y_k + h\,f(t_{k+1}, y_{k+1}) \qquad (\text{rząd } 1)$$
> $$\text{Crank–Nicolson:}\quad y_{k+1} = y_k + \tfrac{h}{2}\left[f(t_k, y_k) + f(t_{k+1}, y_{k+1})\right] \qquad (\text{rząd } 2)$$
> $$\text{RK4:}\quad y_{k+1} = y_k + \tfrac{h}{6}(k_1 + 2k_2 + 2k_3 + k_4) \qquad (\text{rząd } 4)$$
> gdzie $k_1 = f(t_k, y_k)$, $k_2 = f(t_k + \frac h2, y_k + \frac h2 k_1)$, $k_3 = f(t_k + \frac h2, y_k + \frac h2 k_2)$, $k_4 = f(t_k + h, y_k + h k_3)$.
> **Rząd $p$** oznacza, że błąd w ustalonej chwili $\approx C h^p$. Gdy skrócisz krok dwa razy, błąd maleje $2^p$ razy.

> [!wzor] Układy liniowe i forma MES
> Dla $y' = Ay$ Euler niejawny daje $(I - hA)\,y_{k+1} = y_k$, czyli w każdym kroku rozwiązujesz układ liniowy.
> W MES nieustalonym masz $C\dot T + KT = F$ ($C$ to macierz pojemności, $K$ to macierz sztywności, czyli przewodzenia). Schemat θ:
> $$\left(\frac{C}{\Delta t} + \theta K\right) T_{k+1} = \left(\frac{C}{\Delta t} - (1-\theta) K\right) T_k + F$$
> Macierz po lewej jest stała, więc wystarczy ją raz zbudować (i raz rozłożyć LU).

> [!wzor] Stabilność dla $y' = \lambda y$, $\lambda < 0$
> Jeden krok to mnożenie przez współczynnik wzmocnienia $R(z)$, gdzie $z = h\lambda$:
>
> | metoda | $R(z)$ | warunek $\lvert R \rvert \le 1$ |
> |---|---|---|
> | Euler jawny | $1 + z$ | $h \le 2/\lvert\lambda\rvert$ |
> | Euler niejawny | $\frac{1}{1 - z}$ | zawsze (A-stabilny) |
> | Crank–Nicolson | $\frac{1 + z/2}{1 - z/2}$ | zawsze, ale dla dużych $\lvert z \rvert$ $R \to -1$ (oscylacje) |
>
> Obszar stabilności Eulera jawnego to koło o środku $-1$ i promieniu $1$ na płaszczyźnie zespolonej $h\lambda$. Wartości własne $\pm i\omega$ (oscylator bez tłumienia) leżą poza nim, więc amplituda rośnie przy **każdym** $h$.

> [!definicja] Sztywność
> Układ jest **sztywny**, gdy ma jednocześnie bardzo szybkie i bardzo wolne składowe (wartości własne o bardzo różnych modułach). Szybka składowa szybko zanika, ale to ona narzuca jawnej metodzie mikroskopijny krok $h < 2/|\lambda_{\max}|$. W MES $|\lambda_{\max}| \sim k/(\rho c\, h_x^2)$, więc **zagęszczenie siatki dwa razy wymusza czterokrotnie krótszy krok jawny**. Dlatego w projektach MES zwykle stosuje się θ = 1 albo θ = ½.

## Jak to zrobić — krok po kroku
1. Sprowadź równanie do układu **1. rzędu**: $y'' + 2\zeta\omega y' + \omega^2 y = 0$ z $u = [y, y']$ daje $u' = \begin{bmatrix} 0 & 1 \\ -\omega^2 & -2\zeta\omega \end{bmatrix} u$.
2. Oszacuj najszybszą skalę czasu ($|\lambda_{\max}|$). Jeśli jest duża, a interesuje cię długi czas, wybierz metodę niejawną.
3. Wybierz $h$ i policz $n = t_{\text{końc}}/h$ kroków.
4. Pętla po krokach według wzoru metody. Przy metodzie niejawnej w każdym kroku rozwiązujesz układ (dla nieliniowego $f$ metodą Newtona).
5. **Weryfikacja:** porównaj z `scipy.integrate.solve_ivp` z małym `rtol` albo z rozwiązaniem analitycznym. Zmniejsz $h$ dwa razy i sprawdź, czy błąd spada zgodnie z rzędem metody: to jest dokładnie „weryfikacja kroku czasowego” z projektu.

## Przykład
> [!przyklad] Stygnięcie Newtona: $T' = -r(T - T_{ot})$, $r = 0{,}1\ \text{min}^{-1}$, $T_{ot} = 20$ °C, $T(0) = 90$ °C, $h = 2$ min
> Dokładnie: $T(t) = 20 + 70 e^{-0{,}1 t}$, czyli $T(2) = 77{,}3112$, $T(4) = 66{,}9224$. Przy $z = h\lambda = -0{,}2$ nadwyżka $T - 20$ mnoży się w każdym kroku przez $R(z)$:
>
> | metoda | $R(-0{,}2)$ | $T_1$ | $T_2$ |
> |---|---|---|---|
> | Euler jawny | $0{,}8$ | $76{,}0000$ | $64{,}8000$ |
> | Euler niejawny | $1/1{,}2 = 0{,}8333$ | $78{,}3333$ | $68{,}6111$ |
> | Crank–Nicolson | $0{,}9/1{,}1 = 0{,}8182$ | $77{,}2727$ | $66{,}8595$ |
> | RK4 | $0{,}818733$ | $77{,}3113$ | $66{,}9227$ |
> | dokładnie | $e^{-0{,}2} = 0{,}818731$ | $77{,}3112$ | $66{,}9224$ |
>
> Jawny Euler zaniża, niejawny zawyża, Crank–Nicolson jest między nimi i o rząd dokładniejszy, a RK4 trafia prawie idealnie. **Niestabilność:** przy $h = 25 > 2/r = 20$ mamy $R = -1{,}5$ i Euler jawny daje $90 \to -85 \to 177{,}5 \to -216{,}3$. Przy $h = 15$ ($R = -0{,}5$) rozwiązanie jest stabilne, ale oscyluje ($-15;\ 37{,}5;\ 11{,}25$).

```cpp
#include "m07.h"   // Kod/metody_numeryczne, schemat_theta korzysta z gauss() z m02
auto f = [](double x, double y) { return x * x + y; };
double yE = euler(f, 0, 0.3, 0.1, 3);              // 0.1382 (przykład z instrukcji RR)
double r = 0.1, Tot = 20, h = 2;                  // stygnięcie Newtona w formie MES: C T' + K T = F
Mat C = {{1.0}}, K = {{r}};
for (double theta : {0.0, 0.5, 1.0}) {
    Vec T = schemat_theta(C, K, {r * Tot}, {90.0}, h, 30, theta);
    std::printf("%.1f  %.4f
", theta, T[0]);     // 20.0867 / 20.1701 / 20.2949 po 60 min
}
```

> [!kod]- To samo w Pythonie (do szybkiego sprawdzenia)
> ```python
> import numpy as np
> from scipy.integrate import solve_ivp
> r, Tot, h, n = 0.1, 20.0, 2.0, 30
> C, K, F = np.array([[1.0]]), np.array([[r]]), np.array([r * Tot])   # forma MES
> T = np.array([90.0])
> for theta in (0.0, 0.5, 1.0):
>     lewa, prawa = C / h + theta * K, C / h - (1 - theta) * K
>     Tk = T.copy()
>     for _ in range(n):
>         Tk = np.linalg.solve(lewa, prawa @ Tk + F)
>     print(theta, Tk[0])                      # 20.0867 / 20.1701 / 20.2949
> ref = solve_ivp(lambda t, y: -r * (y - Tot), (0, 60), [90.0], rtol=1e-10)
> print(ref.y[0, -1])                          # 20.1735
> ```

## Lab RR — Euler, RK2, RK4: co oddajesz i wyniki do sprawdzenia
Instrukcja: [[Lab RR — Euler i Runge-Kutta.pdf]]. Oddajesz na zajęciach (10 pkt), na UPEL `.cpp` i sprawozdanie.

> [!definicja] Wzory z instrukcji ($h = (b - a)/N$, $x_i = a + ih$)
> - **Euler:** $y_{i+1} = y_i + h f(x_i, y_i)$.
> - **RK2 (Heun):** $k_1 = f(x, y)$, $k_2 = f(x + h, y + hk_1)$, $y_{i+1} = y_i + \frac h2(k_1 + k_2)$.
> - **RK4:** $k_1 = f(x, y)$, $k_2 = f(x + \frac h2, y + \frac h2 k_1)$, $k_3 = f(x + \frac h2, y + \frac h2 k_2)$, $k_4 = f(x + h, y + hk_3)$, $y_{i+1} = y_i + \frac h6(k_1 + 2k_2 + 2k_3 + k_4)$.

> [!przyklad] Przykład z instrukcji: $y' = x^2 + y$, $y(0) = 0{,}1$, $N = 3$
> $y_1 = 0{,}1 + 0{,}1(0 + 0{,}1) = 0{,}11$; $y_2 = 0{,}11 + 0{,}1(0{,}01 + 0{,}11) = 0{,}122$; $y_3 = 0{,}122 + 0{,}1(0{,}04 + 0{,}122) = 0{,}1382$.

> [!przyklad] Zad. 2: rozwiązanie w $x = 1$ i badanie zbieżności
> Rozwiązania dokładne: $y' = 2(x+1)y$, $y(0) = 1$ daje $y = e^{x^2 + 2x}$, więc $y(1) = e^3 = 20{,}085537$. $y' = x + y$, $y(0) = 0{,}1$ daje $y = 1{,}1e^x - x - 1$, więc $y(1) = 1{,}1e - 2 = 0{,}990110$.
>
> | $N$ | Euler (1) | RK2 (1) | RK4 (1) | Euler (2) | RK2 (2) | RK4 (2) |
> |---|---|---|---|---|---|---|
> | 10 | 12,635 | 19,306 | 20,08127 | 0,853117 | 0,985489 | 0,99010772 |
> | 20 | 15,571 | 19,864 | 20,08523 | 0,918627 | 0,988910 | 0,99010986 |
> | 40 | 17,566 | 20,026 | 20,08552 | 0,953570 | 0,989804 | 0,99011000 |
> | 80 | 18,749 | 20,070 | 20,08554 | 0,971633 | 0,990033 | 0,99011001 |
>
> **Zbieżność:** podwojenie $N$ zmniejsza błąd Eulera ok. 2 razy (rząd 1), RK2 ok. 4 razy (rząd 2), RK4 ok. 16 razy (rząd 4). Na wykres daj $\log(\text{błąd})$ od $\log N$: nachylenia −1, −2, −4. Równanie (1) rośnie szybko ($e^{x^2}$), więc Euler przy $N = 10$ myli się o 37%.

> [!przyklad] Zad. 3: kulka w oleju $m\,v' = mg - kv^2$, $v(0) = 0$, wynik w $t = 10$ s
> Prędkość graniczna (gdy $v' = 0$): $v_\infty = \sqrt{mg/k} = \sqrt{0{,}05 \cdot 9{,}81 / 0{,}1} = 2{,}2147$ m/s. Ustala się po ok. 1 s.
>
> | krok $h$ | Euler | RK2 | RK4 |
> |---|---|---|---|
> | 0,5 | **rozbiega się** ($-\infty$) | **rozbiega się** | **rozbiega się** |
> | 0,3 | 0,590 (oscyluje między ~0 a ~3) | 0,173 (ustala się na **fałszywej** wartości) | 2,2060 |
> | 0,1 | 2,21472 | 2,21472 | 2,21472 |
>
> **Komentarz do sprawozdania:** to nie jest błąd programu, tylko **niestabilność metod jawnych**. Blisko $v_\infty$ równanie zachowuje się jak $v' \approx -\lambda(v - v_\infty)$ z $\lambda = 2kv_\infty/m = 8{,}86$ 1/s. Euler jest stabilny dla $h < 2/\lambda = 0{,}23$ s, RK4 dla $h < 2{,}79/\lambda \approx 0{,}31$ s. Dlatego przy $h = 0{,}3$ tylko RK4 daje sensowny wynik (RK2 znajduje wtedy fałszywy stan ustalony schematu, który nie jest rozwiązaniem równania), a przy $h = 0{,}5$ wszystkie trzy wybuchają. Euler przy $h = 0{,}5$: $v_1 = 4{,}905$ (powyżej $v_\infty$!), $v_2 = 4{,}905 + 0{,}5(9{,}81 - 2 \cdot 24{,}06) = -14{,}2$ i dalej coraz gorzej. Ta sama mechanika decyduje o kroku czasowym w MES (patrz [[Schematy całkowania w czasie]]).

> [!kod]- Wypisanie danych do wykresu (plik tekstowy dla Excela albo gnuplota)
> ```cpp
> #include <fstream>
> Vec vE = trajektoria(f, 0, 10, 0, N, 1), v4 = trajektoria(f, 0, 10, 0, N, 4);
> std::ofstream out("kulka_h01.txt");
> for (int i = 0; i <= N; ++i) out << i * h << " " << vE[i] << " " << v4[i] << "\n";
> ```

## Wykład dr Szeligi (W07) — co dochodzi
> [!definicja] Zagadnienie początkowe (Cauchy'ego) i brzegowe
> $y' = f(x, y)$, $x \in [a, b]$, plus **warunek początkowy** $y(a) = y_a$ (zagadnienie Cauchy'ego) albo **warunek brzegowy** $g(y(a), y(b)) = 0$ (zagadnienie brzegowe). Samo $y' = y$ ma nieskończenie wiele rozwiązań $Ce^x$.
> **Twierdzenie (istnienie i jednoznaczność):** jeśli $f$ jest ciągła względem $x$ i spełnia **warunek Lipschitza** względem $y$, $|f(x, y_1) - f(x, y_2)| \le L|y_1 - y_2|$, to dla każdego $y_a$ istnieje dokładnie jedno rozwiązanie na $[a, b]$. Wtedy zadanie jest **poprawnie postawione** (małe zaburzenia $\delta f$, $\delta y_a$ dają małe zmiany rozwiązania).

> [!definicja] Klasy metod różnicowych
> - **Jednokrokowe:** $y_{i+1} = y_i + h\,\Phi_f(x_i, y_i; h)$. Do $y_{i+1}$ wystarcza $y_i$. $\Phi$ to **funkcja przyrostu względnego**; w Eulerze $\Phi = f$.
> - **Liniowe wielokrokowe ($k$-krokowe):** $\alpha_ky_{i+k} + \dots + \alpha_0y_i = h(\beta_kf_{i+k} + \dots + \beta_0f_i)$, $\alpha_k \ne 0$, $|\alpha_0| + |\beta_0| \ne 0$. Potrzeba $k$ wartości startowych (z innej metody). Jednokrokowa z $\Phi$ liniowym w $f$ to przypadek $k = 1$.
> - **Zbieżność:** $y_i(h) \to y(x)$ przy $h \to 0$ dla $x = a + ih$. **Warunek aproksymacji (zgodności):** $\Phi_f(x, y; 0) = f(x, y)$ (konieczny).
> - **Rząd metody $p$:** największe $p$, że $|\Phi_f(x, y; h) - \Delta_f(x, y; h)| \le Ch^p$, gdzie $\Delta_f = \frac{z(x+h) - z(x)}{h}$ to **dokładny przyrost względny** (z rozwiązania dokładnego). Teoretycznie rząd $p$ daje $\Phi$ = suma częściowa Taylora $\sum_{j<p}\frac{d^jf}{dx^j}\frac{h^j}{(j+1)!}$, ale to wymaga drogich pochodnych, stąd Runge–Kutta.

> [!wzor] Euler i jego modyfikacje
> - Euler: $y_{i+1} = y_i + hf(x_i, y_i)$. Błąd lokalny $|g_i| \le M_2h^2$, $M_2 = \max\frac{|y''|}{2}$. Błąd globalny $|e_i| \le M_2h(x_i - a) \le M_2(b-a)h$, a z funkcją Lipschitza $E_L(x) = \frac{e^{Lx} - 1}{L}$: $|e_i| \le M_2h\,E_L(x_i - a)$. **Błąd globalny $O(h)$, rząd 1.**
> - **Metoda Heuna** (ulepszony Euler, rząd 2): $y_{i+1} = y_i + \frac h2\big[f(x_i, y_i) + f(x_i + h, y_i + hf(x_i, y_i))\big]$.
> - **Zmodyfikowany Euler** (punkt środkowy, rząd 2): $y_{i+1} = y_i + hf\big(x_i + \frac h2, y_i + \frac h2f(x_i, y_i)\big)$.
> - Obie modyfikacje przyspieszają zbieżność, ale krok kosztuje 2 wywołania $f$.

> [!wzor] Runge–Kutta
> $\Phi = \sum_{i=1}^rc_ik_i$, $k_i = f\big(x + h\sum_jb_{ij},\ y + h\sum_jb_{ij}k_j\big)$. Gdy $b_{ij} = 0$ dla $j \ge i$, metoda jest **otwarta (jawna)**: $k_i$ liczysz po kolei. Koszt kroku: $r$ wywołań $f$. Wykład: maksymalny rząd metody $r$-etapowej $p(r) = r$. **Doprecyzowanie:** to prawda dla $r \le 4$; dla $r = 5$ maksymalny rząd jawnej RK to 4, dla $r = 6$ to 5 (bariery Butchera), dlatego RK4 jest „najbardziej opłacalna”. Klasyczna RK4: wzór wyżej.

> [!definicja] Metody jawne, niejawne i sztywność (z wykładu)
> - **Jawne:** $y_{i+1} = F(x, y_i, y_i', \dots)$, proste i tanie, ale dokładność i stabilność silnie zależą od kroku.
> - **Niejawne:** $y_{i+1} = G(x, y_{i+1}, y_i, \dots)$, w każdym kroku trzeba rozwiązać (zwykle nieliniowe) równanie $y_{i+1} - hf(x_{i+1}, y_{i+1}) = y_i$, za to **na ogół bezwarunkowo stabilne**.
> - **Układ sztywny:** wartości własne $\lambda_s = a_s + ib_s$ jakobianu $J = [\partial f_l/\partial y_r]$ mają $a_s < 0$ i **współczynnik sztywności** $S = \frac{a_{max}}{a_{min}} \gg 1$ (gdzie $a_{max} = \max(-a_s)$, $a_{min} = \min(-a_s)$). Dla liniowego $y' = Jy + g(x)$: $y = u_1 + u_2$, gdzie $u_2 = \sum c_sw_se^{\lambda_sx} \to 0$ (szybko zanikające składowe), a interesuje nas wolna $u_1$.
> - Równanie $n$-tego rzędu sprowadzasz do układu 1. rzędu: $z_1 = y, z_2 = y', \dots, z_n = y^{(n-1)}$, $z_n' = f(x, z_1, \dots, z_n)$. Przykład z wykładu: $y'' + ky = f(t)$, $y(0) = 0$, $y'(0) = 1$ → $z_1' = z_2$, $z_2' = -kz_1 + f(t)$, $z_1(0) = 0$, $z_2(0) = 1$.

> [!przyklad] Trzy przykłady z wykładu (policzone)
> 1. **Dokładność:** $y' = y(1 - t)$, $y(0) = 1$, dokładnie $y = e^{t - t^2/2}$. W $t = 2$ (dokładnie 1): Euler $h = 0{,}1$ daje $1{,}0690$, $h = 0{,}01$ daje $1{,}0067$. Błąd spada ok. 10×, tak jak dla metody rzędu 1.
> 2. **Niestabilność zadania:** $y' = 3y - 4e^{-x}$, $y(0) = 1$, dokładnie $y = e^{-x}$. Ogólne rozwiązanie to $e^{-x} + Ce^{3x}$. Każdy błąd (zaokrąglenia, metody) wprowadza małe $C \ne 0$, które rośnie jak $e^{3x}$. **Każda** metoda numeryczna się tu rozjedzie, bo zadanie jest źle uwarunkowane, nie algorytm.
> 3. **Równanie sztywne:** $y' = -1000y$, $y(0) = 2$, dokładnie $2e^{-1000x}$. Euler jawny: $y_{i+1} = (1 - 1000h)y_i$, stabilny tylko dla $h < 0{,}002$. Euler niejawny: $y_{i+1} = \frac{y_i}{1 + 1000h}$, stabilny dla każdego $h$; nawet przy $h = 1$ daje $y_i = 2/1001^i$.
>
> ![[Równanie sztywne — Euler jawny i niejawny.png]]

> [!przyklad]- Przykład z wykładu: stygnięcie kuli przez promieniowanie
> $T'(t) = \alpha\,(T(t)^4 - \beta)$, $T(0) = T_0 = 1200$ K (zapis ze slajdu; przy chłodzeniu $T' < 0$, więc $\alpha < 0$, a $\beta = T_{ot}^4$). Pytanie: temperatura po 300 s. Równanie jest nieliniowe, a na początku bardzo szybkie (czynnik $T^3$ w jakobianie), więc jawny Euler wymaga małego kroku na starcie.

## Typowe błędy
> [!blad] „Niejawny = zawsze dokładny”
> Euler niejawny jest **stabilny** przy każdym kroku, ale nadal tylko 1. rzędu. Duży krok daje stabilną, lecz niedokładną odpowiedź. Stabilność i dokładność to dwie różne sprawy.

> [!blad] Crank–Nicolson z dużym krokiem
> Przy $h|\lambda| \gg 2$ mamy $R \approx -1$, więc składowe szybkie nie gasną, tylko zmieniają znak co krok. W MES widać wtedy nierealne oscylacje temperatury przy nagłej zmianie warunku brzegowego. Lekarstwo: mniejszy krok albo θ = 1 (lub θ ≈ 2/3).

> [!blad] Zły znak K w schemacie
> Równanie $C\dot T + KT = F$ ma $K$ po **lewej** stronie. Gdy przepisujesz $y' = \lambda y$, to $K = -\lambda > 0$. Zamiana znaku daje rozwiązanie rosnące wykładniczo.

## Na zajęcia i egzamin
> [!zajecia]
> - **Projekt MES 2D (nieustalony):** pętla czasowa to dokładnie schemat θ z tej notatki. W sprawozdaniu „weryfikacja kroku czasowego” oznacza: policz dla $\Delta t$, $\Delta t/2$, $\Delta t/4$ i pokaż, że wynik się stabilizuje (oraz jakiego rzędu jest zbieżność). Dla schematu jawnego podaj warunek stabilności. **Ćwicz w C++:** `Kod/metody_numeryczne/zadania/m07.cpp` (Euler, RK2, RK4, trajektoria, schemat θ dla układu C·T' + H·T = P), specyfikacja w `m07.h`, sprawdzasz `./sprawdz.sh 07`. Wzorzec: `rozwiazania/m07.cpp`, dopiero po własnej próbie. Potem `m08` (`pret_nieustalony`) i notatka [[Schematy całkowania w czasie]].
> - **Egzamin MES:** „schematy całkowania w czasie: jawny, niejawny, Crank–Nicolson, wady i zalety”, „dlaczego schemat jawny wymaga małego kroku”.
> - **PK Matematyka 2:** RRZ liniowe rozwiązujesz analitycznie (także przez [[Transformata Laplace'a]]); numeryka służy do sprawdzenia wyniku.
> - Łączy się z: [[Schematy całkowania w czasie]], [[Nieustalony przepływ ciepła]], [[Równanie różniczkowe zwyczajne]].

## Zadania
> [!zadanie] Zadanie 1
> $y' = -40y$. Jaki największy krok dopuszcza Euler jawny? Co się stanie przy $h = 0{,}06$?
> > [!odpowiedz]- Odpowiedź
> > $h < 2/40 = 0{,}05$. Przy $h = 0{,}06$ mamy $R = 1 - 2{,}4 = -1{,}4$, więc $|y_k| = 1{,}4^k$ i rozwiązanie rośnie, zmieniając znak.

> [!zadanie] Zadanie 2
> Euler z $h = 0{,}1$ dał błąd $4 \cdot 10^{-3}$. Jaki błąd przewidujesz dla $h = 0{,}025$? A dla RK4, jeśli przy $h = 0{,}1$ błąd wynosił $2 \cdot 10^{-6}$?
> > [!odpowiedz]- Odpowiedź
> > Krok skrócony 4 razy. Euler: $4 \cdot 10^{-3}/4 = 10^{-3}$. RK4: $2 \cdot 10^{-6}/4^4 \approx 7{,}8 \cdot 10^{-9}$.

> [!zadanie] Zadanie 3
> Zapisz $y'' + 0{,}4y' + 4y = 0$ jako układ 1. rzędu i podaj $\zeta$ oraz $\omega$.
> > [!odpowiedz]- Odpowiedź
> > $\omega^2 = 4$, więc $\omega = 2$, a z $2\zeta\omega = 0{,}4$ wychodzi $\zeta = 0{,}1$. Układ: $u_1' = u_2$, $u_2' = -4u_1 - 0{,}4u_2$, czyli $A = \begin{bmatrix} 0 & 1 \\ -4 & -0{,}4 \end{bmatrix}$. Układ RLC ma tę samą postać z $\omega = 1/\sqrt{LC}$ i $\zeta = \frac R2\sqrt{C/L}$.

> [!zadanie] Zadanie 4
> Pokaż, że dla $y' = \lambda y$ schemat θ z $\theta = \frac12$ daje $R(z) = \frac{1 + z/2}{1 - z/2}$.
> > [!odpowiedz]- Odpowiedź
> > $C = 1$, $K = -\lambda$: $(\frac1h - \frac\lambda2)\,y_{k+1} = (\frac1h + \frac\lambda2)\,y_k$. Po pomnożeniu przez $h$ dostajesz $y_{k+1} = \frac{1 + h\lambda/2}{1 - h\lambda/2}\,y_k$.

## Pytania testowe (egzamin UPeL)
> [!zadanie] 1. Zagadnienie $y' = f(x, y)$, $y(a) = y_a$ ma dokładnie jedno rozwiązanie, jeśli:
> a) $f$ jest ciągła względem $x$ i lipschitzowska względem $y$ b) $f$ jest różniczkowalna w nieskończoność c) $f$ jest ograniczona d) $y_a = 0$
> > [!odpowiedz]- Odpowiedź
> > **a.**

> [!zadanie] 2. Które metody są rzędu 2?
> a) Euler b) Heun (ulepszony Euler) c) zmodyfikowany Euler (punkt środkowy) d) klasyczna RK4
> > [!odpowiedz]- Odpowiedź
> > **b, c.** Euler rząd 1, RK4 rząd 4.

> [!zadanie] 3. Metody niejawne:
> a) wymagają w każdym kroku rozwiązania równania b) są na ogół bezwarunkowo stabilne c) nadają się do równań sztywnych d) są zawsze tańsze w jednym kroku od jawnych
> > [!odpowiedz]- Odpowiedź
> > **a, b, c.**

> [!zadanie] 4. Dla $y' = -1000y$ jawny Euler jest stabilny dla:
> a) każdego $h$ b) $h < 0{,}002$ c) $h < 0{,}001$ d) $h > 0{,}002$
> > [!odpowiedz]- Odpowiedź
> > **b.** $|1 - 1000h| < 1$.

> [!zadanie] 5. Układ $y' = f(x, y)$ jest sztywny, gdy wartości własne jakobianu:
> a) mają ujemne części rzeczywiste b) mają bardzo różne moduły części rzeczywistych ($S \gg 1$) c) są czysto urojone d) są równe
> > [!odpowiedz]- Odpowiedź
> > **a, b** (oba warunki naraz).

> [!zadanie] 6. Warunek aproksymacji (zgodności) metody jednokrokowej to:
> a) $\Phi_f(x, y; 0) = f(x, y)$ b) $\Phi_f(x, y; h) = 0$ c) $h < 2/|\lambda|$ d) $\Phi$ liniowa w $f$
> > [!odpowiedz]- Odpowiedź
> > **a.**

## Fiszki #fiszki/numeryczne
Warunek Lipschitza (RRZ)::$|f(x,y_1) - f(x,y_2)| \le L|y_1 - y_2|$; z ciągłością w $x$ daje jednoznaczność zagadnienia Cauchy'ego
Postać ogólna metody jednokrokowej::$y_{i+1} = y_i + h\Phi_f(x_i, y_i; h)$
Warunek aproksymacji (zgodności)::$\Phi_f(x, y; 0) = f(x, y)$
Metoda Heuna::$y_{i+1} = y_i + \frac h2[f(x_i,y_i) + f(x_i+h, y_i + hf(x_i,y_i))]$, rząd 2
Błąd globalny metody Eulera::$O(h)$, $|e_i| \le M_2(b-a)h$
Współczynnik sztywności::$S = a_{max}/a_{min} \gg 1$ (moduły ujemnych części rzeczywistych wartości własnych jakobianu)
Dlaczego $y' = 3y - 4e^{-x}$, $y(0)=1$ psuje każdą metodę?::Ogólne rozwiązanie $e^{-x} + Ce^{3x}$; każdy błąd wprowadza $C \ne 0$, które rośnie wykładniczo
Wzór Eulera jawnego::y(k+1) = y(k) + h·f(t(k), y(k)), rząd 1
Wzór Eulera niejawnego dla y' = Ay::(I − hA)·y(k+1) = y(k), rząd 1, stabilny dla każdego h
Schemat θ w MES nieustalonym::(C/Δt + θK)·T(k+1) = (C/Δt − (1 − θ)K)·T(k) + F
Co daje θ = 0, θ = 1/2, θ = 1?::Euler jawny, Crank–Nicolson (rząd 2), Euler niejawny
Warunek stabilności Eulera jawnego dla y' = λy, λ < 0::h < 2/|λ|, bo |1 + hλ| ≤ 1
Obszar stabilności Eulera jawnego::Koło o środku −1 i promieniu 1 na płaszczyźnie hλ
Co to jest układ sztywny?::Układ z bardzo szybkimi i bardzo wolnymi składowymi; metoda jawna musi brać krok pod najszybszą
Wada Cranka–Nicolsona przy dużym kroku::R → −1, więc szybkie składowe oscylują zamiast gasnąć
Rząd metody RK4 i liczba obliczeń f na krok::Rząd 4, cztery obliczenia f na krok
Jak empirycznie wyznaczyć rząd metody?::Policz błąd dla h i h/2: p = log2(e(h)/e(h/2)); albo nachylenie log(błąd) od log(h)
Jak zapisać y'' + 2ζωy' + ω²y = 0 jako układ?::u = [y, y'], u' = [u2, −ω²·u1 − 2ζω·u2]
Jak skaluje się krok jawny w MES przy zagęszczaniu siatki?::Jak h_x², więc siatka dwa razy gęstsza wymaga kroku cztery razy krótszego

Wzór metody RK2 z instrukcji (Heun)::k₁ = f(x, y), k₂ = f(x + h, y + h·k₁), y_{i+1} = y_i + h/2·(k₁ + k₂)
Wzór metody RK4::y_{i+1} = y_i + h/6·(k₁ + 2k₂ + 2k₃ + k₄); k₂ i k₃ w połowie kroku, k₄ na końcu
Ile razy maleje błąd po podwojeniu N dla Eulera, RK2, RK4?::2, 4 i 16 razy (rzędy 1, 2, 4)
Prędkość graniczna kulki m·v' = mg − kv² i dlaczego h = 0,5 wybucha?::v∞ = √(mg/k) ≈ 2,21 m/s; λ = 2kv∞/m ≈ 8,9, a Euler wymaga h < 2/λ ≈ 0,23 s

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.

## Źródła
- A. Milenin, *Podstawy metody elementów skończonych*, Wydawnictwa AGH, Kraków 2010 (procesy nieustalone, schematy całkowania w czasie).
- O.C. Zienkiewicz, R.L. Taylor, J.Z. Zhu, *The Finite Element Method: Its Basis and Fundamentals*, Elsevier (rozdz. o problemach zależnych od czasu).
- Z. Fortuna, B. Macukow, J. Wąsowski, *Metody numeryczne*, WNT (rozdz. o równaniach różniczkowych zwyczajnych).
- R.L. Burden, J.D. Faires, *Numerical Analysis*, Cengage (Euler, Runge–Kutta, stabilność, równania sztywne).
- Dokumentacja: `scipy.integrate.solve_ivp`.
- Instrukcja: Lab RR — metody rozwiązywania równań różniczkowych (Euler, RK2, RK4)
- Kurs MN w C++: Kod/metody_numeryczne (moduły, testy, rozwiązania wzorcowe)
- H.P. Langtangen, S. Linge — Finite Difference Computing with PDEs (Springer 2017, CC BY), rozdz. 3 i dodatek B (pobrane: Załączniki/MES/Podręczniki)
