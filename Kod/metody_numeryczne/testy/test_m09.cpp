#include "../test.h"
#include "../m09.h"

// Element 1 siatki Test1_4_4 (węzły 1, 2, 6, 5): bok 1/30 m, x i y maleją.
static const double h = 1.0 / 30;
static const double X[4] = {0.1, 0.1 - h, 0.1 - h, 0.1}, Y[4] = {0.005, 0.005, 0.005 - h, 0.005 - h};

TEST("jakobian Test1: J = diag(-0.016667), det J = 0.000277778") {
    double J[2][2]; V4 dx, dy;
    double p = -1 / std::sqrt(3.0);
    double det = jakobian(X, Y, p, p, J, dx, dy);
    BLISKO(J[0][0], -h / 2, 1e-12); BLISKO(J[1][1], -h / 2, 1e-12); BLISKO(J[0][1], 0, 1e-15);
    BLISKO(det, 0.000277778, 1e-9);
    BLISKO(dx[0], 23.6603, 1e-4); BLISKO(dx[2], -6.33975, 1e-4);
    BLISKO(dy[0], 23.6603, 1e-4); BLISKO(dy[1], 6.33975, 1e-4);
}

TEST("jakobian trapezu w pc1: det J = 0.00018943") {
    double x[4] = {0, 0.04, 0.03, 0}, y[4] = {0, 0, 0.02, 0.02}, J[2][2]; V4 dx, dy;
    double p = -1 / std::sqrt(3.0);
    BLISKO(jakobian(x, y, p, p, J, dx, dy), 0.00018943, 1e-8);
    BLISKO(J[1][0], -0.0010566, 1e-7);
}

TEST("H lokalne Test1 (k = 25): 16.6667, -4.16667, -8.33333; sumy wierszy 0") {
    M4 H = macierz_H(X, Y, 25, 2);
    BLISKO(H[0][0], 16.6667, 1e-4); BLISKO(H[0][1], -4.16667, 1e-5); BLISKO(H[0][2], -8.33333, 1e-5);
    for (int i = 0; i < 4; ++i) BLISKO(H[i][0] + H[i][1] + H[i][2] + H[i][3], 0, 1e-12);
    M4 H3 = macierz_H(X, Y, 25, 3);
    BLISKO(H3[0][0], 16.6667, 1e-4);                          // prostokąt: 2 punkty wystarczą
}

TEST("C lokalne Test1 (c rho = 5.46e6): 674.074, 337.037, 168.519") {
    M4 C = macierz_C(X, Y, 700 * 7800, 2);
    BLISKO(C[0][0], 674.074, 1e-3); BLISKO(C[0][1], 337.037, 1e-3); BLISKO(C[0][2], 168.519, 1e-3);
}

TEST("Hbc i P narożnego elementu Test1 (alfa 300, t_ot 1200)") {
    bool bc[4] = {true, true, false, true};                  // węzły 1, 2, 5 mają BC, 6 nie
    M4 Hbc; V4 P;
    brzeg(X, Y, bc, 300, 1200, 2, Hbc, P);
    BLISKO(Hbc[0][0], 6.66667, 1e-5); BLISKO(Hbc[0][1], 1.66667, 1e-5); BLISKO(Hbc[0][3], 1.66667, 1e-5);
    BLISKO(Hbc[1][1], 3.33333, 1e-5); BLISKO(Hbc[2][2], 0, 0); BLISKO(Hbc[1][2], 0, 0);
    BLISKO(P[0], 12000, 1e-8); BLISKO(P[1], 6000, 1e-8); BLISKO(P[2], 0, 0); BLISKO(P[3], 6000, 1e-8);
}

URUCHOM("m09 element 4-węzłowy MES")
