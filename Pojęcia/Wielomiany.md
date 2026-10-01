---
typ: pojęcie
status: uczę się
przedmioty: ["[[PK Matematyka 1]]"]
wymaga: []
powiązane: ["[[Rozkład na ułamki proste]]", "[[Równanie różniczkowe zwyczajne]]"]
tags: [pojęcie, matematyka]
utworzono: 2026-09-27
źródła: []
---
# Wielomiany

> [!intuicja] Po ludzku
> Wyrażenie złożone z potęg $x$ z liczbami: $3x^2 - x + 5$. "Stopień" to najwyższa potęga. Wszystkie transmitancje i mianowniki RRZ to wielomiany — rozkład na czynniki (pierwiastki] to klucz do [[Rozkład na ułamki proste|ułamków prostych]] i do [[Równanie różniczkowe zwyczajne|biegunów]] układu.

> [!metafora] Jak dla idioty
> Wielomian to "receptura z potęgami". Znajdowanie **pierwiastków** (miejsc, gdzie $=0$) to jak znalezienie "krytycznych punktów" receptury — to one rozkładają się na proste czynniki $(x-r)$, a każdy czynnik to jeden [[Rozkład na ułamki proste|ułamek prosty]] w rozkładzie.

## Definicja i fakty
> [!definicja]
> Stopień wielomianu $a_n x^n+\\ldots+a_0$ = najwyższa potęga $n$. **Pierwiastki**: liczby $r$, dla których $W(r)=0$; wielomian stopnia $n$ ma (z krotnością) $n$ pierwiastków (w tym zespolonych). Rozkład na czynniki: $W(x) = a_n(x-r_1)\\cdots(x-r_n)$.

## Jak to działa— twierdzenie o reszcie i dzielenie
> [!wzor] Dzielenie wielomianów (potrzebne, gdy licznik ≥ mianownik)
> $W(x): Q(x)$ z resztą $R(x)$, gdzie $\\deg R < \\deg Q$. Robisz jak dzielenie liczb w słupku, tylko z potęgami. W [[Rozkład na ułamki proste|rozkładzie]] najpierw dzielisz, jeśli stopień licznika ≥ mianownika.

## Typowe błędy
> [!blad] Mylisz stopień z liczbą wyrazów
> $3x^5+x$ to stopień **5**, nie 2.

> [!blad] Szukasz pierwiastków tylko rzeczywistych
> Wielomian może mieć pierwiastki zespolone (np. $x^2+1$); to normalne w [[Liczby zespolone|liczbach zespolonych]] i w biegunach układu.

## Zadania
> [!zadanie] Zadanie 1
> Rozłóż $x^2+3x+2$ na czynniki.
> > [!odpowiedz]- Odpowiedź
> > $(x+1)(x+2)$ (pierwiastki $-1,-2$).

> [!zadanie] Zadanie 2
> Podaj pierwiastek $x^2+1$.
> > [!odpowiedz]- Odpowiedź
> > $x_{1,2} = \\pm j$.

## Fiszki #fiszki/matematyka
Stopień wielomianu :: najwyższa potęga zmiennej
Ile pierwiastków ma wielomian stopnia $n$::do $n$ (z krotnością, w tym zespolone)
Kiedy dzielisz wielomiany?::gdy stopień licznika ≥ mianownika (np. przed ułamkami prostymi)
