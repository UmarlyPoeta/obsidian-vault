---
typ: pojęcie
status: nowe
przedmioty: ["[[AGH Optymalizacja]]"]
wymaga: ["[[Funkcja celu i warunki optymalności]]"]
powiązane: ["[[Metoda Fibonacciego]]", "[[Metoda Hooke'a-Jeevesa]]", "[[Metoda najszybszego spadku]]"]
tags: [pojęcie, optymalizacja]
utworzono: 2026-10-01
źródła: ["Kusiak, Danielewska-Tułecka, Oprocha — Optymalizacja. Wybrane metody z przykładami zastosowań (PWN 2009)", "Stadnicki — Teoria i praktyka rozwiązywania zadań optymalizacji (WNT 2006)", "MATLAB — dokumentacja fminbnd"]
---
# Metoda złotego podziału

> [!skrot] W 30 sekund
> - **Co to:** szukanie minimum funkcji **jednej zmiennej** na przedziale $[a,b]$ przez systematyczne zawężanie przedziału, bez pochodnych.
> - **Klucz:** punkty próbne $x_1 = b - k(b-a)$, $x_2 = a + k(b-a)$, gdzie $k = \frac{\sqrt5 - 1}{2} \approx 0{,}618$. Każda iteracja zostawia 61,8% przedziału i kosztuje **jedno** nowe obliczenie $f$.
> - **Kiedy:** $f$ unimodalna na $[a,b]$, pochodna niedostępna albo droga. To także „poszukiwanie w kierunku” wewnątrz metod wielowymiarowych.

> [!intuicja] Po ludzku
> Szukasz dna doliny na odcinku drogi. Sprawdzasz wysokość w dwóch punktach w środku: jeśli lewy jest niżej, dno na pewno nie leży za prawym punktem, więc tę końcówkę wyrzucasz. Złoty trik polega na rozmieszczeniu punktów tak, że jeden stary punkt **trafia dokładnie w miejsce** nowego punktu w mniejszym przedziale. Dlatego mierzysz tylko raz na iterację, a nie dwa razy.

## Definicja i wzory
> [!wzor] Złoty podział
> $$k = \frac{\sqrt5 - 1}{2} \approx 0{,}618, \qquad k^2 = 1 - k$$
> $$x_1 = b - k\,(b-a), \qquad x_2 = a + k\,(b-a), \qquad x_1 < x_2$$
> - Jeśli $f(x_1) < f(x_2)$, minimum leży w $[a, x_2]$, więc $b \leftarrow x_2$, $x_2 \leftarrow x_1$ i liczysz nowe $x_1$.
> - W przeciwnym razie minimum leży w $[x_1, b]$, więc $a \leftarrow x_1$, $x_1 \leftarrow x_2$ i liczysz nowe $x_2$.
> - Tożsamość $k^2 = 1-k$ sprawia, że stary punkt wypada dokładnie tam, gdzie powinien być nowy.

> [!wzor] Liczba iteracji
> Po $n$ iteracjach długość przedziału wynosi $L_n = k^n L_0$. Dla dokładności $\varepsilon$:
> $$n \ge \frac{\ln(\varepsilon / L_0)}{\ln k}$$
> Przykład: $L_0 = 4$, $\varepsilon = 10^{-3}$ daje $n = 18$. Przy $\varepsilon = 10^{-4}$ wychodzi $n = 23$.

## Jak to zrobić — krok po kroku
1. Upewnij się, że $f$ jest **unimodalna** na $[a,b]$. Jeśli nie znasz przedziału, znajdź go **metodą ekspansji** (krok rośnie, aż $f$ zacznie rosnąć).
2. Policz $x_1, x_2$, $f(x_1)$, $f(x_2)$.
3. Porównaj, wyrzuć część przedziału bez minimum i przesuń stary punkt.
4. Policz jeden nowy punkt.
5. Powtarzaj, aż $b - a < \varepsilon$. Wynik: $x^* \approx \frac{a+b}{2}$.

