#include "../test.h"
#include "../m10.h"

TEST("złoty podział: (x - 2)^2 na [0, 5]") {
    int fc = 0;
    double x = zloty_podzial([](double x) { return (x - 2) * (x - 2); }, 0, 5, 1e-6, fc);
    BLISKO(x, 2.0, 1e-6);
    // długość maleje 0.618 razy na iterację: n = ceil(ln(eps / 5) / ln 0.618) = 33, plus 2 startowe f
    PRAWDA(fc >= 33 && fc <= 36);
}

TEST("złoty podział: x^4 - 3x + 1 na [0, 2], minimum w (3/4)^(1/3)") {
    int fc;
    BLISKO(zloty_podzial([](double x) { return x * x * x * x - 3 * x + 1; }, 0, 2, 1e-8, fc),
           std::cbrt(0.75), 1e-7);
}

static double kwadrat(const Vec& v) { return (v[0] - 1) * (v[0] - 1) + 10 * (v[1] + 2) * (v[1] + 2); }
static double rosen(const Vec& v) {
    return 100 * (v[1] - v[0] * v[0]) * (v[1] - v[0] * v[0]) + (1 - v[0]) * (1 - v[0]);
}

TEST("Newton: forma kwadratowa w jednym kroku") {
    int it;
    Vec x = newton_min(kwadrat, {5, 5}, 1e-6, 20, it);
    BLISKO(x[0], 1, 1e-6); BLISKO(x[1], -2, 1e-6);
    PRAWDA(it <= 2);
}

TEST("Newton: Rosenbrock z (-1.2, 1) do (1, 1)") {
    int it;
    Vec x = newton_min(rosen, {-1.2, 1}, 1e-6, 100, it);
    BLISKO(x[0], 1, 1e-5); BLISKO(x[1], 1, 1e-5);
    PRAWDA(it < 20);
}

TEST("najszybszy spadek: forma kwadratowa, zygzak ale zbieżny") {
    int it;
    Vec x = najszybszy_spadek(kwadrat, {5, 5}, 1.0, 1e-5, 1000, it);
    BLISKO(x[0], 1, 1e-4); BLISKO(x[1], -2, 1e-4);
    PRAWDA(it > 3);                                         // wolniej niż Newton
}

URUCHOM("m10 optymalizacja")
