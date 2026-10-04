---
typ: pojęcie
status: uczę się
przedmioty: ["[[PK Matematyka 1]]"]
wymaga: []
powiązane: ["[[Pochodna]]", "[[Całka niewłaściwa]]"]
tags: [pojęcie, matematyka]
utworzono: 2026-09-27
źródła: ["Gewert, Skoczylas — Analiza matematyczna 1", "Krysicki, Włodarski — Analiza matematyczna w zadaniach cz. I", "Milian — Zbiór zadań z matematyki cz. 1 (Wyd. PK)"]
---
# Granica (lim)

> [!skrot] W 30 sekund
> - **Co to:** wartość, do której zbliża się $f(x)$, gdy $x$ zbliża się do $a$ (lub do $\infty$).
> - **Klucz:** najpierw podstaw; jeśli wyjdzie symbol nieoznaczony ($\frac00$, $\frac\infty\infty$, $0\cdot\infty$…), przekształć: skróć, podziel przez najwyższą potęgę, użyj granicy specjalnej albo reguły de l'Hospitala.
> - **Kiedy:** definicja [[Pochodna|pochodnej]], [[Całka niewłaściwa|całki niewłaściwej]], a przez nią [[Transformata Laplace'a|transformaty Laplace'a]].

> [!intuicja] Po ludzku
> Granica mówi, co funkcja „obiecuje” w pobliżu punktu, a nie co się dzieje w samym punkcie — funkcja może tam nawet nie być określona. Liczy się tylko zachowanie coraz bliżej $a$. Analogia: patrzysz na pociąg dojeżdżający do zerwanego mostu — nie dojedzie, ale dobrze widać, w które miejsce celuje.

## Definicja i wzory
> [!definicja] Granica funkcji (Cauchy)
> $$\lim_{x\to a} f(x) = L \iff \forall \varepsilon>0\ \exists \delta>0:\ 0<|x-a|<\delta \Rightarrow |f(x)-L|<\varepsilon$$
> Równoważnie (Heine): dla każdego ciągu $x_n\to a$, $x_n\neq a$, mamy $f(x_n)\to L$. Granica istnieje, gdy granice jednostronne są równe: $\lim_{x\to a^-}f=\lim_{x\to a^+}f$.

> [!wzor] Granice specjalne
> $$\lim_{x\to0}\frac{\sin x}{x}=1,\qquad \lim_{x\to\pm\infty}\left(1+\frac1x\right)^x=e,\qquad \lim_{x\to0}\frac{e^x-1}{x}=1,\qquad \lim_{x\to0}\frac{\ln(1+x)}{x}=1$$
> Do tego: $\lim_{x\to\infty}\frac{x^n}{e^{x}}=0$ — wykładnicza rośnie szybciej niż każdy wielomian.

> [!wzor] Reguła de l'Hospitala
> Jeśli $\frac{f}{g}$ daje $\frac00$ lub $\frac\infty\infty$ i istnieje $\lim\frac{f'}{g'}$, to $\lim\frac{f}{g}=\lim\frac{f'}{g'}$. Różniczkujesz licznik i mianownik **osobno**, nie jak iloraz.

## Jak to zrobić — krok po kroku
1. Podstaw $x=a$. Jeśli wychodzi zwykła liczba, a funkcja jest ciągła — to jest wynik.
2. Jeśli wychodzi $\frac{c}{0}$ z $c\neq0$ — granica jest nieskończona albo nie istnieje (sprawdź znaki z obu stron).
3. Symbol nieoznaczony: rozłóż na czynniki i skróć, przy $x\to\infty$ podziel przez najwyższą potęgę, sprowadź do granicy specjalnej.
4. Jeśli to nie pomaga — de l'Hospital (tylko dla $\frac00$ i $\frac\infty\infty$; inne symbole najpierw przekształć do ilorazu).

## Przykład
Oblicz $\lim_{x\to2}\frac{x^2-4}{x^2-3x+2}$.

1. Podstawienie: $\frac{4-4}{4-6+2}=\frac00$ — nieoznaczone.
2. Rozkład: $x^2-4=(x-2)(x+2)$, $x^2-3x+2=(x-2)(x-1)$.
3. Skracamy $(x-2)$ (wolno, bo $x\neq2$): $\frac{x+2}{x-1}$.
4. Teraz podstawiamy: $\frac{2+2}{2-1}=4$.

Wynik: $4$.

> [!przyklad]- Przykład 2: $\lim_{x\to\infty}\frac{3x^2+1}{2x^2-x}$
> Dzielimy licznik i mianownik przez $x^2$: $\frac{3+\frac1{x^2}}{2-\frac1x}\to\frac{3}{2}$.

> [!przyklad]- Przykład 3 (Laplace): $\lim_{t\to\infty} t\,e^{-st}$ dla $s>0$
> To $\infty\cdot0$, więc piszemy $\frac{t}{e^{st}}$ (typ $\frac\infty\infty$). De l'Hospital: $\frac{1}{s\,e^{st}}\to0$. Dzięki temu przy liczeniu $\mathcal{L}\{t\}$ przez części wyraz brzegowy znika.

