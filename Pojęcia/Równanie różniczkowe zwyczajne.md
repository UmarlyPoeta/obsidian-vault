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

> [!skrot] W 30 sekund
> - **Co to:** równanie, w którym niewiadomą jest funkcja $y(t)$, powiązana ze swoimi pochodnymi.
> - **Klucz:** dla $y''+ay'+by=f(t)$: równanie charakterystyczne $r^2+ar+b=0$, potem $y = y_o + y_s$, na końcu stałe z warunków początkowych.
> - **Kiedy:** każdy układ dynamiczny — obwód RC, masa na sprężynie, nagrzewanie, obiekt regulacji.

> [!intuicja] Po ludzku
> Zwykłe równanie pyta „jaka liczba?”, różniczkowe — „jaka funkcja?”. Odpowiedzią jest cała rodzina funkcji (ze stałymi $C_1, C_2$), a konkretną wybierają warunki początkowe: gdzie układ był i jak szybko się ruszał w chwili $t=0$. Jak z wiedzy „prędkość zależy od położenia” i „start stąd” odtworzyć całą trasę.

## Definicja i wzory
> [!definicja]
> **Równanie różniczkowe zwyczajne** wiąże funkcję jednej zmiennej $y(t)$ z jej pochodnymi. **Rząd** = najwyższa pochodna. Liniowe o stałych współczynnikach II rzędu:
> $$y'' + a\,y' + b\,y = f(t)$$
> $f(t)=0$ — równanie **jednorodne**; $f(t)\neq 0$ — **niejednorodne** ($f$ to wymuszenie).

> [!wzor] Rozwiązanie jednorodnego $y_o$ — trzy przypadki
> Podstawiasz $y=e^{rt}$ i dostajesz $r^2 + a r + b = 0$, $\Delta = a^2-4b$:
> - $\Delta>0$, pierwiastki $r_1\neq r_2$: $\;y_o = C_1e^{r_1t}+C_2e^{r_2t}$
> - $\Delta=0$, pierwiastek podwójny $r$: $\;y_o = (C_1 + C_2t)\,e^{rt}$
> - $\Delta<0$, $r=\alpha\pm i\beta$: $\;y_o = e^{\alpha t}(C_1\cos\beta t + C_2\sin\beta t)$
>
> I rząd $y' + a y = 0$: $\;y_o = Ce^{-at}$.

> [!wzor] Rozwiązanie szczególne $y_s$ — przewidywanie
> - $f = $ stała $\to y_s = A$
> - $f = $ wielomian stopnia $n$ $\to$ wielomian stopnia $n$
> - $f = e^{kt} \to y_s = Ae^{kt}$
> - $f = \cos\omega t$ lub $\sin\omega t \to y_s = A\cos\omega t + B\sin\omega t$
>
> Gdy wykładnik ($k$ albo $i\omega$) jest pierwiastkiem równania charakterystycznego krotności $m$ — mnożysz przewidywanie przez $t^m$ (rezonans).

## Jak to zrobić — krok po kroku
1. Zapisz równanie charakterystyczne i znajdź pierwiastki.
2. Wybierz postać $y_o$ z jednego z trzech przypadków.
3. Przewiduj $y_s$ wg prawej strony, wstaw do równania, wyznacz $A, B$.
4. Złóż $y = y_o + y_s$.
5. **Dopiero teraz** wstaw $y(0)$, $y'(0)$ i wyznacz $C_1, C_2$.
6. Sprawdź: wstaw wynik do równania i do warunków.

## Przykład
Rozwiąż $y''+3y'+2y=4$, $y(0)=0$, $y'(0)=0$.

1. $r^2+3r+2=(r+1)(r+2)=0 \Rightarrow r_1=-1,\ r_2=-2$.
2. $y_o = C_1e^{-t}+C_2e^{-2t}$.
3. $f=4$ — stała, więc $y_s=A$: $\;0+0+2A=4 \Rightarrow A=2$.
4. $y = C_1e^{-t}+C_2e^{-2t}+2$, $\;y' = -C_1e^{-t}-2C_2e^{-2t}$.
5. Warunki:
$$\begin{cases} C_1 + C_2 + 2 = 0 \\ -C_1 - 2C_2 = 0 \end{cases} \Rightarrow C_2 = 2,\ C_1 = -4$$
6. Wynik: $y(t) = 2 - 4e^{-t} + 2e^{-2t}$. Sprawdzenie: $y''+3y'+2y = (-4+12-8)e^{-t} + (8-12+4)e^{-2t} + 4 = 4$ ✓.

To odpowiedź skokowa układu z transmitancją $\frac{4}{s^2+3s+2}$ — por. [[Transmitancja]].

> [!przyklad]- Przykład 2 — pierwiastek podwójny
> $y''+4y'+4y=0$, $y(0)=1$, $y'(0)=0$. $(r+2)^2=0 \Rightarrow r=-2$, $y=(C_1+C_2t)e^{-2t}$.
> $y(0)=C_1=1$; $y'(0) = C_2 - 2C_1 = 0 \Rightarrow C_2 = 2$. Wynik: $y=(1+2t)e^{-2t}$.

