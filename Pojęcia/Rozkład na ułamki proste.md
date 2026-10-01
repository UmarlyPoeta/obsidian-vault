---
typ: pojęcie
status: uczę się
przedmioty: ["[[PK Matematyka 1]]", "[[PK Matematyka 2]]"]
wymaga: ["[[Wielomiany]]"]
powiązane: ["[[Transformata Laplace'a]]"]
tags: [pojęcie, matematyka]
utworzono: 2026-09-27
źródła: []
---
# Rozkład na ułamki proste

> [!skrot] W 30 sekund
> - **Co to:** zapis funkcji wymiernej $\frac{P(s)}{Q(s)}$ jako sumy prostych ułamków typu $\frac{A}{s-a}$ i $\frac{Bs+C}{s^2+ps+q}$.
> - **Klucz:** rozłóż mianownik na czynniki, napisz szablon z niewiadomymi, wyznacz je (zakrywanie albo porównanie współczynników).
> - **Kiedy:** odwrotna [[Transformata Laplace'a|transformata Laplace'a]] i całkowanie funkcji wymiernych.

> [!intuicja] Po ludzku
> Tablica transformat zna tylko proste kawałki: $\frac{1}{s+a} \to e^{-at}$, $\frac{\omega}{(s+a)^2+\omega^2} \to e^{-at}\sin\omega t$ itd. Skomplikowany ułamek trzeba więc pociąć na takie kawałki. To jak rozmienianie banknotu na monety, które automat (tablica) przyjmuje.

Pierwiastki zespolone zapisujemy z $j$ (konwencja automatyki; w matematyce $i$).

## Definicja i wzory
> [!definicja] Szablon rozkładu
> Warunek: ułamek **właściwy**, tzn. $\deg P < \deg Q$ (jeśli nie — najpierw podziel wielomiany). Każdy czynnik mianownika daje swoje składniki:
> - $(s - a)$ $\to$ $\frac{A}{s-a}$
> - $(s - a)^k$ $\to$ $\frac{A_1}{s-a} + \frac{A_2}{(s-a)^2} + \dots + \frac{A_k}{(s-a)^k}$
> - $s^2 + ps + q$ z $\Delta < 0$ $\to$ $\frac{Bs + C}{s^2 + ps + q}$

> [!wzor] Zakrywanie (pierwiastek jednokrotny)
> $$A = \left.\frac{P(s)}{Q(s)}\,(s - a)\right|_{s = a}$$
> Czyli: „zakryj” czynnik $(s-a)$ w mianowniku i podstaw $s = a$ do reszty. Przy $(s-a)^k$ działa tylko dla najwyższej potęgi $A_k$.

> [!wzor] Tablica do odwrotnej transformaty
> $\frac{1}{s} \to 1$, $\quad \frac{1}{s+a} \to e^{-at}$, $\quad \frac{1}{(s+a)^2} \to te^{-at}$, $\quad \frac{s+a}{(s+a)^2+\omega^2} \to e^{-at}\cos\omega t$, $\quad \frac{\omega}{(s+a)^2+\omega^2} \to e^{-at}\sin\omega t$.

## Jak to zrobić — krok po kroku
1. Sprawdź stopnie; jeśli $\deg P \ge \deg Q$, podziel: wielomian + ułamek właściwy.
2. Rozłóż mianownik na czynniki (patrz [[Wielomiany]]).
3. Napisz szablon z niewiadomymi $A, B, C, \dots$
4. Wyznacz stałe: jednokrotne — zakrywaniem; resztę — mnożąc przez $Q(s)$ i porównując współczynniki przy potęgach $s$ (albo podstawiając wygodne $s$).
5. Sprawdź: sprowadź z powrotem do wspólnego mianownika albo podstaw jedną liczbę, np. $s = 1$.
6. Dla Laplace'a: trójmian z $\Delta < 0$ zapisz jako $(s+a)^2 + \omega^2$ i dopasuj licznik do $s+a$ oraz $\omega$.

