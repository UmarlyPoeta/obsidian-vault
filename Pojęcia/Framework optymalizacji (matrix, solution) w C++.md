---
typ: pojęcie
status: nowe
przedmioty: ["[[AGH Optymalizacja]]"]
wymaga: []
powiązane: ["[[Trening — Optymalizacja w kodzie]]", "[[Funkcja celu i warunki optymalności]]", "[[Metoda złotego podziału]]"]
tags: [pojęcie, optymalizacja, kod, cpp]
utworzono: 2026-10-02
źródła: ["Ł. Sztangret — dokumentacja klas matrix i solution (KISiM AGH)", "Ł. Sztangret — kod źródłowy do ćwiczeń z metod optymalizacji, CC BY-SA 3.0 (Pliki źródłowe-20261002.zip)", "Kusiak, Danielewska-Tułecka, Oprocha — Optymalizacja. Wybrane metody z przykładami zastosowań (PWN 2009)"]
---
# Framework optymalizacji (matrix, solution) w C++

> [!skrot] W 30 sekund
> - **Co to:** projekt C++ dr. Sztangreta, na którym robisz wszystkie laby: `matrix` (liczby, wektory, macierze), `solution` (punkt + wartość f + liczniki), `opt_alg` (metody), `user_funs` (funkcje celu), `main.cpp` z funkcjami `lab0()…lab6()`.
> - **Klucz:** każda funkcja celu ma sygnaturę `matrix ff(matrix x, matrix ud1, matrix ud2)`, a f liczysz **tylko** przez `X.fit_fun(ff, ud1, ud2)`, bo to on zwiększa licznik `f_calls`.
> - **Kiedy:** na każdych labach; trening z testami leży w `~/studia/sem5/optymalizacja/trening_cpp`.

> [!intuicja] Po ludzku
> `matrix` to „pudełko na liczby”, które udaje zarówno liczbę, jak i wektor. `solution` to karteczka z punktem $x$, wartością $f(x)$ i flagą „jak się skończyło”. Metoda optymalizacji bierze funkcję celu przez wskaźnik, przesuwa karteczkę po przestrzeni i zwraca najlepszą. Liczniki wywołań są wspólne dla całego programu, jak licznik prądu w bloku: przed nowym pomiarem trzeba go wyzerować.

## Definicja i wzory
> [!definicja] Klasa matrix — co musisz pamiętać
> - Konstruktory: `matrix(2, 1)` to wektor zer $2 \times 1$, `matrix(2, 1, -5)` to wektor wypełniony $-5$, `matrix(3.5)` to macierz $1 \times 1$. **`matrix(2)` to też $1 \times 1$ z wartością 2**, a nie wektor.
> - `double` i `int` same zamieniają się na `matrix` $1 \times 1$. Dlatego `X.x = x0 + d;` działa w 1D, a `2 * X.x` mnoży wektor przez skalar.
> - `A(i, j)` lub `A(i)` to element (l-wartość). `A[j]` to **kopia kolumny** $j$ jako wektor $n \times 1$; zmiana kolumny tylko przez `A.set_col(v, j)`.
> - `m2d(A)` zamienia $1 \times 1$ na `double` (inaczej rzuca wyjątek). `get_len(x)` to długość wektora $n \times 1$. `get_size(A)` zwraca `new int[2]`, który trzeba zwolnić.
> - Relacje `<`, `>`, `==` działają **tylko dla $1 \times 1$**. Punkty porównujesz normą: `norm(x1 - x2) < eps`.
> - `A / B` liczy `A * inv(B)`. Gotowe: `trans`, `inv`, `det`, `norm`, `hcat`, `vcat`, `ident_mat`, `rand_mat` (U[0,1]), `randn_mat` (N(0,1)).

> [!definicja] Klasa solution
> - Pola publiczne: `x` (punkt), `y` (wartość f), `g` (gradient), `H` (hesjan), `ud` (dane użytkownika), `flag`.
> - `fit_fun`, `grad`, `hess` liczą `y`, `g`, `H` i zwiększają **statyczne** liczniki `f_calls`, `g_calls`, `H_calls`. Zeruje je `clear_calls()`.
> - Kopiowanie pomija `ud`, gdy w źródle `ud(0,0)` to NAN, więc stare `ud` w celu zostaje.
> - Konwencja flag w treningu: 1 = kryterium stopu spełnione, 0 = przekroczone `Nmax` (jak w MC), −1 = metoda się rozsypała.

> [!wzor] Tryb „po kierunku” (szukanie kroku)
> Metody gradientowe ze zmiennym krokiem i metoda Powella minimalizują funkcję jednej zmiennej
> $$\varphi(h) = f(x + h\,d)$$
> Złoty podział dostaje wtedy `ud2 = hcat(x, d)`, a funkcja celu sprawdza `isnan(ud2(0, 0))`. Jeśli to nie NAN, liczy $f$ w punkcie `ud2[0] + h * ud2[1]`. Wskaźnika do funkcji nie da się „domknąć” jak lambdy w Pythonie, więc dane idą przez `ud2`.