> [!przyklad]- Przykład 3 — pierwiastki zespolone (drgania gasnące)
> $y''+2y'+5y=0$, $y(0)=0$, $y'(0)=2$. $r^2+2r+5=0 \Rightarrow r=-1\pm 2i$.
> $y=e^{-t}(C_1\cos 2t + C_2\sin 2t)$; $y(0)=C_1=0$; $y'(0)=2C_2=2 \Rightarrow C_2=1$. Wynik: $y = e^{-t}\sin 2t$.

> [!przyklad]- Przykład 4 — I rząd
> $y'+2y=6$, $y(0)=0$. $y_o=Ce^{-2t}$, $y_s=3$, $y(0)=C+3=0$. Wynik: $y=3(1-e^{-2t})$.

## Typowe błędy
> [!blad] Stałe z warunków początkowych liczone przed dodaniem $y_s$
> Warunki dotyczą **całego** rozwiązania $y_o+y_s$. Wstawione do samego $y_o$ dają złe $C_1, C_2$.

> [!blad] Pierwiastek podwójny zapisany jak dwa różne
> $(r-1)^2=0$ daje $y=(C_1+C_2t)e^{t}$, a nie $C_1e^t+C_2e^t$ (to tylko jedna stała).

> [!blad] Przeoczony rezonans
> $y''+y=\sin t$: przewidywanie $A\cos t+B\sin t$ daje $0=\sin t$. Bo $\pm i$ to pierwiastki — trzeba $t(A\cos t+B\sin t)$.

## Na zajęcia i egzamin
> [!zajecia]
> - **Ćwiczenia:** rozwiązać zagadnienie Cauchy'ego dla II rzędu (każdy z trzech przypadków) z wymuszeniem stałym, wykładniczym lub sinusoidalnym.
> - **Egzamin:** to samo równanie rozwiązać klasycznie i przez [[Transformata Laplace'a|transformatę Laplace'a]].
> - Łączy się z: [[Transmitancja]], [[Liczby zespolone]], [[Drgania mechaniczne]], [[Stany nieustalone w obwodach]].

## Zadania
> [!zadanie] Zadanie 1
> $y'+3y=0$, $y(0)=2$.
> > [!odpowiedz]- Odpowiedź
> > $y=2e^{-3t}$.

> [!zadanie] Zadanie 2
> $y''-y'-6y=0$, $y(0)=0$, $y'(0)=5$.
> > [!odpowiedz]- Odpowiedź
> > $r=3,\,-2$; $C_1+C_2=0$, $3C_1-2C_2=5 \Rightarrow C_1=1,\ C_2=-1$. $y=e^{3t}-e^{-2t}$.

> [!zadanie] Zadanie 3
> $y''+9y=0$, $y(0)=1$, $y'(0)=0$.
> > [!odpowiedz]- Odpowiedź
> > $r=\pm 3i$, $y=C_1\cos 3t + C_2\sin 3t$; $C_1=1$, $C_2=0$. $y=\cos 3t$.

> [!zadanie] Zadanie 4
> $y''+2y'+y=3$, $y(0)=0$, $y'(0)=0$.
> > [!odpowiedz]- Odpowiedź
> > $r=-1$ (podwójny), $y_s=3$, $y=(C_1+C_2t)e^{-t}+3$; $C_1=-3$, $C_2-C_1=0 \Rightarrow C_2=-3$. $y=3-3(1+t)e^{-t}$.

> [!zadanie] Zadanie 5
> Znajdź rozwiązanie szczególne $y''+y=\sin t$.
> > [!odpowiedz]- Odpowiedź
> > Rezonans: $y_s=t(A\cos t+B\sin t)$, po wstawieniu $A=-\frac{1}{2}$, $B=0$. $y_s=-\frac{1}{2}t\cos t$.

## Fiszki #fiszki/matematyka
Co to jest rząd równania różniczkowego?::Najwyższa pochodna niewiadomej funkcji, która w nim występuje.
Rozwiązanie y' + a·y = 0?::y = C·e^(−at).
Równanie charakterystyczne dla y'' + a·y' + b·y = 0?::r² + a·r + b = 0 (podstawienie y = e^(rt)).
Rozwiązanie y_o, gdy Δ>0?::y = C₁e^(r₁t) + C₂e^(r₂t).
Rozwiązanie y_o, gdy Δ=0?::y = (C₁ + C₂t)·e^(rt).
Rozwiązanie y_o, gdy r = α ± iβ?::y = e^(αt)·(C₁cos βt + C₂sin βt).
Z czego składa się rozwiązanie równania niejednorodnego?::y = y_o + y_s (ogólne jednorodnego + szczególne).
Kiedy wyznaczasz stałe z warunków początkowych?::Na końcu, po dodaniu y_s do y_o.
Przewidywanie y_s dla prawej strony A·e^(kt)?::B·e^(kt); gdy k jest pierwiastkiem krotności m — mnożysz przez t^m.
Przewidywanie y_s dla prawej strony sin ωt?::A·cos ωt + B·sin ωt (z czynnikiem t, gdy iω jest pierwiastkiem).
Co dają pierwiastki zespolone z ujemną częścią rzeczywistą?::Drgania gasnące: e^(αt) z α<0 razy sinus/cosinus.

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.

## Źródła
- A. Milian, *Zbiór zadań z matematyki z rozwiązaniami* cz. 2 (Wyd. PK) — równania różniczkowe zwyczajne
- M. Gewert, Z. Skoczylas, *Równania różniczkowe zwyczajne. Teoria, przykłady, zadania* (GiS)
