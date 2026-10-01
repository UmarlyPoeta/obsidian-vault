---
typ: pojęcie
status: uczę się
przedmioty: ["[[PK Matematyka 1]]", "[[PK Elektrotechnika i miernictwo]]"]
wymaga: ["[[Funkcja wykładnicza]]", "[[Granica (lim)]]"]
powiązane: ["[[Transformata Laplace'a]]", "[[Argument i faza (φ)]]"]
tags: [pojęcie, matematyka]
utworzono: 2026-09-27
źródła: []
---
# Liczby zespolone

> [!skrot] W 30 sekund
> - **Co to:** liczba $z = a + bj$, gdzie $j^2 = -1$ — punkt (albo strzałka) na płaszczyźnie.
> - **Klucz:** $z = a + bj = r e^{j\varphi}$, gdzie $r = \sqrt{a^2+b^2}$, a $\varphi$ to [[Argument i faza (φ)|argument]].
> - **Kiedy:** pierwiastki wielomianów z $\Delta < 0$, bieguny w [[Transformata Laplace'a|Laplasie]], obwody prądu przemiennego.

> [!intuicja] Po ludzku
> Zwykła liczba leży na osi, zespolona na płaszczyźnie: ma część „w prawo” ($a$) i część „w górę” ($b$). Można ją też opisać długością $r$ i kątem $\varphi$. Mnożenie przez liczbę zespoloną to **obrót i rozciągnięcie** — jak wskazówka zegara, którą obracasz i wydłużasz.

Konwencja: w automatyce i elektrotechnice jednostkę urojoną oznacza się $j$ (bo $i$ to prąd). W podręcznikach matematyki piszą $i$ — to to samo.

## Definicja i wzory
> [!definicja] Trzy postacie
> - **Algebraiczna:** $z = a + bj$, $a = \operatorname{Re} z$, $b = \operatorname{Im} z$ (część urojona to liczba rzeczywista $b$, bez $j$).
> - **Trygonometryczna:** $z = r(\cos\varphi + j\sin\varphi)$.
> - **Wykładnicza:** $z = r e^{j\varphi}$ (wzór Eulera: $e^{j\varphi} = \cos\varphi + j\sin\varphi$).

> [!wzor] Działania
> - Moduł: $|z| = r = \sqrt{a^2 + b^2}$; zamiana: $a = r\cos\varphi$, $b = r\sin\varphi$.
> - Sprzężenie: $\bar z = a - bj$, a $z\bar z = a^2 + b^2 = |z|^2$.
> - Mnożenie: $r_1 e^{j\varphi_1} \cdot r_2 e^{j\varphi_2} = r_1 r_2\, e^{j(\varphi_1 + \varphi_2)}$.
> - Dzielenie: pomnóż licznik i mianownik przez sprzężenie mianownika.
> - Potęga (wzór de Moivre'a): $z^n = r^n e^{jn\varphi}$.

## Jak to zrobić — krok po kroku
1. Dodawanie/odejmowanie — w postaci algebraicznej, osobno części rzeczywiste i urojone.
2. Mnożenie — wymnóż nawiasy i zamień $j^2$ na $-1$ (albo w postaci wykładniczej: pomnóż moduły, dodaj kąty).
3. Dzielenie — $\frac{z_1}{z_2} = \frac{z_1 \bar z_2}{|z_2|^2}$.
4. Zamiana na postać wykładniczą — policz $r$, potem $\varphi$ z uwzględnieniem ćwiartki (patrz [[Argument i faza (φ)]]).
5. Potęgi i pierwiastki — tylko w postaci wykładniczej.

## Przykład
Dane: $z_1 = 1 + 2j$, $z_2 = 3 - j$.

- Iloczyn: $(1+2j)(3-j) = 3 - j + 6j - 2j^2 = 3 + 5j + 2 = 5 + 5j$.
- Iloraz: $\frac{5+5j}{1+2j} = \frac{(5+5j)(1-2j)}{(1+2j)(1-2j)} = \frac{5 - 10j + 5j - 10j^2}{1+4} = \frac{15 - 5j}{5} = 3 - j$ — wróciliśmy do $z_2$, zgadza się.
- Postać wykładnicza $5 + 5j$: $r = \sqrt{25+25} = 5\sqrt2$, punkt w I ćwiartce, $\varphi = \arctan 1 = \frac{\pi}{4}$, więc $5+5j = 5\sqrt2\, e^{j\pi/4}$.

> [!przyklad]- Przykład 2 — pierwiastki trójmianu z ujemną deltą
> $s^2 + 2s + 5 = 0$: $\Delta = 4 - 20 = -16$, $\sqrt{\Delta} = 4j$.
> $s = \frac{-2 \pm 4j}{2} = -1 \pm 2j$ — para sprzężona. W Laplasie to biegun dający $e^{-t}\cos 2t$ i $e^{-t}\sin 2t$, czyli drganie gasnące.

## Typowe błędy
> [!blad] $j^2 = 1$
> Nie: $j^2 = -1$. Każde $j^2$ po wymnożeniu zamieniaj od razu na $-1$.

> [!blad] Część urojona „z $j$”
> Dla $z = 3 + 4j$ mamy $\operatorname{Im} z = 4$, a nie $4j$.

> [!blad] Kąt z samego arcusa tangensa
> $\arctan\frac{b}{a}$ daje kąt tylko w I i IV ćwiartce. Dla $a < 0$ trzeba dodać lub odjąć $\pi$.

> [!blad] Dzielenie „po składowych”
> $\frac{a+bj}{c+dj} \neq \frac{a}{c} + \frac{b}{d}j$. Zawsze mnóż przez sprzężenie mianownika.

## Na zajęcia i egzamin
> [!zajecia]
> - **Ćwiczenia:** działania w postaci algebraicznej, zamiana na postać wykładniczą, potęgi z de Moivre'a, pierwiastki równań kwadratowych z $\Delta < 0$.
> - **Egzamin:** „zapisz w postaci wykładniczej”, „oblicz $(1+j)^n$”, „rozwiąż równanie w $\mathbb{C}$”.
> - Łączy się z: [[Transformata Laplace'a]] (zmienna $s = \sigma + j\omega$; bieguny zespolone dają drgania), [[Transmitancja]], [[Stany nieustalone w obwodach]].

## Zadania
> [!zadanie] Zadanie 1
> Oblicz $(2+3j)(1-j)$.
> > [!odpowiedz]- Odpowiedź
> > $2 - 2j + 3j - 3j^2 = 2 + j + 3 = 5 + j$.

> [!zadanie] Zadanie 2
> Oblicz $\frac{1+j}{1-j}$.
> > [!odpowiedz]- Odpowiedź
> > $\frac{(1+j)^2}{(1-j)(1+j)} = \frac{1 + 2j - 1}{2} = j$.

> [!zadanie] Zadanie 3
> Zapisz $z = 3 + 4j$ w postaci wykładniczej.
> > [!odpowiedz]- Odpowiedź
> > $r = 5$, I ćwiartka, $\varphi = \arctan\frac43 \approx 0{,}927$ rad $\approx 53{,}13°$; $z = 5 e^{j\cdot 0{,}927}$.

> [!zadanie] Zadanie 4
> Oblicz $(1+j)^8$.
> > [!odpowiedz]- Odpowiedź
> > $1 + j = \sqrt2\, e^{j\pi/4}$, więc $(1+j)^8 = (\sqrt2)^8 e^{j2\pi} = 16$.

> [!zadanie] Zadanie 5
> Rozwiąż $s^2 + 4s + 13 = 0$.
> > [!odpowiedz]- Odpowiedź
> > $\Delta = 16 - 52 = -36$, $\sqrt\Delta = 6j$, $s = -2 \pm 3j$.

## Fiszki #fiszki/matematyka
Ile wynosi $j^2$?::$-1$
Postać algebraiczna liczby zespolonej?::$z = a + bj$, $a = \operatorname{Re} z$, $b = \operatorname{Im} z$
Moduł liczby $a + bj$?::$r = \sqrt{a^2 + b^2}$
Wzór Eulera?::$e^{j\varphi} = \cos\varphi + j\sin\varphi$
Sprzężenie $a + bj$?::$a - bj$; $z\bar z = |z|^2$
Jak mnożyć w postaci wykładniczej?::Moduły mnożysz, argumenty dodajesz
Jak dzielić liczby zespolone w postaci algebraicznej?::Pomnóż licznik i mianownik przez sprzężenie mianownika
Wzór de Moivre'a?::$(re^{j\varphi})^n = r^n e^{jn\varphi}$
Czemu w automatyce $j$, a nie $i$?::Bo $i$ oznacza prąd; matematycy piszą $i$, znaczy to samo
Pierwiastki $s^2 + 2s + 5 = 0$?::$s = -1 \pm 2j$
Co daje para biegunów $-\sigma \pm j\omega$ w Laplasie?::Drganie gasnące $e^{-\sigma t}$ razy $\cos\omega t$ / $\sin\omega t$

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.

## Źródła
- A. Milian, *Zbiór zadań z matematyki z rozwiązaniami*, cz. 1 (Wyd. PK) — liczby zespolone.
- J. Jurlewicz, Z. Skoczylas, *Algebra liniowa 1* — rozdział o liczbach zespolonych.
