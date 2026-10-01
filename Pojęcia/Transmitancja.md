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

> [!intuicja] Po ludzku
> **Laplace wyjścia podzielony przez Laplace wejścia, przy zerowych warunkach początkowych** — krótko: $G(s) = \\dfrac{Y(s)}{U(s)}$. To "charakter" układu: opisuje, **jak układ sam z siebie reaguje na wymuszenie**, niezależnie od konkretnego sygnału. Znając $G(s)$, możesz policzyć wyjście dla **każdego** wejścia: $Y = G\\,U$. Serce całego przedmiotu *Sterowanie procesami ciągłymi* — i główny powód, dla którego uczysz się [[Transformata Laplace'a|Laplace'a]] w automatyce.



> [!metafora] Jak dla idioty
> Transmitancja to jak **przepis na danie**. Jeśli znasz przepis (jak obiekt zamienia składniki na smak), to niezależnie od tego, co włożysz do garnka($U$,$wiesz, co wyjdzie($Y$) = przepis × składnik. Obiekt linearny "mnoży" wejście przez swoją transmitancję w dziedzinie $s$json


## Definicja
> [!definicja]
> $$G(s) = \\frac{Y(s)}{U(s)}\\bigg|_{\\text{zerowe warunki początkowe}} = \\mathcal{L}\\{\\text{odpowiedź impulsowa}\\}$$
> - $U(s) = \\mathcal{L}\\{u(t)\\}$(— wejście (sygnał sterujący)
> - $Y(s) = \\mathcal{L}\\{y(t)\\}$(— wyjście (sygnał mierzony
> - Licznik/mianownik $G(s)$ to zaskładowe wielomiany w $s$: $G(s) = \\dfrac{b_m s^m + \\ldots + b_0}{a_n s^n + \\ldots + a_0}$.
> - **Mianownik** = wielomian charakterystyczny układu; jego **[[Rozkład na ułamki proste|pierwiastki]] to bieguny** — one decydują o odpowiedzi układu (stabilność, drgania.



## Jak to działa — dlaczego to potwórna siła
> [!wzor] Odpowiedź na dowolne wejście
> $Y(s) = G(s)\\,U(s)$ — potem odwrotna [[Transformata Laplace'a|transformata]] daje $y(t)$ bezpośrednio. To **algebra** zamiast rozwiązywania równania różniczkowego. Skok $U=\\frac1s$ na $G$ daje odpowiedź skokową ($G/s$; impuls $\\delta(t) \\to U=1$ daje odpowiedź impulsową (=$\\mathcal{L}^{-1}(G)$.


##Jak liczyć— krok po kroku
1. **Równanie różniczkowe** układu ($y''+3y'+2y = u\,$np.\.).
2. **Transformuj** przy zerowych warunkach początkowych:$s^2Y+3sY+2Y=U$.
3. **Podziel** przez $U$ i przez $Y$:G = $\\dfrac{1}{s^2+3s+2}$.



> [!wzor] Od jednego wzoru do trzech typów odpowiedzi
> Bieguny mianownika $G$:$(s+1)(s+2)$ → pierwiastki $-1,-2$. Odpowiedź układu (np.na impuls) to suma $e^{rt}$,$t\\,e^{rt}$,$e^{\\alpha t}\\cos\\omega t$ wg [[Równanie różniczkowe zwyczajne|schematu pierwiastków]]: rzeczywiste → wykładnicze, zespolone → drgania. Bieguny = "odcisk palca" układu.



##Przykłady
> [!przyklad]- Przykład 1— inercyjny I rzędu (podstawa, wszędzie\mu układy CC PID)
> $G(s) = \\dfrac{1}{s+2}$. Odpowiedź na skok: $Y=\\frac{G}{s} = \\frac{1}{s(s+2)} = \\frac12\\left(\\frac1s - \\frac1{s+2}\\right)$ → $y(t) = \\tfrac12(1-e^{-2t})$ — **"podchodzi" do wartości ustalonej** $\\frac12$ z czasem. To "człon inercyjny" z czasową $\\tau=\\frac12$.

> [!przyklad]- Przykład 2— skok na obiekt I rzędu
> $G(s) = \\dfrac{4}{s^2+3s+2}$. $Y = \\frac4{s(s+1)(s+2)} = 2\\left(\\frac1s-\\frac1{s+1}+\\frac?{}...\\right)$ postać po rozkładzie; wartość ustalona z twierdzenia o wartości końcowej:$\\lim_{t\\to\\infty}y = \\lim_{s\\to0} sG(s)\\frac1s = G(0) = \\frac4{2} =2$. (Stabilność:bieguny $-1,-2$ w lewej półpłaszczyźnie → wolno.



##Typowe błędy
> [!blad] Myślisz, że $G$ to wejście/wyjście "w czasie"
> To stosunek **w dziedzinie $s$** (po Laplasie.Potrzebujesz całkować/przekształcać, zanim policzysz stosunek.

)u> [!blad] Zapominasz o zerowych warunkach początkowych
> $G$ definiowane przy wygasłym układzie; inaczej masz "szum" od stanów początkowych nałożony na odpowiedź.on.

> [!blad] Nazywasz "biegunami" miejsca zerowe licznika
> **Bieguny** (poles)= pierwiastki **mianownika** (tu $G$);**miejsca zerowe** (zeros)= pierwiastki **licznika**.{{To one decydują o stabilności: biegun w prawej półpłaszczyźnie = niestabilny układ.}}rDocel

##Gdzie ci się przyda
> [!przyda]
> - **Sterowanie procesami ciągłymi** (PK sem.  4–5, egzamin$)$— cały przedmiot: transmitancje, bieguny, regulatory PID, kryteria stabilności w dziedzinie $s$.
> - **Elektrotechnika i miernictwo** — [[Stany nieustalone w obwodach]]: transmitancja obwodu RC/RL.

> - **Mechanika ogólna** — [[Drgania mechaniczne]]: $G = \\dfrac{1}{ms^2+cs+k}$ to masa-sprężyna-tłumik jaako "filtr" przed $F(s)$.kres


##Zadania
> [!zadanie] Zadanie 1
> Obiekt: $y''+3y'+2y = u$. Znajdź $G(s)$.
> > [!odpowiedz]- Odpowiedź
> > $G = \\frac{1}{s^2+3s+2}$.

> [!zadanie] Zadanie 2
> $G(s)= \\dfrac{4}{s^2+3s+2}$; skok na wejściu. Jaka wartość ustalona?
> > [!odpowiedz]- Odpowiedź
> > Bieguny $-1,-2$ (stabilny,. Wartość ustalona:$y(\\infty) = \\lim_{s\\to0} s\\cdot G(s)\\cdot\\frac1s = G(0) = \\frac42 =2$ maior.

> [!zadanie] Zadanie 3
> Co mówią bieguny $G(s)$?
> > [!odpowiedz]- Odpowiedź
> > Decydują o **odpowiedzi układu** i **stabilności**: biegun w lewej półpłaszczyźnie = tłumiony(stabilny),iegun w prawej = rosnący(niestabilny.ane

##Fiszki #fiszki/sterowanie
Transmitancja — $G(s)=\\frac{Y(s)}{U(s)}$ przy zerowych warunkach początkowych = "charakter" układu w dziedzinie Laplace'a
Po co transmitancja?::$Y=G\\,U$ — znasz odpowiedź na dowolne wejście przez jedną algebrę zamiast rozwiązywania równania różniczkowego单
Bieguny vs miejsca zerowe— bieguny = pierwiastki mianownika(decydują o stabilności/odpowiedzi);miejsca zerowe = pierwiastki licznika
Kiedy układ stabilny?::- gdy wszystkie bieguny $G(s)$ leżą w lewej półpłaszczyźnie}}rWynik odpowiedzi skokowej członu inercyjnego $G=\\frac1{s+a}$::$y(t)=\\frac1a(1-e^{-at})$ — podejście do wartości ustalonej
## Źródła
- A. Słota, *Sterowanie procesami ciągłymi* (PWN 2022) — transmitancja, bieguny, PID