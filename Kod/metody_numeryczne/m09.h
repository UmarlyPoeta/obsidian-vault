// m09 — element 4-węzłowy MES 2D: jakobian, [H], [C], [Hbc], {P}      (laby MES 3–9)
// Notatki: [[Jakobian]], [[Macierz sztywności]], [[Nieustalony przepływ ciepła]].
// Zależy od: m03 (N4, dN4), m06 (gauss_wezly).
#pragma once
#include <array>
#include "mn.h"

using M4 = std::array<std::array<double, 4>, 4>;
using V4 = std::array<double, 4>;

// Jakobian w punkcie (ksi, eta) w zapisie Kustry: J = [[dx/dksi, dy/dksi], [dx/deta, dy/deta]].
// Zwraca det J; w J[2][2] wpisuje macierz; w dNdx, dNdy pochodne funkcji kształtu po x i y.
double jakobian(const double x[4], const double y[4], double ksi, double eta,
                double J[2][2], V4& dNdx, V4& dNdy);

// [H] = sum_p k (dNdx dNdx^T + dNdy dNdy^T) w_p det J_p, npc = 2..4 punkty w każdym kierunku.
M4 macierz_H(const double x[4], const double y[4], double k, int npc);

// [C] = sum_p c rho N N^T w_p det J_p.
M4 macierz_C(const double x[4], const double y[4], double c_rho, int npc);

// [Hbc] i {P} z boków, których oba węzły mają BC (bc[i] = true). Boki: 0-1, 1-2, 2-3, 3-0.
// Na boku: N_a = (1-s)/2, N_b = (1+s)/2, det J boku = L/2.
void brzeg(const double x[4], const double y[4], const bool bc[4], double alfa, double t_ot, int npc,
           M4& Hbc, V4& P);
