// m07 — TWOJA IMPLEMENTACJA. Specyfikacja jest w ../m07.h, testy w ../testy/test_m07.cpp.
// Sprawdzanie: ./sprawdz.sh 07    Wzorzec (dopiero po swojej próbie): ../rozwiazania/m07.cpp
#include "../m07.h"
#include "../m02.h"

// y' = f(x, y), y(a) = ya, N kroków o długości h = (b - a) / N. Zwracają y w punkcie b.
// y += h f(x, y)
double euler(F2 f, double a, double b, double ya, int N) {
    TODO;
}

// k1 = f(x,y), k2 = f(x+h, y+h k1), y += h/2 (k1+k2)
double rk2(F2 f, double a, double b, double ya, int N) {
    TODO;
}

// klasyczny RK4 z instrukcji
double rk4(F2 f, double a, double b, double ya, int N) {
    TODO;
}

// Cała trajektoria y_0..y_N (do wykresów): metoda = 1 (Euler), 2 (RK2) albo 4 (RK4).
Vec trajektoria(F2 f, double a, double b, double ya, int N, int metoda) {
    TODO;
}

// Schemat θ dla układu C dT/dt + H T = P (MES nieustalony):
// (C/dt + θH) T1 = (C/dt - (1-θ)H) T0 + P, powtórzone kroki razy. Zwraca T po ostatnim kroku.
Vec schemat_theta(const Mat& C, const Mat& H, const Vec& P, Vec T0, double dt, int kroki, double theta) {
    TODO;
}
