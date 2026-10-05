// m00 — arytmetyka zmiennoprzecinkowa i błędy numeryczne
// Notatka: [[Arytmetyka zmiennoprzecinkowa i błędy numeryczne]]
#pragma once
#include <utility>
#include "mn.h"

// Najmniejsze eps = 2^-k, dla którego 1 + eps != 1 (licz w pętli, dziel przez 2).
double eps_maszynowy();

// Suma elementów po kolei (zwykła pętla).
double suma_naiwna(const Vec& x);

// Suma z kompensacją Kahana: trzymasz poprawkę c gubionych bitów.
double suma_kahana(const Vec& x);

// Błąd względny |x - dokladna| / |dokladna|.
double blad_wzgledny(double x, double dokladna);

// Pierwiastki ax^2 + bx + c = 0 (rzeczywiste, b^2 - 4ac >= 0) bez utraty cyfr:
// q = -(b + sign(b) sqrt(delta)) / 2, x1 = q / a, x2 = c / q. Zwraca (mniejszy, większy).
std::pair<double, double> pierwiastki_kwadratowe(double a, double b, double c);

// Schemat Hornera: a[0] + a[1] x + ... + a[n] x^n  (współczynniki od najniższej potęgi).
double horner(const Vec& a, double x);
