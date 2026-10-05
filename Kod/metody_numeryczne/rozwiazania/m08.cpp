#include "../m08.h"
#include "../m02.h"

void pret_HP(const Pret& p, Mat& H, Vec& P) {
    int n = p.ne + 1;
    double Le = p.L / p.ne, C = p.S * p.k / Le;
    H.assign(n, Vec(n, 0.0));
    P.assign(n, 0.0);
    for (int e = 0; e < p.ne; ++e) {                     // element e łączy węzły e i e+1
        int id[2] = {e, e + 1};
        double He[2][2] = {{C, -C}, {-C, C}};
        for (int i = 0; i < 2; ++i)
            for (int j = 0; j < 2; ++j) H[id[i]][id[j]] += He[i][j];
    }
    H[n - 1][n - 1] += p.alfa * p.S;
    P[0] = p.q * p.S;
    P[n - 1] = -p.alfa * p.S * p.t_inf;
}

Vec pret_ustalony(const Pret& p) {
    Mat H; Vec P;
    pret_HP(p, H, P);
    for (double& v : P) v = -v;
    return gauss(H, P);
}

Mat pret_C(const Pret& p) {
    int n = p.ne + 1;
    double Le = p.L / p.ne, c6 = p.rho * p.c * p.S * Le / 6;
    Mat C(n, Vec(n, 0.0));
    for (int e = 0; e < p.ne; ++e) {
        C[e][e] += 2 * c6;     C[e][e + 1] += c6;
        C[e + 1][e] += c6;     C[e + 1][e + 1] += 2 * c6;
    }
    return C;
}

Vec pret_nieustalony(const Pret& p, double t0, double dt, int kroki) {
    Mat H, C = pret_C(p); Vec P;
    pret_HP(p, H, P);
    int n = p.ne + 1;
    Mat A = H;
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j) A[i][j] += C[i][j] / dt;
    Vec t(n, t0);
    for (int s = 0; s < kroki; ++s) {
        Vec b(n, 0.0);
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) b[i] += C[i][j] / dt * t[j];
            b[i] -= P[i];
        }
        t = gauss(A, b);
    }
    return t;
}
