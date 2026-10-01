---
typ: pojęcie
status: uczę się
przedmioty: ["[[PK Matematyka 2]]"]
wymaga: ["[[Transformata Laplace'a]]"]
powiązane: ["[[Równanie różniczkowe zwyczajne]]", "[[Stany nieustalone w obwodach]]", "[[Drgania mechaniczne]]"]
tags: [pojęcie, sterowanie]
utworzono: 2026-09-27
źródła: []
---
# Transmitancja

> [!skrot] W 30 sekund
> - **Co to:** stosunek transformaty Laplace'a wyjścia do transformaty wejścia przy zerowych warunkach początkowych.
> - **Klucz:** $G(s)=\frac{Y(s)}{U(s)}$, więc $Y(s)=G(s)\,U(s)$; bieguny w lewej półpłaszczyźnie — układ stabilny.
> - **Kiedy:** cała automatyka: schematy blokowe, odpowiedź skokowa, stabilność, dobór regulatora.

> [!intuicja] Po ludzku
> Transmitancja to „przepis” układu: mówi, co układ zrobi z dowolnym sygnałem wejściowym. Równanie różniczkowe zamieniasz [[Transformata Laplace'a|Laplace'em]] na ułamek wielomianów — i zamiast rozwiązywać równanie, mnożysz $G(s)$ przez $U(s)$. Jak charakterystyka głośnika: znasz ją raz, a wiesz, jak zabrzmi każda piosenka.

## Definicja i wzory
> [!definicja]
> Dla układu opisanego równaniem $a_2y''+a_1y'+a_0y = b_1u'+b_0u$, po transformacji przy $y(0)=y'(0)=u(0)=0$:
> $$G(s)=\frac{Y(s)}{U(s)}=\frac{b_1s+b_0}{a_2s^2+a_1s+a_0}$$
> - **Zera** — pierwiastki licznika, **bieguny** — pierwiastki mianownika.
> - Mianownik to wielomian charakterystyczny z [[Równanie różniczkowe zwyczajne|równania różniczkowego]]: bieguny = pierwiastki $r$.
> - $G(s)=\mathcal{L}\{g(t)\}$, gdzie $g(t)$ to odpowiedź impulsowa.

> [!wzor] Człony podstawowe i ich odpowiedź skokowa $h(t)$
> - **Proporcjonalny:** $G=k$, $\;h(t)=k$.
> - **Inercyjny I rzędu:** $G=\frac{k}{Ts+1}$, $\;h(t)=k\left(1-e^{-t/T}\right)$; po czasie $T$ osiąga 63,2% wartości końcowej, po $3T$ ok. 95%.
> - **Całkujący:** $G=\frac{k}{s}$, $\;h(t)=k\,t$.
> - **Oscylacyjny** ($0<\zeta<1$): $G=\frac{k\,\omega_n^2}{s^2+2\zeta\omega_n s+\omega_n^2}$,
> $$h(t)=k\left[1-\frac{e^{-\zeta\omega_n t}}{\sqrt{1-\zeta^2}}\sin\left(\omega_d t+\varphi\right)\right],\quad \omega_d=\omega_n\sqrt{1-\zeta^2},\ \varphi=\arccos\zeta$$

> [!wazne] Stabilność
> - Wszystkie bieguny mają $\operatorname{Re}s<0$ (lewa półpłaszczyzna) — układ **stabilny asymptotycznie**, odpowiedź gaśnie.
> - Biegun z $\operatorname{Re}s>0$ (albo wielokrotny na osi urojonej) — **niestabilny**.
> - Pojedynczy biegun w $s=0$ lub para $\pm i\omega$ — **granica stabilności** (np. człon całkujący).
> - Wartość ustalona odpowiedzi skokowej układu stabilnego: $h(\infty)=\lim_{s\to 0}G(s)=G(0)$.

## Jak to zrobić — krok po kroku
1. Weź równanie różniczkowe układu i przetransformuj je przy zerowych warunkach początkowych ($y^{(n)}\to s^nY$).
2. Wyłącz $Y(s)$ i $U(s)$, podziel: $G=Y/U$.
3. Znajdź bieguny (i zera) — oceń stabilność.
4. Odpowiedź skokowa: $U=\frac{1}{s}$, więc $Y=\frac{G(s)}{s}$; [[Rozkład na ułamki proste|rozkład na ułamki proste]] i odwrotna transformata.
5. Sprawdź: $h(0)$ oraz $h(\infty)=G(0)$.

## Przykład
Układ: $y''+3y'+2y=4u$. Znajdź $G(s)$, oceń stabilność, policz odpowiedź skokową.

1. Transformacja: $s^2Y+3sY+2Y=4U$, więc
$$G(s)=\frac{4}{s^2+3s+2}=\frac{4}{(s+1)(s+2)}$$
2. Bieguny $-1$ i $-2$, brak zer. Oba w lewej półpłaszczyźnie — **stabilny**.
3. Skok jednostkowy:
$$Y(s)=\frac{4}{s(s+1)(s+2)}=\frac{A}{s}+\frac{B}{s+1}+\frac{C}{s+2}$$
Zasłanianie: $A=\frac{4}{1\cdot 2}=2$, $\;B=\frac{4}{(-1)(1)}=-4$, $\;C=\frac{4}{(-2)(-1)}=2$.
4. Odwrotna transformata:
$$h(t)=2-4e^{-t}+2e^{-2t}$$
5. Kontrola: $h(0)=0$ ✓, $h(\infty)=2=G(0)=\frac{4}{2}$ ✓. To samo wyszło klasycznie w [[Równanie różniczkowe zwyczajne]].

