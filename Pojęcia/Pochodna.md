---
typ: pojęcie
status: uczę się
przedmioty: ["[[PK Matematyka 1]]"]
wymaga: ["[[Granica (lim)]]"]
powiązane: ["[[Całka oznaczona]]", "[[Równanie różniczkowe zwyczajne]]"]
tags: [pojęcie, matematyka]
utworzono: 2026-09-27
źródła: ["Gewert, Skoczylas — Analiza matematyczna 1", "Krysicki, Włodarski — Analiza matematyczna w zadaniach cz. I", "Milian — Zbiór zadań z matematyki cz. 1 (Wyd. PK)"]
---
# Pochodna

> [!skrot] W 30 sekund
> - **Co to:** chwilowe tempo zmiany funkcji = nachylenie stycznej do wykresu.
> - **Klucz:** $f'(x)=\lim_{h\to0}\frac{f(x+h)-f(x)}{h}$ + tabela pochodnych + reguły: iloczyn, iloraz, łańcuchowa.
> - **Kiedy:** styczne, ekstrema, de l'Hospital, [[Równanie różniczkowe zwyczajne|równania różniczkowe]], własność $\mathcal{L}\{f'\}=sF(s)-f(0)$.

> [!intuicja] Po ludzku
> Pochodna mówi, o ile zmieni się $f$, gdy $x$ ruszy się o odrobinę: $\Delta f\approx f'(x)\,\Delta x$. Analogia: drogomierz pokazuje przejechane kilometry ($f$), a prędkościomierz — pochodną, czyli jak szybko te kilometry przybywają w tej chwili.

## Definicja i wzory
> [!definicja]
> $$f'(x_0)=\lim_{h\to0}\frac{f(x_0+h)-f(x_0)}{h}$$
> Jeśli ta [[Granica (lim)|granica]] istnieje i jest skończona, $f$ jest różniczkowalna w $x_0$. Styczna: $y=f(x_0)+f'(x_0)(x-x_0)$.

> [!wzor] Tabela pochodnych
> | $f(x)$ | $f'(x)$ |
> |---|---|
> | $c$ | $0$ |
> | $x^n$ | $n x^{n-1}$ |
> | $e^{ax}$ | $a e^{ax}$ |
> | $\ln x$ | $\frac1x$ |
> | $\sin(\omega x)$ | $\omega\cos(\omega x)$ |
> | $\cos(\omega x)$ | $-\omega\sin(\omega x)$ |

> [!wzor] Reguły
> $$(f\pm g)'=f'\pm g',\qquad (fg)'=f'g+fg',\qquad \left(\frac fg\right)'=\frac{f'g-fg'}{g^2},\qquad \big(f(g(x))\big)'=f'(g(x))\cdot g'(x)$$

## Jak to zrobić — krok po kroku
1. Rozpoznaj „ostatnie działanie” w wyrażeniu: suma, iloczyn, iloraz czy złożenie.
2. Zastosuj odpowiednią regułę; przy złożeniu różniczkuj od zewnątrz i mnóż przez pochodną środka.
3. Pochodne elementarnych kawałków bierz z tabeli.
4. Uprość: wyciągnij wspólny czynnik (zwłaszcza $e^{\dots}$).

## Przykład
Oblicz $f'(x)$ dla $f(x)=x^2e^{-3x}$.

1. To iloczyn: $u=x^2$, $v=e^{-3x}$.
2. $u'=2x$, $v'=-3e^{-3x}$ (reguła łańcuchowa: pochodna wykładnika to $-3$).
3. $f'=u'v+uv'=2x\,e^{-3x}-3x^2e^{-3x}$.
4. Wyciągamy wspólny czynnik: $f'(x)=x\,e^{-3x}(2-3x)$.

Wniosek dodatkowy: $f'=0$ dla $x=0$ i $x=\frac23$ — kandydaci na ekstrema.

> [!przyklad]- Przykład 2: iloraz $\frac{\sin 2x}{x}$
> $\left(\frac{\sin2x}{x}\right)'=\frac{2\cos2x\cdot x-\sin2x\cdot1}{x^2}=\frac{2x\cos2x-\sin2x}{x^2}$.

> [!przyklad]- Przykład 3 (automatyka): $y(t)=e^{-t}\cos2t$
> $y'(t)=-e^{-t}\cos2t+e^{-t}(-2\sin2t)=-e^{-t}(\cos2t+2\sin2t)$. Tak wygląda pochodna typowej odpowiedzi tłumionej układu II rzędu.

