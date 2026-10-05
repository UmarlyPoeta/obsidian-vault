#include "../m10.h"
#include "../m02.h"
#include "../m05.h"

double zloty_podzial(F1 f, double a, double b, double eps, int& f_calls) {
    const double k = (std::sqrt(5.0) - 1) / 2;
    double c = b - k * (b - a), d = a + k * (b - a);
    double fc = f(c), fd = f(d);
    f_calls = 2;
    while (b - a >= eps) {
        if (fc < fd) {                      // minimum w [a, d]; stare c staje się nowym d
            b = d; d = c; fd = fc;
            c = b - k * (b - a); fc = f(c);
        } else {                            // minimum w [c, b]; stare d staje się nowym c
            a = c; c = d; fc = fd;
            d = a + k * (b - a); fd = f(d);
        }
        ++f_calls;
    }
    return (a + b) / 2;
}

static double norma(const Vec& v) {
    double s = 0;
    for (double x : v) s += x * x;
    return std::sqrt(s);
}

Vec newton_min(FN f, Vec x, double eps, int maxit, int& iteracje) {
    for (iteracje = 0; iteracje < maxit; ++iteracje) {
        Vec g = gradient(f, x);
        if (norma(g) < eps) return x;
        for (double& v : g) v = -v;
        Vec d = gauss(hesjan(f, x), g);
        for (size_t i = 0; i < x.size(); ++i) x[i] += d[i];
    }
    return x;
}

Vec najszybszy_spadek(FN f, Vec x, double h_max, double eps, int maxit, int& iteracje) {
    for (iteracje = 0; iteracje < maxit; ++iteracje) {
        Vec g = gradient(f, x);
        if (norma(g) < eps) return x;
        auto phi = [&](double h) {
            Vec y = x;
            for (size_t i = 0; i < y.size(); ++i) y[i] -= h * g[i];
            return f(y);
        };
        int fc;
        double h = zloty_podzial(phi, 0, h_max, 1e-10, fc);
        for (size_t i = 0; i < x.size(); ++i) x[i] -= h * g[i];
    }
    return x;
}