## Jak to zrobić — krok po kroku
1. Funkcję celu, a w lab 4 także gradient i hesjan, dopisz do `user_funs.cpp`, a deklarację z domyślnymi `= NAN` do `user_funs.h`.
2. Metodę napisz w `opt_alg.cpp` z sygnaturą **dokładnie** z `opt_alg.h`, bo tego pliku nie zmieniasz. Zostaw `try`/`catch (string)`, który dokleja nazwę funkcji do opisu błędu.
3. Sprawdź ją w treningu: `make test` albo `./testy fib`.
4. W `main.cpp` napisz `labN()`: pętla po parametrach i punktach startowych, `clear_calls()` przed **każdym** wywołaniem metody, wiersz wyników do CSV.
5. W `main()` wywołaj `labN()`. Wyjątek z frameworka to `string`: łap go w pętli eksperymentu, żeby jedna porażka nie przerwała 100 przebiegów.
6. CSV otwórz w Excelu albo w Pythonie i zrób tabele i wykresy do sprawozdania.

```cpp
// Szablon eksperymentu (lab 1: ekspansja + Fibonacci ze 100 losowych startów)
void lab1()
{
	double epsilon = 1e-5, d = 1, alfy[] = { 1.5, 2.0, 2.5 };
	int Nmax = 1000;
	solution S;                                         // tylko do liczników (statyczne)
	ofstream Sout("lab1_tabela1.csv");
	Sout << "alpha;x0;a;b;f_calls_exp;x*;y*;f_calls_fib;flag\n";
	for (double alpha : alfy)
		for (int i = 0; i < 100; ++i)
		{
			double x0 = 200 * m2d(rand_mat()) - 100;    // start z U[-100, 100]
			try
			{
				S.clear_calls();
				double* p = expansion(ff1T, x0, d, alpha, Nmax);
				int f_exp = S.f_calls;
				S.clear_calls();
				solution opt = fib(ff1T, p[0], p[1], epsilon);
				matrix w(1, 9);
				w(0) = alpha; w(1) = x0; w(2) = p[0]; w(3) = p[1]; w(4) = f_exp;
				w(5) = m2d(opt.x); w(6) = m2d(opt.y); w(7) = S.f_calls; w(8) = opt.flag;
				Sout << w << endl;                      // „a; b; c; ” z przecinkiem dziesiętnym
				delete[] p;
			}
			catch (string ex)
			{
				cerr << "x0 = " << x0 << ": " << ex << endl;   // jeden przebieg padł, lecimy dalej
			}
		}
	Sout.close();
}
```

## Przykład
> [!przyklad] Od CSV do tabeli w sprawozdaniu
> `operator<<` macierzy pisze liczby rozdzielone `; ` z **przecinkiem dziesiętnym** (`SEP_SYMBOL` w `matrix.h`).
> - **Excel (polski):** plik otwiera się od razu w kolumnach. Tabela przestawna po `alpha` daje średnie: $x^*$, $y^*$, liczbę wywołań i liczbę minimów globalnych.
> - **Python:** zamień przecinki na kropki i obetnij pustą kolumnę po ostatnim `;` (kod niżej). Potem `np.mean` po grupach i wykres zbieżności z `opt.ud`, jeśli metoda zapisuje tam historię, jak MC.
> - **Do tabeli** podajesz średnie ze 100 startów, a nie jeden przebieg. Kolumna „lokalne/globalne” wynika z $x^*$, np. w lab 1 $x^* \approx 62{,}75$ to minimum globalne, a $x^* \approx 0$ lokalne.

```python
import numpy as np
dane = np.genfromtxt((l.replace(',', '.') for l in open('lab1_tabela1.csv')),
                     delimiter=';', skip_header=1)[:, :-1]   # ostatnia kolumna pusta
```

## Typowe błędy
> [!blad] Brak `clear_calls()` między przebiegami
> Liczniki są statyczne, więc drugi przebieg „widzi” wywołania pierwszego. W tabeli rosną liczby wywołań, a warunek `f_calls > Nmax` przerywa metodę za wcześnie.

> [!blad] `f` liczone bezpośrednio, `ff(x, ud1, ud2)`
> Wynik się zgadza, ale `f_calls` nie rośnie. Liczba wywołań w sprawozdaniu jest wtedy fałszywa, a limit `Nmax` nie działa.

> [!blad] `matrix(2)` zamiast `matrix(2, 1)`
> Pierwsze to liczba 2 jako $1 \times 1$. Potem `x(1)` rzuca „indeks jest poza zakresem”.

> [!blad] `D[j] = v` albo `D[j](0) = 1`
> `operator[]` zwraca kopię kolumny, więc zmiana ginie bez ostrzeżenia. Kolumnę zmieniasz przez `D.set_col(v, j)`.

> [!blad] Podwójne zwalnianie wyniku `solve_ode`
> `solve_ode` zwraca `new matrix[2]`. Prowadzący woła destruktory `Y[0].~matrix(); Y[1].~matrix();`. Możesz zamiast tego `delete[] Y;`, ale **nigdy obu naraz**, bo to crash. Bez zwalniania funkcja celu z symulacją, wołana tysiące razy, zjada pamięć.

