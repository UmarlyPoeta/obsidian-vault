// m08 — TWOJA IMPLEMENTACJA. Specyfikacja jest w ../m08.h, testy w ../testy/test_m08.cpp.
// Sprawdzanie: ./sprawdz.sh 08    Wzorzec (dopiero po swojej próbie): ../rozwiazania/m08.cpp
#include "../m08.h"
#include "../m02.h"

// Globalna macierz [H] (z członem alfa S w ostatnim węźle) i wektor {P} = [qS, 0, ..., -alfa S t_inf]
// w zapisie [H]{t} + {P} = 0. Element: (S k / Le) [[1, -1], [-1, 1]], agregacja przez +=.
void pret_HP(const Pret& p, Mat& H, Vec& P) {
    TODO;
}

// Stan ustalony: rozwiąż [H]{t} = -{P}.
Vec pret_ustalony(const Pret& p) {
    TODO;
}

// Macierz pojemności [C]: element (rho c S Le / 6) [[2, 1], [1, 2]], agregacja jak [H].
Mat pret_C(const Pret& p) {
    TODO;
}

// Stan nieustalony, schemat niejawny: ([H] + [C]/dt) t1 = ([C]/dt) t0 - {P}; start t0 w każdym węźle.
Vec pret_nieustalony(const Pret& p, double t0, double dt, int kroki) {
    TODO;
}
