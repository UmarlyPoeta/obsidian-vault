#include "../m07.h"
#include "../m02.h"

static double krok(F2 f, double x, double y, double h, int metoda) {
    if (metoda == 1) return y + h * f(x, y);
    if (metoda == 2) {
        double k1 = f(x, y), k2 = f(x + h, y + h * k1);
        return y + h / 2 * (k1 + k2);
    }
    if (metoda == 4) {
        double k1 = f(x, y);
        double k2 = f(x + h / 2, y + h / 2 * k1);
        double k3 = f(x + h / 2, y + h / 2 * k2);
        double k4 = f(x + h, y + h * k3);
        return y + h / 6 * (k1 + 2 * k2 + 2 * k3 + k4);
    }
    throw std::invalid_argument("metoda = 1, 2 albo 4");
}

Vec trajektoria(F2 f, double a, double b, double ya, int N, int metoda) {
    double h = (b - a) / N, y = ya;
    Vec w{ya};
    for (int i = 0; i < N; ++i) {
        y = krok(f, a + i * h, y, h, metoda);   // x_i = a + i h, bez sumowania h (mniej błędów)
        w.push_back(y);
    }
    return w;
}

double euler(F2 f, double a, double b, double ya, int N) { return trajektoria(f, a, b, ya, N, 1).back(); }
double rk2(F2 f, double a, double b, double ya, int N) { return trajektoria(f, a, b, ya, N, 2).back(); }
double rk4(F2 f, double a, double b, double ya, int N) { return trajektoria(f, a, b, ya, N, 4).back(); }

Vec schemat_theta(const Mat& C, const Mat& H, const Vec& P, Vec T, double dt, int kroki, double theta) {
    size_t n = T.size();
    Mat A(n, Vec(n)), B(n, Vec(n));
    for (size_t i = 0; i < n; ++i)
        for (size_t j = 0; j < n; ++j) {
            A[i][j] = C[i][j] / dt + theta * H[i][j];
            B[i][j] = C[i][j] / dt - (1 - theta) * H[i][j];
        }
    for (int k = 0; k < kroki; ++k) {
        Vec b = mnoz(B, T);
        for (size_t i = 0; i < n; ++i) b[i] += P[i];
        T = gauss(A, b);
    }
    return T;
}
