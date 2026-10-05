// m02 — układy równań liniowych Ax = b
// Notatka: [[Układy równań liniowych]]. Używane przez: m04 (aproksymacja), m07 (schemat θ), m08 (MES).
#pragma once
#include "mn.h"

// --- Lab „Eliminacja Gaussa” (bez wyboru elementu) ---------------------------------------
// Macierz rozszerzona [A | b] (n wierszy, n + 1 kolumn).
Mat macierz_rozszerzona(const Mat& A, const Vec& b);

// Postępowanie proste BEZ wyboru elementu: m_ik = a_ik / a_kk, wiersz_i -= m_ik * wiersz_k.
// Gdy |a_kk| < 1e-12, rzuć std::runtime_error („zero na przekątnej”).
void postepowanie_proste(Mat& Ab);

// Postępowanie odwrotne na macierzy trójkątnej górnej [U | b]:
// x_n = b_n / a_nn, x_i = (b_i - sum_{k>i} a_ik x_k) / a_ii.
Vec postepowanie_odwrotne(const Mat& Ab);

// Gauss bez wyboru: macierz_rozszerzona + postepowanie_proste + postepowanie_odwrotne.
Vec gauss_bez_wyboru(const Mat& A, const Vec& b);

// --- Lab „Pivoting” -------------------------------------------------------------------------
// Eliminacja Gaussa z częściowym wyborem elementu głównego (zamiana wierszy na max |a_ik|).
// A i b dostajesz przez wartość, więc możesz je zmieniać. Osobliwa macierz -> std::runtime_error.
Vec gauss(Mat A, Vec b);

// Gauss–Crout: wybór elementu głównego W WIERSZU k (największy |a_kj| dla j >= k) i zamiana KOLUMN.
// Kolumn nie przestawiasz w pamięci ani w wyniku: kolumny[j] mówi, która niewiadoma stoi w kolumnie j.
// Na wejściu kolumny może być puste; na wyjściu ma rozmiar n. Zwraca x w ORYGINALNEJ kolejności.
Vec gauss_crout(Mat A, Vec b, std::vector<int>& kolumny);

// --- Lab „LU Doolittle'a” -------------------------------------------------------------------
// Rozkład LU Doolittle'a (bez wyboru elementu): A = L U, L ma jedynki na przekątnej.
// Zwraca jedną macierz: pod przekątną L, na i nad przekątną U.
Mat rozklad_lu(Mat A);

// Rozwiązanie z gotowego rozkładu: L y = b (w przód), U x = y (wstecz).
Vec lu_przod(const Mat& LU, const Vec& b);      // y (L ma jedynki na przekątnej)
Vec lu_wstecz(const Mat& LU, const Vec& y);     // x
Vec rozwiaz_lu(const Mat& LU, const Vec& b);    // lu_wstecz(LU, lu_przod(LU, b))

// Rozdziela wynik rozklad_lu na osobne L (z jedynkami) i U — do wypisania na labie.
void rozdziel_lu(const Mat& LU, Mat& L, Mat& U);

// --- Lab „Metoda Jacobiego” -----------------------------------------------------------------
// |a_ii| >= sum_{j != i} |a_ij| dla każdego i ORAZ > dla co najmniej jednego i.
bool diagonalnie_dominujaca(const Mat& A);

// Jacobi: x^(i+1) = -D^-1 (L + U) x^(i) + D^-1 b, start x = 0, wszystkie składowe ze STAREGO x.
// Stop: max_j |x_j^(i+1) - x_j^(i)| < eps (dla każdej składowej) albo maxit; iteracje = wykonane.
Vec jacobi(const Mat& A, const Vec& b, double eps, int maxit, int& iteracje);

// Jacobi z zadaną liczbą iteracji (zadanie 1 z instrukcji).
Vec jacobi_n(const Mat& A, const Vec& b, int n);

// Algorytm Thomasa dla układu trójdiagonalnego:
// a[i] x[i-1] + d[i] x[i] + c[i] x[i+1] = b[i]  (a[0] i c[n-1] są pomijane). O(n).
Vec thomas(const Vec& a, const Vec& d, const Vec& c, const Vec& b);

// Gauss–Seidel: x_i <- (b_i - sum_{j != i} a_ij x_j) / a_ii, od razu nowymi wartościami.
// Stop: max |x_nowe - x_stare| < eps. Ustaw iteracje na liczbę wykonanych przebiegów.
Vec gauss_seidel(const Mat& A, const Vec& b, double eps, int maxit, int& iteracje);

// Iloczyn macierz-wektor i norma maksimum residuum ||A x - b||_inf.
Vec mnoz(const Mat& A, const Vec& x);
double residuum(const Mat& A, const Vec& x, const Vec& b);
