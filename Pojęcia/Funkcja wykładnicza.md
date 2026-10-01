---
typ: pojęcie
status: uczę się
przedmioty: ["[[PK Matematyka 1]]"]
wymaga: ["[[Granica (lim)]]"]
powiązane: ["[[Liczby zespolone]]", "[[Transformata Laplace'a]]"]
tags: [pojęcie, matematyka]
utworzono: 2026-09-27
źródła: ["Gewert, Skoczylas — Analiza matematyczna 1", "Krysicki, Włodarski — Analiza matematyczna w zadaniach cz. I", "Milian — Zbiór zadań z matematyki cz. 1 (Wyd. PK)"]
---
# Funkcja wykładnicza

> [!skrot] W 30 sekund
> - **Co to:** $f(x)=a^x$, w praktyce prawie zawsze $e^{x}$ (lub $e^{at}$), gdzie $e\approx2{,}71828$.
> - **Klucz:** $(e^{at})'=a\,e^{at}$ i $\int e^{at}dt=\frac1a e^{at}+C$ — różniczkowanie i całkowanie tylko mnoży przez stałą.
> - **Kiedy:** rozwiązania [[Równanie różniczkowe zwyczajne|równań różniczkowych]], stany nieustalone, jądro $e^{-st}$ [[Transformata Laplace'a|transformaty Laplace'a]].

> [!intuicja] Po ludzku
> Funkcja wykładnicza opisuje zjawiska, w których tempo zmiany jest proporcjonalne do aktualnej wartości: $y'=ay$. Dla $a>0$ to wzrost (procent składany), dla $a<0$ zanik (rozładowanie kondensatora, stygnięcie). Analogia: lokata z kapitalizacją co chwilę — im więcej masz, tym szybciej przybywa.

## Definicja i wzory
> [!definicja]
> $$e=\lim_{n\to\infty}\left(1+\frac1n\right)^n,\qquad e^{x}=\lim_{n\to\infty}\left(1+\frac xn\right)^n=\sum_{k=0}^{\infty}\frac{x^k}{k!}$$
> Dziedzina $\mathbb{R}$, zbiór wartości $(0,\infty)$; funkcją odwrotną jest $\ln x$. Ogólnie $a^x=e^{x\ln a}$.

> [!wzor] Własności
> $$e^{a+b}=e^ae^b,\qquad e^{-a}=\frac1{e^a},\qquad (e^a)^b=e^{ab},\qquad e^{0}=1$$
> $$(e^{at})'=a\,e^{at},\qquad \int e^{at}\,dt=\frac1a e^{at}+C\ (a\neq0)$$

> [!wzor] Wzór Eulera (most do [[Liczby zespolone|liczb zespolonych]])
> $$e^{i\omega t}=\cos\omega t+i\sin\omega t,\qquad \cos\omega t=\frac{e^{i\omega t}+e^{-i\omega t}}{2},\qquad \sin\omega t=\frac{e^{i\omega t}-e^{-i\omega t}}{2i}$$
> Stąd $e^{(\sigma+i\omega)t}=e^{\sigma t}(\cos\omega t+i\sin\omega t)$: $\sigma<0$ — drgania gasnące, $\sigma>0$ — narastające.

> [!wazne] Stała czasowa
> Zanik $e^{-t/\tau}$: po czasie $\tau$ zostaje $e^{-1}\approx36{,}8\%$, po $5\tau$ — $e^{-5}\approx0{,}67\%$ (praktycznie stan ustalony). Spadek o połowę: $t=\tau\ln2\approx0{,}693\,\tau$.

## Jak to zrobić — krok po kroku
Typowe równanie wykładnicze/zanik:
1. Sprowadź wszystko do podstawy $e$ (z $a^x=e^{x\ln a}$) i połącz potęgi własnościami.
2. Izoluj $e^{(\dots)}$ po jednej stronie.
3. Zlogarytmuj: $e^{u}=c\iff u=\ln c$ (tylko dla $c>0$).
4. Przy całkach z $e^{at}$ pamiętaj o czynniku $\frac1a$ i — dla granicy $\infty$ — o znaku $a$.

## Przykład
Oblicz $\mathcal{L}\{e^{at}\}=\int_0^\infty e^{at}e^{-st}\,dt$ i podaj, kiedy istnieje.

1. Łączymy wykładniki: $e^{at}e^{-st}=e^{-(s-a)t}$.
2. Całka do skończonej granicy $b$: $\int_0^b e^{-(s-a)t}dt=\left[-\frac{e^{-(s-a)t}}{s-a}\right]_0^b=\frac{1-e^{-(s-a)b}}{s-a}$.
3. Granica $b\to\infty$: $e^{-(s-a)b}\to0$ wtedy i tylko wtedy, gdy $s-a>0$.
4. Wynik: $\mathcal{L}\{e^{at}\}=\frac{1}{s-a}$ dla $s>a$ (zespolone: $\operatorname{Re}s>a$).

To najważniejsza pozycja w tablicy transformat — z niej przez wzór Eulera wychodzą też $\mathcal{L}\{\cos\omega t\}$ i $\mathcal{L}\{\sin\omega t\}$.

> [!przyklad]- Przykład 2: równanie $y'=-2y$, $y(0)=3$
> Szukamy $y=Ce^{\lambda t}$: $\lambda Ce^{\lambda t}=-2Ce^{\lambda t}\Rightarrow\lambda=-2$. Warunek $y(0)=C=3$. Wynik: $y(t)=3e^{-2t}$, stała czasowa $\tau=\frac12$.