## Przykład
> [!przyklad] $f(x) = x^4 - 3x^3 + 2$ na $[0, 4]$
> $f'(x) = x^2(4x - 9)$, więc dokładnie $x^* = 2{,}25$, $f(x^*) \approx -6{,}543$. Sprawdźmy metodą:
>
> | it. | $a$ | $b$ | $x_1$ | $x_2$ | $f(x_1)$ | $f(x_2)$ | decyzja |
> |---|---|---|---|---|---|---|---|
> | 1 | 0 | 4 | 1,528 | 2,472 | −3,251 | −5,975 | $f_1 > f_2$: $a \leftarrow x_1$ |
> | 2 | 1,528 | 4 | 2,472 | 3,056 | −5,975 | 3,590 | $f_1 < f_2$: $b \leftarrow x_2$ |
> | 3 | 1,528 | 3,056 | 2,111 | 2,472 | −6,364 | −5,975 | $b \leftarrow x_2$ |
> | 4 | 1,528 | 2,472 | 1,889 | 2,111 | −5,486 | −6,364 | $a \leftarrow x_1$ |
> | 5 | 1,889 | 2,472 | 2,111 | 2,249 | −6,364 | −6,543 | $a \leftarrow x_1$ |
>
> Po 5 iteracjach przedział $[1{,}889;\ 2{,}472]$ (długość $0{,}58 = 0{,}618^5 \cdot 4$) zawiera $2{,}25$. Zwróć uwagę, że w każdym wierszu jeden z punktów jest **przepisany** z poprzedniego.

```python
import math

def zloty_podzial(f, a, b, eps=1e-4):
    k = (math.sqrt(5) - 1) / 2          # 0.618…
    x1, x2 = b - k * (b - a), a + k * (b - a)
    f1, f2 = f(x1), f(x2)
    n = 0
    while b - a > eps:
        if f1 < f2:                     # minimum na lewo od x2 → wyrzuć (x2, b]
            b, x2, f2 = x2, x1, f1
            x1 = b - k * (b - a); f1 = f(x1)
        else:                           # minimum na prawo od x1 → wyrzuć [a, x1)
            a, x1, f1 = x1, x2, f2
            x2 = a + k * (b - a); f2 = f(x2)
        n += 1
    return (a + b) / 2, n

x, n = zloty_podzial(lambda x: x**4 - 3*x**3 + 2, 0, 4)
print(f"x* ≈ {x:.4f} po {n} iteracjach")   # x* ≈ 2.2500 po 23 iteracjach
```

> [!kod]- To samo w MATLAB-ie
> ```matlab
> f = @(x) x.^4 - 3*x.^3 + 2;
> [x, fval] = fminbnd(f, 0, 4)   % wbudowane: złoty podział + interpolacja paraboliczna
> ```
> `fminbnd` to metoda 1D na przedziale. `fminsearch` to coś innego: wielowymiarowy simpleks Neldera–Meada.

> [!przyklad]- Ekspansja: skąd wziąć przedział $[a,b]$
> Start $x_0$, krok $d$. Liczysz $f(x_0 + d), f(x_0 + 2d), f(x_0 + 4d), \dots$ (krok się podwaja), dopóki $f$ maleje. Gdy pierwszy raz wzrośnie, minimum jest między przedostatnim a ostatnim punktem: to twój przedział startowy. Jeśli już $f(x_0 + d) > f(x_0)$, idziesz w drugą stronę ($d \to -d$).

> [!przyklad]- Metoda Fibonacciego w jednym akapicie
> Działa tak samo, ale przedział skraca się w proporcjach kolejnych liczb Fibonacciego $F_{n-1}/F_n$, a nie stałym $k$. Liczbę iteracji ustalasz **z góry** z $F_n \ge L_0/\varepsilon$. Przy tej samej liczbie obliczeń $f$ daje minimalnie krótszy przedział (jest optymalna). Dla dużego $n$ stosunek $F_{n-1}/F_n \to 0{,}618$, więc złoty podział to jej granica.

## Typowe błędy
> [!blad] Funkcja nie jest unimodalna
> Przy kilku dolinach metoda zbiegnie do **którejś** z nich, bez ostrzeżenia. Najpierw narysuj wykres albo zawęź przedział.

> [!blad] Liczenie dwóch nowych punktów na iterację
> Wtedy to zwykła metoda połowienia z dwoma próbami, dwa razy droższa. Cała zaleta złotego podziału to **jeden** nowy punkt na iterację.

