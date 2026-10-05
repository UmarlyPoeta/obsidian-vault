// m03 — TWOJA IMPLEMENTACJA. Specyfikacja jest w ../m03.h, testy w ../testy/test_m03.cpp.
// Sprawdzanie: ./sprawdz.sh 03    Wzorzec (dopiero po swojej próbie): ../rozwiazania/m03.cpp
#include "../m03.h"

// Wielomian Lagrange'a przez punkty (xs[i], ys[i]) w punkcie x: sum y_i l_i(x),
// l_i(x) = prod_{j != i} (x - x_j) / (x_i - x_j).
double lagrange(const Vec& xs, const Vec& ys, double x) {
    TODO;
}

// Ilorazy różnicowe Newtona: zwraca b[0..n] (b[k] = f[x0, ..., xk]).
Vec ilorazy_roznicowe(const Vec& xs, const Vec& ys) {
    TODO;
}

// Wartość wielomianu Newtona b0 + b1 (x-x0) + b2 (x-x0)(x-x1) + ... (schemat Hornera).
double newton_wartosc(const Vec& xs, const Vec& b, double x) {
    TODO;
}

// Liniowa interpolacja temperatury w elemencie 1D [x1, x2] z wartościami T1, T2 (lab 1 MES).
double interpolacja_1d(double x1, double x2, double T1, double T2, double x) {
    TODO;
}

// Funkcje kształtu elementu 4-węzłowego w (ksi, eta), kolejność węzłów jak u Kustry:
// 1 (-1,-1), 2 (1,-1), 3 (1,1), 4 (-1,1). N1 = 0.25 (1-ksi)(1-eta) itd.
std::array<double, 4> N4(double ksi, double eta) {
    TODO;
}

// pochodne po ksi (zależą tylko od eta)
std::array<double, 4> dN4_dksi(double eta) {
    TODO;
}

// pochodne po eta (zależą tylko od ksi)
std::array<double, 4> dN4_deta(double ksi) {
    TODO;
}
