// m06 — całkowanie numeryczne: Newton–Cotes, Gauss–Legendre, całka po elemencie MES
// Notatka: [[Całkowanie numeryczne i kwadratury Gaussa]]. Zależy od: m03 (N4, dN4) — tylko calka_czworokat.
#pragma once
#include "mn.h"

// Złożone kwadratury na n podprzedziałach długości h = (b - a) / n.
double prostokaty(F1 f, double a, double b, int n);   // wartość w środku podprzedziału
double trapezy(F1 f, double a, double b, int n);
double simpson(F1 f, double a, double b, int n);       // klasyczny złożony Simpson, n parzyste (inaczej invalid_argument)
// „Parabole” w wersji z instrukcji „Całkowanie — metody geometryczne”: wzór (6) na KAŻDYM z n podprzedziałów,
// sum h/6 (f(x_i) + 4 f(x_i + h/2) + f(x_i + h)); działa dla dowolnego n (to Simpson na 2n podziałach).
double parabole(F1 f, double a, double b, int n);

// Węzły i wagi Gaussa–Legendre'a na [-1, 1] dla n = 1..4 (tablica z instrukcji).
void gauss_wezly(int n, Vec& x, Vec& A);

// Kwadratura Gaussa–Legendre'a na [a, b]: (b-a)/2 * sum A_i f(t_i), t_i = (a+b)/2 + (b-a)/2 x_i.
double gauss_legendre(F1 f, double a, double b, int n);

// Gauss na kwadracie [-1, 1]^2: sum_i sum_j A_i A_j f(x_i, x_j).
double gauss_2d(F2 f, int n);

// Całka z f(x, y) po czworokącie o węzłach (x[k], y[k]) (kolejność przeciwna do wskazówek zegara)
// przez przekształcenie izoparametryczne: sum A_i A_j f(x(ksi, eta), y(ksi, eta)) det J.
double calka_czworokat(F2 f, const double x[4], const double y[4], int n);
