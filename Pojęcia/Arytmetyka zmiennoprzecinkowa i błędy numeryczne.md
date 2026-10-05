---
typ: pojęcie
status: nowe
przedmioty: ["[[AGH Metoda elementów skończonych]]", "[[AGH Optymalizacja]]", "[[AGH Programowanie równoległe]]"]
wymaga: ["[[Pochodna]]"]
powiązane: ["[[Rozwiązywanie równań nieliniowych]]", "[[Układy równań liniowych]]", "[[Różniczkowanie numeryczne]]", "[[Całkowanie numeryczne i kwadratury Gaussa]]", "[[Numeryczne rozwiązywanie RRZ]]"]
tags: [pojęcie, metody-numeryczne]
utworzono: 2026-10-02
źródła: ["Fortuna, Macukow, Wąsowski — Metody numeryczne (WNT)", "Kincaid, Cheney — Analiza numeryczna (WNT)", "Burden, Faires — Numerical Analysis (Brooks/Cole), rozdz. 1", "D. Goldberg — What Every Computer Scientist Should Know About Floating-Point Arithmetic (ACM Computing Surveys 23(1), 1991)", "N. J. Higham — Accuracy and Stability of Numerical Algorithms (SIAM 2002)", "Dokumentacja numpy: numpy.finfo, numpy.spacing", "Kurs MN w C++: Kod/metody_numeryczne (moduły, testy, rozwiązania wzorcowe)"]
---
# Arytmetyka zmiennoprzecinkowa i błędy numeryczne

> [!skrot] W 30 sekund
> - **Co to:** komputer liczy na skończonej siatce liczb (double: 52 bity mantysy), więc każde działanie ma błąd względny rzędu $\varepsilon \approx 2{,}2\cdot10^{-16}$.
> - **Klucz:** $\mathrm{fl}(x) = x(1+\delta)$, $|\delta| \le \varepsilon/2$. Odejmowanie bliskich liczb niszczy cyfry. Błąd wyniku ≈ **uwarunkowanie zadania** × błąd danych, a algorytm **stabilny** nie dokłada od siebie więcej.
> - **Kiedy:** zawsze, gdy oceniasz, czy wynik (Newtona, Gaussa, MES) jest wiarygodny i jak szybko metoda zbiega.

> [!intuicja] Po ludzku
> Liczba w komputerze to jak odczyt z linijki z podziałką: między kreskami nic nie ma, więc wszystko zaokrąglasz do najbliższej kreski. Podziałka jest gęsta przy zerze i rzadka przy dużych liczbach (zawsze ok. 16 cyfr znaczących). Jeśli odejmiesz dwa prawie równe odczyty, zostaje sama niepewność odczytu, czyli śmieci.

## Definicja i wzory
> [!definicja] IEEE 754 double (float64, `float` w Pythonie)
> 1 bit znaku, 11 bitów wykładnika, 52 bity mantysy (plus ukryta jedynka):
> $$x = (-1)^s \cdot 1{,}m_1 m_2 \dots m_{52} \cdot 2^{e}, \qquad e \in [-1022, 1023]$$
> - **Epsilon maszynowy** $\varepsilon = 2^{-52} \approx 2{,}22\cdot10^{-16}$: odstęp między $1$ a następną liczbą maszynową. To ok. 15–16 cyfr dziesiętnych.
> - Zakres: największa $\approx 1{,}8\cdot10^{308}$, najmniejsza znormalizowana $\approx 2{,}2\cdot10^{-308}$. Poza nim `inf` albo liczby podnormalne i zero.
> - Odstęp między sąsiednimi liczbami rośnie z $|x|$: przy $10^{16}$ wynosi już $2$, więc `1e16 + 1 == 1e16`.

> [!wzor] Błędy
> $$\Delta x = |x - \tilde x| \quad\text{(bezwzględny)}, \qquad \delta x = \frac{|x - \tilde x|}{|x|} \quad\text{(względny)}$$
> Liczba poprawnych cyfr znaczących $\approx -\log_{10} \delta x$.