## Przykład
Znajdź $f(t) = \mathcal{L}^{-1}\left\{\frac{s+3}{(s+1)(s+2)}\right\}$.

1. Ułamek właściwy (stopień 1 < 2), mianownik już rozłożony.
2. Szablon: $\frac{s+3}{(s+1)(s+2)} = \frac{A}{s+1} + \frac{B}{s+2}$.
3. Zakrywanie: $A = \left.\frac{s+3}{s+2}\right|_{s=-1} = \frac{2}{1} = 2$, $\quad B = \left.\frac{s+3}{s+1}\right|_{s=-2} = \frac{1}{-1} = -1$.
4. Sprawdzenie: $2(s+2) - (s+1) = s + 3$. Zgadza się.
5. Z tablicy: $f(t) = 2e^{-t} - e^{-2t}$.

> [!przyklad]- Przykład 2 — pierwiastek podwójny
> $\frac{1}{s(s+1)^2} = \frac{A}{s} + \frac{B}{s+1} + \frac{C}{(s+1)^2}$.
> Zakrywanie: $A = \left.\frac{1}{(s+1)^2}\right|_{s=0} = 1$, $C = \left.\frac{1}{s}\right|_{s=-1} = -1$.
> Z równania $1 = A(s+1)^2 + Bs(s+1) + Cs$ współczynnik przy $s^2$: $A + B = 0$, więc $B = -1$.
> Sprawdzenie: $(s+1)^2 - s(s+1) - s = 1$. Wynik: $\frac{1}{s} - \frac{1}{s+1} - \frac{1}{(s+1)^2}$, czyli $f(t) = 1 - e^{-t} - te^{-t}$.

> [!przyklad]- Przykład 3 — bieguny zespolone (drgania)
> $\frac{5}{s(s^2+2s+5)} = \frac{A}{s} + \frac{Bs + C}{s^2+2s+5}$, $\Delta = -16$, bieguny $-1 \pm 2j$.
> $A = \frac{5}{5} = 1$. Z $5 = (s^2+2s+5) + (Bs + C)s$: przy $s^2$: $1 + B = 0 \Rightarrow B = -1$; przy $s$: $2 + C = 0 \Rightarrow C = -2$.
> $\frac{s+2}{s^2+2s+5} = \frac{(s+1) + \frac12\cdot 2}{(s+1)^2 + 2^2}$, więc
> $f(t) = 1 - e^{-t}\cos 2t - \frac12 e^{-t}\sin 2t$ — skok do 1 z gasnącym drganiem.

## Typowe błędy
> [!blad] Rozkład ułamka niewłaściwego
> $\frac{s^2+1}{s(s+1)}$ ma równe stopnie — najpierw dzielenie: $1 + \frac{1-s}{s(s+1)}$.

> [!blad] Jeden ułamek na pierwiastek podwójny
> $(s+1)^2$ wymaga dwóch składników: $\frac{B}{s+1} + \frac{C}{(s+1)^2}$.

> [!blad] Zakrywanie przy pierwiastku wielokrotnym
> Zakrywanie daje tylko współczynnik przy najwyższej potędze. Pozostałe licz z porównania współczynników.

> [!blad] Stała w liczniku nad trójmianem
> Nad $s^2+ps+q$ z $\Delta < 0$ licznik to $Bs + C$, nie samo $C$.

