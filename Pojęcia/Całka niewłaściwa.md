---
typ: pojęcie
status: uczę się
przedmioty: ["[[PK Matematyka 1]]", "[[PK Matematyka 2]]"]
wymaga: ["[[Całka oznaczona]]", "[[Granica (lim)]]"]
powiązane: ["[[Transformata Laplace'a]]", "[[Funkcja wykładnicza]]"]
tags: [pojęcie, matematyka]
utworzono: 2026-09-27
źródła: []
---
# Całka niewłaściwa

> [!skrot] W 30 sekund
> - **Co to:** całka po przedziale nieskończonym albo z funkcji, która gdzieś „wybucha” do $\infty$.
> - **Klucz:** $\int_a^{\infty} f(x)\,dx = \lim_{b\to\infty}\int_a^{b} f(x)\,dx$ — najpierw zwykła całka, potem granica.
> - **Kiedy:** definicja [[Transformata Laplace'a|transformaty Laplace'a]], pola „do nieskończoności”, rachunek prawdopodobieństwa.

> [!intuicja] Po ludzku
> Zwykła [[Całka oznaczona]] liczy pole między dwiema liczbami. Tu jeden koniec ucieka do nieskończoności (albo funkcja ma pionową asymptotę), więc liczysz pole „do $b$” i patrzysz, co się dzieje, gdy $b\to\infty$. Jeśli pole się ustala — całka jest **zbieżna**. Jak rynna, która się zwęża: nieskończenie długa, a mieści skończenie dużo wody.

## Definicja i wzory
> [!definicja] Dwa rodzaje
> **1. Nieskończona granica całkowania:**
> $$\int_a^{\infty} f(x)\,dx = \lim_{b\to\infty}\int_a^{b} f(x)\,dx$$
> **2. Funkcja nieograniczona przy końcu przedziału** (np. $\frac{1}{x}$ przy $x=0$):
> $$\int_0^{a} f(x)\,dx = \lim_{\varepsilon\to 0^+}\int_{\varepsilon}^{a} f(x)\,dx$$
> Granica skończona — całka **zbieżna**; granica nieskończona albo nie istnieje — **rozbieżna**.

> [!wzor] Wzorce do porównań
> - $\int_1^{\infty} \frac{dx}{x^p}$ zbieżna $\iff p>1$, wtedy równa $\frac{1}{p-1}$.
> - $\int_0^{1} \frac{dx}{x^p}$ zbieżna $\iff p<1$, wtedy równa $\frac{1}{1-p}$.
> - $\int_0^{\infty} e^{-ax}\,dx = \frac{1}{a}$ dla $a>0$; dla $a\le 0$ rozbieżna.

**Kryterium porównawcze:** jeśli $0\le f(x)\le g(x)$ i $\int g$ jest zbieżna, to $\int f$ też. Jeśli $\int f$ rozbieżna, to $\int g$ też.

## Jak to zrobić — krok po kroku
1. Zamień „złą” granicę na literę: $\infty \to b$ (albo punkt osobliwy $\to \varepsilon$).
2. Policz zwykłą całkę oznaczoną: funkcja pierwotna, podstawienie granic.
3. Weź granicę $b\to\infty$ (lub $\varepsilon\to 0^+$). Skończona liczba — wynik; $\infty$ — rozbieżna.
4. Gdy funkcji pierwotnej nie da się policzyć — porównaj z wzorcem ($x^{-p}$ albo $e^{-ax}$).

## Przykład
Policz $\int_0^{\infty} t\,e^{-st}\,dt$ dla $s>0$ (to $\mathcal{L}\{t\}$).

1. Zamiana: $\lim_{b\to\infty}\int_0^{b} t\,e^{-st}\,dt$.
2. Przez części: $u=t$, $dv=e^{-st}dt$, więc $du=dt$, $v=-\frac{1}{s}e^{-st}$:
$$\int_0^{b} t\,e^{-st}\,dt = \left[-\frac{t}{s}e^{-st}\right]_0^{b} + \frac{1}{s}\int_0^{b} e^{-st}\,dt = -\frac{b}{s}e^{-sb} + \frac{1}{s^2}\left(1-e^{-sb}\right)$$
3. Granica: dla $s>0$ mamy $e^{-sb}\to 0$ i $b\,e^{-sb}\to 0$ (wykładnicza wygrywa z wielomianem), więc
$$\int_0^{\infty} t\,e^{-st}\,dt = \frac{1}{s^2}$$

To właśnie wiersz $\mathcal{L}\{t\} = \frac{1}{s^2}$ z tablicy transformat.

> [!przyklad]- Przykład 2 — zbieżna: $\int_1^{\infty} \frac{dx}{x^2}$
> $$\lim_{b\to\infty}\left[-\frac{1}{x}\right]_1^{b} = \lim_{b\to\infty}\left(1-\frac{1}{b}\right) = 1$$

> [!przyklad]- Przykład 3 — rozbieżna: $\int_1^{\infty} \frac{dx}{x}$
> $$\lim_{b\to\infty}\left[\ln x\right]_1^{b} = \lim_{b\to\infty}\ln b = \infty$$
> Funkcja maleje do zera, a pole i tak jest nieskończone — maleje za wolno.