> [!wzor] Uwarunkowanie zadania $y = f(x)$
> $$\operatorname{cond}_f(x) = \left|\frac{x\,f'(x)}{f(x)}\right|, \qquad \delta y \approx \operatorname{cond}_f(x)\cdot \delta x$$
> Dla odejmowania $f(x) = x - 1$ przy $x = 1{,}0001$ mamy $\operatorname{cond} = 10001$: błąd danych rośnie dziesięć tysięcy razy, **niezależnie od algorytmu**.

> [!definicja] Stabilność algorytmu
> Algorytm jest **numerycznie stabilny**, jeśli nie powiększa błędu bardziej, niż wynika to z uwarunkowania. Złe uwarunkowanie to cecha **zadania** (nic nie poradzisz, najwyżej lepsze dane lub większa precyzja). Niestabilność to wina **algorytmu** (przepisz wzór).

> [!wzor] Rząd zbieżności $p$
> $$e_{k+1} \approx C\, e_k^{\,p} \quad\Longrightarrow\quad p \approx \frac{\ln(e_{k+1}/e_k)}{\ln(e_k/e_{k-1})}$$
> $p = 1$: liniowa (stała liczba nowych cyfr na krok, potrzebne $C < 1$), $p = 2$: kwadratowa (liczba cyfr się podwaja). Gdy nie znasz rozwiązania, zamiast $e_k$ użyj $|x_{k+1} - x_k|$.

## Jak to zrobić — krok po kroku
1. **Porównuj floaty z tolerancją**: `math.isclose(a, b, rel_tol=1e-9)`, nigdy `a == b` (bo `0.1 + 0.2 == 0.30000000000000004`).
2. **Szukaj odejmowania bliskich liczb** we wzorze: $-b + \sqrt{b^2 - 4ac}$, $\sqrt{x+1} - \sqrt{x}$, $1 - \cos x$ dla małego $x$. Przekształć algebraicznie, żeby było dodawanie albo iloczyn.
3. **Duże sumy**: sumuj algorytmem Kahana albo `math.fsum`. Uwaga: wbudowane `sum()` od Pythona 3.12 samo kompensuje błąd, a `np.sum` sumuje parami (błąd $O(\varepsilon \log n)$).
4. **Oceń uwarunkowanie**, zanim zaczniesz winić kod: duży $\operatorname{cond}$ oznacza, że żaden algorytm nie da wielu cyfr.
5. **Zmierz rząd zbieżności** z trzech ostatnich błędów **powyżej szumu** (odrzuć te rzędu $10^{-15}$, bo to już zaokrąglenia).

## Przykład
> [!przyklad] Pierwiastki $x^2 + 10^8 x + 1 = 0$
> Dokładnie: $x_1 \approx -10^8$, $x_2 \approx -10^{-8}$.
>
> **Wzór szkolny** dla małego pierwiastka: $x_2 = \frac{-b + \sqrt{\Delta}}{2a}$.
> - $\Delta = 10^{16} - 4$ w double to `9999999999999996.0` (jeszcze dokładnie).
> - $\sqrt{\Delta}$ = `99999999.99999999` (dokładnie: $99999999{,}99999998$, ostatnia cyfra już zaokrąglona).
> - $-10^8 + \sqrt\Delta$ = `-1.4901161193847656e-08` zamiast $-2\cdot10^{-8}$. Odjęliśmy dwie liczby zgodne na 16 cyfrach, więc z wyniku zostało jedynie zaokrąglenie.
> - $x_2$ = `-7.45e-09`, **błąd względny 25%**.
>
> **Wersja stabilna**: $q = -\frac{b + \operatorname{sign}(b)\sqrt\Delta}{2} = -10^8$ (dodawanie liczb o tym samym znaku), $x_1 = q/a = -10^8$, a z Viète'a $x_1 x_2 = c/a$:
> $$x_2 = \frac{c}{q} = -10^{-8}$$
> Błąd względny $1{,}7\cdot10^{-16}$, czyli pełna precyzja. Zadanie było **dobrze uwarunkowane**: winny był algorytm.

```cpp
#include "m00.h"   // Kod/metody_numeryczne
#include <cstdio>
int main() {
    std::printf("%.3e\n", eps_maszynowy());           // 2.220e-16
    Vec x(10000000, 0.1);
    std::printf("%.10f\n", suma_naiwna(x));            // 999999.9998389754  (błąd ~1,6e-10 względnie)
    std::printf("%.10f\n", suma_kahana(x));            // 1000000.0000000000
    auto [x1, x2] = pierwiastki_kwadratowe(1, -1e8, 1); // 1e-8 i 1e8 bez utraty cyfr
}
```

> [!kod]- To samo w Pythonie (do szybkiego sprawdzenia)
> ```python
> import math
> xs = [1.0] + [1e-16] * 10**6
> s = 0.0
> for x in xs:            # zwykłe s += x
>     s += x
> print(s, math.fsum(xs))  # 1.0  1.0000000001  (każde 1e-16 ginie przy dodaniu do 1)
> ```

> [!kod]- Suma Kahana w C++ (pełna wersja: `Kod/metody_numeryczne/rozwiazania/m00.cpp`)
> ```cpp
> double suma_kahana(const Vec& x) {
>     double s = 0, c = 0;
>     for (double v : x) {
>         double y = v - c;      // dołóż to, co zgubiliśmy poprzednio
>         double t = s + y;      // tu młodsze bity y przepadają
>         c = (t - s) - y;       // = minus to, co przepadło
>         s = t;
>     }
>     return s;
> }
> ```

## W C++ — o czym pamiętać
> [!kod] Typy i kompilacja
> - `double` ma 53 bity mantysy ($\varepsilon \approx 2{,}2\cdot10^{-16}$), `float` tylko 24 ($\approx 1{,}2\cdot10^{-7}$). W numeryce zawsze `double`.
> - `std::numeric_limits<double>::epsilon()` z `<limits>` to gotowe $\varepsilon$. `std::fabs`, `std::sqrt`, `std::exp` z `<cmath>`.
> - **Nie** kompiluj z `-ffast-math`: pozwala kompilatorowi przestawiać działania i niszczy np. sumę Kahana (kompilator „upraszcza” poprawkę do zera).
> - Porównanie z zerem: zamiast `if (x == 0.0)` pisz `if (std::fabs(x) < eps)`. Wyjątek: dzielenie przez dokładne zero (np. $f'(x_k) = 0$ w Newtonie) sprawdzasz `== 0`.
> - Wypisywanie: `printf("%.10f", x)` albo `std::cout << std::setprecision(12)` (z `<iomanip>`). Domyślne 6 cyfr ukrywa błędy rzędu $10^{-7}$.

## Typowe błędy
> [!blad] `==` na floatach i „dziwne” wyniki typu `0.30000000000000004`
> To nie błąd Pythona, tylko $0{,}1$ nie ma skończonego rozwinięcia dwójkowego. Kryteria stopu w metodach iteracyjnych zawsze z tolerancją.

> [!blad] Tolerancja mniejsza niż $\varepsilon \cdot |x|$
> Warunek $|x_{k+1} - x_k| < 10^{-20}$ dla $x \approx 1$ nigdy się nie spełni, bo kolejne liczby maszynowe różnią się o $2\cdot10^{-16}$. Stosuj kryterium mieszane $\text{tol}\cdot\max(1, |x|)$.

> [!blad] Mylenie uwarunkowania ze stabilnością
> „Źle uwarunkowany algorytm” to błąd językowy. Uwarunkowane (dobrze lub źle) jest **zadanie**, a **algorytm** jest stabilny albo nie.

> [!blad] Rząd zbieżności liczony z błędów na poziomie szumu
> Gdy $e_k \approx 10^{-16}$, ilorazy błędów są losowe i wychodzi np. $p = 0{,}3$ albo $p = 7$. Bierz ostatnie trzy błędy wyraźnie powyżej $\varepsilon$.

## Na zajęcia i egzamin
> [!zajecia]
> - **MES (projekt):** rozwiązanie układu $KT = F$ traci tyle cyfr, ile wynosi $\log_{10}\operatorname{cond}(K)$. Bardzo drobna siatka lub ogromne różnice przewodności między materiałami zwiększają $\operatorname{cond}$. Rząd zbieżności mierzysz przy weryfikacji: zagęszczasz siatkę lub krok czasu i patrzysz, jak maleje błąd.
> - **Optymalizacja:** kryteria stopu ($\|\nabla f\| < \varepsilon$) i różnicowe gradienty, gdzie za mały krok $h$ daje utratę cyfr.
> - **Programowanie równoległe:** suma równoległa dodaje w innej kolejności, więc wynik różni się w ostatnich cyfrach od sekwencyjnego. Dodawanie float nie jest łączne.
> - **Egzamin:** „czym jest epsilon maszynowy”, „podaj przykład utraty cyfr znaczących i jak jej uniknąć”, „uwarunkowanie a stabilność”, „co to rząd zbieżności”.
> - **Ćwicz w C++:** `Kod/metody_numeryczne/zadania/m00.cpp` (eps maszynowy, suma naiwna i Kahana, stabilne pierwiastki, Horner), specyfikacja w `m00.h`, sprawdzasz `./sprawdz.sh 00`. Wzorzec: `rozwiazania/m00.cpp`, dopiero po własnej próbie.

## Zadania
> [!zadanie] Zadanie 1
> Przybliżasz $\pi$ ułamkiem $22/7$. Podaj błąd bezwzględny, względny i liczbę poprawnych cyfr.
> > [!odpowiedz]- Odpowiedź
> > $\Delta = 1{,}264\cdot10^{-3}$, $\delta = 4{,}02\cdot10^{-4}$, czyli ok. $-\log_{10}(4\cdot10^{-4}) \approx 3{,}4$, a więc 3 cyfry ($3{,}14$).

> [!zadanie] Zadanie 2
> Dlaczego dla $x = 10^{12}$ wyrażenie $\sqrt{x+1} - \sqrt{x}$ jest niedokładne i jak je przepisać?
> > [!odpowiedz]- Odpowiedź
> > Odejmowanie bliskich liczb ($10^6$ i $10^6 + 5\cdot10^{-7}$). Mnożymy przez sprzężenie: $\frac{1}{\sqrt{x+1} + \sqrt{x}}$. W double pierwsza wersja daje `5.00003807e-07` (5 cyfr), druga `4.99999999999875e-07` (pełna precyzja).

> [!zadanie] Zadanie 3
> Kolejne błędy metody to $10^{-2}, 10^{-4}, 10^{-8}$. Jaki jest rząd zbieżności? A dla $0{,}1;\ 0{,}05;\ 0{,}025$?
> > [!odpowiedz]- Odpowiedź
> > $p = \ln(10^{-4})/\ln(10^{-2}) = 2$ (kwadratowa). W drugim przypadku $p = \ln 0{,}5 / \ln 0{,}5 = 1$ (liniowa ze stałą $C = 0{,}5$, jak bisekcja).

> [!zadanie] Zadanie 4
> Policz $\operatorname{cond}$ zadania $y = \ln x$ w $x = 1{,}001$. Co to oznacza?
> > [!odpowiedz]- Odpowiedź
> > $\operatorname{cond} = |x \cdot \frac1x / \ln x| = 1/|\ln x| \approx 1000{,}5$. Błąd względny $x$ rzędu $10^{-16}$ daje błąd $\ln x$ rzędu $10^{-13}$: tracisz 3 cyfry przez samo zadanie.

## Fiszki #fiszki/numeryczne
Ile wynosi epsilon maszynowy dla double?::2^−52 ≈ 2,22·10^−16 (odstęp między 1 a następną liczbą maszynową)
<!--SR:!2026-10-03,1,230-->
Ile bitów mantysy, wykładnika i znaku ma IEEE 754 double?::52 mantysy (plus ukryta jedynka), 11 wykładnika, 1 znaku
Ile cyfr dziesiętnych daje double?::Około 15–16 cyfr znaczących
Czym jest błąd względny?::|x − x̃| / |x|, czyli błąd bezwzględny odniesiony do wielkości dokładnej
Na czym polega katastrofalna redukcja?::Odejmowanie prawie równych liczb: zgodne cyfry się znoszą i zostaje głównie błąd zaokrąglenia
Jak stabilnie policzyć mały pierwiastek równania kwadratowego?::q = −(b + sign(b)√Δ)/2, x1 = q/a, a drugi ze wzoru Viète'a: x2 = c/q
Co robi suma Kahana?::Przechowuje w zmiennej c zgubione młodsze bity i dodaje je z powrotem, błąd O(ε) zamiast O(nε)
Wzór na względny wskaźnik uwarunkowania zadania y = f(x)::cond = |x f'(x) / f(x)|
Uwarunkowanie a stabilność — czym się różnią?::Uwarunkowanie to cecha zadania (wrażliwość na dane), stabilność to cecha algorytmu (czy nie dokłada błędów)
Jak zmierzyć rząd zbieżności z danych?::p ≈ ln(e_{k+1}/e_k) / ln(e_k/e_{k−1}) z trzech ostatnich błędów powyżej szumu
Co oznacza zbieżność kwadratowa?::e_{k+1} ≈ C e_k², liczba poprawnych cyfr podwaja się w każdym kroku
Dlaczego 1e16 + 1 == 1e16 w double?::Przy 10^16 odstęp między liczbami maszynowymi wynosi 2, więc +1 ginie w zaokrągleniu

Jak w C++ dostać epsilon maszynowy double?::std::numeric_limits<double>::epsilon() z <limits>, ok. 2,2·10⁻¹⁶
Dlaczego nie używać -ffast-math w kodzie numerycznym?::Kompilator może przestawiać działania i np. wyzerować poprawkę w sumie Kahana

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.

## Źródła
- Z. Fortuna, B. Macukow, J. Wąsowski, *Metody numeryczne*, WNT — rozdz. o błędach i arytmetyce zmiennopozycyjnej.
- D. Kincaid, W. Cheney, *Analiza numeryczna*, WNT — arytmetyka zmiennopozycyjna, uwarunkowanie, stabilność.
- R. L. Burden, J. D. Faires, *Numerical Analysis*, Brooks/Cole — rozdz. 1.2–1.3 (błędy zaokrągleń, rząd zbieżności).
- D. Goldberg, *What Every Computer Scientist Should Know About Floating-Point Arithmetic*, ACM Computing Surveys 23(1), 1991.
- N. J. Higham, *Accuracy and Stability of Numerical Algorithms*, SIAM 2002 — rozdz. 4 „Summation” (algorytm Kahana).
- Dokumentacja numpy: `numpy.finfo`, `numpy.spacing`; Python: `math.fsum`, `sum` (od 3.12 sumowanie z kompensacją).
- Kurs MN w C++: Kod/metody_numeryczne (moduły, testy, rozwiązania wzorcowe)
