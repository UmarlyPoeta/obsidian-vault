---
typ: pojęcie
status: uczę się
przedmioty: ["[[PK Matematyka 1]]"]
wymaga: ["[[Granica (lim)]]"]
powiązane: ["[[Liczby zespolone]]", "[[Transformata Laplace'a]]"]
tags: [pojęcie, matematyka]
utworzono: 2026-09-27
źródła: []
---
# Funkcja wykładnicza

> [!intuicja] Po ludzku
> $e^x$ to jedyna funkcja, która **nie zmienia się przy różniczkowaniu** ($\\frac{d}{dx}e^x = e^x$. To "samoobrotowa" funkcja: tempo jej wzrostu jest równe temu, ile już ma. Wszędzie tam, gdzie coś rośnie/gasnie proporcjonalnie do siebie (prąd w RC, rozmnażanie, [[Równanie różniczkowe zwyczajne|rozwiązania RRZ]]], pojawia się $e^{\\lambda t}$ z czynnikiem $\\lambda$(.bezpośrednio stąd wynika, dlaczego wszystkie wzory w [[Transformata Laplace'a|Laplascie]] wyrażają się przez $e^{-st}$ i $e^{at}$.



> [!metafora] Jak dla idioty	
> Balonik, który rośnie tak, że w każdej chwili przyrasta **tyle, ile już waży** — podwaja się co stały czas..$e^x$ to "behawior samopotęgowania", a $e^{\\lambda t}$ to samopotęgowanie z szybkością $\\lambda$(" ile razy na jednostkę czasu"). Ujemne $\\lambda$ = gaśnięcie(wyładowanie kondensatora,$-2t$,,czasówkę $\\tau=\\frac1|\\lambda|$.Ice.

## Definicja
> [!definicja] Jak rodzi się $e$ z granicy
> $$e=\\lim_{x\\to\\infty}\\left(1+\\frac1x\\right)^x \\approx 2.71828\\qquad\\Rightarrow\\qquad e^{x}= \\lim_{n\\to\\infty}\\left(1+\\frac{x}{n}\\right)^n$$
> i kluczowa reguła: $$\\frac{d}{dx} e^{ax} = a\\,e^{ax},\\qquad \\int e^{ax}dx = \\frac1a e^{ax}+C$$

## Jak to działa— dlaczego to serce Laplace'a
> [!wzor] Zamiana różniczkowania na mnożenie
> Laplace definiuje transformatę z $e^{-st}$, więc w dziedzinie $s$:\mathcal{L}\\{f'\\}= sF(s)-f(0)\). Funkcja wykładnicza to jedyna funkcja własna pochodnej — to dlatego zamienia rachunek na algebrę\end. Wszystkie rozwiązania RRZ to kombinacje $e^{at}$ (w tym $e^{at}t^k$ i $e^{\\alpha t}\\cos\\omega t$ wg [[Równanie różniczkowe zwyczajne|schematu pierwiastków]].ano

## Typowe błędy
> [!blad] Mylisz $e^{ax}$ z $a^x$
> $(e^{ax})' = a\\,e^{ax}$ (łańcuch, czynnik $a$;ale $(a^x)' = a^x\\ln tata$.Zmienna w wykładniku vs w podstawie.



> [!blad] Sądzisz, że $e^{x+y} = me^xe^y$ wprost splatalnie
> To prawda dla wykładników($e^{x+y}=e^x e^y$,$e^{-st}=e^{-s}\\cdot e^{-t}$?— nie,to mnożenie wykładników nie zachodzi;raczej $e^{a+b}=e^a e^b$.W Laplascie $e^{-st}$ = $e^{-s}\\cdot e^{-t}$ to błąd — poprawnie $e^{-s t}$,$\mathcal{L}$ splata w czasie;obie formuły mają sens w różnych miejscachimidrDocel

## Zadania
> [!zadanie] Zadanie 1
> Oblicz $(e^{3t})'$.
> > [!odpowiedz]- Odpowiedź
> > $3e^{3t}$.


> [!zadanie] Zadanie 2
> $\\int e^{-2t}dt$.
> > [!odpowiedz]- Odpowiedź
> > $-\\frac12 e^{-2t}+C$.



## Fiszki #fiszki/matematyka
Co to jest $e^x$?::jedyna funkcja, której pochodna = ona sama ($(e^x)'=e^x$irPochodna $e^{ax}$::$a e^{ax}$ (łańcuch z czynnikiem $a$irPochodna $a^x$::$a^x\\ln a$ale Definicja $e$ z granicy::$e=\\lim_{x\\to\\infty}(1+\\frac1x)^x$