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

> [!intuicja] Po ludzku
> **Argument liczby zespolonej** ($\\arg z$) to kąt, o jaki strzałka (wektor) $z$ jest obrócona względem osi rzeczywistej, w radianach. Przelicza się z części rzeczywistej/urojonej: $\\varphi = \\arg(a+bj) = \\arctan\\frac{b}{a}$ (z poprawką na ćwiartkę!. w dziedzinie $s$ argumenty transmitancji zamieniają się w **fazę** — po to Bodego rysuje się w log-skali.

> [!metafora] Jak dla idioty
> Zegar: strzałka długości $r$ obrócona o $\\varphi$ to liczba $re^{j\\varphi}$. Kąt na tarczy = argument. Faza transmitancji przy częstotliwości $\\omega$ to "o ile obiekt opóźnia/przyspiesza sygnał sinusoidalny" w tym punkcie. Dodawanie kątów to mnożenie, odejmowanie to dzielenie ([[Liczby zespolone|postaci wykładnicza]],.

## Definicja
> [!definicja]
> $$z = a+bj \\Rightarrow r=|z|=\\sqrt{a^2+b^2},\\quad \\varphi=\\arg z = \\operatorname{atan2}(b,a)$$
> ($\\operatorname{atan2}$ to "arctangens z poprawką na ćwiartkę" — inaczej dostaniesz kąt $\\pm180°$ błędny dla punktów w II/III ćwiartce.)

## Jak to działa— po co w automatyce
> [!wzor] Człon inercyjny a faza
> Dla $G(j\\omega)=\\frac{1}{j\\omega+1}$, faza spada od $0°$ do $-90°$ (na przejściu $-45°$ przy $\\omega=1$). To ten sam kąt z argumentu — obiekty o coraz szybszym wejściu "tracą fazę", aż do $-90°$. Bodego to wykres tej zależności w log-skali.

## Typowe błędy
> [!blad] Zapominasz o ćwiartce, wyciągasz zły kąt
> $z=-1+j$:$\\arctan(1/-1)=-45°$, ale faktyczny kąt to $135°$. Musisz użyć $\\operatorname{atan2}$ (dodaj $180°$ dla II/III ćwiartki).

> [!blad] Mylisz moduł z argumentem
> $|z|$ to długość (większa przy większej amplitudzie),$\\arg z$ to kąt (przesunięcie fazowe). Oba rysujesz na Bode— oddzielne wykresy.

## Zadania
> [!zadanie] Zadanie 1
> $\\arg(1+j)$.
> > [!odpowiedz]- Odpowiedź
> > $45°$ (I ćwiartka, $b=a$).

> [!zadanie] Zadanie 2
> $\\arg(-1+j)$.
> > [!odpowiedz]- Odpowiedź
> > $135°$ (a<0, b>0 → dodajesz 180° względem $-45°$).

## Fiszki #fiszki/matematyka
Argument liczby zespolonej::kąt wektora $z$ względem osi realnej = $\\operatorname{atan2}(b,a)$ (z poprawką ćwiartki)
Faza w Bodego::argument transmitancji $G(j\\omega)$ przy tej częstotliwości
Po co znać fazę?::żeby wiedzieć, czy obiekt opóźnia (ujemna faza) czy wzmacnia sygnał
