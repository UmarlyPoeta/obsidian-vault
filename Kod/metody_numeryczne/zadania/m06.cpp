// m06 — TWOJA IMPLEMENTACJA. Specyfikacja jest w ../m06.h, testy w ../testy/test_m06.cpp.
// Sprawdzanie: ./sprawdz.sh 06    Wzorzec (dopiero po swojej próbie): ../rozwiazania/m06.cpp
#include "../m06.h"
#include "../m03.h"

// Złożone kwadratury na n podprzedziałach długości h = (b - a) / n.
// wartość w środku podprzedziału
double prostokaty(F1 f, double a, double b, int n) {
    TODO;
}

double trapezy(F1 f, double a, double b, int n) {
    TODO;
}

// klasyczny złożony Simpson, n parzyste (inaczej invalid_argument)
double simpson(F1 f, double a, double b, int n) {
    TODO;
}

// „Parabole” w wersji z instrukcji „Całkowanie — metody geometryczne”: wzór (6) na KAŻDYM z n podprzedziałów,
// sum h/6 (f(x_i) + 4 f(x_i + h/2) + f(x_i + h)); działa dla dowolnego n (to Simpson na 2n podziałach).
double parabole(F1 f, double a, double b, int n) {
    TODO;
}

// Węzły i wagi Gaussa–Legendre'a na [-1, 1] dla n = 1..4 (tablica z instrukcji).
void gauss_wezly(int n, Vec& x, Vec& A) {
    TODO;
}

// Kwadratura Gaussa–Legendre'a na [a, b]: (b-a)/2 * sum A_i f(t_i), t_i = (a+b)/2 + (b-a)/2 x_i.
double gauss_legendre(F1 f, double a, double b, int n) {
    TODO;
}

// Gauss na kwadracie [-1, 1]^2: sum_i sum_j A_i A_j f(x_i, x_j).
double gauss_2d(F2 f, int n) {
    TODO;
}

// Całka z f(x, y) po czworokącie o węzłach (x[k], y[k]) (kolejność przeciwna do wskazówek zegara)
// przez przekształcenie izoparametryczne: sum A_i A_j f(x(ksi, eta), y(ksi, eta)) det J.
double calka_czworokat(F2 f, const double x[4], const double y[4], int n) {
    TODO;
}
