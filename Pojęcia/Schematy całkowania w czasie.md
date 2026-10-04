---
typ: pojęcie
status: nowe
aliases: ["Schemat jawny i niejawny", "Schemat theta"]
przedmioty: ["[[AGH Metoda elementów skończonych]]"]
wymaga: ["[[Numeryczne rozwiązywanie RRZ]]", "[[Nieustalony przepływ ciepła]]"]
powiązane: ["[[MES — kurs dr. Kustry (mapa labów)]]", "[[Macierz sztywności]]", "[[Układy równań liniowych]]", "[[Równanie różniczkowe zwyczajne]]"]
tags: [pojęcie, mes, metody-numeryczne]
utworzono: 2026-10-05
źródła: ["P. Kustra / A. Milenin — 6. Symulacja nieustalonych procesów cieplnych (AGH WIMiIP)", "H.P. Langtangen, S. Linge — Finite Difference Computing with PDEs (Springer 2017, CC BY), rozdz. 3", "H.P. Langtangen, K.-A. Mardal — Introduction to Numerical Methods for Variational Problems (Springer 2019, CC BY), rozdz. 7"]
---
# Schematy całkowania w czasie

> [!skrot] W 30 sekund
> - **Co to:** sposób przejścia od $[C]\dot{\{t\}} + [H]\{t\} = \{P\}$ do kroku $\{t_0\} \to \{t_1\}$. Różnią się tym, w której chwili liczysz człon $[H]\{t\}$.
> - **Klucz (schemat θ):** $\left(\frac{[C]}{\Delta\tau} + \theta[H]\right)\{t_1\} = \left(\frac{[C]}{\Delta\tau} - (1-\theta)[H]\right)\{t_0\} + \{P\}$. $\theta = 0$ to jawny, $\theta = 1$ niejawny (kurs), $\theta = 1/2$ Crank–Nicolson.
> - **Kiedy:** wybór schematu w projekcie i pytania egzaminacyjne o stabilność.

> [!intuicja] Po ludzku
> Jawny schemat patrzy tylko na „teraz”: szybki, ale przy dużym kroku przestrzela i wynik zaczyna oscylować aż do eksplozji. Niejawny pyta „jaka temperatura za chwilę jest zgodna z fizyką za chwilę”. Musi rozwiązać układ równań, ale nigdy się nie rozjeżdża. Crank–Nicolson bierze średnią z obu chwil, więc jest dokładniejszy, ale przy dużym kroku potrafi lekko oscylować.

## Definicja i wzory
> [!wzor] Rodzina schematów θ (w konwencji kodu: $[H]$ zawiera $[H_{bc}]$, $P > 0$)
> $$\left(\frac{[C]}{\Delta\tau} + \theta[H]\right)\{t_1\} = \left(\frac{[C]}{\Delta\tau} - (1-\theta)[H]\right)\{t_0\} + \{P\}$$
>
> | $\theta$ | Nazwa | Rząd w czasie | Stabilność |
> |---|---|---|---|
> | 0 | jawny (Euler w przód) | 1 | tylko gdy $\Delta\tau < \frac{2}{\lambda_{max}}$, gdzie $\lambda_{max}$ to największa wartość własna $[C]^{-1}[H]$ |
> | 1/2 | Crank–Nicolson | **2** | bezwarunkowa, ale przy dużym $\Delta\tau$ możliwe oscylacje |
> | 1 | niejawny (Euler wstecz), **kurs** | 1 | bezwarunkowa, bez oscylacji |
>
> Dla jawnego schematu z macierzą $[C]$ diagonalną (skupioną) nie trzeba rozwiązywać układu: $t_1 = t_0 + \Delta\tau\,C^{-1}(P - Ht_0)$ to dzielenie przez przekątną.

> [!wzor] Szacowanie kroku krytycznego (jawny)
> Dla elementu o boku $h$: $\Delta\tau_{kr} \sim \frac{h^2}{a}\cdot\text{stała}$, gdzie $a = \frac{k}{\rho c}$ to dyfuzyjność cieplna. Zagęszczenie siatki 2× zmniejsza dopuszczalny krok 4×.

## Jak to zrobić — krok po kroku
1. Wybierz $\theta$ (w kursie $\theta = 1$).
2. Zbuduj raz $A = [C]/\Delta\tau + \theta[H]$ i $B = [C]/\Delta\tau - (1-\theta)[H]$.
3. W każdym kroku: $b = B\{t_0\} + \{P\}$, rozwiąż $A\{t_1\} = b$.
4. Badanie zbieżności w czasie: powtórz z $\Delta\tau/2$ i $\Delta\tau/4$. Różnica powinna maleć ~2× (θ = 1) albo ~4× (θ = 1/2).

