#include "../test.h"
#include "../m00.h"

TEST("eps maszynowy = 2^-52") { BLISKO(eps_maszynowy(), std::ldexp(1.0, -52), 0); }

TEST("Kahan: 1 + 10^6 razy 1e-16") {
    Vec x(1000001, 1e-16); x[0] = 1.0;
    BLISKO(suma_kahana(x), 1.0 + 1e-10, 1e-15);
    PRAWDA(std::fabs(suma_naiwna(x) - 1.0) < 1e-15);       // naiwna gubi wszystko
}

TEST("Kahan: 0.1 dziesięć milionów razy") {
    Vec x(10000000, 0.1);
    PRAWDA(blad_wzgledny(suma_kahana(x), 1e6) < 1e-15);
    PRAWDA(blad_wzgledny(suma_naiwna(x), 1e6) > 1e-11);
}

TEST("pierwiastki bez utraty cyfr: x^2 - 1e8 x + 1") {
    auto [x1, x2] = pierwiastki_kwadratowe(1, -1e8, 1);
    PRAWDA(blad_wzgledny(x1, 1e-8) < 1e-14);                // wzór szkolny daje tu błąd ~25%
    PRAWDA(blad_wzgledny(x2, 1e8) < 1e-14);
}

TEST("pierwiastki: x^2 - 3x + 2") {
    auto [x1, x2] = pierwiastki_kwadratowe(1, -3, 2);
    BLISKO(x1, 1, 1e-15); BLISKO(x2, 2, 1e-15);
}

TEST("Horner: 2 - 3x + x^3 w x = 2") { BLISKO(horner({2, -3, 0, 1}, 2.0), 4.0, 1e-15); }

URUCHOM("m00 arytmetyka")
