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

> [!intuicja] Po ludzku
> Liczby zespolone to **liczby 2D** — mają "długość" i "kierunek" (jak strzałka na mapie, wektor\. Zwykła liczba $a$ żyje na osi, zespolona $a+bj$ na płaszczyźnie. Wprowadzają się, bo okazuje się, że **$\\sqrt{-1}$ jednak istnieje** — to jednostka urojona $j$, dla której $j^2 = -1$. Bez nich nie dałbyś rady zapisać. co dzieje się z [[Transformata Laplace'a|Laplace'em]] dla **biegunów zespolonych**, czyli drgających rozwiązanych.



> [!metafora] Jak dla idioty
> Wyobraź sobie zegar ze wskazówką. Liczba zespolona $re^{j\\varphi}$ to wskazówka długości $r$ obrócona o kąt $\\varphi$. **Mnożenie dwóch liczb zespolonych = dodawanie kątów i mnożenie długości** — to jak obrót zegara o dwa wskazania. W elektrotechnice te "wskazówki" to prądy i napięcia (wskazowy; stąd ten trik jest wszędzie.


## Definicja
> [!definicja] Formy zapisu liczby zespolonej
> **Postać algebraiczna** (kartezjańska):$z = a + bj\qquad (a,b \\in \\mathbb R,\\ j^2=-1)$
> - $a = \\operatorname{Re} z$— część rzeczywista (oś x)
> - $b = \\operatorname{Im} z$— część urojona(oś y; to **liczba**, nie "jednostka"!
>
> **Postać trygonometryczna / wykładnicza**:
> $$z = r(\\cos\\varphi + j\\sin\\varphi) = r\\,e^{j\\varphi}$$
> - $r = |z| = \\sqrt{a^2+b^2}$ — moduł (długość strzałki
> - $\\varphi = \\arg z$ — argument(nkąt od osi x; $\\tan\\varphi = \\frac ba$ zależnie od ćwiartki
>
> **Zamiana** (co musisz robić płynnie):$a=r\\cos\\varphi,\\ b=r\\sin\\varphi$⟪dla sprzężenia:$\\bar z = a - bj$⟫mnożenie:$\\;re^{j\\varphi}\\,\\rho e^{j\\psi} = r\\rho\\,e^{j(\\varphi+\\psi)}$⟪obrót+długość!⟫sprzężona: zamiana $j\\to-j$ (odbicie po osi x).


## Jak to działa — dlaczego to ważne w Laplace'u:
- **Postać wykładnicza zamienia mnożenie w dodawanie kątów** — dokładnie jak logarytmy, których używasz w [[Transformata Laplace'a|Laplasie]].
 Analogia: Laplace zamienia całki w algebrę, a postać wykładnicza zamienia drgania w proste obrotyichw
- **Bieguny zespolone** $s=-\\sigma+j\\omega$ mówią o **drganiach gasnących**: $\\sigma$ = tłumienie (jak szybko gaśnie$, a $\\omega$ = częstość(jak szybko drga). Docz
- **Na osi urojonej** ($\\sigma=0$,$s=j\\omega$) dostajesz [[Transformata Fouriera|Fouriera]]— rozkład na częstotliwości.Zeń Związek Laplace-Fourier$=$ spójnyobraz, który Cię czeka w wyższych semestrach.


##Typowe błędy
> [!blad] Imaginujesz, że $bj$ to "$b$-ta kopia $j$"
> $b$ to liczba na osi urojonej, nie mnożnik przez "jednostkę".$z =3+4j$ ma część urojoną **$4$**, nie "cztery $j$".

> [!blad] Myślisz, że $j^2=1$
> $j\\cdot j = -1$. Dlatego $\\frac ba$ w $\\tan$ daje kąt, a nie odwrotny.

> [!blad] Nie umiesz odczytać fazy z ćwiartki
> $\\tan\\varphi = \\frac ba$ jest dwuznaczne($\\varphi$ i $\\varphi+\\pi$). Patrz, w której ćwiartce leży punkt $(a,b)$, żeby dobrać właściwy kątjąt.



## Gdzie ci się przyda
> [!przyda]
> - Zmienna $s$ w [[Transformata Laplace'a|Laplasie]] jest zespolona: $s = \\sigma+j\\omega$. Bieguny transmitancji to miejsca, gdzie mianownik $=0$ — one decydują o stabilności układu.

##Zadania
> [!zadanie] Zadanie 1
> Zapisz $z = 3+4j$ w postaci wykładniczej.

> > [!odpowiedz]- Odpowiedź
> > $r = \\sqrt{9+16} =5$; $\\varphi = \\arctan(4/3) \\approx 0.927$ rad ($\\approx53.13°$); $z = 5e^{j\\cdot0.927}$.

> [!zadanie] Zadanie 2
> Policz $(2e^{j30°})(3e^{j60°})$.
> > [!odpowiedz]- Odpowiedź
> > $6e^{j90°} =  ​​6j$ (mnożenie długości, dodawanie kątów.

> [!zadanie] Zadanie 3 — bieguna
> Gdzie leży biegun mianownika $s^2 + 2s + 5 =0$
> > [!odpowiedz]- Odpowiedź
> > $s = -1\\pm\\sqrt{1-5}=-1\\pm\\sqrt{-4}=-1\\pm2j$. Część rzeczywista $-1$A(u tłumienie), urojona $\\pm2$A(u częstość$\\omega=2$→ [[Drgania mechaniczne|drganie gasnące]]$.

##Fiszki #fiszki/matematyka
Definicja liczby zespolonej::$z = a+bj$ z $j^2=-1$; $a$=Re,$b$=Imeng
Postać wykładnicza liczby zespolonej::$re^{j\\varphi}$, $r=|z|$, $\\varphi=\\arg z$
Mnożenie liczb zespolonych( postać wykładnicza)::mnożysz $r$, dodajesz $\\varphi$$:$re^{j\\varphi}\\,\\rho e^{j\\psi}=r\\rho e^{j(\\varphi+\\psi)}
Co robi mnożenie przez $j$?::obraca o $90°$(bo $j=e^{j90°}$)
Zamiana na postać wykładniczą:: $a+bj=r(\\cos\\varphi+j\\sin\\varphi)$,$r=\\sqrt{a^2+b^2}$,$\\tan\\varphi=\\frac ba$
##Źródła
- A. Milian, *Zbiór zadań z matematyki* cz. 2 (PK) — rozdział o liczbach zespolonych