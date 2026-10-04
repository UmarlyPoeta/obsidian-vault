---
typ: pojęcie
status: nowe
aliases: ["Macierz C", "Macierz pojemności cieplnej"]
przedmioty: ["[[AGH Metoda elementów skończonych]]"]
wymaga: ["[[Macierz sztywności]]", "[[Jakobian]]", "[[Numeryczne rozwiązywanie RRZ]]"]
powiązane: ["[[MES — kurs dr. Kustry (mapa labów)]]", "[[Schematy całkowania w czasie]]", "[[Metoda Galerkina]]", "[[MES 1D — od różnic skończonych do macierzy sztywności]]", "[[Układy równań liniowych]]"]
tags: [pojęcie, mes]
utworzono: 2026-10-05
źródła: ["P. Kustra / A. Milenin — 6. Symulacja nieustalonych procesów cieplnych; 9.1 Sformułowanie problemu (AGH WIMiIP)", "P. Kustra — Całkowanie macierzy [C] metodą Gaussa (AGH WIMiIP)", "P. Kustra — TestCase 2D transient solution", "A. Milenin — Podstawy metody elementów skończonych (Wyd. AGH 2010)"]
---
# Nieustalony przepływ ciepła

> [!skrot] W 30 sekund
> - **Co to:** temperatura zmienia się w czasie, więc do $[H]\{t\} + \{P\} = 0$ dochodzi człon akumulacji ciepła $[C]\frac{\partial\{t\}}{\partial\tau}$.
> - **Klucz:** $[C] = \int_V c\rho\{N\}\{N\}^T dV$ i schemat niejawny $\left([H] + \frac{[C]}{\Delta\tau}\right)\{t_1\} = \frac{[C]}{\Delta\tau}\{t_0\} + \{P\}$, powtarzany w każdym kroku czasowym ($\{t_0\} \leftarrow \{t_1\}$).
> - **Kiedy:** lab 9 i finał projektu: siatki testowe, problem rzeczywisty, sprawozdanie.

> [!intuicja] Po ludzku
> $H$ mówi, jak szybko ciepło przepływa, a $C$ ile ciepła trzeba „wlać” w węzeł, żeby podnieść jego temperaturę o 1 K (jak pojemność wiadra). Duże $\rho c$ to duże wiadro: stal nagrzewa się wolno. W każdym kroku pytasz: jaka będzie temperatura za $\Delta\tau$, jeśli wiem, jaka jest teraz? Odpowiedź daje jeden układ równań na krok.

## Definicja i wzory
> [!definicja] Równanie
> $$\operatorname{div}(k\,\operatorname{grad} t) + Q = c\rho\frac{\partial t}{\partial\tau}$$
> W danej chwili czasu pochodną $\partial t/\partial\tau$ traktuje się jak dodatkowe źródło $Q$ w funkcjonale ze stanu ustalonego. Wynik:
> $$[H]\{t\} + [C]\frac{\partial\{t\}}{\partial\tau} + \{P\} = 0, \qquad [C] = \int_V c\rho\{N\}\{N\}^T dV$$

> [!wzor] Dyskretyzacja w czasie (rozdz. 6 „Rozwiązania niestacjonarnego”)
> Liniowo w kroku: $\frac{\partial\{t\}}{\partial\tau} \approx \frac{\{t_1\} - \{t_0\}}{\Delta\tau}$. Zależnie od tego, gdzie weźmiesz $\{t\}$ przy $[H]$:
> - $\{t\} = \{t_0\}$, **schemat jawny:** $\{t_1\} = \{t_0\} - \Delta\tau[C]^{-1}([H]\{t_0\} + \{P\})$. Słaba stabilność.
> - $\{t\} = \{t_1\}$, **schemat niejawny** (używany w kursie):
> $$\left([H] + \frac{[C]}{\Delta\tau}\right)\{t_1\} - \frac{[C]}{\Delta\tau}\{t_0\} + \{P\} = 0$$
> W kodzie, z $P > 0$ jak w plikach prowadzącego: $[\hat H] = [H + H_{bc}] + [C]/\Delta\tau$, $\{\hat P\} = \{P\} + [C]/\Delta\tau \cdot \{t_0\}$, rozwiązujesz $[\hat H]\{t_1\} = \{\hat P\}$.
> Więcej schematów i stabilność: [[Schematy całkowania w czasie]].

