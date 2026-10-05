#include "../m05.h"
#include <limits>

double roznica_przednia(F1 f, double x, double h) { return (f(x + h) - f(x)) / h; }
double roznica_centralna(F1 f, double x, double h) { return (f(x + h) - f(x - h)) / (2 * h); }
double druga_pochodna(F1 f, double x, double h) { return (f(x + h) - 2 * f(x) + f(x - h)) / (h * h); }

Vec gradient(FN f, const Vec& x, double h) {
    Vec g(x.size()), xp = x, xm = x;
    for (size_t i = 0; i < x.size(); ++i) {
        xp[i] = x[i] + h; xm[i] = x[i] - h;
        g[i] = (f(xp) - f(xm)) / (2 * h);
        xp[i] = xm[i] = x[i];
    }
    return g;
}

Mat hesjan(FN f, const Vec& x, double h) {
    size_t n = x.size();
    Mat H(n, Vec(n));
    double f0 = f(x);
    for (size_t i = 0; i < n; ++i) {
        Vec a = x, b = x;
        a[i] += h; b[i] -= h;
        H[i][i] = (f(a) - 2 * f0 + f(b)) / (h * h);
        for (size_t j = i + 1; j < n; ++j) {
            Vec pp = x, pm = x, mp = x, mm = x;
            pp[i] += h; pp[j] += h;  pm[i] += h; pm[j] -= h;
            mp[i] -= h; mp[j] += h;  mm[i] -= h; mm[j] -= h;
            H[i][j] = H[j][i] = (f(pp) - f(pm) - f(mp) + f(mm)) / (4 * h * h);
        }
    }
    return H;
}

double krok_optymalny_centralny(double x) {
    return std::cbrt(std::numeric_limits<double>::epsilon()) * std::max(1.0, std::fabs(x));
}
