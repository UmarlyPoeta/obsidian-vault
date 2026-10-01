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

> [!skrot] W 30 sekund
> - **Co to:** funkcja $W(s) = a_n s^n + \dots + a_1 s + a_0$, $a_n \neq 0$; $n$ to stopień.
> - **Klucz:** twierdzenie Bézouta — $s_0$ jest pierwiastkiem $\iff$ $(s - s_0)$ dzieli $W(s)$; dzielisz schematem Hornera.
> - **Kiedy:** szukanie biegunów (pierwiastków mianownika) przed [[Rozkład na ułamki proste|rozkładem na ułamki proste]] i odwrotną [[Transformata Laplace'a|transformatą Laplace'a]].

> [!intuicja] Po ludzku
> Wielomian to „klocki” z potęg zmiennej. Najważniejsza umiejętność: rozbić go na iloczyn prostych nawiasów — liniowych $(s - s_0)$ i kwadratowych bez pierwiastków rzeczywistych. To jak rozkład liczby na czynniki pierwsze: $60 = 2\cdot2\cdot3\cdot5$, tylko zamiast liczb pierwszych masz nawiasy.

Pierwiastki zespolone zapisujemy z $j$ (konwencja automatyki; w matematyce $i$).

## Definicja i wzory
> [!definicja]
> $s_0$ jest **pierwiastkiem** $W$, gdy $W(s_0) = 0$. Jest **$k$-krotny**, gdy $(s - s_0)^k$ dzieli $W$, a $(s - s_0)^{k+1}$ już nie.

> [!wzor] Najważniejsze twierdzenia
> - **Bézout:** reszta z dzielenia $W(s)$ przez $(s - s_0)$ równa się $W(s_0)$.
> - **Pierwiastki wymierne:** jeśli współczynniki są całkowite, a $\frac{p}{q}$ (ułamek nieskracalny) jest pierwiastkiem, to $p \mid a_0$ i $q \mid a_n$. Dla $a_n = 1$ kandydaci to dzielniki wyrazu wolnego.
> - **Zasadnicze twierdzenie algebry:** wielomian stopnia $n$ ma dokładnie $n$ pierwiastków zespolonych (licząc krotności).
> - **Współczynniki rzeczywiste:** pierwiastki zespolone występują w parach sprzężonych $\sigma \pm j\omega$, więc nad $\mathbb{R}$ wielomian rozkłada się na czynniki liniowe i kwadratowe z $\Delta < 0$.

## Jak to zrobić — krok po kroku
1. Wyłącz przed nawias, co się da (np. $s^3 + s = s(s^2 + 1)$), i sprawdź wzory skróconego mnożenia.
2. Zgadnij pierwiastek wśród dzielników wyrazu wolnego: $\pm1, \pm2, \dots$ — podstaw i sprawdź, czy wychodzi $0$.
3. Podziel schematem Hornera przez $(s - s_0)$; dostajesz wielomian o stopień niższy.
4. Powtarzaj, aż zostanie trójmian — rozwiąż go z $\Delta$ (dla $\Delta < 0$ pierwiastki zespolone).
5. Zapisz pełny rozkład i sprawdź, wymnażając z powrotem (albo podstawiając jedną liczbę).

## Przykład
Rozłóż $W(s) = s^3 + 4s^2 + 9s + 10$.

1. Kandydaci: $\pm1, \pm2, \pm5, \pm10$. Wszystkie współczynniki są dodatnie, więc pierwiastek rzeczywisty musi być ujemny. $W(-1) = -1 + 4 - 9 + 10 = 4$, $W(-2) = -8 + 16 - 18 + 10 = 0$ — mamy $s_0 = -2$.
2. Horner dla $-2$ (współczynniki $1, 4, 9, 10$): $1$; $4 + (-2)\cdot1 = 2$; $9 + (-2)\cdot2 = 5$; $10 + (-2)\cdot5 = 0$ (reszta).
3. Iloraz: $s^2 + 2s + 5$, więc $W(s) = (s + 2)(s^2 + 2s + 5)$.
4. $\Delta = 4 - 20 = -16 < 0$, pierwiastki $s = -1 \pm 2j$.
5. Wynik: nad $\mathbb{R}$: $(s+2)(s^2 + 2s + 5)$; nad $\mathbb{C}$: $(s+2)(s + 1 - 2j)(s + 1 + 2j)$.

Jako mianownik transmitancji: biegun $-2$ daje $e^{-2t}$, para $-1 \pm 2j$ daje drganie gasnące $e^{-t}\cos 2t$, $e^{-t}\sin 2t$.

