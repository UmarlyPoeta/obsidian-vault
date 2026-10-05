#include "../test.h"
#include "../m06.h"

static double sin_(double x) { return std::sin(x); }
static const double SIN_DOKL = std::cos(0.5) - std::cos(2.5);           // 1.678727...

TEST("prostokąty, trapezy, Simpson dla sin na [0.5, 2.5], n = 20") {
    BLISKO(prostokaty(sin_, 0.5, 2.5, 20), SIN_DOKL, 1e-3);
    BLISKO(trapezy(sin_, 0.5, 2.5, 20), SIN_DOKL, 2e-3);
    BLISKO(simpson(sin_, 0.5, 2.5, 20), SIN_DOKL, 1e-6);
    PRAWDA(std::fabs(prostokaty(sin_, 0.5, 2.5, 20) - SIN_DOKL) < std::fabs(trapezy(sin_, 0.5, 2.5, 20) - SIN_DOKL));
}

TEST("Simpson: nieparzyste n rzuca wyjątek") {
    bool rzucil = false;
    try { simpson(sin_, 0, 1, 3); } catch (const std::invalid_argument&) { rzucil = true; }
    PRAWDA(rzucil);
}

TEST("węzły Gaussa: suma wag = 2 dla n = 1..4") {
    for (int n = 1; n <= 4; ++n) {
        Vec x, A; gauss_wezly(n, x, A);
        double s = 0;
        for (double a : A) s += a;
        BLISKO(s, 2.0, 1e-14);
        PRAWDA((int)x.size() == n);
    }
}

TEST("Gauss 2 węzły: przykład z instrukcji (1.67163)") {
    BLISKO(gauss_legendre(sin_, 0.5, 2.5, 2), 1.67163, 1e-5);
}

TEST("Gauss 4 węzły: sin na [0.5, 2.5] z błędem < 1e-6") {
    BLISKO(gauss_legendre(sin_, 0.5, 2.5, 4), SIN_DOKL, 1e-6);
}

TEST("dokładność: n węzłów całkuje dokładnie wielomiany stopnia 2n - 1") {
    auto I3 = [](double x) { return x * x * x * x + x * x + 1; };          // stopień 4
    BLISKO(gauss_legendre(I3, -1, 1, 3), 2.0 / 5 + 2.0 / 3 + 2, 1e-14);
    PRAWDA(std::fabs(gauss_legendre(I3, -1, 1, 2) - (2.0 / 5 + 2.0 / 3 + 2)) > 1e-3);
    auto I5 = [](double x) { return std::pow(x, 6) + x * x; };             // stopień 6
    BLISKO(gauss_legendre(I5, -1, 1, 4), 2.0 / 7 + 2.0 / 3, 1e-14);
    PRAWDA(std::fabs(gauss_legendre(I5, -1, 1, 3) - (2.0 / 7 + 2.0 / 3)) > 1e-3);
}

TEST("Kustra: 0.5x^2 + 2x + 3 na [-5, 4] = 49.5 (2 i 3 węzły)") {
    auto f = [](double x) { return 0.5 * x * x + 2 * x + 3; };
    BLISKO(gauss_legendre(f, -5, 4, 2), 49.5, 1e-12);
    BLISKO(gauss_legendre(f, -5, 4, 3), 49.5, 1e-12);
}

TEST("Gauss 2D: 5x^2y^2 + 3xy + 6 na [-1,1]^2 = 26.222 (praca domowa MES)") {
    auto f = [](double x, double y) { return 5 * x * x * y * y + 3 * x * y + 6; };
    BLISKO(gauss_2d(f, 2), 24 + 20.0 / 9, 1e-12);
    BLISKO(gauss_2d(f, 3), 24 + 20.0 / 9, 1e-12);
}

TEST("całka po czworokącie: pole trapezu = 0.0007") {
    double x[4] = {0, 0.04, 0.03, 0}, y[4] = {0, 0, 0.02, 0.02};
    BLISKO(calka_czworokat([](double, double) { return 1.0; }, x, y, 2), 0.0007, 1e-15);
}

TEST("całka po czworokącie: x po kwadracie [1,3]x[0,2] = 8") {
    double x[4] = {1, 3, 3, 1}, y[4] = {0, 0, 2, 2};
    BLISKO(calka_czworokat([](double X, double) { return X; }, x, y, 2), 8.0, 1e-13);
}

URUCHOM("m06 całkowanie")
