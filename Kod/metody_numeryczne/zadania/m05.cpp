// m05 — TWOJA IMPLEMENTACJA. Specyfikacja jest w ../m05.h, testy w ../testy/test_m05.cpp.
// Sprawdzanie: ./sprawdz.sh 05    Wzorzec (dopiero po swojej próbie): ../rozwiazania/m05.cpp
#include "../m05.h"
#include <limits>

// (f(x+h) - f(x)) / h,          błąd O(h)
double roznica_przednia(F1 f, double x, double h) {
    TODO;
}

// (f(x+h) - f(x-h)) / (2h),     błąd O(h^2)
double roznica_centralna(F1 f, double x, double h) {
    TODO;
}

// (f(x+h) - 2f(x) + f(x-h)) / h^2
double druga_pochodna(F1 f, double x, double h) {
    TODO;
}

// Gradient różnicą centralną w każdym kierunku: g_i = (f(x + h e_i) - f(x - h e_i)) / (2h).
Vec gradient(FN f, const Vec& x, double h) {
    TODO;
}

// Hesjan: H_ii = druga różnica po x_i,
// H_ij = (f(++) - f(+-) - f(-+) + f(--)) / (4 h^2) dla i != j. Wynik symetryczny.
Mat hesjan(FN f, const Vec& x, double h) {
    TODO;
}

// Krok h minimalizujący błąd różnicy centralnej ~ eps^(1/3) * max(1, |x|).
double krok_optymalny_centralny(double x) {
    TODO;
}
