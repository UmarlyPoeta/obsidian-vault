---
typ: pojęcie
status: uczę się
przedmioty: ["[[PK Matematyka 2]]"]
wymaga: ["[[Liczby zespolone]]"]
powiązane: ["[[Transmitancja]]"]
tags: [pojęcie, matematyka]
utworzono: 2026-09-27
źródła: []
---
# Argument i faza (φ)

> [!skrot] W 30 sekund
> - **Co to:** argument $\varphi = \arg z$ to kąt, jaki strzałka $z = a + bj$ tworzy z dodatnią półosią rzeczywistą.
> - **Klucz:** $\cos\varphi = \frac{a}{r}$, $\sin\varphi = \frac{b}{r}$; $\arctan\frac{b}{a}$ wystarcza tylko dla $a > 0$, inaczej poprawka o $\pm\pi$.
> - **Kiedy:** postać wykładnicza $re^{j\varphi}$, mnożenie i dzielenie liczb zespolonych, faza [[Transmitancja|transmitancji]] $G(j\omega)$.

> [!intuicja] Po ludzku
> Moduł mówi, jak długa jest strzałka, argument — w którą stronę patrzy. W automatyce ten sam kąt nazywa się **fazą**: mówi, o ile sinusoida na wyjściu układu jest przesunięta względem sinusoidy na wejściu. To jak kompas: sama odległość nie wystarczy, żeby trafić do celu, potrzebny jest jeszcze kierunek.

Jednostkę urojoną piszemy $j$ (konwencja automatyki); w matematyce to $i$.

## Definicja i wzory
> [!definicja]
> Dla $z = a + bj \neq 0$, $r = |z| = \sqrt{a^2+b^2}$, argument $\varphi$ spełnia
> $$\cos\varphi = \frac{a}{r}, \qquad \sin\varphi = \frac{b}{r}.$$
> Argument główny $\operatorname{Arg} z$ wybiera się z przedziału $(-\pi, \pi]$ (w części podręczników $[0, 2\pi)$ — sprawdź, jak robi prowadzący). Dla $z = 0$ argument nie jest określony.

> [!wzor] Poprawka na ćwiartkę
> - $a > 0$: $\varphi = \arctan\frac{b}{a}$ (I i IV ćwiartka).
> - $a < 0$, $b \ge 0$: $\varphi = \arctan\frac{b}{a} + \pi$ (II ćwiartka i ujemna półoś).
> - $a < 0$, $b < 0$: $\varphi = \arctan\frac{b}{a} - \pi$ (III ćwiartka).
> - $a = 0$: $\varphi = \frac{\pi}{2}$ dla $b > 0$, $\varphi = -\frac{\pi}{2}$ dla $b < 0$.
>
> W programowaniu robi to funkcja `atan2(b, a)`.

> [!wzor] Reguły
> $\arg(z_1 z_2) = \arg z_1 + \arg z_2$, $\quad \arg\frac{z_1}{z_2} = \arg z_1 - \arg z_2$, $\quad \arg \bar z = -\arg z$ (z dokładnością do $2\pi$).

## Jak to zrobić — krok po kroku
1. Zaznacz punkt $(a, b)$ i ustal ćwiartkę.
2. Policz kąt pomocniczy $\alpha = \arctan\left|\frac{b}{a}\right|$ (ostry, często z tabeli: $\frac{\pi}{6}, \frac{\pi}{4}, \frac{\pi}{3}$).
3. Dobierz $\varphi$ z ćwiartki: I — $\alpha$, II — $\pi - \alpha$, III — $-(\pi - \alpha)$, IV — $-\alpha$.
4. Sprawdź znaki: $\cos\varphi$ ma znak $a$, $\sin\varphi$ ma znak $b$.

## Przykład
$z = -\sqrt3 + j$.

1. $a = -\sqrt3 < 0$, $b = 1 > 0$ — II ćwiartka.
2. $r = \sqrt{3 + 1} = 2$; $\alpha = \arctan\frac{1}{\sqrt3} = \frac{\pi}{6}$.
3. II ćwiartka: $\varphi = \pi - \frac{\pi}{6} = \frac{5\pi}{6}$ ($150°$).
4. Kontrola: $\cos\frac{5\pi}{6} = -\frac{\sqrt3}{2} = \frac{a}{r}$, $\sin\frac{5\pi}{6} = \frac12 = \frac{b}{r}$. Zatem $z = 2e^{j5\pi/6}$.

Uwaga: kalkulator dla $\arctan\frac{1}{-\sqrt3}$ zwróci $-\frac{\pi}{6}$ — to kąt w IV ćwiartce, czyli zły.

