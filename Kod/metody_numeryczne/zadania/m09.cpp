// m09 — TWOJA IMPLEMENTACJA. Specyfikacja jest w ../m09.h, testy w ../testy/test_m09.cpp.
// Sprawdzanie: ./sprawdz.sh 09    Wzorzec (dopiero po swojej próbie): ../rozwiazania/m09.cpp
#include "../m09.h"
#include "../m03.h"
#include "../m06.h"

// Jakobian w punkcie (ksi, eta) w zapisie Kustry: J = [[dx/dksi, dy/dksi], [dx/deta, dy/deta]].
// Zwraca det J; w J[2][2] wpisuje macierz; w dNdx, dNdy pochodne funkcji kształtu po x i y.
double jakobian(const double x[4], const double y[4], double ksi, double eta, double J[2][2], V4& dNdx, V4& dNdy) {
    TODO;
}

// [H] = sum_p k (dNdx dNdx^T + dNdy dNdy^T) w_p det J_p, npc = 2..4 punkty w każdym kierunku.
M4 macierz_H(const double x[4], const double y[4], double k, int npc) {
    TODO;
}

// [C] = sum_p c rho N N^T w_p det J_p.
M4 macierz_C(const double x[4], const double y[4], double c_rho, int npc) {
    TODO;
}

// [Hbc] i {P} z boków, których oba węzły mają BC (bc[i] = true). Boki: 0-1, 1-2, 2-3, 3-0.
// Na boku: N_a = (1-s)/2, N_b = (1+s)/2, det J boku = L/2.
void brzeg(const double x[4], const double y[4], const bool bc[4], double alfa, double t_ot, int npc, M4& Hbc, V4& P) {
    TODO;
}
