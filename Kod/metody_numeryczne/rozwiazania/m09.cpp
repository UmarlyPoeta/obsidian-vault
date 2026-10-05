#include "../m09.h"
#include "../m03.h"
#include "../m06.h"

double jakobian(const double x[4], const double y[4], double ksi, double eta,
                double J[2][2], V4& dNdx, V4& dNdy) {
    auto dk = dN4_dksi(eta), de = dN4_deta(ksi);
    J[0][0] = J[0][1] = J[1][0] = J[1][1] = 0;
    for (int i = 0; i < 4; ++i) {
        J[0][0] += dk[i] * x[i]; J[0][1] += dk[i] * y[i];
        J[1][0] += de[i] * x[i]; J[1][1] += de[i] * y[i];
    }
    double det = J[0][0] * J[1][1] - J[0][1] * J[1][0];
    for (int i = 0; i < 4; ++i) {          // [dN/dx; dN/dy] = J^-1 [dN/dksi; dN/deta]
        dNdx[i] = ( J[1][1] * dk[i] - J[0][1] * de[i]) / det;
        dNdy[i] = (-J[1][0] * dk[i] + J[0][0] * de[i]) / det;
    }
    return det;
}

M4 macierz_H(const double x[4], const double y[4], double k, int npc) {
    Vec p, w; gauss_wezly(npc, p, w);
    M4 H{};
    for (int b = 0; b < npc; ++b)
        for (int a = 0; a < npc; ++a) {
            double J[2][2]; V4 dx, dy;
            double det = jakobian(x, y, p[a], p[b], J, dx, dy);
            for (int i = 0; i < 4; ++i)
                for (int j = 0; j < 4; ++j) H[i][j] += k * (dx[i] * dx[j] + dy[i] * dy[j]) * w[a] * w[b] * det;
        }
    return H;
}

M4 macierz_C(const double x[4], const double y[4], double c_rho, int npc) {
    Vec p, w; gauss_wezly(npc, p, w);
    M4 C{};
    for (int b = 0; b < npc; ++b)
        for (int a = 0; a < npc; ++a) {
            double J[2][2]; V4 dx, dy;
            double det = jakobian(x, y, p[a], p[b], J, dx, dy);
            auto N = N4(p[a], p[b]);
            for (int i = 0; i < 4; ++i)
                for (int j = 0; j < 4; ++j) C[i][j] += c_rho * N[i] * N[j] * w[a] * w[b] * det;
        }
    return C;
}

void brzeg(const double x[4], const double y[4], const bool bc[4], double alfa, double t_ot, int npc,
           M4& Hbc, V4& P) {
    Vec p, w; gauss_wezly(npc, p, w);
    Hbc = M4{}; P = V4{};
    for (int a = 0; a < 4; ++a) {
        int b = (a + 1) % 4;
        if (!bc[a] || !bc[b]) continue;
        double detJ = std::hypot(x[b] - x[a], y[b] - y[a]) / 2;
        for (int i = 0; i < npc; ++i) {
            V4 N{};
            N[a] = 0.5 * (1 - p[i]); N[b] = 0.5 * (1 + p[i]);
            for (int r = 0; r < 4; ++r) {
                for (int c = 0; c < 4; ++c) Hbc[r][c] += alfa * N[r] * N[c] * w[i] * detJ;
                P[r] += alfa * N[r] * t_ot * w[i] * detJ;
            }
        }
    }
}