## Na zajęcia i egzamin
> [!zajecia]
> - **Ćwiczenia:** rozkład z pierwiastkami pojedynczymi, podwójnymi i zespolonymi; całki z funkcji wymiernych.
> - **Egzamin:** rozwiązanie równania różniczkowego transformatą Laplace'a — rozkład na ułamki proste to krok przed odczytaniem $f(t)$ z tablicy.
> - Łączy się z: [[Transformata Laplace'a]] (krok odwrotnej transformaty; bieguny zespolone dają drgania $e^{-at}\sin\omega t$), [[Wielomiany]], [[Równanie różniczkowe zwyczajne]], [[Transmitancja]].

## Zadania
> [!zadanie] Zadanie 1
> Rozłóż $\frac{s}{(s+1)(s-2)}$ i podaj $\mathcal{L}^{-1}$.
> > [!odpowiedz]- Odpowiedź
> > $A = \frac{-1}{-3} = \frac13$, $B = \frac{2}{3}$: $\frac{1}{3(s+1)} + \frac{2}{3(s-2)}$, $f(t) = \frac13 e^{-t} + \frac23 e^{2t}$.

> [!zadanie] Zadanie 2
> Rozłóż $\frac{2s+3}{s^2+3s+2}$.
> > [!odpowiedz]- Odpowiedź
> > $s^2+3s+2 = (s+1)(s+2)$; $A = \frac{1}{1} = 1$, $B = \frac{-1}{-1} = 1$: $\frac{1}{s+1} + \frac{1}{s+2}$, $f(t) = e^{-t} + e^{-2t}$.

> [!zadanie] Zadanie 3
> Znajdź $\mathcal{L}^{-1}\left\{\frac{4}{s(s+2)}\right\}$.
> > [!odpowiedz]- Odpowiedź
> > $A = \frac42 = 2$, $B = \frac{4}{-2} = -2$: $f(t) = 2 - 2e^{-2t}$ (odpowiedź skokowa członu inercyjnego).

> [!zadanie] Zadanie 4
> Rozłóż $\frac{s^2+1}{s(s+1)}$.
> > [!odpowiedz]- Odpowiedź
> > Dzielenie: $1 + \frac{1-s}{s(s+1)}$; $A = 1$, $B = \frac{2}{-1} = -2$. Wynik: $1 + \frac1s - \frac{2}{s+1}$.

> [!zadanie] Zadanie 5
> Znajdź $\mathcal{L}^{-1}\left\{\frac{s+5}{s^2+4s+13}\right\}$.
> > [!odpowiedz]- Odpowiedź
> > $s^2+4s+13 = (s+2)^2 + 9$, licznik $(s+2) + 3$: $f(t) = e^{-2t}\cos 3t + e^{-2t}\sin 3t$.

## Fiszki #fiszki/matematyka
Kiedy można od razu rozkładać na ułamki proste?::Gdy ułamek jest właściwy: $\deg P < \deg Q$
Co zrobić, gdy $\deg P \ge \deg Q$?::Podzielić wielomiany: wielomian + ułamek właściwy
Składnik dla czynnika $(s-a)$?::$\frac{A}{s-a}$
Składniki dla $(s-a)^2$?::$\frac{A_1}{s-a} + \frac{A_2}{(s-a)^2}$
Składnik dla $s^2+ps+q$ z $\Delta<0$?::$\frac{Bs+C}{s^2+ps+q}$
Metoda zakrywania?::Zakryj $(s-a)$ w mianowniku i podstaw $s=a$ do reszty ułamka
Kiedy zakrywanie nie wystarcza?::Przy pierwiastku wielokrotnym (poza najwyższą potęgą) i przy trójmianach
$\mathcal{L}^{-1}\left\{\frac{1}{s+a}\right\}$?::$e^{-at}$
$\mathcal{L}^{-1}\left\{\frac{1}{(s+a)^2}\right\}$?::$te^{-at}$
$\mathcal{L}^{-1}\left\{\frac{\omega}{(s+a)^2+\omega^2}\right\}$?::$e^{-at}\sin\omega t$
Rozkład $\frac{s+3}{(s+1)(s+2)}$?::$\frac{2}{s+1} - \frac{1}{s+2}$
Co dają bieguny zespolone po odwrotnej transformacie?::Drgania: $e^{-at}\cos\omega t$ i $e^{-at}\sin\omega t$

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.

## Źródła
- A. Milian, *Zbiór zadań z matematyki z rozwiązaniami*, cz. 1–2 (Wyd. PK) — funkcje wymierne, transformata Laplace'a.
- M. Gewert, Z. Skoczylas, *Analiza matematyczna 1* — całkowanie funkcji wymiernych (rozkład na ułamki proste).