> [!przyklad]- Przykład 3: równanie $e^{2x}-3e^{x}+2=0$
> Podstawienie $u=e^x>0$: $u^2-3u+2=0\Rightarrow u=1$ lub $u=2$. Stąd $x=0$ lub $x=\ln2$.

## Typowe błędy
> [!blad] $e^{a+b}=e^a+e^b$
> Wykładniki się dodaje przy **mnożeniu** potęg: $e^{a}e^{b}=e^{a+b}$. Suma potęg nie upraszcza się.

> [!blad] Zgubione $\frac1a$ w całce
> $\int e^{-3t}dt=-\frac13e^{-3t}+C$, a nie $e^{-3t}+C$ ani $-3e^{-3t}+C$ (to byłaby pochodna).

> [!blad] Logarytm z liczby ujemnej
> $e^{x}=-2$ nie ma rozwiązań rzeczywistych — $e^{x}>0$ zawsze.

## Na zajęcia i egzamin
> [!zajecia]
> - **Ćwiczenia:** przekształcać wyrażenia wykładnicze, rozwiązywać równania $e^{\dots}=c$, różniczkować i całkować $e^{at}$, $t e^{at}$, $e^{at}\sin\omega t$.
> - **Egzamin (sem. 2):** liczenie $\mathcal{L}\{e^{at}\}$ z definicji, rozwiązania RRZ w postaci $Ce^{\lambda t}$, przesunięcie w dziedzinie $s$: $\mathcal{L}\{e^{at}f(t)\}=F(s-a)$.
> - Łączy się z: [[Transformata Laplace'a]], [[Liczby zespolone]], [[Stany nieustalone w obwodach]], [[Całka niewłaściwa]].

## Zadania
> [!zadanie] Zadanie 1
> Uprość $\frac{e^{2t}\cdot e^{-5t}}{e^{-t}}$.
> > [!odpowiedz]- Odpowiedź
> > $e^{2t-5t+t}=e^{-2t}$.

> [!zadanie] Zadanie 2
> Po jakim czasie $e^{-t/\tau}$ spadnie do połowy?
> > [!odpowiedz]- Odpowiedź
> > $e^{-t/\tau}=\frac12\Rightarrow t=\tau\ln2\approx0{,}693\,\tau$.

> [!zadanie] Zadanie 3
> Podaj część rzeczywistą $e^{(-1+2i)t}$.
> > [!odpowiedz]- Odpowiedź
> > $e^{-t}(\cos2t+i\sin2t)$, więc $\operatorname{Re}=e^{-t}\cos2t$ — drganie gasnące.

> [!zadanie] Zadanie 4
> Oblicz $\int_0^\infty e^{-3t}\,dt$.
> > [!odpowiedz]- Odpowiedź
> > $\left[-\frac13e^{-3t}\right]_0^\infty=0-\left(-\frac13\right)=\frac13$ (to $\mathcal{L}\{1\}$ dla $s=3$).

> [!zadanie] Zadanie 5
> Korzystając z $\mathcal{L}\{e^{at}\}=\frac1{s-a}$ i wzoru Eulera, wyznacz $\mathcal{L}\{\cos\omega t\}$.
> > [!odpowiedz]- Odpowiedź
> > $\frac12\left(\frac1{s-i\omega}+\frac1{s+i\omega}\right)=\frac12\cdot\frac{2s}{s^2+\omega^2}=\frac{s}{s^2+\omega^2}$.

## Fiszki #fiszki/matematyka
Definicja liczby $e$ jako granicy?::$e=\lim_{n\to\infty}\left(1+\frac1n\right)^n\approx2{,}718$
$(e^{at})'$?::$a\,e^{at}$
$\int e^{at}\,dt$?::$\frac1a e^{at}+C$ dla $a\neq0$
$e^{a}\cdot e^{b}$?::$e^{a+b}$
Wzór Eulera?::$e^{i\omega t}=\cos\omega t+i\sin\omega t$
$\cos\omega t$ przez wykładnicze?::$\frac{e^{i\omega t}+e^{-i\omega t}}{2}$
Ile zostaje z $e^{-t/\tau}$ po czasie $\tau$, a ile po $5\tau$?::Ok. 36,8% oraz ok. 0,67%.
$\mathcal{L}\{e^{at}\}$ i warunek istnienia?::$\frac1{s-a}$ dla $s>a$ ($\operatorname{Re}s>a$)
Co oznacza znak $\sigma$ w $e^{(\sigma+i\omega)t}$?::$\sigma<0$ — drgania gasną, $\sigma>0$ — narastają, $\sigma=0$ — stała amplituda.
Jakie równanie spełnia $y=Ce^{at}$?::$y'=ay$ — tempo zmiany proporcjonalne do wartości.

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.

## Źródła
- R. Gewert, Z. Skoczylas, *Analiza matematyczna 1. Definicje, twierdzenia, wzory* (GiS) — funkcje elementarne.
- W. Krysicki, L. Włodarski, *Analiza matematyczna w zadaniach*, cz. I (PWN).
- A. Milian, *Zbiór zadań z matematyki z rozwiązaniami*, cz. 1 (Wyd. PK).
