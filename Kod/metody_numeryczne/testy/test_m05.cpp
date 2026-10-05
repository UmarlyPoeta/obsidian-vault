#include "../test.h"
#include "../m05.h"

TEST("różnica przednia: błąd ~ h (połowa kroku = połowa błędu)") {
    auto f = [](double x) { return std::exp(x); };
    double e1 = std::fabs(roznica_przednia(f, 1, 1e-2) - std::exp(1.0));
    double e2 = std::fabs(roznica_przednia(f, 1, 5e-3) - std::exp(1.0));
    BLISKO(e1 / e2, 2.0, 0.05);
}

TEST("różnica centralna: błąd ~ h^2 (połowa kroku = 1/4 błędu)") {
    auto f = [](double x) { return std::sin(x); };
    double e1 = std::fabs(roznica_centralna(f, 1, 1e-2) - std::cos(1.0));
    double e2 = std::fabs(roznica_centralna(f, 1, 5e-3) - std::cos(1.0));
    BLISKO(e1 / e2, 4.0, 0.05);
}

TEST("druga pochodna x^4 w x = 1 (12)") {
    BLISKO(druga_pochodna([](double x) { return x * x * x * x; }, 1, 1e-3), 12.0, 1e-5);
}

TEST("krok optymalny daje błąd ~1e-10 dla e^x") {
    double h = krok_optymalny_centralny(1.0);
    BLISKO(roznica_centralna([](double x) { return std::exp(x); }, 1, h), std::exp(1.0), 1e-9);
}

static double rosen(const Vec& v) {                       // funkcja Rosenbrocka
    double x = v[0], y = v[1];
    return 100 * (y - x * x) * (y - x * x) + (1 - x) * (1 - x);
}

TEST("gradient Rosenbrocka w (-1, 2): (396, 200)") {
    Vec g = gradient(rosen, {-1, 2});
    BLISKO(g[0], 396, 1e-5); BLISKO(g[1], 200, 1e-5);
}

TEST("hesjan Rosenbrocka w (1, 1): [[802, -400], [-400, 200]]") {
    Mat H = hesjan(rosen, {1, 1});
    BLISKO(H[0][0], 802, 1e-3); BLISKO(H[0][1], -400, 1e-3);
    BLISKO(H[1][0], -400, 1e-3); BLISKO(H[1][1], 200, 1e-3);
}

TEST("hesjan formy kwadratowej x^2 + 3xy + 5y^2 + z^2") {
    Mat H = hesjan([](const Vec& v) { return v[0] * v[0] + 3 * v[0] * v[1] + 5 * v[1] * v[1] + v[2] * v[2]; },
                   {0.3, -0.2, 1.0});
    BLISKO(H[0][0], 2, 1e-5); BLISKO(H[0][1], 3, 1e-5); BLISKO(H[1][1], 10, 1e-5);
    BLISKO(H[2][2], 2, 1e-5); BLISKO(H[0][2], 0, 1e-5);
}

URUCHOM("m05 różniczkowanie")