> [!wzor] Macierz $[C]$ elementu w punktach Gaussa (lab 9)
> $$[C]_e \approx \sum_{p} c\rho\,\{N_p\}\{N_p\}^T\,w_p\,\det J_p$$
> W pc1 przy 2 punktach: $N = [0{,}6220;\ 0{,}1667;\ 0{,}0447;\ 0{,}1667]$.
> **Wynik kontrolny:** element prostokątny o polu $A$:
> $$[C] = \frac{c\rho A}{36}\begin{bmatrix} 4 & 2 & 1 & 2 \\ 2 & 4 & 2 & 1 \\ 1 & 2 & 4 & 2 \\ 2 & 1 & 2 & 4 \end{bmatrix}$$
> Suma wszystkich wyrazów $[C]_e$ to $c\rho A$, czyli pojemność cieplna elementu (na metr grubości).

## Jak to zrobić — krok po kroku
1. Liczysz $[C]_e$ w tej samej pętli co $[H]_e$: te same punkty, ten sam $\det J$, tylko zamiast pochodnych bierzesz $N N^T$.
2. Agregujesz $[C]$ tak samo jak $[H]$.
3. Raz budujesz $[\hat H] = [H + H_{bc}] + [C]/\Delta\tau$ (nie zmienia się w czasie, gdy własności są stałe).
4. $\{t_0\} = $ `InitialTemp` we wszystkich węzłach.
5. Pętla po krokach $\tau = \Delta\tau, 2\Delta\tau, \ldots$, `SimulationTime`: $\{\hat P\} = \{P\} + [C]/\Delta\tau\,\{t_0\}$, rozwiąż, wypisz min i max, $\{t_0\} \leftarrow \{t_1\}$.
6. Porównaj z tabelą w [[MES — kurs dr. Kustry (mapa labów)]].

## Przykład
> [!przyklad] Test1_4_4, pierwszy krok
> **Dane:** $c\rho = 700 \cdot 7800 = 5{,}46\cdot10^6$ J/(m³K), element $h = 1/30$ m, $A = h^2 = 1/900$ m², $\Delta\tau = 50$ s.
>
> **1. C elementu:** $\frac{c\rho A}{36} = \frac{5{,}46\cdot10^6}{900 \cdot 36} = 168{,}52$. Przekątna $4 \cdot 168{,}52 = 674{,}07$, sąsiedzi $337{,}04$, naprzeciw $168{,}52$. Plik *TestCase*: $C^G_{11} = 674{,}074$, $C^G_{12} = 337{,}037$, $C^G_{16} = 168{,}519$.
>
> **2. Macierz układu w węźle 1:** $\hat H_{11} = (H + H_{bc})_{11} + C_{11}/\Delta\tau = 23{,}333 + 674{,}074/50 = 23{,}333 + 13{,}481 = 36{,}815$. Tak jest w *TestCase*.
>
> **3. Prawa strona w węźle 1:** $\hat P_1 = P_1 + \frac{100}{50}\sum_j C_{1j} = 12000 + 2 \cdot (674{,}07 + 2 \cdot 337{,}04 + 168{,}52) = 12000 + 3033{,}3 = 15\,033{,}3$. Też zgadza się z plikiem.
>
> **4. Wynik po 50 s:** min $110{,}04$ °C, max $365{,}82$ °C. Po 500 s: $679{,}91$ i $881{,}06$ °C.

```python
import numpy as np
# H, C, P zagregowane jak w mes2d.py (H zawiera już Hbc)
def symuluj(H, C, P, t0, dt, czas):
    A = H + C / dt                    # stała macierz układu
    t = np.full(len(P), t0)
    for krok in range(1, int(round(czas / dt)) + 1):
        t = np.linalg.solve(A, C / dt @ t + P)
        print(f"{krok * dt:6.0f}  {t.min():10.3f}  {t.max():10.3f}")
    return t
```

## Typowe błędy
> [!blad] Budowanie $\hat H$ od nowa w każdym kroku
> Przy stałych $k$, $\rho$, $c$ i $\Delta\tau$ macierz $[\hat H]$ się nie zmienia. Buduj ją raz (a w C++ możesz nawet raz zrobić rozkład LU). Zmienia się tylko $\{\hat P\}$.

> [!blad] Dodawanie $C/\Delta\tau \cdot t_0$ do $P$ na stałe
> Jeśli zrobisz `P += C/dt * t0` na wektorze $P$ z konwekcji, w następnym kroku dodasz kolejny raz. Trzymaj $\{P\}$ z konwekcji osobno i w każdym kroku licz $\{\hat P\}$ od nowa.

> [!blad] Zbyt duży krok czasowy
> Schemat niejawny jest stabilny przy każdym $\Delta\tau$, ale nie zawsze dokładny. Dla Test1 przy $\Delta\tau = 50$ s po 500 s max wynosi $881{,}1$ °C, a przy $\Delta\tau = 1$ s już $893{,}5$ °C. W sprawozdaniu pokaż, jak wynik zależy od kroku.

