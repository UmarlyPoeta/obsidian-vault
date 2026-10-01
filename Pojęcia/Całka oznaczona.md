---
typ: pojęcie
status: uczę się
przedmioty: ["[[PK Matematyka 1]]"]
wymaga: ["[[Pochodna]]", "[[Granica (lim)]]"]
powiązane: ["[[Całka niewłaściwa]]", "[[Rozkład na ułamki proste]]"]
tags: [pojęcie, matematyka]
utworzono: 2026-09-27
źródła: []
---
# Całka oznaczona

> [!intuicja] Po ludzku
> Całka oznaczona $\\int_a^b f(x)dx$ liczy **pole pod krzywą** między $a$ i $b$ — z uwagą na znak (pod osią → ujemne\. To "suma nieskończenie wielu cienkich pasków" $f(x)\\cdot dx$. Gdy jedna z granic sięga $\\infty$, dostajesz [[Całka niewłaściwa|całkę niewłaściwą]] — fundament Laplace'a.

> [!metafora] Jak dla idioty
> Chcesz policzyć, ile wody wpadło do wanny, gdy strumień zmienia się w czasie. Sumujesz cieniutkie "plastry" (strumień × chwilka). Im cieńsze plastry (mniejszy $dx$),tym dokładniej — aż w granicy ($dx\\to0$) dostajesz dokładne pole.Całka to "dokładna suma plastrów".

## Definicja
> [!definicja] Prawda o całce (Fundamentalne Twierdzenie Rachunku)
> $$\\int_a^b f(x)\\,dx = F(b) - F(a),\\qquad F'(x) = f(x)$$
> Całkowanie to **odwrotność różniczkowania**: znajdź funkcję pierwotną $F$ (taką, że $F'=f$) i odejmij wartości na krańcach.

## Jak to działa — reguły bazowe (podstawa dla [[Rozkład na ułamki proste|funkcji wymiernych]])
> [!wzor] Kilka pierwiastków całek (odwrotność pochodnych)
> | całka | wynik |
> |---|---|
> | $\\int x^n dx$ | $\\frac{x^{n+1}}{n+1}+C$ ($n\\neq -1$) |
> | $\\int \\frac1x dx$ | $\\ln|x|+C$ |
> | $\\int e^{ax} dx$ | $\\frac1a e^{ax}+C$ |
> | $\\int \\frac{1}{s-a} ds$ | $\\ln|s-a|+C$ |

Klucz dla [[Transformata Laplace'a|Laplace'a]]: $\\int_b^\\infty \\frac1{s-a}$ prowadzi do dzienników — stąd $\\mathcal{L}^{-1}\\{\\frac1{s-a}\\} = e^{at}$ wynika z liniowości i wykładniczych, nie z dziennika, ale w tablicy transformat zapamiętujesz gotowce.

## Przykład
> [!przyklad] $\\int_0^1 (2x+1)dx$
> $F = x^2 + x$; wartość:$F(1)-F(0) = (1+1)-0 =2$. To pole trapezu pod prostą od 0 do 1.

## Typowe błędy
> [!blad] Zapominasz, że stała całkowania znika przy oznaczonej
> $\\int_a^b$ = różnica $F(b)-F(a)$ — stała $C$ się skraca. Dodajesz ją tylko do nieoznaczonej.

> [!blad] Mylisz całkę z $\\frac1x$ i $x^n$
> $\\int x^{-1}dx = \\ln|x|$, nie $\\frac{x^0}{0}$ (dzielenie przez 0!).

## Gdzie ci się przyda
> [!przyda]
> - [[Całka niewłaściwa]] — pole pod krzywą do $\\infty$.
> - Laplace:$\\mathcal{L}\\{f\\} = \\int_0^\\infty f e^{-st}dt$ — to całka oznaczona z granicą w $\\infty$, czyli niewłaściwa.

## Zadania
> [!zadanie] Zadanie 1
> $\\int_0^2 (x^2) dx$.
> > [!odpowiedz]- Odpowiedź
> > $\\frac{x^3}{3}\\big|_0^2 = \\frac83$.

> [!zadanie] Zadanie 2
> $\\int_1^3 \\frac1x dx$.
> > [!odpowiedz]- Odpowiedź
> > $\\ln 3 - \\ln 1 = \\ln 3$.

## Fiszki #fiszki/matematyka
Całka oznaczona $\\int_a^b f$= :: pole pod krzywą = $F(b)-F(a)$, $F'=f$ (odwrotność pochodnej)
$\\int x^n dx$::$\\frac{x^{n+1}}{n+1}+C$
$\\int e^{ax}dx$::$\\frac1a e^{ax}+C$
Po co jest całka oznaczona Laplace'owi?::to ona (z granicą w $\\infty$) definiuje transformatę: $\\mathcal{L}\\{f\\}=\\int_0^\\infty f e^{-st}dt$
