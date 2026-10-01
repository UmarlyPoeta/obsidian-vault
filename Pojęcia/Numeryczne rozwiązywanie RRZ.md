---
typ: pojęcie
status: nowe
przedmioty: ["[[AGH Metoda elementów skończonych]]", "[[PK Matematyka 2]]"]
wymaga: ["[[Równanie różniczkowe zwyczajne]]", "[[Układy równań liniowych]]"]
powiązane: ["[[Schematy całkowania w czasie]]", "[[Nieustalony przepływ ciepła]]", "[[Transformata Laplace'a]]", "[[Arytmetyka zmiennoprzecinkowa i błędy numeryczne]]", "[[Rozwiązywanie równań nieliniowych]]", "[[Różniczkowanie numeryczne]]", "[[Całkowanie numeryczne i kwadratury Gaussa]]", "[[MES 1D — od różnic skończonych do macierzy sztywności]]"]
tags: [pojęcie, metody-numeryczne, mes]
utworzono: 2026-10-02
źródła: ["A. Milenin — Podstawy metody elementów skończonych (Wyd. AGH 2010)", "O.C. Zienkiewicz, R.L. Taylor, J.Z. Zhu — The Finite Element Method: Its Basis and Fundamentals", "Z. Fortuna, B. Macukow, J. Wąsowski — Metody numeryczne (WNT)", "R.L. Burden, J.D. Faires — Numerical Analysis", "Dokumentacja scipy.integrate.solve_ivp"]
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

```python
import numpy as np
from scipy.integrate import solve_ivp
r, Tot, h, n = 0.1, 20.0, 2.0, 30
C, K, F = np.array([[1.0]]), np.array([[r]]), np.array([r * Tot])   # forma MES
T = np.array([90.0])
for theta in (0.0, 0.5, 1.0):
    lewa, prawa = C / h + theta * K, C / h - (1 - theta) * K
    Tk = T.copy()
    for _ in range(n):
        Tk = np.linalg.solve(lewa, prawa @ Tk + F)
    print(theta, Tk[0])                      # 20.0867 / 20.1701 / 20.2949
ref = solve_ivp(lambda t, y: -r * (y - Tot), (0, 60), [90.0], rtol=1e-10)
print(ref.y[0, -1])                          # 20.1735
```

## Typowe błędy
> [!blad] „Niejawny = zawsze dokładny”
> Euler niejawny jest **stabilny** przy każdym kroku, ale nadal tylko 1. rzędu. Duży krok daje stabilną, lecz niedokładną odpowiedź. Stabilność i dokładność to dwie różne sprawy.

> [!blad] Crank–Nicolson z dużym krokiem
> Przy $h|\lambda| \gg 2$ mamy $R \approx -1$, więc składowe szybkie nie gasną, tylko zmieniają znak co krok. W MES widać wtedy nierealne oscylacje temperatury przy nagłej zmianie warunku brzegowego. Lekarstwo: mniejszy krok albo θ = 1 (lub θ ≈ 2/3).

> [!blad] Zły znak K w schemacie
> Równanie $C\dot T + KT = F$ ma $K$ po **lewej** stronie. Gdy przepisujesz $y' = \lambda y$, to $K = -\lambda > 0$. Zamiana znaku daje rozwiązanie rosnące wykładniczo.

## Na zajęcia i egzamin
> [!zajecia]
> - **Projekt MES 2D (nieustalony):** pętla czasowa to dokładnie schemat θ z tej notatki. W sprawozdaniu „weryfikacja kroku czasowego” oznacza: policz dla $\Delta t$, $\Delta t/2$, $\Delta t/4$ i pokaż, że wynik się stabilizuje (oraz jakiego rzędu jest zbieżność). Dla schematu jawnego podaj warunek stabilności. Ćwicz na `m07_rrz.py` (`schemat_theta`, `rzad_zbieznosci`), potem przenieś to do `m08_mes1d.py` (`nieustalony`).
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

## Fiszki #fiszki/numeryczne
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

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.

## Źródła
- A. Milenin, *Podstawy metody elementów skończonych*, Wydawnictwa AGH, Kraków 2010 (procesy nieustalone, schematy całkowania w czasie).
- O.C. Zienkiewicz, R.L. Taylor, J.Z. Zhu, *The Finite Element Method: Its Basis and Fundamentals*, Elsevier (rozdz. o problemach zależnych od czasu).
- Z. Fortuna, B. Macukow, J. Wąsowski, *Metody numeryczne*, WNT (rozdz. o równaniach różniczkowych zwyczajnych).
- R.L. Burden, J.D. Faires, *Numerical Analysis*, Cengage (Euler, Runge–Kutta, stabilność, równania sztywne).
- Dokumentacja: `scipy.integrate.solve_ivp`.
