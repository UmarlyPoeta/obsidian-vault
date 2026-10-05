---
typ: pojęcie
status: nowe
przedmioty: ["[[AGH Optymalizacja]]", "[[AGH Metody numeryczne]]"]
wymaga: []
powiązane: ["[[Optymalizacja Lab 0 — Monte Carlo i wahadło]]", "[[Całkowanie numeryczne i kwadratury Gaussa]]", "[[Optymalizacja W01 — wprowadzenie i klasyfikacja problemów]]", "[[Algorytm genetyczny]]", "[[Symulowane wyżarzanie]]"]
tags: [pojęcie, optymalizacja, numeryczne, stochastyczne]
utworzono: 2026-10-05
źródła: ["J. Kusiak — Optymalizacja, wykład 1 (2026/27)", "D. Szeliga — Metody numeryczne, wykład Kwadratury, rozdz. Metoda Monte Carlo (2025/26)", "Ł. Sztangret — kod MC w opt_alg.cpp", "Kusiak, Danielewska-Tułecka, Oprocha — Optymalizacja (PWN 2009)"]
---
# Metoda Monte Carlo

> [!skrot] W 30 sekund
> - **Co to:** metody, które zamiast liczyć wzorem **losują** dużo punktów i uśredniają albo wybierają najlepszy. Autor nazwy: Stanisław Ulam (projekt Manhattan, lata 40.).
> - **Klucz (całkowanie):** $\displaystyle \int_\Omega z(x)\,dx \approx \frac{|\Omega|}{N}\sum_{i=1}^N z(X_i)$, błąd $\sim \sigma/\sqrt N$ **niezależnie od wymiaru**. **Klucz (optymalizacja):** losuj $x \in X$, pamiętaj najlepszy.
> - **Kiedy:** całki w wielu wymiarach, funkcje bez wzoru i pochodnych, start dla lepszych metod, punkt odniesienia („czy moja metoda jest lepsza od losowania?”).

> [!intuicja] Po ludzku
> Chcesz znać pole jeziora o dziwnym kształcie. Rzucasz na mapę 10 000 ziarenek ryżu równomiernie po prostokącie o znanym polu i liczysz, ile wpadło do jeziora. Udział ziarenek w jeziorze razy pole prostokąta to pole jeziora. Im więcej ziarenek, tym dokładniej, ale **powoli**: 100× więcej ziarenek daje tylko 10× mniejszy błąd.

## Definicja i wzory
> [!definicja] Całkowanie Monte Carlo (wykład MN, Kwadratury)
> Wartość oczekiwana zmiennej $z(x)$, gdzie $x$ ma gęstość $f$ na obszarze $V$:
> $$\langle z \rangle = \int_V z(x) f(x)\,dx \approx \bar z = \frac{1}{N}\sum_{i=1}^N z(x_i)$$
> Błąd oszacowania (odchylenie standardowe średniej):
> $$\sigma(\bar z) = \frac{\sigma(z)}{\sqrt N}, \qquad \sigma^2(z) \approx \frac{1}{N-1}\left(\sum z_i^2 - \frac1N\Big(\sum z_i\Big)^2\right)$$
> Dla całki po obszarze $V$ zawartym w prostym $\Omega$ (np. kostce) używasz funkcji charakterystycznej $1_V$ i rozkładu jednostajnego na $\Omega$:
> $$I = \int_V z(x)\,dx = \int_\Omega 1_V(x)\,z(x)\,dx \approx \frac{|\Omega|}{N}\sum_{i=1}^N 1_V(X_i)\,z(X_i)$$
> Efektywność zależy od tego, jak duże jest $\Omega$ względem $V$: im więcej punktów „pudłuje”, tym gorzej.

> [!wzor] Pole i liczba π
> Pole obszaru $S \subset \Omega$: $\displaystyle S \approx |\Omega|\,\frac{n}{N}$, gdzie $n$ to liczba punktów wewnątrz $S$.
> Koło o promieniu $0{,}5$ w kwadracie $1\times1$: $\displaystyle \pi \approx 4\,\frac{N_{in}}{N_{all}}$.

