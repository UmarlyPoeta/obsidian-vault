#include "../m06.h"
#include "../m03.h"

double prostokaty(F1 f, double a, double b, int n) {
    double h = (b - a) / n, s = 0;
    for (int i = 0; i < n; ++i) s += f(a + (i + 0.5) * h);
    return h * s;
}

double trapezy(F1 f, double a, double b, int n) {
    double h = (b - a) / n, s = 0.5 * (f(a) + f(b));
    for (int i = 1; i < n; ++i) s += f(a + i * h);
    return h * s;
}

double simpson(F1 f, double a, double b, int n) {
    if (n % 2) throw std::invalid_argument("simpson: n musi być parzyste");
    double h = (b - a) / n, s = f(a) + f(b);
    for (int i = 1; i < n; ++i) s += (i % 2 ? 4 : 2) * f(a + i * h);
    return h / 3 * s;
}

void gauss_wezly(int n, Vec& x, Vec& A) {
    switch (n) {
    case 1: x = {0}; A = {2}; break;
    case 2: x = {-1 / std::sqrt(3.0), 1 / std::sqrt(3.0)}; A = {1, 1}; break;
    case 3: x = {-std::sqrt(0.6), 0, std::sqrt(0.6)}; A = {5.0 / 9, 8.0 / 9, 5.0 / 9}; break;
    case 4: {
        double s = std::sqrt(30.0);
        double x1 = std::sqrt(525 + 70 * s) / 35, x2 = std::sqrt(525 - 70 * s) / 35;
        x = {-x1, -x2, x2, x1};
        A = {(18 - s) / 36, (18 + s) / 36, (18 + s) / 36, (18 - s) / 36};
        break;
    }
    default: throw std::invalid_argument("gauss_wezly: n = 1..4");
    }
}

double gauss_legendre(F1 f, double a, double b, int n) {
    Vec x, A;
    gauss_wezly(n, x, A);
    double s = 0;
    for (int i = 0; i < n; ++i) s += A[i] * f((a + b) / 2 + (b - a) / 2 * x[i]);
    return (b - a) / 2 * s;
}

double gauss_2d(F2 f, int n) {
    Vec x, A;
    gauss_wezly(n, x, A);
    double s = 0;
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j) s += A[i] * A[j] * f(x[i], x[j]);
    return s;
}

double calka_czworokat(F2 f, const double x[4], const double y[4], int n) {
    Vec p, A;
    gauss_wezly(n, p, A);
    double s = 0;
    for (int j = 0; j < n; ++j)                   // eta
        for (int i = 0; i < n; ++i) {             // ksi
            auto N = N4(p[i], p[j]);
            auto dk = dN4_dksi(p[j]), de = dN4_deta(p[i]);
            double X = 0, Y = 0, xk = 0, yk = 0, xe = 0, ye = 0;
            for (int k = 0; k < 4; ++k) {
                X += N[k] * x[k];   Y += N[k] * y[k];
                xk += dk[k] * x[k]; yk += dk[k] * y[k];
                xe += de[k] * x[k]; ye += de[k] * y[k];
            }
            double detJ = xk * ye - yk * xe;
            s += A[i] * A[j] * f(X, Y) * detJ;
        }
    return s;
}
