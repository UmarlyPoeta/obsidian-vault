#include "../test.h"
#include "../m01.h"

static double f_rn1(double x) { return -x * x * x + 10 * x + 5; }
static double df_rn1(double x) { return -3 * x * x + 10; }

TEST("styczne: przykład z instrukcji RN1 (x1, x2, x3)") {
    Wynik w = styczne(f_rn1, df_rn1, 6.0, 1e-12);
    BLISKO(w.historia[0], 4.459, 1e-3);
    BLISKO(w.historia[1], 3.672, 1e-3);
    BLISKO(w.historia[2], 3.416, 1e-3);
    PRAWDA(w.zbiezna);
    BLISKO(f_rn1(w.x), 0.0, 1e-12);
}

TEST("styczne: rząd zbieżności ~2 dla x^2 - 2") {
    Wynik w = styczne([](double x) { return x * x - 2; }, [](double x) { return 2 * x; }, 1.0, 1e-10);
    BLISKO(w.x, std::sqrt(2.0), 1e-10);
    w.historia.pop_back();                          // ostatni błąd bywa zerem w double
    BLISKO(rzad_zbieznosci(w.historia, std::sqrt(2.0)), 2.0, 0.15);
}

TEST("sieczne: ten sam pierwiastek co styczne, x_{-1} = x0 - 0.1") {
    Wynik ws = sieczne(f_rn1, 6.0, 5.9, 1e-10);
    Wynik wn = styczne(f_rn1, df_rn1, 6.0, 1e-10);
    BLISKO(ws.x, wn.x, 1e-9);
    PRAWDA(ws.iteracje >= wn.iteracje);
}

TEST("styczne: f'(x) = 0 rzuca wyjątek") {
    bool rzucil = false;
    try { styczne([](double x) { return x * x + 1; }, [](double x) { return 2 * x; }, 0.0, 1e-8); }
    catch (const std::runtime_error&) { rzucil = true; }
    PRAWDA(rzucil);
}

TEST("bisekcja: przykład z RN2, kroki 1, 1.5, 1.75") {
    auto f = [](double x) { return std::cos(x * x * x - 2 * x); };
    Wynik w = bisekcja(f, 0, 2, 0.01);
    BLISKO(w.historia[0], 1.0, 0); BLISKO(w.historia[1], 1.5, 0); BLISKO(w.historia[2], 1.75, 0);
    PRAWDA(w.zbiezna);
    PRAWDA(std::fabs(f(w.x)) < 0.01);
}

TEST("bisekcja: brak zmiany znaku rzuca wyjątek") {
    bool rzucil = false;
    try { bisekcja([](double x) { return x * x + 1; }, -1, 1, 1e-6); }
    catch (const std::invalid_argument&) { rzucil = true; }
    PRAWDA(rzucil);
}

TEST("RN2 zad 3: e^x - 100 na [0, 10], bisekcja i falsi") {
    auto f = [](double x) { return std::exp(x) - 100; };
    Wynik b = bisekcja(f, 0, 10, 1e-6), r = regula_falsi(f, 0, 10, 1e-6);
    BLISKO(b.x, std::log(100.0), 1e-8);
    BLISKO(r.x, std::log(100.0), 1e-8);
    PRAWDA(b.zbiezna && r.zbiezna);
}

TEST("regula falsi: x^2 - 2 na [0, 2]") {
    Wynik w = regula_falsi([](double x) { return x * x - 2; }, 0, 2, 1e-12);
    BLISKO(w.x, std::sqrt(2.0), 1e-12);
    BLISKO(w.historia[0], 1.0, 1e-15);              // pierwsza cięciwa: 0 - (-2)(2)/(2+2) = 1
}

TEST("limit iteracji: zbiezna = false") {
    Wynik w = bisekcja([](double x) { return x - 0.3; }, 0, 1, 1e-15, 5);
    PRAWDA(!w.zbiezna);
    PRAWDA(w.iteracje == 5);
}

TEST("Newton dla układu: x^2 + y^2 = 4, xy = 1") {
    Uklad2 u{[](double x, double y) { return x * x + y * y - 4; },
             [](double x, double y) { return x * y - 1; },
             [](double x, double) { return 2 * x; }, [](double, double y) { return 2 * y; },
             [](double, double y) { return y; },     [](double x, double) { return x; }};
    double y;
    Wynik w = newton_uklad2(u, 2.0, 0.5, 1e-12, 50, y);
    PRAWDA(w.zbiezna);
    BLISKO(w.x * w.x + y * y, 4.0, 1e-10);
    BLISKO(w.x * y, 1.0, 1e-10);
    BLISKO(w.x, std::sqrt(2 + std::sqrt(3.0)), 1e-10);
}

URUCHOM("m01 równania nieliniowe")
