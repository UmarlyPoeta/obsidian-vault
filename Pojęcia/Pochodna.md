---
typ: pojęcie
status: uczę się
przedmioty: ["[[PK Matematyka 1]]"]
wymaga: ["[[Granica (lim)]]"]
powiązane: ["[[Całka oznaczona]]", "[[Równanie różniczkowe zwyczajne]]"]
tags: [pojęcie, matematyka]
utworzono: 2026-09-27
źródła: []
---
# Pochodna

> [!intuicja] Po ludzku
> Pochodna odpowiada na pytanie: **jak szybko coś się zmienia w danym punkcie?** To nachylenie stycznej do wykresu — "tempo" funkcji. $f'(x)$ z jednej liczby $f(x)$ robi inną liczbę: ile $f$ "rośnie na jednostkę" przy $x$. Bez tego ani rusz w [[Całka oznaczona|całkach]] (całka to odwrotność) ani w [[Równanie różniczkowe zwyczajne|równaniach różniczkowych]] (to równania o pochodnych).

> [!metafora] Jak dla idioty
> Prędkościomierz w aucie to pochodna przebiegu (położenia po czasie). Spojrzysz na wskazówkę — i wiesz, jak szybko zmienia się pozycja w tej chwili, mimo że sam przebieg to sinusoida podróży. Pochodna to "prędkościomierz funkcji".

## Definicja
> [!definicja] Nachylenie stycznej
> $$f'(x) = \\lim_{h\\to 0}\\frac{f(x+h)-f(x)}{h}$$
> Nachylenie siecznej między $(x, f(x))$ i $(x+h, f(x+h))$, gdy punkty zlewają się ($h\\to0$).

## Jak to działa — reguły, których naprawdę używasz
> [!wzor] Najczęstsze pochodne i reguły
> | funkcja | pochodna |
> |---|---|
> | $x^n$ | $n x^{n-1}$ |
> | $e^x$ | $e^x$ |
> | $\\sin x,\\ \\cos x$ | $\\cos x,\\ -\\sin x$ |
> | $af+bg$ (liniowość) | $af' + bg'$ |
> | $f\\cdot g$ (iloczyn) | $f'g + fg'$ |
> | $f(g(x))$ (łańcuchowa) | $f'(g(x))\\cdot g'(x)$ |

Kluczowe dla dalej: pochodna $e^{at}$ to $a e^{at}$ — bo $e^{at} = e^{(at)}$, a łańcuchowa daje czynnik $a$. To dlatego w [[Równanie różniczkowe zwyczajne|RRZ]] $y'=\\lambda y$ "zgadujemy" $Ce^{\\lambda t}$.

## Jak to robić — przykład
> [!przyklad] $f(t) = t^3 - 2t + e^{4t}$
> $f'(t) = 3t^2 - 2 + 4e^{4t}$ (liniowość + reguły $t^n$ i $e^{at}$).

## Typowe błędy
> [!blad] Pochodna iloczynu ≠ iloczyn pochodnych
> $(fg)' = f'g + fg'$, nie $f'g'$. Test: $(x\\cdot x)' = 2x$, a $1\\cdot 1 =1$ — sam byś nie wyszedł.

> [!blad] Mylisz pochodną $x^n$ i $a^x$
> $x^n \\to n x^{n-1}$ (zmienna w podstawie), ale $a^x \\to a^x\\ln a$ (zmienna w wykładniku).

## Gdzie ci się przyda
> [!przyda]
> - Fundament [[Całka oznaczona|całek]] (całkowanie to odwrotność różniczkowania).
> - [[Równanie różniczkowe zwyczajne]] — "równania o pochodnych".
> - [[Transformata Laplace'a|Laplace]]: $\\mathcal{L}\\{f'\\} = sF(s)-f(0)$ — pochodna w czasie staje się mnożeniem przez $s$ w dziedzinie transformacji.

## Zadania
> [!zadanie] Zadanie 1
> $f(x) = 5x^4$. Oblicz $f'(x)$.
> > [!odpowiedz]- Odpowiedź
> > $20x^3$.

> [!zadanie] Zadanie 2
> $g(t) = e^{-2t}$. Oblicz $g'(t)$.
> > [!odpowiedz]- Odpowiedź
> > $-2e^{-2t}$.

> [!zadanie] Zadanie 3
> $h(x) = x\\sin x$. Oblicz $h'(x)$.
> > [!odpowiedz]- Odpowiedź
> > $\\sin x + x\\cos x$ (reguła iloczynu).

## Fiszki #fiszki/matematyka
Definicja pochodnej::$f'(x)=\\lim_{h\\to0}\\frac{f(x+h)-f(x)}{h}$ — nachylenie stycznej
Pochodna $x^n$::$nx^{n-1}$
Pochodna $e^{at}$::$a e^{at}$
Reguła iloczynu::$(fg)'=f'g+fg'$
Reguła łańcuchowa::$(f(g))'=f'(g)\\cdot g'$
