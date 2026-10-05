#include "../m01.h"

Wynik bisekcja(F1 f, double a, double b, double eps, int maxit) {
    if (f(a) * f(b) >= 0) throw std::invalid_argument("bisekcja: brak zmiany znaku na [a, b]");
    Wynik w;
    for (w.iteracje = 1; w.iteracje <= maxit; ++w.iteracje) {
        double x0 = (a + b) / 2, fx = f(x0);
        w.x = x0;
        w.historia.push_back(x0);
        if (std::fabs(fx) < eps) { w.zbiezna = true; return w; }
        if (f(a) * fx < 0) b = x0; else a = x0;
    }
    w.iteracje = maxit;
    return w;
}

Wynik regula_falsi(F1 f, double a, double b, double eps, int maxit) {
    if (f(a) * f(b) >= 0) throw std::invalid_argument("regula falsi: brak zmiany znaku na [a, b]");
    Wynik w;
    for (w.iteracje = 1; w.iteracje <= maxit; ++w.iteracje) {
        double fa = f(a), fb = f(b);
        double x1 = a - fa * (b - a) / (fb - fa), fx = f(x1);
        w.x = x1;
        w.historia.push_back(x1);
        if (std::fabs(fx) < eps) { w.zbiezna = true; return w; }
        if (fx * fa < 0) b = x1; else a = x1;
    }
    w.iteracje = maxit;
    return w;
}

Wynik styczne(F1 f, F1 df, double x0, double eps, int maxit) {
    Wynik w;
    double x = x0;
    for (w.iteracje = 1; w.iteracje <= maxit; ++w.iteracje) {
        double d = df(x);
        if (d == 0) throw std::runtime_error("styczne: f'(x) = 0");
        x = x - f(x) / d;
        w.x = x;
        w.historia.push_back(x);
        if (std::fabs(f(x)) < eps) { w.zbiezna = true; return w; }
    }
    w.iteracje = maxit;
    return w;
}

Wynik sieczne(F1 f, double x0, double x_poprz, double eps, int maxit) {
    Wynik w;
    double xk = x0, xp = x_poprz;
    for (w.iteracje = 1; w.iteracje <= maxit; ++w.iteracje) {
        double fk = f(xk), fp = f(xp);
        if (fk == fp) throw std::runtime_error("sieczne: f(x_k) = f(x_{k-1})");
        double xn = xk - fk * (xk - xp) / (fk - fp);
        xp = xk; xk = xn;
        w.x = xk;
        w.historia.push_back(xk);
        if (std::fabs(f(xk)) < eps) { w.zbiezna = true; return w; }
    }
    w.iteracje = maxit;
    return w;
}

Wynik newton_uklad2(const Uklad2& u, double x, double y, double eps, int maxit, double& y_wynik) {
    Wynik w;
    for (w.iteracje = 1; w.iteracje <= maxit; ++w.iteracje) {
        double F = u.F(x, y), G = u.G(x, y);
        double a = u.Fx(x, y), b = u.Fy(x, y), c = u.Gx(x, y), d = u.Gy(x, y);
        double det = a * d - b * c;
        if (det == 0) throw std::runtime_error("newton_uklad2: det J = 0");
        double dx = (-F * d + b * G) / det;    // J [dx dy]^T = -[F G]^T, Cramer
        double dy = (-a * G + c * F) / det;
        x += dx; y += dy;
        w.x = x; y_wynik = y;
        w.historia.push_back(x);
        if (std::fabs(u.F(x, y)) + std::fabs(u.G(x, y)) < eps) { w.zbiezna = true; return w; }
    }
    w.iteracje = maxit;
    return w;
}

double rzad_zbieznosci(const Vec& h, double dokladne) {
    if (h.size() < 3) throw std::invalid_argument("rzad_zbieznosci: za mało iteracji");
    size_t n = h.size();
    double e2 = std::fabs(h[n - 1] - dokladne), e1 = std::fabs(h[n - 2] - dokladne),
           e0 = std::fabs(h[n - 3] - dokladne);
    return std::log(e2 / e1) / std::log(e1 / e0);
}