> [!przyklad]- Przykład 2 — człon inercyjny
> $G(s)=\frac{5}{2s+1}$: $k=5$, $T=2$ s. $Y=\frac{5}{s(2s+1)}=\frac{5}{s}-\frac{5}{s+\frac{1}{2}}$, więc $h(t)=5\left(1-e^{-t/2}\right)$. Po 2 s: $5\cdot 0{,}632\approx 3{,}16$.

> [!przyklad]- Przykład 3 — stabilny czy nie?
> $G_1=\frac{s+3}{s^2+2s+5}$: zero $-3$, bieguny $-1\pm 2i$ — stabilny (z oscylacjami).
> $G_2=\frac{1}{s^2-s-2}=\frac{1}{(s-2)(s+1)}$: biegun $+2$ — niestabilny, odpowiedź rośnie jak $e^{2t}$.

## Typowe błędy
> [!blad] Mylenie biegunów z zerami
> Bieguny to pierwiastki **mianownika** i to one decydują o stabilności. Zera (licznik) wpływają na kształt odpowiedzi, nie na stabilność.

> [!blad] Transmitancja z niezerowymi warunkami początkowymi
> Gdy w transformacie zostaną $y(0)$, $y'(0)$, stosunek $Y/U$ zależy od sygnału — to już nie jest $G(s)$.

> [!blad] $h(\infty)=G(0)$ dla układu niestabilnego
> Twierdzenie o wartości końcowej działa tylko, gdy $sY(s)$ ma bieguny w lewej półpłaszczyźnie. Dla $G=\frac{1}{s-1}$ wzór daje $-1$, a odpowiedź ucieka do $\infty$.

## Na zajęcia i egzamin
> [!zajecia]
> - **Ćwiczenia:** wyprowadzić $G(s)$ z równania lub obwodu, rozpoznać człon, narysować $h(t)$, ocenić stabilność z biegunów.
> - **Egzamin:** „Wyznacz odpowiedź skokową układu o transmitancji …” — ułamki proste i tablica transformat.
> - Łączy się z: [[Transformata Laplace'a]], [[Rozkład na ułamki proste]], [[Liczby zespolone]], [[Stany nieustalone w obwodach]], [[Drgania mechaniczne]]. Rozwinięcie na *Sterowaniu procesami ciągłymi* (Bode, Nyquist, Hurwitz, PID).

## Zadania
> [!zadanie] Zadanie 1
> Układ: $y'+4y=8u$. Podaj $G(s)$ w postaci $\frac{k}{Ts+1}$.
> > [!odpowiedz]- Odpowiedź
> > $G=\frac{8}{s+4}=\frac{2}{0{,}25s+1}$: $k=2$, $T=0{,}25$.

> [!zadanie] Zadanie 2
> Znajdź zera i bieguny $G(s)=\frac{s+1}{s^2+5s+6}$. Czy układ jest stabilny?
> > [!odpowiedz]- Odpowiedź
> > Zero $-1$, bieguny $-2$, $-3$. Stabilny.

> [!zadanie] Zadanie 3
> Odpowiedź skokowa $G(s)=\frac{3}{s}$?
> > [!odpowiedz]- Odpowiedź
> > $Y=\frac{3}{s^2}$, więc $h(t)=3t$ — człon całkujący, granica stabilności.

> [!zadanie] Zadanie 4
> $G(s)=\frac{10}{s^2+2s+10}$. Podaj $\omega_n$, $\zeta$, bieguny i $h(\infty)$.
> > [!odpowiedz]- Odpowiedź
> > $\omega_n=\sqrt{10}$, $\zeta=\frac{1}{\sqrt{10}}\approx 0{,}32$, bieguny $-1\pm 3i$, $h(\infty)=G(0)=1$.

> [!zadanie] Zadanie 5
> Czy układ $G(s)=\frac{4}{s^2-4}$ jest stabilny?
> > [!odpowiedz]- Odpowiedź
> > Bieguny $\pm 2$; biegun $+2$ w prawej półpłaszczyźnie — niestabilny.

## Fiszki #fiszki/automatyka
Definicja transmitancji?::G(s) = Y(s)/U(s) przy zerowych warunkach początkowych.
Jak policzyć wyjście, znając G(s)?::Y(s) = G(s)·U(s), potem odwrotna transformata Laplace'a.
Co to są bieguny transmitancji?::Pierwiastki mianownika G(s) — decydują o stabilności i charakterze odpowiedzi.
Co to są zera transmitancji?::Pierwiastki licznika G(s).
Kiedy układ jest stabilny asymptotycznie?::Gdy wszystkie bieguny leżą w lewej półpłaszczyźnie (Re s < 0).
Transmitancja członu inercyjnego I rzędu?::G = k/(Ts+1); odpowiedź skokowa k(1 − e^(−t/T)).
Ile procent wartości końcowej osiąga człon inercyjny po czasie T?::Ok. 63,2% (po 3T ok. 95%).
Transmitancja i odpowiedź skokowa członu całkującego?::G = k/s; h(t) = k·t.
Transmitancja członu oscylacyjnego?::G = kω_n²/(s² + 2ζω_n s + ω_n²), 0<ζ<1.
Jak policzyć odpowiedź skokową z G(s)?::Y = G(s)/s, rozkład na ułamki proste, odwrotna transformata.
Wartość ustalona odpowiedzi skokowej układu stabilnego?::h(∞) = G(0).
Jaki związek mają bieguny z równaniem różniczkowym?::Mianownik G(s) to wielomian charakterystyczny — bieguny to pierwiastki równania charakterystycznego.

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.

## Źródła
- K. Ogata, *Modern Control Engineering* (Pearson) — transmitancja, człony, stabilność
- T. Kaczorek, *Teoria sterowania i systemów* (PWN) — opis transmitancyjny układów liniowych
