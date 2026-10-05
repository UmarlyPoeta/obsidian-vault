#include "../test.h"
#include "../m03.h"

TEST("Lagrange: przez 3 punkty paraboli x^2") {
    Vec xs = {0, 1, 3}, ys = {0, 1, 9};
    BLISKO(lagrange(xs, ys, 2.0), 4.0, 1e-14);
    BLISKO(lagrange(xs, ys, -1.5), 2.25, 1e-14);
}

TEST("Lagrange: odtwarza wartości w węzłach") {
    Vec xs = {-1, 0.5, 2, 4}, ys = {3, -1, 7, 0.25};
    for (size_t i = 0; i < xs.size(); ++i) BLISKO(lagrange(xs, ys, xs[i]), ys[i], 1e-13);
}

TEST("ilorazy różnicowe: f = x^3, węzły 0, 1, 2, 3") {
    Vec b = ilorazy_roznicowe({0, 1, 2, 3}, {0, 1, 8, 27});
    BLISKO(b[0], 0, 1e-14); BLISKO(b[1], 1, 1e-14); BLISKO(b[2], 3, 1e-14); BLISKO(b[3], 1, 1e-14);
}

TEST("Newton = Lagrange w dowolnym punkcie") {
    Vec xs = {-1, 0.5, 2, 4}, ys = {3, -1, 7, 0.25};
    Vec b = ilorazy_roznicowe(xs, ys);
    for (double x : {-0.7, 1.1, 3.3}) BLISKO(newton_wartosc(xs, b, x), lagrange(xs, ys, x), 1e-12);
}

TEST("interpolacja 1D: przykład z labu 1 MES (38,92)") {
    BLISKO(interpolacja_1d(-5, 4, 20, 46, 1.55), 38.92222, 1e-4);
    BLISKO(interpolacja_1d(-5, 4, 20, 46, -0.5), 33.0, 1e-12);
}

TEST("N4: suma = 1, delta Kroneckera w węzłach") {
    auto N = N4(0.3, -0.7);
    BLISKO(N[0] + N[1] + N[2] + N[3], 1.0, 1e-15);
    const double w[4][2] = {{-1, -1}, {1, -1}, {1, 1}, {-1, 1}};
    for (int i = 0; i < 4; ++i) {
        auto Ni = N4(w[i][0], w[i][1]);
        for (int j = 0; j < 4; ++j) BLISKO(Ni[j], i == j ? 1.0 : 0.0, 1e-15);
    }
}

TEST("dN4: wartości w pc1 = (-1/sqrt3, -1/sqrt3) jak w pliku Kustry") {
    double p = -1 / std::sqrt(3.0);
    auto dk = dN4_dksi(p), de = dN4_deta(p);
    BLISKO(dk[0], -0.394338, 1e-6); BLISKO(dk[1], 0.394338, 1e-6);
    BLISKO(dk[2], 0.105662, 1e-6);  BLISKO(dk[3], -0.105662, 1e-6);
    BLISKO(de[0], -0.394338, 1e-6); BLISKO(de[1], -0.105662, 1e-6);
    BLISKO(dk[0] + dk[1] + dk[2] + dk[3], 0.0, 1e-15);
}

TEST("interpolacja 2D w elemencie prostokątnym (43,125)") {
    auto N = N4(0.5, -0.5);
    double T[4] = {20, 40, 80, 30}, t = 0;
    for (int i = 0; i < 4; ++i) t += N[i] * T[i];
    BLISKO(t, 43.125, 1e-12);
}

URUCHOM("m03 interpolacja i funkcje kształtu")
