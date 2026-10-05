#include "../test.h"
#include "../m08.h"

// Przykład z notatki [[Metoda Galerkina]]: doprowadzone 10 kW/m^2 (q = -1e4), konwekcja 100, 20 °C.
static const Pret PR{0.1, 4e-4, 50, 2, -1e4, 100, 20};

TEST("macierz H i wektor P pręta na 2 elementach") {
    Mat H; Vec P;
    pret_HP(PR, H, P);
    BLISKO(H[0][0], 0.4, 1e-14); BLISKO(H[0][1], -0.4, 1e-14); BLISKO(H[1][1], 0.8, 1e-14);
    BLISKO(H[2][2], 0.44, 1e-14); BLISKO(H[0][2], 0.0, 0);
    BLISKO(P[0], -4.0, 1e-12); BLISKO(P[1], 0.0, 0); BLISKO(P[2], -0.8, 1e-12);
}

TEST("stan ustalony: t = 140, 130, 120") {
    Vec t = pret_ustalony(PR);
    BLISKO(t[0], 140, 1e-9); BLISKO(t[1], 130, 1e-9); BLISKO(t[2], 120, 1e-9);
}

TEST("więcej elementów: wartości w węzłach dalej dokładne (liniowy rozkład)") {
    Pret p = PR; p.ne = 10;
    Vec t = pret_ustalony(p);
    for (int i = 0; i <= 10; ++i) BLISKO(t[i], 140 - 200 * (0.01 * i), 1e-8);
}

TEST("bilans: wpływa |q| S = 4 W, odpływa alfa S (t_L - t_inf)") {
    Vec t = pret_ustalony(PR);
    BLISKO(PR.alfa * PR.S * (t.back() - PR.t_inf), -PR.q * PR.S, 1e-9);
}

TEST("macierz C: suma wyrazów = rho c S L") {
    Pret p = PR; p.rho = 7800; p.c = 460; p.ne = 5;
    Mat C = pret_C(p);
    double s = 0;
    for (auto& w : C) for (double v : w) s += v;
    BLISKO(s, 7800 * 460 * 4e-4 * 0.1, 1e-9);
}

TEST("nieustalony: po długim czasie wychodzi stan ustalony") {
    Pret p = PR; p.rho = 7800; p.c = 460;
    Vec t = pret_nieustalony(p, 20, 50, 2000);
    BLISKO(t[0], 140, 1e-6); BLISKO(t[2], 120, 1e-6);
    Vec t1 = pret_nieustalony(p, 20, 50, 1);
    PRAWDA(t1[0] > 20 && t1[0] < 140);                // po jednym kroku ciało dopiero się nagrzewa
}

URUCHOM("m08 MES 1D")
