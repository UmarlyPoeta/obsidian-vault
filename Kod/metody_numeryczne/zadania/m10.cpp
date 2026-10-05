// m10 — TWOJA IMPLEMENTACJA. Specyfikacja jest w ../m10.h, testy w ../testy/test_m10.cpp.
// Sprawdzanie: ./sprawdz.sh 10    Wzorzec (dopiero po swojej próbie): ../rozwiazania/m10.cpp
#include "../m10.h"
#include "../m02.h"
#include "../m05.h"

// Minimum funkcji unimodalnej na [a, b] metodą złotego podziału, stop: b - a < eps.
// k = (sqrt(5) - 1) / 2; c = b - k (b - a), d = a + k (b - a); jedno nowe f na iterację.
// W f_calls zwróć liczbę wywołań f.
double zloty_podzial(F1 f, double a, double b, double eps, int& f_calls) {
    TODO;
}

// Newton dla minimum: H(x_k) d = -grad f(x_k), x_{k+1} = x_k + d (gradient i hesjan numerycznie, m05).
// Stop: ||grad|| < eps (norma euklidesowa) albo maxit.
Vec newton_min(FN f, Vec x, double eps, int maxit, int& iteracje) {
    TODO;
}

// Najszybszy spadek: d = -grad f, krok h z min f(x + h d) złotym podziałem na [0, h_max].
// Stop: ||grad|| < eps albo maxit.
Vec najszybszy_spadek(FN f, Vec x, double h_max, double eps, int maxit, int& iteracje) {
    TODO;
}