> [!przyklad]- Przykład 4 — drugi rodzaj: $\int_0^{1} \frac{dx}{\sqrt{x}}$
> $$\lim_{\varepsilon\to 0^+}\left[2\sqrt{x}\right]_{\varepsilon}^{1} = \lim_{\varepsilon\to 0^+}\left(2-2\sqrt{\varepsilon}\right) = 2$$

## Typowe błędy
> [!blad] „Skoro $f(x)\to 0$, to całka jest zbieżna”
> $\frac{1}{x}\to 0$, a $\int_1^{\infty}\frac{dx}{x}$ jest rozbieżna. Liczy się **tempo** malenia: szybciej niż $\frac{1}{x}$.

> [!blad] Wstawianie $\infty$ jak liczby
> Pisz granicę jawnie. $b\,e^{-sb}$ to symbol „$\infty\cdot 0$” — trzeba go policzyć (np. regułą de l'Hospitala), a nie zgadywać.

> [!blad] Przeoczona osobliwość w środku przedziału
> $\int_{-1}^{1}\frac{dx}{x^2}$ to nie $-2$: funkcja wybucha w $x=0$, całka jest rozbieżna. Dziel przedział w punkcie osobliwym.

## Na zajęcia i egzamin
> [!zajecia]
> - **Ćwiczenia:** zbadać zbieżność i policzyć wartość, porównać z $\frac{1}{x^p}$.
> - **Egzamin:** „Oblicz z definicji transformatę Laplace'a funkcji $e^{at}$ albo $t$” — to zawsze całka niewłaściwa.
> - Łączy się z: [[Transformata Laplace'a]], [[Funkcja wykładnicza]], [[Granica (lim)]].

## Zadania
> [!zadanie] Zadanie 1
> Oblicz $\int_0^{\infty} e^{-2t}\,dt$.
> > [!odpowiedz]- Odpowiedź
> > $\lim_{b\to\infty}\left[-\frac{1}{2}e^{-2t}\right]_0^{b} = \frac{1}{2}$.

> [!zadanie] Zadanie 2
> Oblicz $\int_0^{\infty} e^{at}e^{-st}\,dt$. Dla jakich $s$ jest zbieżna?
> > [!odpowiedz]- Odpowiedź
> > $\int_0^{\infty} e^{-(s-a)t}\,dt = \frac{1}{s-a}$, zbieżna dla $s>a$. To $\mathcal{L}\{e^{at}\}$.

> [!zadanie] Zadanie 3
> Oblicz $\int_1^{\infty} \frac{dx}{x^3}$.
> > [!odpowiedz]- Odpowiedź
> > $\lim_{b\to\infty}\left[-\frac{1}{2x^2}\right]_1^{b} = \frac{1}{2}$.

> [!zadanie] Zadanie 4
> Oblicz $\int_0^{4} \frac{dx}{\sqrt{x}}$.
> > [!odpowiedz]- Odpowiedź
> > $\lim_{\varepsilon\to 0^+}\left[2\sqrt{x}\right]_{\varepsilon}^{4} = 4$.

> [!zadanie] Zadanie 5
> Oblicz $\int_0^{\infty} \frac{dx}{1+x^2}$.
> > [!odpowiedz]- Odpowiedź
> > $\lim_{b\to\infty}\operatorname{arctg} b = \frac{\pi}{2}$.

## Fiszki #fiszki/matematyka
Jak liczysz całkę z granicą ∞?::Zamieniasz ∞ na b, liczysz zwykłą całkę oznaczoną i bierzesz granicę b→∞.
Kiedy całka niewłaściwa jest zbieżna?::Gdy granica istnieje i jest skończona.
Kiedy ∫ od 1 do ∞ z 1/x^p jest zbieżna?::Dla p>1; wtedy wynosi 1/(p−1).
Kiedy ∫ od 0 do 1 z 1/x^p jest zbieżna?::Dla p<1; wtedy wynosi 1/(1−p).
Ile wynosi ∫ od 0 do ∞ z e^(−at) dla a>0?::1/a.
Czy ∫ od 1 do ∞ z 1/x jest zbieżna?::Nie — wynik to lim ln b = ∞.
Co mówi kryterium porównawcze?::Jeśli 0≤f≤g i ∫g zbieżna, to ∫f zbieżna; jeśli ∫f rozbieżna, to ∫g rozbieżna.
Jaka całka niewłaściwa definiuje transformatę Laplace'a?::F(s) = ∫ od 0 do ∞ z f(t)·e^(−st) dt.
Ile wynosi ∫ od 0 do ∞ z t·e^(−st) dla s>0?::1/s² (przez części).
Czy f(x)→0 wystarcza do zbieżności ∫ do ∞?::Nie — f musi maleć dość szybko (np. 1/x maleje, a całka jest rozbieżna).

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.

## Źródła
- A. Milian, *Zbiór zadań z matematyki z rozwiązaniami* cz. 1–2 (Wyd. PK) — całki niewłaściwe
- M. Gewert, Z. Skoczylas, *Analiza matematyczna 1* (GiS) — całki niewłaściwe, kryteria zbieżności
