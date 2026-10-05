// Wspólne typy kursu metod numerycznych (C++17).
#pragma once
#include <cmath>
#include <functional>
#include <stdexcept>
#include <string>
#include <vector>

using Vec = std::vector<double>;                          // wektor
using Mat = std::vector<Vec>;                             // macierz: A[wiersz][kolumna]
using F1  = std::function<double(double)>;                // f(x)
using F2  = std::function<double(double, double)>;        // f(x, y)
using FN  = std::function<double(const Vec&)>;            // f(x1, ..., xn)

// Rzucana przez funkcje, których jeszcze nie napisałeś (pliki w zadania/).
struct BrakImplementacji : std::logic_error {
    using std::logic_error::logic_error;
};
#define TODO throw BrakImplementacji(__func__)

// Wynik metody iteracyjnej.
struct Wynik {
    double x = NAN;            // znalezione przybliżenie
    int iteracje = 0;          // ile iteracji wykonano
    bool zbiezna = false;      // czy spełniono kryterium stopu przed limitem
    Vec historia;              // kolejne przybliżenia x1, x2, ... (do tabel i rzędu zbieżności)
};
