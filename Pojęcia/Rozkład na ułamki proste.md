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

> [!intuicja] Po ludzku
> Sztuczka: rozbić ładny ułamek $\\frac{s+3}{(s+1)(s+2}$ na sumę **prostych** ułamków $\\frac{A}{s+1} + \\frac{B}{s+2}$, gdzie $A,B$ to liczby. Po co? Bo w odwrotnej [[Transformata Laplace'a|transformacie Laplace'a]] **każdy pojedynczy ułamek $\\frac{A}{s-a}$ to jedna pozycja w tablicy** ($\\mathcal{L}^{-1}\\{\\frac{1}{s-a}\\} = e^{at}$\). To jak rozbicie tortu na kawałki przed zjedzeniem — każdego zjadasz osobnoto



> [!metafora] Jak dla idioty	
> Masz ułamek $\\frac{s+3}{(s+1)(s+2)}$. Mianownik to "iloczyn dwóch mianowników": $(s+1)$ i $(s+2)$. Chcesz go napisać jako **sumę dwóch ułamków, każdy z jednym z tych mianowników**, z nieznanymi licznikami $A,B$apdokładnie jak rozkład wektorów na składowej. Potem wyznaczasz $A,B$ metodą "zakrywania": podstawiasz miejsce, gdzie dany mianownik się zeruje — i reszta "sama się liczy".



## Definicja
> [!definicja] Cel rozkładu
> Dany iloraz wielomianów $\\frac{P(s)}{Q(s)}$ (ułamek wymierny, stopnia $\\deg P < \\deg Q$) rozkładamy na sumę **ułamków prostych**, to jest ułamków postaci:
> - $\\dfrac{A}{(s-a)^k}$ (biegun rzeczywisty; $k = 1,2,\\ldots$)
> - $\\dfrac{As+B}{(s^2+ps+q)^k}$ (biegun zespolony sprzężony; $p^2<4q$.
>
> "Prosty" bo każdy ma **tylko jedną pozycję w tablicy Laplace'a**. To fundament transformaty odwrotnej.



## Jak to działa — metody wyznaczania $A,B$
> [!wzor] Metoda 1 — "zakrywanie" (najszybsza, dla pojedyńczych pierwiastków)

> Dla $\\dfrac{P(s)}{(s-r_1)(s-r_2)\\ldots}$, współczynnik przy $\\frac{1}{s-r_i}$ to:
> $$A_i = \\left.\\frac{P(s)}{\\prod_{k\\neq i}(s-r_k)}\\right|_{s=r_i} = \\frac{P(r_i)}{\\prod_{k\\neq i}(r_i-r_k)}$$
> Czyli **podstaw $s=r_i$ do wszystkiego oprócz czynnika, przy którym jesteś** (i "zakryj" ten czynnik dłonią. 

> [!wzor] Metoda 2 — porównanie wielomianów (niezawodna, na wszelki wypadek)
> Pomnóż obie strony przez wspólny mianownik, rozwiń i porównaj współczynniki przy kolejnych potęgach $s$. Dostaniesz liniowy układ równań na $A,B$.

## Jak liczyć— krok po kroku
1. **Sprawdź stopień**: licznik niższy niż mianownik(np. $s+3$ vs $(s+1)(s+2)$). Jeśli wyższy — najpierw podziel (dzielenie wielomianów.

2. **Rozłóż mianownik** na czynniki  (pierwiastki<!--- to są bieguny--->.
3. **Zapisz ogólną postać** $\\frac{A}{s-r_1} + \\frac{B}{s-r_2} + \\ldots$.
4. **Wyznacz $A,B$** "zakrywaniem"( lub porównaniem.
5. **Odczytaj $e^{rt}$ z tablicy** dla każdego ułamka — sumujesz i masz $y(t)$.



## Przykłady
> [!przyklad]- Przykład 1— najprostszy (podstawa)
> $$\\frac{s+3}{(s+1)(s+2)} = \\frac{A}{s+1} + \\frac{B}{s+2}$$
> $A$ (zakryj $(s+1)$, podstaw $s=-1$):$A = \\frac{-1+3}{-1+2} = \\frac{2}{1} =2$
> $B$ (zakryj $(s+2)$, podstaw $s=-2$):$B = \\frac{-2+3}{-2+1} = \\frac{1}{-1} = -1$
> Czyli $$\\frac{s+3}{(s+1)(s+2)} = \\frac{2}{s+1} - \\frac{1}{s+2}$$
> Odwrotnie: $$y(t) = 2e^{-t} - e^{-2t}$$ — dokładnie wynik z [[Transformata Laplace'a|notatki Laplace'a]].

> [!przyklad]- Przykład 2 — powtórzony pierwiastek

> $$\\frac{1}{(s+1)^2} \\Rightarrow \\frac{A}{(s+1)} + \\frac{B}{(s+1)^2}$$ (nie dwa ułamki z tym samym mianownikiem. Zakrywanie wartość: przy $\\frac{B}{(s+1)^2}$: $B =  ​​1$; dla $A$ najłatwiej porównaniem albo różniczkowaniem; wynik:$\\frac{A}{s+1} + \\frac{1}{(s+1)^2}$ z $A =-1$ → $y(t)=(-1+t)e^{-t}$(bo $\\mathcal{L}^{-1}\\{\\frac{1}{(s+a)^2}\\}= t\\,e^{-at}$.



## Typowe błędy
> [!blad] Licznik ma wyższy stopień niż mianownik
> $\\frac{s^3}{s+1}$ — najpierw podziel, zanim rozkładasz (wynik: wielomian + właściwy ułamek).

> [!blad] Pomijasz powtórzone pierwiastki
> Mianownik $(s+1)^2$ wymaga **dwóch** ułamków: $\\frac{A}{s+1}+\\frac{B}{(s+1)^2}$. Sam jeden nie odda kształtu.



> [!blad] Stosujesz "zakrywanie" do pierwiastków powtórzonych
> Dla $\\frac{1}{(s+1)^2}$ zakrywanie podstawia $s=-1$ i nie daje prostej odpowiedzi. Do powtórzonych użyj porównania albo rozbij $\\frac{(s+a)}{(s+a)^2}$ najpierw.



## Gdzie ci się przyda
> [!przyda]
> - **Odwrotna [[Transformata Laplace'a]]** — bez rozkładu nie odczytasz wyniku z tablicy. To krok 3 w algorytmie rozwiązania.



## Zadania
> [!zadanie] Zadanie 1
> Rozłóż $\\frac{s}{(s+1)(s-2)}$.
> > [!odpowiedz]- Odpowiedź
> > $A=\\frac{-1}{-3} =\\frac13$, $B=\\frac{2}{3}$ → $\\frac{1}{3(s+1)} + \\frac{2}{3(s-2)}$ → $y(t)=\\frac13 e^{-t} + \\frac23 e^{2t}$.

> [!zadanie] Zadanie 2
> Rozłóż $\\frac{2s+3}{s^2+3s+2}$.
> > [!odpowiedz]- Odpowiedź
> > $s^2+3s+2=(s+1)(s+2)$; $A=\\frac{-2+3}{1}=1$, $B=\\frac{-4+3}{-1}=1$ → $\\frac{1}{s+1}+\\frac{1}{s+2}$ → $e^{-t}+e^{-2t}$.

> [!zadanie] Zadanie 3 — na start transformaty odwrotnej

> Znajdź $\\mathcal{L}^{-1}\\{\\frac{4}{s(s+2)}\\}$.(Wskazówka: rozłóż najpierw.
> > [!odpowiedz]- Odpowiedź
> > $\\frac{4}{(s)(s+2)} = \\frac{A}{s}+\\frac{B}{s+2}$: $A = \\frac{4}{2}=2$, $B = \\frac{4}{-2}=-2$ → $y(t)=2-2e^{-2t}$.("skok do wartości ustalonej 2","człon inercyjny".}



##Fiszki #fiszki/matematyka
Rozkład na ułamki proste — rozbijanie $\\frac{P(s)}{Q(s)}$ na sumę pojedyńczych ułamków $\\frac{A}{s-a}$,$\\frac{As+B}{s^2+ps+q}$
Metoda "zakrywania" — współczynnik przy $\\frac1{s-r_i}$ = wartość $\\frac{P(s)}{\\prod_{k\\neq i}(s-r_k)}$ w $s=r_i$
Powtórzony pierwiastek $(s+a)^2$ wymaga?::dwóch ułamków $\\frac{A}{(s+a)}+\\frac{B}{(s+a)^2}$
Kiedy najpierw dzielimy wielomiany?::gdy licznik ma stopień ≥ mianownika
## Źródła
- A. Milian, *Zbiór zadań z matematyki* cz. 2 (PK) — rozdział o funkcjach wymiernych i ułamkach prostych