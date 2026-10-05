// m00 — TWOJA IMPLEMENTACJA. Specyfikacja jest w ../m00.h, testy w ../testy/test_m00.cpp.
// Sprawdzanie: ./sprawdz.sh 00    Wzorzec (dopiero po swojej próbie): ../rozwiazania/m00.cpp
#include "../m00.h"

// Najmniejsze eps = 2^-k, dla którego 1 + eps != 1 (licz w pętli, dziel przez 2).
double eps_maszynowy() {
    TODO;
}

// Suma elementów po kolei (zwykła pętla).
double suma_naiwna(const Vec& x) {
    TODO;
}

// Suma z kompensacją Kahana: trzymasz poprawkę c gubionych bitów.
double suma_kahana(const Vec& x) {
    TODO;
}

// Błąd względny |x - dokladna| / |dokladna|.
double blad_wzgledny(double x, double dokladna) {
    TODO;
}

// Pierwiastki ax^2 + bx + c = 0 (rzeczywiste, b^2 - 4ac >= 0) bez utraty cyfr:
// q = -(b + sign(b) sqrt(delta)) / 2, x1 = q / a, x2 = c / q. Zwraca (mniejszy, większy).
std::pair<double, double> pierwiastki_kwadratowe(double a, double b, double c) {
    TODO;
}

// Schemat Hornera: a[0] + a[1] x + ... + a[n] x^n  (współczynniki od najniższej potęgi).
double horner(const Vec& a, double x) {
    TODO;
}
