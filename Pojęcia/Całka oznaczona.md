---
typ: pojęcie
status: uczę się
przedmioty: ["[[PK Matematyka 1]]"]
wymaga: ["[[Pochodna]]", "[[Granica (lim)]]"]
powiązane: ["[[Całka niewłaściwa]]", "[[Rozkład na ułamki proste]]"]
tags: [pojęcie, matematyka]
utworzono: 2026-09-27
źródła: ["Gewert, Skoczylas — Analiza matematyczna 1", "Krysicki, Włodarski — Analiza matematyczna w zadaniach cz. I", "Milian — Zbiór zadań z matematyki cz. 1 (Wyd. PK)"]
---
# Całka oznaczona

> [!skrot] W 30 sekund
> - **Co to:** liczba $\int_a^b f(x)\,dx$ — „pole ze znakiem” pod wykresem $f$ na $[a,b]$.
> - **Klucz:** wzór Newtona–Leibniza $\int_a^b f(x)\,dx=F(b)-F(a)$, gdzie $F'=f$; techniki: podstawienie i całkowanie przez części.
> - **Kiedy:** pola, wartości średnie, praca, ładunek; z granicą $\infty$ — [[Całka niewłaściwa]] i [[Transformata Laplace'a]].

> [!intuicja] Po ludzku
> Całka sumuje nieskończenie wiele cienkich pasków $f(x)\,dx$. Pochodna mówi, jak szybko coś przyrasta, całka — ile przyrosło łącznie, dlatego są do siebie odwrotne. Analogia: z wykresu prędkości samochodu odczytujesz przejechaną drogę jako pole pod tym wykresem.

## Definicja i wzory
> [!definicja]
> $$\int_a^b f(x)\,dx=\lim_{n\to\infty}\sum_{i=1}^{n} f(\xi_i)\,\Delta x_i$$
> (granica sum Riemanna przy średnicy podziału dążącej do 0). Dla $f$ ciągłej i $F'=f$:
> $$\int_a^b f(x)\,dx=\big[F(x)\big]_a^b=F(b)-F(a)$$

> [!wzor] Całki podstawowe (odwrotność tabeli pochodnych)
> | $f(x)$ | $F(x)$ |
> |---|---|
> | $x^n$, $n\neq-1$ | $\frac{x^{n+1}}{n+1}$ |
> | $\frac1x$ | $\ln\lvert x\rvert$ |
> | $e^{ax}$ | $\frac1a e^{ax}$ |
> | $\sin\omega x$ | $-\frac1\omega\cos\omega x$ |
> | $\cos\omega x$ | $\frac1\omega\sin\omega x$ |
> | $\frac{1}{x-a}$ | $\ln\lvert x-a\rvert$ |

> [!wzor] Własności i techniki
> - Liniowość: $\int_a^b(\alpha f+\beta g)=\alpha\int_a^b f+\beta\int_a^b g$; addytywność: $\int_a^c=\int_a^b+\int_b^c$; $\int_b^a=-\int_a^b$.
> - Przez części: $\int_a^b u\,v'\,dx=\big[uv\big]_a^b-\int_a^b u'v\,dx$.
> - Podstawienie $u=g(x)$: $\int_a^b f(g(x))g'(x)\,dx=\int_{g(a)}^{g(b)}f(u)\,du$ — **zmieniasz granice**.

## Jak to zrobić — krok po kroku
1. Uprość funkcję podcałkową; wymierną rozbij na [[Rozkład na ułamki proste|ułamki proste]].
2. Wybierz metodę: tabela → podstawienie (widać „funkcję i jej pochodną”) → przez części (iloczyn wielomianu z $e^{x}$, $\sin$, $\cos$, $\ln$).
3. Przy częściach za $u$ bierz wielomian (zmaleje po zróżniczkowaniu), za $v'$ — $e^{ax}$ lub $\sin$/$\cos$. Wyjątek: przy $\ln x$ za $u$ bierz logarytm.
4. Wstaw granice: $F(b)-F(a)$. Stałej $C$ nie piszesz.

## Przykład
Oblicz $\int_0^1 x\,e^{-2x}\,dx$.

1. Przez części: $u=x$, $v'=e^{-2x}$, więc $u'=1$, $v=-\frac12e^{-2x}$.
2. $\int_0^1 xe^{-2x}dx=\left[-\frac x2e^{-2x}\right]_0^1+\frac12\int_0^1e^{-2x}dx$.
3. Wyraz brzegowy: $-\frac12e^{-2}-0=-\frac12e^{-2}$.
4. Reszta: $\frac12\left[-\frac12e^{-2x}\right]_0^1=\frac12\cdot\frac12\left(1-e^{-2}\right)=\frac14-\frac14e^{-2}$.
5. Suma: $\frac14-\frac14e^{-2}-\frac12e^{-2}=\frac14-\frac34e^{-2}\approx0{,}1485$.

Ten sam schemat z granicą $\infty$ i $e^{-st}$ daje $\mathcal{L}\{t\}=\frac1{s^2}$.

> [!przyklad]- Przykład 2: podstawienie, $\int_0^{\pi/2}\sin x\cos x\,dx$
> $u=\sin x$, $du=\cos x\,dx$; granice: $x=0\to u=0$, $x=\frac\pi2\to u=1$. $\int_0^1u\,du=\left[\frac{u^2}2\right]_0^1=\frac12$.

> [!przyklad]- Przykład 3: znak pola, $\int_0^{2\pi}\sin x\,dx$
> $\left[-\cos x\right]_0^{2\pi}=-1-(-1)=0$. Pola nad i pod osią się znoszą; prawdziwe pole to $2\int_0^\pi\sin x\,dx=2\cdot2=4$.

## Typowe błędy
> [!blad] Podstawienie bez zmiany granic
> Po $u=g(x)$ albo zmieniasz granice na $g(a)$, $g(b)$, albo wracasz do $x$ przed wstawieniem. Mieszanie obu daje zły wynik.

> [!blad] Zgubiony wyraz brzegowy przy częściach
> W całce oznaczonej $\big[uv\big]_a^b$ to liczba, którą trzeba policzyć — często nie jest zerem.

> [!blad] $\int\frac1x\,dx=\frac{x^0}{0}$
> Wzór na $x^n$ nie działa dla $n=-1$; tu wynikiem jest $\ln\lvert x\rvert$.

> [!blad] Całka = pole zawsze
> Gdy $f<0$, całka jest ujemna. Pole liczysz jako $\int_a^b\lvert f(x)\rvert\,dx$.

## Na zajęcia i egzamin
> [!zajecia]
> - **Ćwiczenia:** całki przez części i przez podstawienie, całki funkcji wymiernych (ułamki proste), pola obszarów między krzywymi.
> - **Egzamin:** obliczyć całkę oznaczoną lub pole figury; w teorii — wzór Newtona–Leibniza.
> - Łączy się z: [[Transformata Laplace'a]] — $\mathcal{L}\{f\}=\int_0^\infty f(t)e^{-st}dt=\lim_{T\to\infty}\int_0^T f(t)e^{-st}dt$, czyli całka oznaczona plus [[Granica (lim)|granica]]; liczenie $\mathcal{L}\{t^n\}$, $\mathcal{L}\{\sin\omega t\}$ z definicji to całkowanie przez części.

## Zadania
> [!zadanie] Zadanie 1
> $\int_0^2(3x^2-2x)\,dx$
> > [!odpowiedz]- Odpowiedź
> > $\left[x^3-x^2\right]_0^2=8-4=4$.

> [!zadanie] Zadanie 2
> $\int_1^e\frac1x\,dx$
> > [!odpowiedz]- Odpowiedź
> > $\ln e-\ln1=1$.

> [!zadanie] Zadanie 3
> $\int_0^1e^{-3t}\,dt$
> > [!odpowiedz]- Odpowiedź
> > $\left[-\frac13e^{-3t}\right]_0^1=\frac{1-e^{-3}}{3}\approx0{,}3167$.

> [!zadanie] Zadanie 4
> $\int_0^\pi x\sin x\,dx$
> > [!odpowiedz]- Odpowiedź
> > $u=x$, $v=-\cos x$: $\left[-x\cos x\right]_0^\pi+\int_0^\pi\cos x\,dx=\pi+\left[\sin x\right]_0^\pi=\pi$.

> [!zadanie] Zadanie 5
> $\int_0^1\frac{2x}{x^2+1}\,dx$
> > [!odpowiedz]- Odpowiedź
> > $u=x^2+1$, granice $1\to2$: $\int_1^2\frac{du}{u}=\ln2$.

## Fiszki #fiszki/matematyka
Wzór Newtona–Leibniza?::$\int_a^b f(x)\,dx=F(b)-F(a)$, gdzie $F'=f$
Interpretacja geometryczna całki oznaczonej?::Pole ze znakiem pod wykresem: części pod osią $OX$ liczą się ujemnie.
$\int x^n\,dx$ dla $n\neq-1$?::$\frac{x^{n+1}}{n+1}+C$
$\int\frac1x\,dx$?::$\ln\lvert x\rvert+C$
$\int e^{ax}\,dx$?::$\frac1a e^{ax}+C$
Wzór na całkowanie przez części (oznaczona)?::$\int_a^b uv'\,dx=\big[uv\big]_a^b-\int_a^b u'v\,dx$
Co trzeba zrobić z granicami przy podstawieniu $u=g(x)$?::Zamienić je na $g(a)$ i $g(b)$.
Co brać za $u$ w $\int x e^{ax}dx$?::$u=x$ (wielomian), $v'=e^{ax}$.
$\int_b^a f\,dx$ a $\int_a^b f\,dx$?::$\int_b^a f=-\int_a^b f$
Jak definicja Laplace'a wiąże się z całką oznaczoną?::$\mathcal{L}\{f\}=\lim_{T\to\infty}\int_0^T f(t)e^{-st}dt$ — całka oznaczona przechodząca w niewłaściwą.

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.

## Źródła
- R. Gewert, Z. Skoczylas, *Analiza matematyczna 1. Definicje, twierdzenia, wzory* (GiS) — rachunek całkowy.
- W. Krysicki, L. Włodarski, *Analiza matematyczna w zadaniach*, cz. I (PWN) — całki oznaczone i ich zastosowania.
- A. Milian, *Zbiór zadań z matematyki z rozwiązaniami*, cz. 1 (Wyd. PK).