> [!blad] Odwrócona decyzja
> Gdy $f(x_1) < f(x_2)$, wyrzucasz **prawą** końcówkę $(x_2, b]$, bo minimum jest bliżej mniejszej wartości. Łatwo to pomylić na kolokwium.

## Na zajęcia i egzamin
> [!zajecia]
> - **Projekt:** złoty podział to klasyczny „pierwszy algorytm” do własnej implementacji (Python lub MATLAB). Porównaj go z `fminbnd`, policz liczbę wywołań $f$ i pokaż zbieżność długości przedziału na wykresie (półlogarytmicznie widać prostą o nachyleniu $\ln 0{,}618$).
> - **Egzamin:** „opisz metodę złotego podziału i uzasadnij wartość 0,618”, „ile iteracji potrzeba do dokładności ε”, „czym różni się od Fibonacciego”.
> - Łączy się z: [[Funkcja celu i warunki optymalności]] (unimodalność), [[Metoda najszybszego spadku]] (szukanie kroku w kierunku), [[Metoda Hooke'a-Jeevesa]].

## Zadania
> [!zadanie] Zadanie 1
> Przedział $[0, 10]$. Policz pierwsze dwa punkty próbne.
> > [!odpowiedz]- Odpowiedź
> > $x_1 = 10 - 6{,}18 = 3{,}82$, $x_2 = 0 + 6{,}18 = 6{,}18$.

> [!zadanie] Zadanie 2
> Ile iteracji złotego podziału potrzeba, żeby przedział długości 1 zawęzić do $10^{-6}$?
> > [!odpowiedz]- Odpowiedź
> > $n \ge \ln(10^{-6}) / \ln 0{,}618 = 13{,}82 / 0{,}481 \approx 28{,}7$, czyli $n = 29$ iteracji (i ok. 30 obliczeń $f$).

> [!zadanie] Zadanie 3
> Na $[a,b] = [2,6]$ wyszło $f(x_1) = 5$, $f(x_2) = 7$. Jaki jest nowy przedział i który punkt przechodzi dalej?
> > [!odpowiedz]- Odpowiedź
> > $f(x_1) < f(x_2)$, więc nowy przedział to $[2;\ x_2] = [2;\ 4{,}472]$. Stary $x_1 = 3{,}528$ staje się nowym $x_2$. Nowe $x_1 = 4{,}472 - 0{,}618 \cdot 2{,}472 \approx 2{,}944$.

## Fiszki #fiszki/optymalizacja
Ile wynosi współczynnik złotego podziału k?::(√5 − 1)/2 ≈ 0,618
Jaka tożsamość sprawia, że w złotym podziale liczysz jeden nowy punkt na iterację?::k² = 1 − k, więc stary punkt trafia w miejsce nowego
Wzory na punkty próbne w złotym podziale::x1 = b − k(b − a), x2 = a + k(b − a)
Co wyrzucasz, gdy f(x1) < f(x2)?::Prawą końcówkę (x2, b], nowy przedział to [a, x2]
Jakie założenie o funkcji wymaga złoty podział?::Unimodalność na przedziale [a, b]
Długość przedziału po n iteracjach złotego podziału::L_n = 0,618ⁿ · L_0
Czym różni się metoda Fibonacciego od złotego podziału?::Proporcje F(n−1)/F(n) zamiast stałego k, liczba iteracji ustalona z góry; jest optymalna
Jak znaleźć przedział startowy dla metod 1D?::Metodą ekspansji: krok rośnie (podwaja się), aż funkcja zacznie rosnąć
Którą funkcję MATLAB-a użyć do minimum 1D na przedziale?::fminbnd (złoty podział z interpolacją paraboliczną)
Czym jest fminsearch w MATLAB-ie?::Wielowymiarową metodą simpleksu Neldera–Meada, bez gradientu

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.

## Źródła
- J. Kusiak, A. Danielewska-Tułecka, P. Oprocha, *Optymalizacja. Wybrane metody z przykładami zastosowań*, PWN 2009.
- J. Stadnicki, *Teoria i praktyka rozwiązywania zadań optymalizacji z przykładami zastosowań technicznych*, WNT 2006.
- Dokumentacja MATLAB: `fminbnd`, `fminsearch`.
