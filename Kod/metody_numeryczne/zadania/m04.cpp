// m04 — TWOJA IMPLEMENTACJA. Specyfikacja jest w ../m04.h, testy w ../testy/test_m04.cpp.
// Sprawdzanie: ./sprawdz.sh 04    Wzorzec (dopiero po swojej próbie): ../rozwiazania/m04.cpp
#include "../m04.h"
#include "../m02.h"

// Macierz układu normalnego g_kj = sum_i w_i x_i^k x_i^j oraz prawa strona F_k = sum_i w_i x_i^k y_i,
// k, j = 0..stopien (wzory 7 i 8 z instrukcji).
void uklad_normalny(const Vec& x, const Vec& y, const Vec& w, int stopien, Mat& g, Vec& F) {
    TODO;
}

// Współczynniki a_0..a_n wielomianu F(x) = sum a_j x^j: zbuduj układ normalny i rozwiąż gauss().
Vec aproksymacja(const Vec& x, const Vec& y, const Vec& w, int stopien) {
    TODO;
}

// Wartość wielomianu o współczynnikach a (od a_0) w punkcie x (Horner).
double wielomian(const Vec& a, double x) {
    TODO;
}

// Błąd aproksymacji E = sum_i w_i (y_i - F(x_i))^2 (wzór 1 z instrukcji).
double blad_aproksymacji(const Vec& x, const Vec& y, const Vec& w, const Vec& a) {
    TODO;
}
