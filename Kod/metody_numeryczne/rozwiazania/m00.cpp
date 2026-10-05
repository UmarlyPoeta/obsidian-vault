#include "../m00.h"

double eps_maszynowy() {
    double eps = 1.0;
    while (1.0 + eps / 2 != 1.0) eps /= 2;
    return eps;
}

double suma_naiwna(const Vec& x) {
    double s = 0;
    for (double v : x) s += v;
    return s;
}

double suma_kahana(const Vec& x) {
    double s = 0, c = 0;                       // c: zgubiona część poprzednich dodawań
    for (double v : x) {
        double y = v - c;
        double t = s + y;
        c = (t - s) - y;                       // to, czego nie udało się dodać
        s = t;
    }
    return s;
}

double blad_wzgledny(double x, double dokladna) {
    return std::fabs(x - dokladna) / std::fabs(dokladna);
}

std::pair<double, double> pierwiastki_kwadratowe(double a, double b, double c) {
    double delta = b * b - 4 * a * c;
    if (delta < 0) throw std::domain_error("delta < 0");
    double q = -0.5 * (b + std::copysign(std::sqrt(delta), b));
    double x1 = q / a, x2 = c / q;
    return x1 < x2 ? std::make_pair(x1, x2) : std::make_pair(x2, x1);
}

double horner(const Vec& a, double x) {
    double w = 0;
    for (int i = (int)a.size() - 1; i >= 0; --i) w = w * x + a[i];
    return w;
}