> [!przyklad]- Przykład 2 — faza członu inercyjnego
> $G(s) = \frac{1}{1+s}$, podstawiamy $s = j\omega$: $G(j\omega) = \frac{1}{1 + j\omega}$.
> Argument ilorazu: $\varphi(\omega) = \arg 1 - \arg(1 + j\omega) = 0 - \arctan\omega$.
> Dla $\omega = 1$: $\varphi = -\frac{\pi}{4} = -45°$, $|G| = \frac{1}{\sqrt2}$. Dla $\omega \to \infty$: $\varphi \to -90°$. Ujemna faza = wyjście opóźnia się względem wejścia.

## Typowe błędy
> [!blad] Sam $\arctan\frac{b}{a}$ dla $a < 0$
> $z = -1 + j$: $\arctan(-1) = -\frac{\pi}{4}$, ale punkt leży w II ćwiartce, więc $\varphi = \frac{3\pi}{4}$.

> [!blad] Mieszanie stopni i radianów
> W wykładniku $e^{j\varphi}$ i w kalkulatorze ustaw radiany; stopnie tylko do opisu (np. na wykresie Bodego).

> [!blad] Mylenie modułu z argumentem
> $|z|$ to długość (wzmocnienie), $\arg z$ to kąt (przesunięcie fazowe). To dwie różne informacje — na wykresie Bodego dwa osobne wykresy.

## Na zajęcia i egzamin
> [!zajecia]
> - **Ćwiczenia:** wyznaczanie argumentu z poprawką na ćwiartkę, zamiana na postać wykładniczą, potęgowanie z de Moivre'a.
> - **Egzamin:** „zapisz $z$ w postaci trygonometrycznej/wykładniczej”, „oblicz $z^n$”.
> - Łączy się z: [[Liczby zespolone]], [[Transmitancja]] (faza $\arg G(j\omega)$), [[Transformata Laplace'a]] (bieguny zespolone dają drgania; ich położenie opisuje się modułem i kątem), [[Transformata Fouriera]].

## Zadania
> [!zadanie] Zadanie 1
> Wyznacz $\arg(-1 - j)$.
> > [!odpowiedz]- Odpowiedź
> > III ćwiartka, $\alpha = \frac{\pi}{4}$, więc $\varphi = -\frac{3\pi}{4}$ (albo $\frac{5\pi}{4}$ w przedziale $[0, 2\pi)$).

> [!zadanie] Zadanie 2
> Wyznacz argumenty liczb $-2$ i $3j$.
> > [!odpowiedz]- Odpowiedź
> > $\arg(-2) = \pi$, $\arg(3j) = \frac{\pi}{2}$.

> [!zadanie] Zadanie 3
> Zapisz $z = 1 - j\sqrt3$ w postaci wykładniczej.
> > [!odpowiedz]- Odpowiedź
> > $r = 2$, IV ćwiartka, $\alpha = \frac{\pi}{3}$, $z = 2e^{-j\pi/3}$.

> [!zadanie] Zadanie 4
> Bez liczenia ilorazu wyznacz $\arg\frac{1+j}{1-j}$.
> > [!odpowiedz]- Odpowiedź
> > $\frac{\pi}{4} - \left(-\frac{\pi}{4}\right) = \frac{\pi}{2}$ — zgadza się z $\frac{1+j}{1-j} = j$.

> [!zadanie] Zadanie 5
> Oblicz fazę $G(j\omega) = \frac{1}{j\omega(1 + j\omega)}$ dla $\omega = 1$.
> > [!odpowiedz]- Odpowiedź
> > $\varphi = 0 - \frac{\pi}{2} - \arctan 1 = -\frac{3\pi}{4} = -135°$.

## Fiszki #fiszki/matematyka
Co to jest argument liczby zespolonej?::Kąt strzałki $z$ względem dodatniej półosi rzeczywistej
Przedział argumentu głównego?::Zwykle $(-\pi, \pi]$ (czasem $[0, 2\pi)$)
Kiedy $\varphi = \arctan\frac{b}{a}$ jest poprawne?::Tylko gdy $a > 0$ (I i IV ćwiartka)
Argument w II ćwiartce?::$\varphi = \pi - \arctan\left|\frac{b}{a}\right|$
Argument iloczynu?::Suma argumentów
Argument ilorazu?::Różnica argumentów
$\arg(-1)$?::$\pi$
$\arg(-j)$?::$-\frac{\pi}{2}$
Co to jest faza transmitancji?::$\arg G(j\omega)$ — przesunięcie sinusoidy wyjściowej względem wejściowej
Faza $\frac{1}{1 + j\omega}$ przy $\omega = 1$?::$-45°$
Ujemna faza oznacza?::Wyjście opóźnia się względem wejścia

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.

## Źródła
- A. Milian, *Zbiór zadań z matematyki z rozwiązaniami*, cz. 1 (Wyd. PK) — liczby zespolone.
- J. Jurlewicz, Z. Skoczylas, *Algebra liniowa 1* — postać trygonometryczna i wykładnicza.
