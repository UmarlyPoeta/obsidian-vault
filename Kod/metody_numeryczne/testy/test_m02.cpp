#include "../test.h"
#include "../m02.h"

static const Mat A3 = {{2, 1, -1}, {-3, -1, 2}, {-2, 1, 2}};
static const Vec b3 = {8, -11, -3};                         // x = (2, 3, -1)

TEST("gauss: klasyczny układ 3x3") {
    Vec x = gauss(A3, b3);
    BLISKO(x[0], 2, 1e-12); BLISKO(x[1], 3, 1e-12); BLISKO(x[2], -1, 1e-12);
}

TEST("gauss: zero na przekątnej wymaga zamiany wierszy") {
    Vec x = gauss({{0, 1}, {1, 1}}, {1, 3});
    BLISKO(x[0], 2, 1e-14); BLISKO(x[1], 1, 1e-14);
}

TEST("gauss: macierz osobliwa rzuca wyjątek") {
    bool rzucil = false;
    try { gauss({{1, 2}, {2, 4}}, {1, 2}); } catch (const std::runtime_error&) { rzucil = true; }
    PRAWDA(rzucil);
}

TEST("gauss: układ z labu aproksymacji (8, 36; 36, 204)") {
    Vec a = gauss({{8, 36}, {36, 204}}, {51, 288});
    BLISKO(a[0], 0.107, 1e-3); BLISKO(a[1], 1.392, 1e-3);
}

TEST("LU: rozkład i dwa różne prawe strony") {
    Mat LU = rozklad_lu({{4, 3}, {6, 3}});
    BLISKO(LU[1][0], 1.5, 1e-15);                      // l_21
    BLISKO(LU[1][1], -1.5, 1e-15);                     // u_22
    Vec x = rozwiaz_lu(LU, {10, 12});
    BLISKO(x[0], 1, 1e-14); BLISKO(x[1], 2, 1e-14);
    x = rozwiaz_lu(LU, {7, 9});
    BLISKO(x[0], 1, 1e-14); BLISKO(x[1], 1, 1e-14);
}

TEST("Thomas: pręt -T'' = 2 na 4 węzłach wewnętrznych") {
    int n = 4; double h = 1.0 / (n + 1);
    Vec a(n, -1), d(n, 2), c(n, -1), b(n, 2 * h * h);
    Vec x = thomas(a, d, c, b);
    for (int i = 0; i < n; ++i) { double xi = (i + 1) * h; BLISKO(x[i], xi * (1 - xi), 1e-12); }
}

TEST("Gauss-Seidel: macierz diagonalnie dominująca") {
    Mat A = {{10, -1, 2, 0}, {-1, 11, -1, 3}, {2, -1, 10, -1}, {0, 3, -1, 8}};
    Vec b = {6, 25, -11, 15};
    int it = 0;
    Vec x = gauss_seidel(A, b, 1e-12, 100, it);
    BLISKO(x[0], 1, 1e-10); BLISKO(x[1], 2, 1e-10); BLISKO(x[2], -1, 1e-10); BLISKO(x[3], 1, 1e-10);
    PRAWDA(it > 5 && it < 40);
    PRAWDA(residuum(A, x, b) < 1e-10);
}

URUCHOM("m02 układy liniowe")
