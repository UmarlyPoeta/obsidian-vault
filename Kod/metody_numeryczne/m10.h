// m10 — numeryka w optymalizacji: złoty podział, Newton, najszybszy spadek
// Notatki: [[Metoda złotego podziału]], [[Funkcja celu i warunki optymalności]], [[Trening — Optymalizacja w kodzie]].
// Zależy od: m02 (gauss), m05 (gradient, hesjan).
#pragma once
#include "mn.h"

// Minimum funkcji unimodalnej na [a, b] metodą złotego podziału, stop: b - a < eps.
// k = (sqrt(5) - 1) / 2; c = b - k (b - a), d = a + k (b - a); jedno nowe f na iterację.
// W f_calls zwróć liczbę wywołań f.
double zloty_podzial(F1 f, double a, double b, double eps, int& f_calls);

// Newton dla minimum: H(x_k) d = -grad f(x_k), x_{k+1} = x_k + d (gradient i hesjan numerycznie, m05).
// Stop: ||grad|| < eps (norma euklidesowa) albo maxit.
Vec newton_min(FN f, Vec x, double eps, int maxit, int& iteracje);

// Najszybszy spadek: d = -grad f, krok h z min f(x + h d) złotym podziałem na [0, h_max].
// Stop: ||grad|| < eps albo maxit.
Vec najszybszy_spadek(FN f, Vec x, double h_max, double eps, int maxit, int& iteracje);
