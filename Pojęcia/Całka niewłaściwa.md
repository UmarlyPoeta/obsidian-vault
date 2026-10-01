---
typ: pojęcie
status: uczę się
przedmioty: ["[[PK Matematyka 1]]", "[[PK Matematyka 2]]"]
wymaga: ["[[Całka oznaczona]]", "[[Granica (lim)]]"]
powiązane: ["[[Transformata Laplace'a]]", "[[Funkcja wykładnicza]]"]
tags: [pojęcie, matematyka]
utworzono: 2026-09-27
źródła: []
---
# Całka niewłaściwa

> [!intuicja] Po ludzku
> Zwykła [[Całka oznaczona]] liczy pole pod krzywą między dwiema liczbami. Całka niewłaściwa liczy pole, **kiedy jedna z granic sięga nieskończoności** albo **funkcja gdzieś "wybucha" do ∞**. Pole może być skończone mimo nieskończonego obszaru — to właśnie głębia tego pojęcia. Na niej stoi cała [[Transformata Laplace'a|transformata Laplace'a]]: $\\int_0^\\infty f(t)e^{-st}\\,dt$.

> [!metafora] Jak dla idioty
> Wyobraź sobie zbieranie wody z rynny, która nigdy się nie kończy. Możesz zebrać skończoną ilość ($\\frac1s$), mimo że rynna jest nieskończenie długa, bo na końcu robi się coraz ciaśniejsza i woda płynie coraz wolniej. Całka niewłaściwa to policzenie, ile wody zbierzesz, gdy dasz czas do nieskończonościople

## Definicja
> [!definicja] Dwa typy
> **1. Nieskończona granica całkowania:**
> $$\\int_a^{\\infty} f(x)\\,dx = \\lim_{b\\to\\infty}\\int_a^{b} f(x)\\,dx$$
> **2. Funkcja nieograniczona w punkcie** (np.$\\frac1x$ przy $x=0$):$
> $$\\int_0^{a} \\frac{1}{x}\\,dx = \\lim_{\\varepsilon\\to 0^+}\\int_\\varepsilon^{a} \\frac{1}{x}\\,dx$$

Mówimy, że całka **jest zbieżna**, gdy ta granica istnieje (skończona). W przeciwnym razie **rozbieżna** (wynik ∞ lub nie istnieje).



## Jak to działa — trzy wzorce do zapamiętania  
Najważniejsza intuicja: całka niewłaściwa może być zbieżna **tylko, gdy funkcja wystarczająco szybko maleje**. Klasyczne wzorce:



> [!wzor]- $\\int_1^\\infty \\frac{1}{x^p} dx$(do porównań)
> | $p$ | zbieżna? | wartość |
> |---|---:|---:|
> | $p> 1$ | ✅ tak | $\\dfrac{1}{p-1}$ |
> | $p \\le 1$ | ❌ nie | $\\infty$ |

> [!wzor]- $\\int_0^\\infty e^{-ax}dx$
> | $a > 0$ | ✅ tak | $\\dfrac{1}{a}$ |
> | $a \\le 0$ | ❌ nie | $\\infty$ |

Dlatego **$e^{-st}$ w całce Laplace'a gasi sygnał** — dzięki temu całka jest zbieżna dla $\\operatorname{Re}s > \\sigma_0$ (to tzw. obszar zbieżności, który w praktyce inżynierskiej prawie ignorujesz).



## Jak liczyć — krok po kroku
1. **Zamień** górną (lub dolną) granicę na symbol $b$(lub $\\varepsilon$).
2. **Policz** całkę oznaczoną $\\int^b$ jak zwykle (najpierw funkcja pierwotna, potem wartości).
3. **Weź granicę** $\\lim_{b\\to\\infty}(\ldots)$ — podstaw $\\infty$ i sprawdź, czy wynik jest skończony.




## Przykłady
  
