#include "../m03.h"

double lagrange(const Vec& xs, const Vec& ys, double x) {
    double w = 0;
    for (size_t i = 0; i < xs.size(); ++i) {
        double l = 1;
        for (size_t j = 0; j < xs.size(); ++j)
            if (j != i) l *= (x - xs[j]) / (xs[i] - xs[j]);
        w += ys[i] * l;
    }
    return w;
}

Vec ilorazy_roznicowe(const Vec& xs, const Vec& ys) {
    Vec b = ys;
    size_t n = xs.size();
    for (size_t k = 1; k < n; ++k)                       // kolumna k tablicy ilorazów, w miejscu
        for (size_t i = n - 1; i >= k; --i) b[i] = (b[i] - b[i - 1]) / (xs[i] - xs[i - k]);
    return b;
}

double newton_wartosc(const Vec& xs, const Vec& b, double x) {
    double w = b.back();
    for (int k = (int)b.size() - 2; k >= 0; --k) w = w * (x - xs[k]) + b[k];
    return w;
}

double interpolacja_1d(double x1, double x2, double T1, double T2, double x) {
    double N1 = (x2 - x) / (x2 - x1), N2 = (x - x1) / (x2 - x1);
    return N1 * T1 + N2 * T2;
}

std::array<double, 4> N4(double k, double e) {
    return {0.25 * (1 - k) * (1 - e), 0.25 * (1 + k) * (1 - e),
            0.25 * (1 + k) * (1 + e), 0.25 * (1 - k) * (1 + e)};
}
std::array<double, 4> dN4_dksi(double e) {
    return {-0.25 * (1 - e), 0.25 * (1 - e), 0.25 * (1 + e), -0.25 * (1 + e)};
}
std::array<double, 4> dN4_deta(double k) {
    return {-0.25 * (1 - k), -0.25 * (1 + k), 0.25 * (1 + k), 0.25 * (1 - k)};
}