> [!blad] `randn_mat()` po jednej liczbie w pętli
> Każde wywołanie tworzy nowy generator ziarniony zegarem. Losuj całą macierz naraz: `randn_mat(N, lambda)`.

## Na zajęcia i egzamin
> [!zajecia]
> - **Laby (pt 8:00):** przychodzisz z działającymi metodami danego labu i z gotowym szkieletem `labN()`. Na zajęciach dostajesz funkcję testową, problem rzeczywisty (zwykle model z `solve_ode`) i parametry.
> - **Przed lab 1:** `expansion`, `fib`, `lag` i od razu `golden` zielone w `make test`. Do tego szablon `lab1()` z zapisem CSV.
> - **Sprawozdanie:** tabele średnich ze 100 startów, liczba wywołań f i minima lokalne/globalne. Wnioski: która metoda tańsza i dlaczego.
> - Łączy się z: [[Trening — Optymalizacja w kodzie]], [[Metoda złotego podziału]], [[Funkcja celu i warunki optymalności]].

## Zadania
> [!zadanie] Zadanie 1
> Po `opt = HJ(...)` od razu wołasz `opt2 = Rosen(...)` i drukujesz `opt2`. Wypisane `f_calls = 1384`, a Rosen sam zużył 1005. Co jest nie tak?
> > [!odpowiedz]- Odpowiedź
> > Brakuje `clear_calls()` między metodami. Licznik jest statyczny, więc pokazuje sumę: 379 z HJ i 1005 z Rosena.

> [!zadanie] Zadanie 2
> Napisz funkcję celu $f(x) = (x_1 - 1)^2 + 2(x_2 + 2)^2$ obsługującą tryb „po kierunku”.
> > [!odpowiedz]- Odpowiedź
> > ```cpp
> > matrix ff(matrix x, matrix ud1, matrix ud2)
> > {
> > 	if (!isnan(ud2(0, 0)))                      // x to krok h, ud2 = [punkt | kierunek]
> > 		return ff(ud2[0] + x * ud2[1], ud1);
> > 	return pow(x(0) - 1, 2) + 2 * pow(x(1) + 2, 2);
> > }
> > ```

> [!zadanie] Zadanie 3
> Dlaczego `if (X.y < XB.y)` się kompiluje, a `if (X.x < XB.x)` w 2D rzuca wyjątek?
> > [!odpowiedz]- Odpowiedź
> > Operatory relacji są zdefiniowane tylko dla macierzy $1 \times 1$. `y` jest $1 \times 1$, a `x` w 2D to $2 \times 1$. Punkty porównuje się normą różnicy.

## Fiszki #fiszki/optymalizacja
Sygnatura funkcji celu we frameworku Sztangreta::matrix ff(matrix x, matrix ud1, matrix ud2)
Dlaczego f liczysz przez fit_fun, a nie wprost?::Bo tylko fit_fun zwiększa licznik f_calls, a od niego zależą Nmax i liczba wywołań w sprawozdaniu
Co trzeba zrobić przed każdym eksperymentem i dlaczego?::Wywołać clear_calls(), bo liczniki f_calls, g_calls i H_calls są statyczne i wspólne dla programu
Co zwraca A[j] dla macierzy matrix?::Kopię kolumny j jako wektor n×1; zmienić kolumnę można tylko przez set_col
Czym jest matrix(2) w tym frameworku?::Macierzą 1×1 z wartością 2, a nie wektorem; wektor to matrix(2, 1)
Dla jakich macierzy działają operatory relacji?::Tylko dla 1×1; punkty porównuje się normą różnicy
Jak zamienić matrix 1×1 na double?::m2d(A) albo A(0)
Jak golden szuka kroku po kierunku d?::Dostaje ud2 = hcat(x, d), a funkcja celu liczy f(ud2[0] + h·ud2[1])
Jak zwolnić wynik solve_ode?::Albo destruktory Y[0] i Y[1] jak prowadzący, albo delete[] Y, nigdy oba naraz
W jakim formacie operator << zapisuje macierz do CSV?::Liczby rozdzielone średnikiem, z przecinkiem dziesiętnym (SEP_SYMBOL)
Co oznacza flag 0 w MC prowadzącego?::Przekroczono Nmax wywołań funkcji celu
Jakiego typu wyjątki rzuca framework?::string z opisem; każda funkcja dokleja swoją nazwę i rzuca dalej

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.

## Źródła
- Ł. Sztangret, dokumentacja klas `matrix` i `solution`, KISiM AGH (streszczenie: `~/studia/sem5/optymalizacja/materialy/klasy-matrix-solution.md`).
- Ł. Sztangret, kod źródłowy do ćwiczeń z metod optymalizacji, licencja CC BY-SA 3.0 (`~/github/optimalization_labs/od_prof/`, archiwum `Pliki źródłowe-20261002.zip`).
- J. Kusiak, A. Danielewska-Tułecka, P. Oprocha, *Optymalizacja. Wybrane metody z przykładami zastosowań*, PWN 2009.
- Trening z testami: `~/studia/sem5/optymalizacja/trening_cpp` (README, `rozwiazania/opt_alg.cpp`).
