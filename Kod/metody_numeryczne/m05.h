// m05 — różniczkowanie numeryczne, gradient i hesjan
// Notatka: [[Różniczkowanie numeryczne]]. Używane w Optymalizacji (metody gradientowe, Newton).
#pragma once
#include "mn.h"

double roznica_przednia(F1 f, double x, double h);      // (f(x+h) - f(x)) / h,          błąd O(h)
double roznica_centralna(F1 f, double x, double h);     // (f(x+h) - f(x-h)) / (2h),     błąd O(h^2)
double druga_pochodna(F1 f, double x, double h);        // (f(x+h) - 2f(x) + f(x-h)) / h^2

// Gradient różnicą centralną w każdym kierunku: g_i = (f(x + h e_i) - f(x - h e_i)) / (2h).
Vec gradient(FN f, const Vec& x, double h = 1e-6);

// Hesjan: H_ii = druga różnica po x_i,
// H_ij = (f(++) - f(+-) - f(-+) + f(--)) / (4 h^2) dla i != j. Wynik symetryczny.
Mat hesjan(FN f, const Vec& x, double h = 1e-4);

// Krok h minimalizujący błąd różnicy centralnej ~ eps^(1/3) * max(1, |x|).
double krok_optymalny_centralny(double x);