> [!definicja] Optymalizacja Monte Carlo (losowe przeszukiwanie)
> 1. Losuj $x \sim U(X)$, policz $f(x)$.
> 2. Jeśli $f(x) < f_{best}$, zapamiętaj $x$.
> 3. Stop po `Nmax` wywołaniach (albo gdy $f_{best} < \varepsilon$, jeśli znasz minimum).
>
> Prawdopodobieństwo, że po $N$ losowaniach **nie** trafisz w obszar dobrych rozwiązań o udziale $p$: $(1-p)^N$. Średnia liczba losowań do trafienia: $1/p$.

## Dlaczego MC wygrywa w wielu wymiarach
> [!wzor] Porównanie z kwadraturą na siatce
> | | metoda trapezów na siatce | Monte Carlo |
> |---|---|---|
> | punktów w $d$ wymiarach przy $n$ na oś | $(n+1)^d$ | $N$ (dowolne) |
> | błąd | $O(h^2) = O(N^{-2/d})$ | $O(N^{-1/2})$, **bez $d$** |
> | opłaca się | $d \lesssim 4$ | $d \gtrsim 4$–5 |
>
> Przykład z wykładu MN: $\int_{[0,1]^5}\sqrt{x_1 + \dots + x_5}\,dx$. Trapezy z $n = 10$ na oś to już $11^5 \approx 161\,000$ wywołań. MC z $N = 161\,000$ ma błąd rzędu $\sigma/\sqrt N \approx 0{,}2/400 \approx 5\cdot10^{-4}$ (dla tej funkcji $\sigma(z) \approx 0{,}2$), czyli porównywalny. Przy $d = 10$ siatka jest już niewykonalna ($11^{10} \approx 2{,}6\cdot10^{10}$), a MC działa tak samo.

## Jak to zrobić — krok po kroku
1. Określ obszar losowania $\Omega$ (jak najciaśniej wokół $V$ albo zbioru dopuszczalnego).
2. Losuj jednostajnie: $x_i = lb_i + (ub_i - lb_i)\,r$, $r \sim U[0,1]$.
3. Całka: sumuj $z(x)$ (z $1_V$), pomnóż przez $|\Omega|/N$, podaj **wynik ± błąd** $|\Omega|\,\sigma/\sqrt N$.
4. Optymalizacja: pamiętaj najlepszy punkt, licz wywołania, podaj kilka przebiegów (wynik jest losowy).

## Przykład
> [!przyklad] π z 1000 punktów
> Załóżmy, że z 1000 losowych punktów w $[0,1]^2$ do koła $(x-0{,}5)^2 + (y-0{,}5)^2 \le 0{,}25$ wpadło 789. Wtedy $\pi \approx 4\cdot0{,}789 = 3{,}156$. Błąd: $z \in \{0, 4\}$ z $p = \pi/4$, więc $\sigma(z) = 4\sqrt{p(1-p)} \approx 1{,}64$, $\sigma(\bar z) \approx 1{,}64/\sqrt{1000} \approx 0{,}052$. Wynik: $\pi = 3{,}16 \pm 0{,}05$, zgodny z $3{,}1416$. Żeby mieć 3 cyfry ($\pm 0{,}001$), potrzeba ok. $(1{,}64/0{,}001)^2 \approx 2{,}7$ mln punktów.

> [!przyklad]- Lab 0 z Optymalizacji
> Wahadło: $p \approx 0{,}013$, czyli średnio ok. 77 losowań. Funkcja testowa 2D z $\varepsilon = 0{,}01$: $p \approx 3{,}1\cdot10^{-4}$, średnio ok. 3200 losowań, a w ok. 4,6% przebiegów `Nmax = 10000` nie wystarcza. Szczegóły i wykresy: [[Optymalizacja Lab 0 — Monte Carlo i wahadło]].

