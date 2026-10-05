#include "../test.h"
#include "../m04.h"

static const Vec X = {1, 2, 3, 4, 5, 6, 7, 8}, Y = {2, 4, 3, 5, 6, 9, 11, 11}, W(8, 1.0);

TEST("układ normalny z przykładu w instrukcji (g00=8, g01=36, g11=204, F=51, 288)") {
    Mat g; Vec F;
    uklad_normalny(X, Y, W, 1, g, F);
    BLISKO(g[0][0], 8, 0); BLISKO(g[0][1], 36, 0); BLISKO(g[1][0], 36, 0); BLISKO(g[1][1], 204, 0);
    BLISKO(F[0], 51, 0); BLISKO(F[1], 288, 0);
}

TEST("prosta z przykładu: a0 = 0.107, a1 = 1.392") {
    Vec a = aproksymacja(X, Y, W, 1);
    BLISKO(a[0], 0.107142857, 1e-8); BLISKO(a[1], 1.392857143, 1e-8);
}

TEST("wielomian stopnia n odtwarza dokładnie dane z wielomianu stopnia n") {
    Vec x = {-2, -1, 0, 1, 2, 3}, y;
    for (double v : x) y.push_back(1 - 2 * v + 0.5 * v * v);
    Vec a = aproksymacja(x, y, Vec(6, 1.0), 2);
    BLISKO(a[0], 1, 1e-10); BLISKO(a[1], -2, 1e-10); BLISKO(a[2], 0.5, 1e-10);
    BLISKO(blad_aproksymacji(x, y, Vec(6, 1.0), a), 0, 1e-18);
}

TEST("błąd maleje (nie rośnie) ze stopniem wielomianu") {
    double E1 = blad_aproksymacji(X, Y, W, aproksymacja(X, Y, W, 1));
    double E2 = blad_aproksymacji(X, Y, W, aproksymacja(X, Y, W, 2));
    double E3 = blad_aproksymacji(X, Y, W, aproksymacja(X, Y, W, 3));
    PRAWDA(E2 <= E1 + 1e-12 && E3 <= E2 + 1e-12);
    BLISKO(E1, 6.3928571, 1e-6);
}

TEST("waga: punkt z dużą wagą przyciąga prostą") {
    Vec w = W; w[0] = 1000;
    Vec a = aproksymacja(X, Y, w, 1);
    PRAWDA(std::fabs(wielomian(a, 1) - 2) < 0.05);
}

TEST("lab zad 2: stopień 2 wystarcza (E2 = 1.0055, a = 1.896; 0.110; 0.986)") {
    Vec x = {1, 2, 3, 4, 5, 6, 7, 8}, y = {3.2, 5.7, 11.4, 17.6, 27.5, 37.8, 51.4, 65.6}, w(8, 1.0);
    Vec a = aproksymacja(x, y, w, 2);
    BLISKO(a[0], 1.896429, 1e-5); BLISKO(a[1], 0.109524, 1e-5); BLISKO(a[2], 0.985714, 1e-5);
    BLISKO(blad_aproksymacji(x, y, w, a), 1.00548, 1e-4);
    PRAWDA(blad_aproksymacji(x, y, w, aproksymacja(x, y, w, 1)) > 100);
}

URUCHOM("m04 aproksymacja")