## Typowe błędy
> [!blad] „$\frac00$ to 0” albo „$\frac00$ to 1”
> To symbol nieoznaczony, a nie wynik. $\lim_{x\to0}\frac{\sin x}{x}=1$, $\lim_{x\to0}\frac{x^2}{x}=0$, $\lim_{x\to0}\frac{x}{x^2}$ nie istnieje — wszystkie trzy to „$\frac00$”.

> [!blad] De l'Hospital jako pochodna ilorazu
> Liczysz $\frac{f'}{g'}$, a nie $\left(\frac fg\right)'$. I tylko wtedy, gdy naprawdę jest $\frac00$ lub $\frac\infty\infty$.

> [!blad] Ignorowanie znaku parametru
> $\lim_{b\to\infty}e^{-sb}=0$ **tylko** dla $s>0$ (ogólnie $\operatorname{Re}s>0$). Dla $s<0$ wyrażenie ucieka do $\infty$ — stąd warunki zbieżności w tablicach Laplace'a.

## Na zajęcia i egzamin
> [!zajecia]
> - **Ćwiczenia:** liczyć granice ilorazów wielomianów ($x\to a$ i $x\to\infty$), granice z $\sin$ i $e$, de l'Hospital, granice jednostronne.
> - **Egzamin:** zadanie „oblicz granicę” w części zadaniowej; w teorii — definicja granicy i ciągłości.
> - Łączy się z: [[Pochodna]] (iloraz różnicowy, $h\to0$), [[Całka niewłaściwa]] ($b\to\infty$), [[Transformata Laplace'a]] (zbieżność $\int_0^\infty f(t)e^{-st}dt$).

## Zadania
> [!zadanie] Zadanie 1
> $\lim_{x\to3}\frac{x^2-9}{x-3}$
> > [!odpowiedz]- Odpowiedź
> > $\frac{(x-3)(x+3)}{x-3}=x+3\to6$.

> [!zadanie] Zadanie 2
> $\lim_{x\to\infty}\frac{2x^3-x}{5x^3+4}$
> > [!odpowiedz]- Odpowiedź
> > Dzielimy przez $x^3$: $\frac{2-\frac1{x^2}}{5+\frac4{x^3}}\to\frac25$.

> [!zadanie] Zadanie 3
> $\lim_{x\to0}\frac{1-\cos x}{x^2}$
> > [!odpowiedz]- Odpowiedź
> > De l'Hospital dwa razy: $\frac{\sin x}{2x}\to\frac12$. Wynik: $\frac12$.

> [!zadanie] Zadanie 4
> $\lim_{x\to0}\frac{e^{2x}-1}{x}$
> > [!odpowiedz]- Odpowiedź
> > $2\cdot\frac{e^{2x}-1}{2x}\to2\cdot1=2$.

> [!zadanie] Zadanie 5
> $\lim_{b\to\infty}\frac{1-e^{-sb}}{s}$ dla $s>0$
> > [!odpowiedz]- Odpowiedź
> > $e^{-sb}\to0$, więc wynik to $\frac1s$ — to właśnie $\mathcal{L}\{1\}$.

## Fiszki #fiszki/matematyka
Co znaczy $\lim_{x\to a}f(x)=L$?::Gdy $x$ zbliża się do $a$ (bez $x=a$), $f(x)$ zbliża się dowolnie blisko do $L$.
Kiedy granica w punkcie istnieje?::Gdy obie granice jednostronne istnieją i są równe.
$\lim_{x\to0}\frac{\sin x}{x}$?::$1$
$\lim_{x\to\infty}\left(1+\frac1x\right)^x$?::$e$
$\lim_{x\to0}\frac{e^x-1}{x}$?::$1$
<!--SR:!2026-10-06,1,230-->
Wymień symbole nieoznaczone.::$\frac00$, $\frac\infty\infty$, $0\cdot\infty$, $\infty-\infty$, $1^\infty$, $0^0$, $\infty^0$
Co mówi reguła de l'Hospitala?::Przy $\frac00$ lub $\frac\infty\infty$: $\lim\frac fg=\lim\frac{f'}{g'}$ (pochodne licznika i mianownika osobno).
Jak liczyć granicę ilorazu wielomianów przy $x\to\infty$?::Podzielić licznik i mianownik przez najwyższą potęgę $x$ z mianownika.
$\lim_{t\to\infty}t^n e^{-st}$ dla $s>0$?::$0$ — wykładnicza wygrywa z każdym wielomianem.
Dla jakich $s$ zachodzi $\lim_{b\to\infty}e^{-sb}=0$?::Dla $s>0$ (zespolone: $\operatorname{Re}s>0$).

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.

## Źródła
- R. Gewert, Z. Skoczylas, *Analiza matematyczna 1. Definicje, twierdzenia, wzory* (GiS) — literatura PK Matematyka.
- W. Krysicki, L. Włodarski, *Analiza matematyczna w zadaniach*, cz. I (PWN) — rozdział o granicach funkcji.
- A. Milian, *Zbiór zadań z matematyki z rozwiązaniami*, cz. 1 (Wyd. PK).
