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

static const Mat AG = {{2, 4, 2, 1}, {2, 2, 3, 3}, {4, 2, 2, 1}, {0, 2, 1, 1}};
static const Vec bG = {10, 6, 6, 4};

TEST("lab Gauss: macierz po postępowaniu prostym jak w instrukcji (13)") {
    Mat Ab = macierz_rozszerzona(AG, bG);
    PRAWDA(Ab[0].size() == 5);
    postepowanie_proste(Ab);
    BLISKO(Ab[1][1], -2, 1e-14); BLISKO(Ab[1][4], -4, 1e-14);
    BLISKO(Ab[2][2], -5, 1e-14); BLISKO(Ab[2][3], -7, 1e-14); BLISKO(Ab[2][4], -2, 1e-14);
    BLISKO(Ab[3][3], 0.2, 1e-14); BLISKO(Ab[3][4], -0.8, 1e-14);
    BLISKO(Ab[3][0], 0, 0); BLISKO(Ab[3][1], 0, 1e-15); BLISKO(Ab[3][2], 0, 1e-15);
}

TEST("lab Gauss: rozwiązanie (-1, 1, 6, -4)") {
    Vec x = gauss_bez_wyboru(AG, bG);
    BLISKO(x[0], -1, 1e-13); BLISKO(x[1], 1, 1e-13); BLISKO(x[2], 6, 1e-13); BLISKO(x[3], -4, 1e-13);
}

static const Mat AP = {{2, 4, 2, 1}, {1, 2, 3, 3}, {4, 5, 2, 1}, {0, 1, 2, 9}};
static const Vec bP = {10, 6, 6, 1};

TEST("lab pivoting: bez wyboru zero na przekątnej, z wyborem działa") {
    bool rzucil = false;
    try { gauss_bez_wyboru(AP, bP); } catch (const std::runtime_error&) { rzucil = true; }
    PRAWDA(rzucil);
    Vec x = gauss(AP, bP);
    BLISKO(x[0], -4.01190476, 1e-7); BLISKO(x[1], 4.02380952, 1e-7);
    BLISKO(x[2], 1.27380952, 1e-7); BLISKO(x[3], -0.61904762, 1e-7);
}

TEST("lab pivoting: Gauss-Crout (zamiana kolumn) daje to samo i pamięta kolejność") {
    std::vector<int> kol;
    Vec x = gauss_crout(AP, bP, kol);
    PRAWDA(kol.size() == 4);
    PRAWDA(kol[0] == 1);                       // w wierszu 0 największe jest 4 w kolumnie 1
    BLISKO(x[0], -4.01190476, 1e-7); BLISKO(x[3], -0.61904762, 1e-7);
    PRAWDA(residuum(AP, x, bP) < 1e-12);
}

TEST("lab LU: przykład z instrukcji, L z jedynkami, L U = A") {
    Mat A = {{1, 2, 3}, {4, 5, 6}, {7, 8, 10}};
    Mat LU = rozklad_lu(A), L, U;
    rozdziel_lu(LU, L, U);
    BLISKO(L[1][0], 4, 1e-14); BLISKO(L[2][1], 2, 1e-14); BLISKO(L[0][0], 1, 0); BLISKO(L[0][1], 0, 0);
    BLISKO(U[1][1], -3, 1e-14); BLISKO(U[2][2], 1, 1e-14); BLISKO(U[1][0], 0, 0);
    Vec y = lu_przod(LU, {6, 8, 3});
    BLISKO(y[1], -16, 1e-13); BLISKO(y[2], -7, 1e-13);
    Vec x = lu_wstecz(LU, y);
    BLISKO(x[0], -35.0 / 3, 1e-12); BLISKO(x[1], 58.0 / 3, 1e-12); BLISKO(x[2], -7, 1e-12);
}

TEST("lab LU: skalowanie 1e14 daje dokładny wynik, oryginał nie") {
    Mat A = {{1, 1}, {1, 1.00000000000001}};
    Vec x = rozwiaz_lu(rozklad_lu(A), {2, 2.00000000000001});
    PRAWDA(std::fabs(x[0] - 1) > 1e-3);                  // utrata cyfr w u22 = a22 - a12
    Mat As = {{1e14, 1e14}, {1e14, 1.00000000000001e14}};
    Vec xs = rozwiaz_lu(rozklad_lu(As), {2e14, 2.00000000000001e14});
    BLISKO(xs[0], 1, 1e-12); BLISKO(xs[1], 1, 1e-12);
}

static const Mat AJ = {{4, -1, 1}, {-1, 4, -2}, {1, -2, 4}};
static const Vec bJ = {12, -1, 5};                        // x = (3, 1, 1)

TEST("lab Jacobi: diagonalna dominacja") {
    PRAWDA(diagonalnie_dominujaca(AJ));
    PRAWDA(!diagonalnie_dominujaca({{1, 2}, {3, 4}}));
    PRAWDA(!diagonalnie_dominujaca({{1, 1}, {1, 1}}));    // tylko równości: nie wystarcza
}

TEST("lab Jacobi: zadana liczba iteracji i warunek stopu") {
    Vec x1 = jacobi_n(AJ, bJ, 1);
    BLISKO(x1[0], 3, 1e-15); BLISKO(x1[1], -0.25, 1e-15); BLISKO(x1[2], 1.25, 1e-15);   // D^-1 b
    int it3 = 0, it6 = 0;
    Vec x = jacobi(AJ, bJ, 1e-3, 1000, it3);
    BLISKO(x[0], 3, 1e-2);
    Vec y = jacobi(AJ, bJ, 1e-6, 1000, it6);
    BLISKO(y[0], 3, 1e-5); BLISKO(y[1], 1, 1e-5); BLISKO(y[2], 1, 1e-5);
    PRAWDA(it6 > it3);
}

TEST("Jacobi wolniejszy od Gaussa-Seidla na tej samej macierzy") {
    int itJ = 0, itGS = 0;
    jacobi(AJ, bJ, 1e-10, 1000, itJ);
    gauss_seidel(AJ, bJ, 1e-10, 1000, itGS);
    PRAWDA(itGS < itJ);
}

URUCHOM("m02 układy liniowe")
