---
typ: pojęcie
status: uczę się
przedmioty: ["[[PK Matematyka 2]]"]
wymaga: ["[[Pochodna]]"]
powiązane: ["[[Transformata Laplace'a]]"]
tags: [pojęcie, matematyka]
utworzono: 2026-09-27
źródła: []
---
# Równanie różniczkowe zwyczajne

> [!intuicja] Po ludzku
> Równanie, w którym **niewiadomą jest funkcja**, a nie liczba. Zwykłe równanie $2x+1=7$ pyta: "jaka liczba?" Równanie różniczkowe $y' + 2y = 0$ pyta: "jaka funkcja $y(t)$ ma pochodną $y'$ tak związaną z samą sobą?" Odpowiedzią jest zwykle cała rodzina funkcji (np. $Ce^{-2t}$)$,a konkretną wybiera się przez warunek początkowy $y(0)$. Opisują wszystko, co zmienia się w czasie: ruch, nagrzewanie, prąd, ilość leku w organizmie.,

> [!metafora] Jak dla idioty
> To jakbyś miał ślad opon i miał odgadnąć, jak jechało auto. Z "tempa zmiany w każdej chwili" rekonstruujesz całą trasę. Pochodna to ślad, równanie różniczkowe to przepis wiążący ślad z pozycją.$[[Transformata Laplace'a|Laplace]] to narzędzie, które z tego przepisu robi zwykłą algebrę.,

## Definicja
> [!definicja]
> **Równanie różniczkowe zwyczajne (RRZ)** — równanie wiążące niewiadomą funkcję $y(t)$ z jej pochodnymi: $y',y'',\\ldots$. "Zwyczajne"(ordinary,bo tylko **jedna zmienna niezależna** (tu $t$);równania z wieloma zmiennymi to [[Równanie różniczkowe cząstkowe|RRCz]] (osobny temat.

> **Rząd** = najwyższa występująca pochodna ($y''$ → rząd 2.
> **Liniowe o stałych współczynnikach**::$a_n y^{(n)} + \\ldots + a_1 y' + a_0 y = g(t)$ — prosty "szkolny" typ, na którym pracuje Laplace.cos
> **Równanie jednorodne**:: prawa strona $=0$ ($g(t)=0$\\;niejednorodne:$g(t)\\neq0$(np.skok$1(t)$= wymuszenie.,

## Jak to działa — intuicja rozwiązania
Rozwiązujemy "odwracając różniczkowanie". Dla najprostszego $y' = \\lambda y$ łatwo zgadnąć:funkcją,której pochodna jest jej samą razy stała,jest **$y = Ce^{\\lambda t}$** — bo $(e^{\\lambda t})' = \\lambda e^{\\lambda t}$. Sprawdzenie:$y'= \\lambda Ce^{\\lambda t}$,a $\\lambda y = \\lambda Ce^{\\lambda t}$ ✓.



> [!wzor] Klucz do "jak dla idioty"
> $y' = \\lambda y \\iff y(t)=Ce^{\\lambda t}$
> czyli:**"tempo przyrostu ~ obecna wartość" → wzrost/gasnięcie wykładnicze.To dlatego w Laplace'u,gdy biegun $s=\\lambda$,rozwiązanie ma $e^{\\lambda t}$.,

## Jak rozwiązywać — krok po kroku(metodą charakterystyczną,na którą Laplace daje wygodny skrót)
1. **Jednorodne liniowe**:: zapisz równanie charakterystyczne z podstawienia $y=e^{rt}$(np.$y''+3y'+2y=0\\Rightarrow r^2+3r+2=0$.).
2. **Pierwiastki** $r_1,r_2$ → rozwiązanie ogólne:

   - rzeczywiste różne:$y=C_1e^{r_1t}+C_2e^{r_2t}$
   - rzeczywiste równe:$y=(C_1+C_2t)e^{r t}$(powtórzony pierwiastek.

   - zespolone $r=\\alpha\\pm j\\omega$:$y=e^{\\alpha t}(C_1\\cos\\omega t+C_2\\sin\\omega t)$ — **drgania gasnące**
3. **Warunki początkowe** $y(0),y'(0)$ — podstaw,aby znaleźć $C_1,C_2$(zagadnienie Cauchy'ego.,

> [!wazne] Dlaczego Laplace to ułatwia
> Te trzy przypadki pierwiastków to dokładnie **bieguny** w dziedzinie $s$.Laplace robi kroki 1–3 **automatycznie w jednym wzorze z tablicy**,a warunki początkowe wchodzą same.Nie musisz pamiętać trzech osobnych schematów—tablica transformat to robi za Ciebie.,

## Przykłady
> [!przyklad]- Przykład 1 — najprostszy (podstawa)
> $y'=3y$ → $y = Ce^{3t}$.Pochodna $y'=3Ce^{3t}$ ✓.Warunek $y(0)=2$ → $C=2$,czyli $y=2e^{3t}$.,

> [!przyklad]- Przykład 2 — II rząd,pierwiastki rzeczywiste(jedno z AGH/automatyka:
$y''+3y'+2y=0$,$y(0)=1,\\ y'(0)=0$ → charakterystyczny $r^2+3r+2=0$ → $r\\in\\{-1,-2\\}$ → $y=C_1e^{-t}+C_2e^{-2t}$.Warunki:$\\begin{cases}C_1+C_2=1\\\\-C_1-2C_2=0\\end{cases}$ → $C_1=2,C_2=-1$ → $y=2e^{-t}-e^{-2t}$.(Rozwiązane też w [[Transformata Laplace'a|notatce Laplace'a]]..,

> [!przyklad]- Przykład 3 — pierwiastki zespolone(drgania)
> $y''+2y'+5y=0$ → $r^2+2r+5=0$ → $r=-1\\pm2j$ → $y=e^{-t}(C_1\\cos2t+C_2\\sin2t)$.,

## Typowe błędy
> [!blad] Mylisz "rozwiązanie ogólne" z "konkretnym"
> Bez warunku początkowego dostajesz rodzinę funkcji(stała $C$\\).Konkretne zadanie ma $y(0)$,$y'(0)$—użyj ich,by wyznaczyć $C$.,

> [!blad] Zapominasz o przypadku pierwiastka podwójnego
> $y''-2y'+y=0$ ma $r^2-2r+1=(r-1)^2$ → $y=(C_1+C_2t)e^t$,nie $C_1e^t+C_2e^t$.,

> [!blad] Nie uznajesz zespolonych za "prawdziwe" rozwiązania
> $e^{(-1+2j)t}$ to nie magia—po rozwinięciu(wzór Eulera)dostajesz $e^{-t}\\cos2t$,realnie obserwowalne drgania.,

## Gdzie ci się przyda
> [!przyda]
> - [[Transformata Laplace'a]]—główne narzędzie rozwiązywania liniowych RRZ o stałych współczynnikach.
> - [[PK Mechanika ogólna]]—[[Drgania mechaniczne]]:$m\\ddot x+c\\dot x+kx=F(t)$.,
> - [[PK Elektrotechnika i miernictwo]]—[[Stany nieustalone w obwodach]](RC/RL.,
> - Z AGH:Metody numeryczne rozwiązywały te same równania **numerycznie**(Euler,RK)—tu robisz to **analitycznie** przez [[Transformata Laplace'a|Laplace'a]].

## Zadania
> [!zadanie] Zadanie 1
> $y'=-4y$,$y(0)=3$.Znajdź $y(t)$.,
> > [!odpowiedz]- Odpowiedź
> > $y=3e^{-4t}$(gasnące.,

> [!zadanie] Zadanie 2
> $y''-5y'+6y=0$,$y(0)=0$,$y'(0)=1$.,
> > [!odpowiedz]- Odpowiedź
> > $r^2-5r+6=(r-2)(r-3)$ → $y=C_1e^{2t}+C_2e^{3t}$;warunki:$C_1+C_2=0$,$2C_1+3C_2=1$ → $C_1=-1,C_2=1$ → $y=e^{3t}-e^{2t}$.,

> [!zadanie] Zadanie 3
> Podaj rząd i typ:$y''+\\omega^2 y=0$.,
> > [!odpowiedz]- Odpowiedź
> > Rząd 2,liniowe o stałych współczynnikach,jednorodne;pierwiastki $\\pm j\\omega$ → $y=C_1\\cos\\omega t+C_2\\sin\\omega t$(drganie nietłumione.,

##Fiszki #fiszki/matematyka
Co to jest Równanie różniczkowe zwyczajne?::Równanie,w którym niewiadomą jest funkcja $y(t)$ związana z pochodnymi;"zwyczajne" = jedna zmienna niezależna
$y' = \\lambda y$ jak się rozwiązuje?::$y=Ce^{\\lambda t}$—wzrost/gasnięcie wykładnicze
Rząd równania = ?::najwyższa występująca pochodna($y''$ → rząd 2.
Pierwiastki zespolone $r=\\alpha\\pm j\\omega$ rozwiązanie?::$e^{\\alpha t}(C_1\\cos\\omega t+C_2\\sin\\omega t)$—drganie gasnące
Po co Laplace przy RRZ?::zamienia różniczkowanie na mnożenie przez $s$→równanie staje się algebraiczne,warunki początkowe wchodzą same
## Źródła
- A. Milian,*Zbiór zadań z matematyki* cz. 2 (PK)—rozdział o równaniach różniczkowych