> [!przyklad]- Przykład 1 — najprostszy (podstawa całki Laplace'a\
> $$\\int_0^\\infty e^{-st}dt = \\lim_{b\\to\\infty}\\left[-\\tfrac1s e^{-st}\\right]_0^b = \\lim_{b\\to\\infty}\\left(-\\tfrac1s e^{-sb} + \\tfrac1s\\right) = \\frac1s \\quad (\\operatorname{Re} s > 0$$
> Zauważ: wymaga $s> 0$(żeby $e^{-sb}\\to 0$),czyli $$\\mathcal{L}\\{1\\} = \\frac1s$$ — to pierwszy wiersz [[Transformata Laplace'a|tablicy transformat]]\. 

> [!przyklad]- Przykład 2 — harmonia geometryczna
> $$\\int_1^\\infty \\frac{1}{x^2}dx = \\lim_{b\\to\\infty}[-x^{-1}]_1^b = -0 + 1 =  ​​1$ — pole skończone $=1$!

> [!przyklad]- Przykład 3 — rozbieżna
> $$\\int_1^\\infty \\frac{1}{x}dx = \\lim_{b\\to\\infty}[\\ln x]_1^b = \[\\infty - 0\\] = \\infty$ — $\\ln$ rośnie za wolno, pole ucieka do nieskończoności. Mimo że $\\frac1x$ maleje! Wniosek: sama "malejąca" nie wystarczy — musi maleć szybciej niż $\\frac1x$.\)

## Typowe błędy
> [!blad] Sądzisz, że każde malejące $f(x)\\to0$ daje zbieżną całkę
> $\\frac1x$ maleje do 0, a całka jest rozbieżna. Liczy się **szybkość**: potrzeba $x^{-p}$ z $p>1$ albo coś na podobieństwo eksponenty.


> [!blad] Zakładasz, że pochodna "wygasza" wszystko
> $\\int_0^\\infty 1\\,dx = \\infty$ — sam "gasiący" wykładniczy czynnik robi robotę. Całka Laplace'a ma sens, bo $e^{-st}$ szybko maleje do 0, nie dlatego, że $f(t)$ maleje.



## Gdzie ci się przyda
> [!przyda]
> - Definicja [[Transformata Laplace'a|Laplace'a]] — to jest dokładnie ta całka, z $f(t)$ w środku.

## Zadania
> [!zadanie] Zadanie 1
> Oblicz $\\int_0^\\infty e^{-2t}dt$.
> > [!odpowiedz]- Odpowiedź
> > $\\big[-\\frac12 e^{-2t}\\big]_0^\\infty = 0 - (-\\frac12) = \\frac12$. Skoro zbieżna przy $a>0$.

> [!zadanie] Zadanie 2 — rozbieżna czy zbieżna?
> $\\int_1^\\infty \\frac{1}{\\sqrt{x}}dx$
> > [!odpowiedz]- Odpowiedź
> > $\\frac{1}{x^{1/2}}$, $p=\\tfrac12 \\le 1$ → **rozbieżna** → $\\infty$.

> [!zadanie] Zadanie 3 — na wagę Laplace'a
> Dlaczego $\\mathcal{L}\\{e^{5t}\\}$ nie istnieje na całej płaszczyźnie?Na czym się zbiega?
> > [!odpowiedz]- Odpowiedź
> > $f(t)=e^{5t}$ rośnie,gdzie $e^{-st}$ maleje. Policz: $\\int_0^\\infty e^{5t}e^{-st}dt = \\int e^{(5-s)t}dt$ — zbieżne, gdy $\\operatorname{Re}s >5$ (bo wtedy wykładnik ujemny. Obszar zbieżności to półpłaszczyzna $\\operatorname{Re}s >5$.

## Fiszki #fiszki/matematyka
Całka niewłaściwa — całka z nieskończoną granicą lub nieograniczoną funkcją liczona jako granica:$\\int_a^\\infty f = \\lim_{b\\to\\infty}\\int_a^b f$
$\\int_1^\\infty \\frac{1}{x^p} dx$ kiedy zbieżna?::$p>1$, wartość $\\frac1{p-1}$
$\\int_0^\\infty e^{-ax}dx$ kiedy zbieżna?::Dla $a>0$, wartość $\\frac1a$
Dlaczego całka Laplace'a ma sens mimo $f(t)$ rosnącej?::Bo $e^{-st}$ maleje szybciej dla $\\operatorname{Re}s$ dużej — obszar zbieżności
## Źródła
- A. Milian, *Zbiór zadań z matematyki* cz. 2 (PK) — rozdział o całce niewłaściwej