> [!przyklad]- Przykład 2 — trzy pierwiastki całkowite
> $W(s) = s^3 - 6s^2 + 11s - 6$. $W(1) = 1 - 6 + 11 - 6 = 0$.
> Horner dla $1$: $1$; $-5$; $6$; $0$, więc $W = (s - 1)(s^2 - 5s + 6) = (s-1)(s-2)(s-3)$.

## Typowe błędy
> [!blad] Pominięty współczynnik zerowy w Hornerze
> Dla $s^3 + 2s + 1$ wpisujesz $1, 0, 2, 1$ — bez zera wynik jest śmieciem.

> [!blad] Znak w nawiasie
> Pierwiastek $-2$ daje czynnik $(s + 2)$, a nie $(s - 2)$.

> [!blad] Rozkładanie trójmianu z $\Delta < 0$ nad $\mathbb{R}$
> $s^2 + 2s + 5$ jest już nierozkładalny nad $\mathbb{R}$. Do ułamków prostych zostawiasz go w całości (albo w postaci $(s+1)^2 + 4$).

> [!blad] Zgubiona krotność
> $s^3 + 2s^2 + s = s(s+1)^2$ — pierwiastek $-1$ jest dwukrotny i trzeba to zapisać.

## Na zajęcia i egzamin
> [!zajecia]
> - **Ćwiczenia:** Horner, zgadywanie pierwiastków, pełny rozkład na czynniki, dzielenie wielomianów z resztą.
> - **Egzamin:** „rozłóż na czynniki nad $\mathbb{R}$ i nad $\mathbb{C}$”, „wyznacz resztę z dzielenia”.
> - Łączy się z: [[Rozkład na ułamki proste]] (najpierw rozkładasz mianownik), [[Transformata Laplace'a]] (pierwiastki mianownika to bieguny; zespolone dają drgania), [[Równanie różniczkowe zwyczajne]] (wielomian charakterystyczny), [[Liczby zespolone]].

## Zadania
> [!zadanie] Zadanie 1
> Rozłóż $W(x) = x^3 - 2x^2 - 5x + 6$.
> > [!odpowiedz]- Odpowiedź
> > $W(1) = 0$; Horner: $1, -1, -6, 0$, więc $W = (x-1)(x^2 - x - 6) = (x-1)(x-3)(x+2)$.

> [!zadanie] Zadanie 2
> Wyznacz resztę z dzielenia $x^4 + 1$ przez $x - 1$.
> > [!odpowiedz]- Odpowiedź
> > Z Bézouta: $W(1) = 2$.

> [!zadanie] Zadanie 3
> Podziel $s^3 + 2s + 1$ przez $s + 1$.
> > [!odpowiedz]- Odpowiedź
> > Horner dla $-1$ ($1, 0, 2, 1$): $1, -1, 3$, reszta $-2$. Zatem $s^3 + 2s + 1 = (s+1)(s^2 - s + 3) - 2$.

> [!zadanie] Zadanie 4
> Rozłóż $s^4 - 1$ nad $\mathbb{R}$ i podaj wszystkie pierwiastki zespolone.
> > [!odpowiedz]- Odpowiedź
> > $(s^2 - 1)(s^2 + 1) = (s-1)(s+1)(s^2+1)$; pierwiastki: $1, -1, j, -j$.

## Fiszki #fiszki/matematyka
Twierdzenie Bézouta?::Reszta z dzielenia $W(s)$ przez $(s - s_0)$ równa się $W(s_0)$
Kiedy $(s - s_0)$ dzieli $W(s)$?::Gdy $W(s_0) = 0$
Gdzie szukać pierwiastków całkowitych?::Wśród dzielników wyrazu wolnego $a_0$
Pierwiastek wymierny $\frac{p}{q}$ spełnia?::$p$ dzieli $a_0$, $q$ dzieli $a_n$
Ile pierwiastków ma wielomian stopnia $n$ w $\mathbb{C}$?::Dokładnie $n$, licząc krotności
Jak występują pierwiastki zespolone wielomianu o współczynnikach rzeczywistych?::W parach sprzężonych $\sigma \pm j\omega$
Na jakie czynniki rozkłada się wielomian nad $\mathbb{R}$?::Liniowe i kwadratowe z $\Delta < 0$
Co robić ze współczynnikiem zerowym w Hornerze?::Wpisać $0$ — nie pomijać
Rozkład $s^3 + 4s^2 + 9s + 10$?::$(s+2)(s^2+2s+5)$, pierwiastki $-2$, $-1 \pm 2j$
Co to jest pierwiastek dwukrotny?::$(s - s_0)^2$ dzieli $W$, a $(s - s_0)^3$ już nie

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.

## Źródła
- A. Milian, *Zbiór zadań z matematyki z rozwiązaniami*, cz. 1 (Wyd. PK) — wielomiany.
- J. Jurlewicz, Z. Skoczylas, *Algebra liniowa 1* — wielomiany i funkcje wymierne.
