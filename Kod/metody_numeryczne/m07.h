// m07 — równania różniczkowe zwyczajne: zagadnienie Cauchy'ego, schemat θ     (lab „RR”)
// Notatka: [[Numeryczne rozwiązywanie RRZ]]. Zależy od: m02 (gauss) — tylko schemat_theta.
#pragma once
#include "mn.h"

// y' = f(x, y), y(a) = ya, N kroków o długości h = (b - a) / N. Zwracają y w punkcie b.
double euler(F2 f, double a, double b, double ya, int N);   // y += h f(x, y)
double rk2(F2 f, double a, double b, double ya, int N);     // k1 = f(x,y), k2 = f(x+h, y+h k1), y += h/2 (k1+k2)
double rk4(F2 f, double a, double b, double ya, int N);     // klasyczny RK4 z instrukcji

// Cała trajektoria y_0..y_N (do wykresów): metoda = 1 (Euler), 2 (RK2) albo 4 (RK4).
Vec trajektoria(F2 f, double a, double b, double ya, int N, int metoda);

// Schemat θ dla układu C dT/dt + H T = P (MES nieustalony):
// (C/dt + θH) T1 = (C/dt - (1-θ)H) T0 + P, powtórzone kroki razy. Zwraca T po ostatnim kroku.
Vec schemat_theta(const Mat& C, const Mat& H, const Vec& P, Vec T0, double dt, int kroki, double theta);