> [!blad] Mieszanie konwencji znaku $P$
> W teorii $[H]\{t\} + [C]\dot t + \{P\} = 0$ z $P < 0$, a w kodzie $P > 0$ i $[\hat H]t_1 = \hat P$. Objaw pomieszania: ciało stygnie poniżej 100 °C zamiast się nagrzewać.

## Na zajęcia i egzamin
> [!zajecia]
> - **Lab 9:** $[C]$ lokalna i globalna, pętla czasowa, tabela min/max. Porównujesz z plikiem *TestCase* (Test1) i z wynikami MixGrid i 31×31 w mapie kursu.
> - **Sprawozdanie:** wyniki dla 3 siatek, wpływ $\Delta\tau$ i liczby punktów całkowania, rozkład temperatury (np. ParaView z plików `.vtu`), problem rzeczywisty (np. ściana wielowarstwowa, hartowanie, radiator).
> - **Egzamin:** „wyprowadź schemat jawny i niejawny”, „dlaczego niejawny jest stabilny”, „wzór na $[C]$”, „co się stanie, gdy wyzerujesz $[C]$” (dostaniesz stan ustalony).
> - Łączy się z: [[Schematy całkowania w czasie]], [[Numeryczne rozwiązywanie RRZ]], [[Macierz sztywności]].

## Zadania
> [!zadanie] Zadanie 1
> Policz $C_{11}$ elementu kwadratowego $0{,}025 \times 0{,}025$ m dla $\rho = 7800$, $c = 700$.
> > [!odpowiedz]- Odpowiedź
> > $\frac{c\rho A}{36} \cdot 4 = \frac{5{,}46\cdot10^6 \cdot 6{,}25\cdot10^{-4}}{9} = 379{,}17$. Na slajdzie prowadzącego wkład samego pc1 to $330{,}07$, a suma po 4 punktach daje właśnie $379{,}17$.

> [!zadanie] Zadanie 2
> Co się stanie, gdy w programie ustawisz $[C] = 0$?
> > [!odpowiedz]- Odpowiedź
> > Zostaje $[H]\{t\} = \{P\}$, czyli stan ustalony. Dla Test1 wszystkie temperatury wyjdą $1200$ °C już w pierwszym kroku.

> [!zadanie] Zadanie 3
> Dlaczego w Test3 (31×31, $\Delta\tau = 1$ s) temperatura minimalna przez pierwsze sekundy wynosi dokładnie 100 °C, a w Test1 nie?
> > [!odpowiedz]- Odpowiedź
> > W Test3 środek płytki jest 5 cm od brzegu, a ciepło przez kilka sekund nie zdąży tam dotrzeć (dyfuzyjność stali $a = k/(\rho c) \approx 4{,}6\cdot10^{-6}$ m²/s, droga $\sqrt{a\tau} \approx 5$ mm po 5 s). W Test1 każdy węzeł leży co najwyżej 3,3 cm od brzegu, a krok wynosi 50 s, więc ciepło dociera wszędzie już w pierwszym kroku.

## Fiszki #fiszki/mes
Równanie MES dla procesu nieustalonego::[H]{t} + [C]·∂{t}/∂τ + {P} = 0
Wzór na macierz C::[C] = ∫ c·ρ·{N}{N}ᵀ dV
Schemat niejawny w kursie Kustry::([H] + [C]/Δτ)·{t1} − ([C]/Δτ)·{t0} + {P} = 0
Schemat jawny::{t1} = {t0} − Δτ·[C]⁻¹·([H]{t0} + {P}); słaba stabilność
Macierz C elementu prostokątnego o polu A::cρA/36 · [[4,2,1,2], [2,4,2,1], [1,2,4,2], [2,1,2,4]]
Co się zmienia w pętli czasowej, a co nie?::Ĥ = H + C/Δτ stała; P̂ = P + (C/Δτ)·t0 liczony w każdym kroku
Co da wyzerowanie [C]?::Rozwiązanie stanu ustalonego
C_11 globalne dla Test1_4_4::674,074 (narożny węzeł, jeden element)

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.

## Źródła
- P. Kustra / A. Milenin, *6. Symulacja nieustalonych procesów cieplnych* i *9.1 Sformułowanie problemu*: ![[09 Rozwiązanie niestacjonarne.pdf]]
- P. Kustra, *Całkowanie macierzy [C] metodą Gaussa*: [[09.1 Całkowanie macierzy C.pdf]]
- P. Kustra, *Test case 2D transient solution*: [[TestCase — rozwiązanie niestacjonarne (min, max T).pdf]]
- A. Milenin, *Podstawy metody elementów skończonych*, Wydawnictwa AGH 2010, rozdz. 6 i 9.
- H.P. Langtangen, K.-A. Mardal, *Introduction to Numerical Methods for Variational Problems*, rozdz. 7 (Forward i Backward Euler dla równania dyfuzji w MES).
