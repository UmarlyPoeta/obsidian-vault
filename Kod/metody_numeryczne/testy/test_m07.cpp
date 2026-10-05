#include "../test.h"
#include "../m07.h"

static double f_przyklad(double x, double y) { return x * x + y; }

TEST("Euler: przykład z instrukcji y' = x^2 + y, y(0) = 0.1, N = 3 -> 0.1382") {
    Vec t = trajektoria(f_przyklad, 0, 0.3, 0.1, 3, 1);
    BLISKO(t[1], 0.11, 1e-14); BLISKO(t[2], 0.122, 1e-14); BLISKO(t[3], 0.1382, 1e-14);
    BLISKO(euler(f_przyklad, 0, 0.3, 0.1, 3), 0.1382, 1e-14);
}

TEST("RK2 i RK4: y' = y, y(0) = 1, y(1) = e") {
    auto f = [](double, double y) { return y; };
    BLISKO(rk2(f, 0, 1, 1, 100), std::exp(1.0), 1e-4);
    BLISKO(rk4(f, 0, 1, 1, 10), std::exp(1.0), 3e-6);
}

TEST("rząd zbieżności: Euler 1, RK2 2, RK4 4 (y' = x + y)") {
    auto f = [](double x, double y) { return x + y; };
    double dokl = 1.1 * std::exp(1.0) - 2;                       // y = 1.1 e^x - x - 1
    auto rzad = [&](double (*m)(F2, double, double, double, int)) {
        double e1 = std::fabs(m(f, 0, 1, 0.1, 20) - dokl), e2 = std::fabs(m(f, 0, 1, 0.1, 40) - dokl);
        return std::log2(e1 / e2);
    };
    BLISKO(rzad(euler), 1.0, 0.1);
    BLISKO(rzad(rk2), 2.0, 0.1);
    BLISKO(rzad(rk4), 4.0, 0.15);
}

TEST("lab zad 2: y' = 2(x+1) y, y(0) = 1, RK4 N = 10 blisko e^3") {
    auto f = [](double x, double y) { return 2 * (x + 1) * y; };
    BLISKO(rk4(f, 0, 1, 1, 10), std::exp(3.0), 0.02);
    PRAWDA(euler(f, 0, 1, 1, 10) < rk2(f, 0, 1, 1, 10));       // Euler zaniża najmocniej
}

TEST("lab zad 3: kulka w oleju, RK4 h = 0.1 -> prędkość graniczna sqrt(mg/k)") {
    double m = 0.05, g = 9.81, k = 0.1;
    auto f = [=](double, double v) { return g - k / m * v * v; };
    BLISKO(rk4(f, 0, 10, 0, 100), std::sqrt(m * g / k), 1e-8);
    BLISKO(euler(f, 0, 10, 0, 100), std::sqrt(m * g / k), 1e-8);
}

TEST("schemat θ: jeden węzeł C dT/dt + H T = P dąży do P/H") {
    Mat C = {{2.0}}, H = {{0.5}};
    Vec T = schemat_theta(C, H, {10.0}, {0.0}, 1.0, 200, 1.0);
    BLISKO(T[0], 20.0, 1e-8);
    Vec T1 = schemat_theta(C, H, {10.0}, {0.0}, 1.0, 1, 1.0);  // (2 + 0.5) T1 = 10
    BLISKO(T1[0], 4.0, 1e-14);
    Vec Tcn = schemat_theta(C, H, {10.0}, {0.0}, 1.0, 1, 0.5); // (2 + 0.25) T1 = 10
    BLISKO(Tcn[0], 10.0 / 2.25, 1e-14);
}

URUCHOM("m07 równania różniczkowe")
