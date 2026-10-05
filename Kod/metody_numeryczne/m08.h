// m08 — MES 1D: pręt z doprowadzonym strumieniem i konwekcją (konwencja „Wstępu” Kustry/Milenina)
// Notatki: [[Metoda Galerkina]], [[MES 1D — od różnic skończonych do macierzy sztywności]],
//          [[Macierz sztywności]], [[Nieustalony przepływ ciepła]]. Zależy od: m02 (gauss).
#pragma once
#include "mn.h"

struct Pret {
    double L, S, k;        // długość [m], przekrój [m^2], przewodność [W/(m K)]
    int ne;                // liczba elementów (równe długości)
    double q;              // strumień na x = 0 [W/m^2]; q > 0 oznacza ciepło ODPROWADZANE (jak we „Wstępie”)
    double alfa, t_inf;    // konwekcja na x = L
    double rho = 0, c = 0; // tylko dla zadania nieustalonego
};

// Globalna macierz [H] (z członem alfa S w ostatnim węźle) i wektor {P} = [qS, 0, ..., -alfa S t_inf]
// w zapisie [H]{t} + {P} = 0. Element: (S k / Le) [[1, -1], [-1, 1]], agregacja przez +=.
void pret_HP(const Pret& p, Mat& H, Vec& P);

// Stan ustalony: rozwiąż [H]{t} = -{P}.
Vec pret_ustalony(const Pret& p);

// Macierz pojemności [C]: element (rho c S Le / 6) [[2, 1], [1, 2]], agregacja jak [H].
Mat pret_C(const Pret& p);

// Stan nieustalony, schemat niejawny: ([H] + [C]/dt) t1 = ([C]/dt) t0 - {P}; start t0 w każdym węźle.
Vec pret_nieustalony(const Pret& p, double t0, double dt, int kroki);