## Przykład
> [!przyklad] Test1_4_4 po 500 s, różne schematy i kroki (solver `mes2d.py`, ta sama siatka)
> Dla tej siatki i spójnej $[C]$: $\lambda_{max}([C]^{-1}[H]) = 0{,}1059$ 1/s, więc jawny schemat jest stabilny tylko dla $\Delta\tau < 2/0{,}1059 = 18{,}9$ s.
>
> | Schemat | $\Delta\tau = 50$ s | $\Delta\tau = 10$ s | $\Delta\tau = 1$ s |
> |---|---|---|---|
> | jawny ($\theta = 0$) | **rozbiega się** (min $-19\,459$, max $9\,864$) | 704,9 / 896,4 | 701,0 / 894,0 |
> | Crank–Nicolson ($\theta = 1/2$) | 700,9 / 894,0 | 700,6 / 893,8 | 700,55 / 893,76 |
> | niejawny ($\theta = 1$, kurs) | **679,9 / 881,1** (wynik testowy) | 696,3 / 891,1 | 700,1 / 893,5 |
>
> Wnioski: (1) jawny przy kroku większym od krytycznego daje bzdury; (2) niejawny jest stabilny, ale przy $\Delta\tau = 50$ s zaniża temperaturę o ok. 20 K; (3) Crank–Nicolson już przy 50 s jest bliżej rozwiązania „dokładnego” (ok. 700,5 / 893,8) niż niejawny przy 10 s.

```python
import numpy as np
def krok_theta(H, C, P, t0, dt, theta):
    A = C / dt + theta * H
    B = C / dt - (1 - theta) * H
    return np.linalg.solve(A, B @ t0 + P)
```

## Typowe błędy
> [!blad] „Niejawny jest stabilny, więc krok może być dowolny”
> Stabilny nie znaczy dokładny. Krok dobierasz tak, żeby wynik nie zmieniał się istotnie po jego zmniejszeniu. To jest punkt sprawozdania.

> [!blad] Zły znak przy $(1-\theta)[H]$
> Przy $\theta = 1$ ten człon znika i błąd się nie ujawni. Ujawni się dopiero przy Cranku–Nicolsonie. Sprawdź oba warianty na Test1.

## Na zajęcia i egzamin
> [!zajecia]
> - **Projekt:** kurs wymaga schematu niejawnego. Crank–Nicolson albo porównanie schematów to dobry dodatek do sprawozdania (ocena powyżej 3.0).
> - **Egzamin:** „wyprowadź schemat jawny i niejawny z interpolacji liniowej w czasie”, „który jest stabilny i dlaczego”, „co oznacza warunek $\Delta\tau < \Delta\tau_{kr}$”.
> - Łączy się z: [[Numeryczne rozwiązywanie RRZ]] (ten sam schemat θ dla RRZ), [[Nieustalony przepływ ciepła]].

## Zadania
> [!zadanie] Zadanie 1
> Zagęszczasz siatkę Test1 z 3×3 do 6×6 elementów. Jak zmieni się krok krytyczny schematu jawnego?
> > [!odpowiedz]- Odpowiedź
> > $\Delta\tau_{kr} \sim h^2$, a $h$ maleje 2×, więc krok krytyczny maleje ok. 4×, z ok. 19 s do ok. 5 s.

> [!zadanie] Zadanie 2
> Zapisz schemat θ dla $\theta = 1$ i pokaż, że to wzór z kursu.
> > [!odpowiedz]- Odpowiedź
> > $\left(\frac{C}{\Delta\tau} + H\right)t_1 = \frac{C}{\Delta\tau}t_0 + P$, czyli $\left(H + \frac{C}{\Delta\tau}\right)t_1 = \frac{C}{\Delta\tau}t_0 + P$. To jest wzór (6.8) w konwencji $P > 0$.

## Fiszki #fiszki/mes
Schemat θ dla MES nieustalonego::(C/Δτ + θH)·t1 = (C/Δτ − (1−θ)H)·t0 + P
θ = 0, ½, 1 to odpowiednio::jawny (Euler w przód), Crank–Nicolson, niejawny (Euler wstecz)
Który schemat stosuje się w kursie MES Kustry?::Niejawny (θ = 1)
Rząd dokładności w czasie: Euler wstecz i Crank–Nicolson::1 i 2
Warunek stabilności schematu jawnego::Δτ < 2/λ_max(C⁻¹H); maleje jak h²
Krok krytyczny jawnego schematu dla Test1_4_4::Ok. 18,9 s (przy Δτ = 50 s rozwiązanie się rozbiega)

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.

## Źródła
- P. Kustra / A. Milenin, *6. Symulacja nieustalonych procesów cieplnych*: [[09 Rozwiązanie niestacjonarne.pdf]]
- H.P. Langtangen, S. Linge, *Finite Difference Computing with PDEs*, Springer 2017 (CC BY), rozdz. 3 „Diffusion equations”, w tym 3.2.4 *The unifying θ rule*: [[Langtangen, Linge — Finite Difference Computing with PDEs (2017, CC BY).pdf]]
- H.P. Langtangen, K.-A. Mardal, *Introduction to Numerical Methods for Variational Problems*, rozdz. 7.1–7.2: [[Langtangen, Mardal — Introduction to Numerical Methods for Variational Problems (2019, CC BY).pdf]]
- Liczby w tabeli: `Załączniki/MES/kod/mes2d.py` (sprawdzony na wynikach prowadzącego).