```python
import random, math

def pi_mc(N):
    n_in = sum((random.random() - 0.5)**2 + (random.random() - 0.5)**2 <= 0.25 for _ in range(N))
    p = n_in / N
    return 4 * p, 4 * math.sqrt(p * (1 - p) / N)      # wynik i błąd standardowy

def mc_min(f, lb, ub, Nmax):
    best_x, best_f = None, math.inf
    for _ in range(Nmax):
        x = [a + (b - a) * random.random() for a, b in zip(lb, ub)]
        fx = f(x)
        if fx < best_f:
            best_x, best_f = x, fx
    return best_x, best_f
```

## Typowe błędy
> [!blad] Pułapki
> - Wynik bez błędu: MC zawsze podaje się jako $\bar z \pm \sigma/\sqrt N$.
> - Zbyt duży obszar $\Omega$ w stosunku do $V$: większość punktów nic nie wnosi.
> - Spodziewanie się szybkiej zbieżności: zysk jednej cyfry dokładności kosztuje **100×** więcej punktów.
> - Jedno uruchomienie jako „wynik” optymalizacji: trzeba kilku przebiegów albo statystyki.
> - Słaby generator albo stałe ziarno przy porównaniach. W C++ używaj `std::mt19937` z `std::uniform_real_distribution`, nie `rand() % N`.

## Na zajęcia i egzamin
> [!zajecia]
> - **MN (egzamin testowy):** wzór na oszacowanie całki, błąd $\sim 1/\sqrt N$ niezależny od wymiaru, kiedy MC lepsze od kwadratur, funkcja charakterystyczna obszaru.
> - **Optymalizacja:** MC to metoda niedeterministyczna, globalna, bezgradientowa. Używana jako punkt odniesienia i do generowania punktów startowych.
> - Łączy się z: [[Całkowanie numeryczne i kwadratury Gaussa]], [[Optymalizacja Lab 0 — Monte Carlo i wahadło]].

## Zadania
> [!zadanie] Zadanie 1
> Masz błąd $0{,}02$ przy $N = 10\,000$. Ile punktów potrzeba do błędu $0{,}002$?
> > [!odpowiedz]- Odpowiedź
> > Błąd $\sim 1/\sqrt N$, więc 10× mniejszy błąd wymaga 100× więcej punktów: $N = 10^6$.

> [!zadanie] Zadanie 2
> Zadanie optymalizacji w 10 wymiarach na $[0,1]^{10}$. Dobre rozwiązania to kostka o boku 0,1. Ile średnio losowań potrzebuje MC?
> > [!odpowiedz]- Odpowiedź
> > $p = 0{,}1^{10} = 10^{-10}$, czyli średnio $10^{10}$ losowań. Dla optymalizacji MC jest bezużyteczne w wielu wymiarach (inaczej niż dla całkowania, gdzie błąd nie zależy od $d$).

## Fiszki #fiszki/numeryczne
Błąd całkowania Monte Carlo?::$\sigma(z)/\sqrt N$, niezależny od wymiaru
Ile razy więcej punktów MC daje 10× mniejszy błąd?::100×
Wzór MC na całkę po $V \subset \Omega$?::$\frac{|\Omega|}{N}\sum 1_V(X_i)\,z(X_i)$
Kiedy MC lepsze od kwadratury na siatce?::W wielu wymiarach (od ok. 4–5)
Kto wymyślił metodę Monte Carlo?::Stanisław Ulam (z J. von Neumannem)
Średnia liczba losowań do trafienia w obszar o udziale $p$?::$1/p$

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.

## Źródła
- [[W01 Wprowadzenie (Kusiak 2026-27).pdf]] (π metodą MC, slajdy 47–51).
- [[W05 Kwadratury.pdf]] (D. Szeliga, rozdz. „Metoda Monte Carlo”, elementy statystyki, przykład całki 5D).
- Kod `MC` w `Załączniki/Optymalizacja/Kod lab (Sztangret)/opt_alg.cpp`.
- Demo π: [academo.org/demos/estimating-pi-monte-carlo](https://academo.org/demos/estimating-pi-monte-carlo/) (link z wykładu).