## Typowe błędy
> [!blad] Zapominanie o pochodnej wewnętrznej
> $(e^{-3x})'=-3e^{-3x}$, a nie $e^{-3x}$. $(\sin2x)'=2\cos2x$, a nie $\cos2x$.

> [!blad] $(fg)'=f'g'$
> Pochodna iloczynu to $f'g+fg'$. Analogicznie w ilorazie: kolejność w liczniku $f'g-fg'$ (minus!) i mianownik $g^2$.

> [!blad] Znak przy cosinusie
> $(\cos x)'=-\sin x$, ale $(\sin x)'=+\cos x$.

## Na zajęcia i egzamin
> [!zajecia]
> - **Ćwiczenia:** sprawnie liczyć pochodne złożonych wyrażeń, styczna w punkcie, ekstrema i monotoniczność, de l'Hospital.
> - **Egzamin:** badanie przebiegu funkcji lub zadanie optymalizacyjne; w teorii — definicja pochodnej.
> - Łączy się z: [[Transformata Laplace'a]] — $\mathcal{L}\{f'\}=sF(s)-f(0)$ zamienia równanie różniczkowe w algebraiczne; $\mathcal{L}\{t f(t)\}=-F'(s)$ wymaga liczenia pochodnej po $s$. Odwrotność pochodnej to [[Całka oznaczona|całka]].

## Zadania
> [!zadanie] Zadanie 1
> $(3x^4-2x+7)'$
> > [!odpowiedz]- Odpowiedź
> > $12x^3-2$.

> [!zadanie] Zadanie 2
> $(x\ln x)'$
> > [!odpowiedz]- Odpowiedź
> > $1\cdot\ln x+x\cdot\frac1x=\ln x+1$.

> [!zadanie] Zadanie 3
> $\left(e^{-2t}\sin3t\right)'$
> > [!odpowiedz]- Odpowiedź
> > $-2e^{-2t}\sin3t+3e^{-2t}\cos3t=e^{-2t}(3\cos3t-2\sin3t)$.

> [!zadanie] Zadanie 4
> Oblicz $\frac{d}{ds}\frac{1}{s+a}$ i wyciągnij wniosek o $\mathcal{L}\{t e^{-at}\}$, wiedząc, że $\mathcal{L}\{e^{-at}\}=\frac{1}{s+a}$ i $\mathcal{L}\{tf(t)\}=-F'(s)$.
> > [!odpowiedz]- Odpowiedź
> > $\frac{d}{ds}(s+a)^{-1}=-\frac{1}{(s+a)^2}$, więc $\mathcal{L}\{te^{-at}\}=\frac{1}{(s+a)^2}$.

> [!zadanie] Zadanie 5
> Równanie stycznej do $y=x^3$ w punkcie $x_0=1$.
> > [!odpowiedz]- Odpowiedź
> > $f(1)=1$, $f'(1)=3$: $y=1+3(x-1)=3x-2$.

## Fiszki #fiszki/matematyka
Definicja pochodnej $f'(x_0)$?::$\lim_{h\to0}\frac{f(x_0+h)-f(x_0)}{h}$
Interpretacja geometryczna pochodnej?::Nachylenie (tangens kąta) stycznej do wykresu w danym punkcie.
Równanie stycznej w $x_0$?::$y=f(x_0)+f'(x_0)(x-x_0)$
Pochodna iloczynu $(fg)'$?::$f'g+fg'$
Pochodna ilorazu $\left(\frac fg\right)'$?::$\frac{f'g-fg'}{g^2}$
Reguła łańcuchowa?::$\big(f(g(x))\big)'=f'(g(x))\cdot g'(x)$
$(e^{ax})'$?::$a e^{ax}$
$(\cos\omega x)'$?::$-\omega\sin\omega x$
$(\ln x)'$?::$\frac1x$
Transformata Laplace'a pochodnej $\mathcal{L}\{f'\}$?::$sF(s)-f(0)$
$\mathcal{L}\{t f(t)\}$ w języku pochodnej?::$-F'(s)$ — pochodna transformaty po $s$ ze znakiem minus.

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.

## Źródła
- R. Gewert, Z. Skoczylas, *Analiza matematyczna 1. Definicje, twierdzenia, wzory* (GiS) — rozdział o rachunku różniczkowym.
- W. Krysicki, L. Włodarski, *Analiza matematyczna w zadaniach*, cz. I (PWN).
- A. Milian, *Zbiór zadań z matematyki z rozwiązaniami*, cz. 1 (Wyd. PK).
