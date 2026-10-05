// m01 — TWOJA IMPLEMENTACJA. Specyfikacja jest w ../m01.h, testy w ../testy/test_m01.cpp.
// Sprawdzanie: ./sprawdz.sh 01    Wzorzec (dopiero po swojej próbie): ../rozwiazania/m01.cpp
#include "../m01.h"

// Bisekcja na [a, b], wymaga f(a) f(b) < 0 (inaczej rzuć std::invalid_argument).
// x0 = (a + b) / 2; jeśli f(a) f(x0) < 0, to b = x0, w przeciwnym razie a = x0.
Wynik bisekcja(F1 f, double a, double b, double eps, int maxit) {
    TODO;
}

// Regula falsi: x1 = a - f(a) (b - a) / (f(b) - f(a)); jeśli f(x1) f(a) < 0, to b = x1, inaczej a = x1.
Wynik regula_falsi(F1 f, double a, double b, double eps, int maxit) {
    TODO;
}

// Metoda stycznych (Newtona): x_{k+1} = x_k - f(x_k) / f'(x_k). Pochodna analitycznie (df).
// Gdy f'(x_k) == 0, rzuć std::runtime_error.
Wynik styczne(F1 f, F1 df, double x0, double eps, int maxit) {
    TODO;
}

// Metoda siecznych: x_{k+1} = x_k - f(x_k) (x_k - x_{k-1}) / (f(x_k) - f(x_{k-1})).
// W labie RN1: x_poprz = x0 - 0.1.
Wynik sieczne(F1 f, double x0, double x_poprz, double eps, int maxit) {
    TODO;
}

// Newton dla układu 2 równań (struktura Uklad2 w ../m01.h): w każdym kroku J = [[Fx, Fy], [Gx, Gy]],
// J [dx dy]^T = -[F G]^T rozwiąż wzorami Cramera, x += dx, y += dy; stop: |F| + |G| < eps.
Wynik newton_uklad2(const Uklad2& u, double x0, double y0, double eps, int maxit, double& y_wynik) {
    TODO;
}

// Rząd zbieżności z trzech ostatnich błędów e_k = |x_k - dokladne| z historii:
// p = ln(e_k / e_{k-1}) / ln(e_{k-1} / e_{k-2}).
double rzad_zbieznosci(const Vec& historia, double dokladne) {
    TODO;
}
