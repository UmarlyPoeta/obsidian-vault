// m01 — równania nieliniowe f(x) = 0          (laby RN1 i RN2)
// Notatka: [[Rozwiązywanie równań nieliniowych]]
#pragma once
#include "mn.h"

// Kryterium stopu we wszystkich metodach jak w instrukcjach: |f(x_k)| < eps
// albo przekroczony limit maxit (wtedy zbiezna = false). W historia zapisz każde nowe x_k.

// Bisekcja na [a, b], wymaga f(a) f(b) < 0 (inaczej rzuć std::invalid_argument).
// x0 = (a + b) / 2; jeśli f(a) f(x0) < 0, to b = x0, w przeciwnym razie a = x0.
Wynik bisekcja(F1 f, double a, double b, double eps, int maxit = 1000);

// Regula falsi: x1 = a - f(a) (b - a) / (f(b) - f(a)); jeśli f(x1) f(a) < 0, to b = x1, inaczej a = x1.
Wynik regula_falsi(F1 f, double a, double b, double eps, int maxit = 1000);

// Metoda stycznych (Newtona): x_{k+1} = x_k - f(x_k) / f'(x_k). Pochodna analitycznie (df).
// Gdy f'(x_k) == 0, rzuć std::runtime_error.
Wynik styczne(F1 f, F1 df, double x0, double eps, int maxit = 100);

// Metoda siecznych: x_{k+1} = x_k - f(x_k) (x_k - x_{k-1}) / (f(x_k) - f(x_{k-1})).
// W labie RN1: x_poprz = x0 - 0.1.
Wynik sieczne(F1 f, double x0, double x_poprz, double eps, int maxit = 100);

// Newton dla układu 2 równań F(x, y) = 0, G(x, y) = 0.
// J = [[Fx, Fy], [Gx, Gy]] liczysz w każdym kroku; krok z Cramera; stop: |F| + |G| < eps.
struct Uklad2 {
    F2 F, G;                   // równania
    F2 Fx, Fy, Gx, Gy;         // pochodne cząstkowe
};
Wynik newton_uklad2(const Uklad2& u, double x0, double y0, double eps, int maxit, double& y_wynik);

// Rząd zbieżności z trzech ostatnich błędów e_k = |x_k - dokladne| z historii:
// p = ln(e_k / e_{k-1}) / ln(e_{k-1} / e_{k-2}).
double rzad_zbieznosci(const Vec& historia, double dokladne);
