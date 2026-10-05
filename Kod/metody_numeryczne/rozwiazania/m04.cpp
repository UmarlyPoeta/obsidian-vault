#include "../m04.h"
#include "../m02.h"

void uklad_normalny(const Vec& x, const Vec& y, const Vec& w, int n, Mat& g, Vec& F) {
    g.assign(n + 1, Vec(n + 1, 0.0));
    F.assign(n + 1, 0.0);
    for (size_t i = 0; i < x.size(); ++i) {
        Vec pot(2 * n + 1, 1.0);                         // x_i^0 .. x_i^(2n)
        for (int p = 1; p <= 2 * n; ++p) pot[p] = pot[p - 1] * x[i];
        for (int k = 0; k <= n; ++k) {
            for (int j = 0; j <= n; ++j) g[k][j] += w[i] * pot[k + j];
            F[k] += w[i] * pot[k] * y[i];
        }
    }
}

Vec aproksymacja(const Vec& x, const Vec& y, const Vec& w, int stopien) {
    Mat g; Vec F;
    uklad_normalny(x, y, w, stopien, g, F);
    return gauss(g, F);
}

double wielomian(const Vec& a, double x) {
    double s = 0;
    for (int j = (int)a.size() - 1; j >= 0; --j) s = s * x + a[j];
    return s;
}

double blad_aproksymacji(const Vec& x, const Vec& y, const Vec& w, const Vec& a) {
    double E = 0;
    for (size_t i = 0; i < x.size(); ++i) { double r = y[i] - wielomian(a, x[i]); E += w[i] * r * r; }
    return E;
}
