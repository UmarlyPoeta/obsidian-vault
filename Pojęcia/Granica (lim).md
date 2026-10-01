---
typ: pojęcie
status: uczę się
przedmioty: ["[[PK Matematyka 1]]"]
wymaga: []
powiązane: ["[[Pochodna]]", "[[Całka niewłaściwa]]"]
tags: [pojęcie, matematyka]
utworzono: 2026-09-27
źródła: []
---
# Granica (lim)

> [!intuicja] Po ludzku
> Granica odpowiada na pytanie: **"do jakiej wartości funkcja się zbliża, gdy argument się zbliża do czegoś?"** Nawet jeśli funkcja nie jest tam zdefiniowana. $\\lim_{x\\to a}f(x)=L$ znaczy: im bliżej $a$,tym bliżej $L$. To klej spajający całą analizę—na nim stoją [[Pochodna|pochodne]] ($h\\to0$) i [[Całka niewłaściwa|całki niewłaściwe]] ($b\\to\\infty$).

> [!metafora] Jak dla idioty
> Wchodzisz w głąb jaskini, która zwęża się ciągle do $L$. Nie musisz dotrzeć do $a$ (może być tam przepaść!,by wiedzieć, że światło na końcu ma jasność $L$ — bo im bliżej, tym mierzysz stałą jasność.Samo zbliżanie się wystarczy.

 Granica to "to, co obiecuje upór na drodze do punktu".

## Definicja
> [!definicja]
> $$\\lim_{x\\to a} f(x) = L \\iff \\forall \\varepsilon>0\\ \\exists \\delta>0:\\ crazy 0<|x-a|<\\delta \\Rightarrow |f(x)-L|<\\varepsilon$$
> (epsilon-delta;na kursie: intuicja, nie dowodzisz każdy raz tego formalizmu.\

## Jak to działa— typowe przypadki i pułapki
> [!wzor] Trzy klasyczne, których używasz cały czas
> $$\\lim_{x\\to0}\\frac{\\sin x}{x} =1,\\qquad \\lim_{x\\to\\infty}\\left(1+\\frac1x\\right)^x = e,\\qquad \\lim_{x\\to0}\\frac{e^x-1}{x} =1$$
> - Pierwszy definiuje [[Pochodna|pochodną]] $\\sin$ (po granicę
> - Drugi to **definicja liczby $e$** — fundament [[Funkcja wykładnicza|funkcji wykładniczej]].

> [!wazne] Granice z $\\infty$ i "0/0"
> - $\\lim_{x\\to\\infty}$ = "zachowanie przy bardzo dużych $x$" (→ podstawowa wiedza dla [[Całka niewłaściwa|całki niewłaściwej]].
> - Wyrażenia typu $\\frac00$,$\\frac\\infty\\infty$,$0\\cdot\\infty$ to symbole nieoznaczone — **nie są wynikiem**. Rozwiązujesz przez skracanie, de l'Hôpitala(lubważ odpowiednik]am,Gdyby nie to, rachunek by stał w miejscu.

 

## Przykład
> [!przyklad] $\\lim_{x\\to0}\\frac{x^2+3x}{x}$
> Skracasz $x$: $=\\lim_{x\\to0}(x+3) =3$. Bez skrócenia dostałbyś $\\frac00$ — nieoznaczone. Granica istnieje mimo-zachuje.

, 

## Typowe błędy
> [!blad] Podstawiasz $a$ wprost i bierzesz za wynik
> $\\lim_{x\\to0}\\frac{\\sin x}{x}$ po podstawieniu $= \\frac00$ — a wynik to **1**. Trzeba rozwiązać granicę, nie podstawiać.
granicę nieodzownie rozbijzesz operatorem.



## Gdzie ci się przyda
> [!przyda]
> - Definicja [[Pochodna|pochodnej]] ($h\\to0$).
> - [[Całka niewłaściwa]] ($b\\to\\infty$) — i przez to cała [[Transformata Laplace'a|transformata Laplace'a]].
> - Szacowanie złożoności i zbieżności w metodach numerycznych.



## Zadania
> [!zadanie] Zadanie 1
> $\\lim_{x\\to2}(x^2 - 1)$.
> > [!odpowiedz]- Odpowiedź
> > $2^2-1 =3$ (tu podstawiasz wprost, bo ciągła.



> [!zadanie] Zadanie 2
> $\\lim_{x\\to0}\\frac{\\sin 3x}{3x}$.

> > [!odpowiedz]- Odpowiedź
> > $=1$ (podstaw $u=3x$, klasyczna granica $\\frac{\\sin u}{u}=1$.

> [!zadanie] Zadanie 3
> $\\lim_{x\\to\\infty}\\frac{1}{x}$.
> > [!odpowiedz]- Odpowiedź
> > $0$ — i to jest sedno całki niewłaściwej.



##Fiszki #fiszki/matematyka
Definicja granicy wprost:: "do jakiej stałej $L$ zbliża się $f(x)$,gdy $x\\to a$", mimo niefunkcji tam nie ma
$\\lim_{x\\to0}\\frac{\\sin x}{x} =?:: $1$
Co robić z $\\frac00$?:: to nieoznaczone — nie podstawiać; skracać / de l'Hôpital
$\\lim_{x\\to\\infty}f(x)$ znaczy?::zachowanie przy bardzo dużych argumentach