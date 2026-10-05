// m02 — układy równań liniowych Ax = b
// Notatka: [[Układy równań liniowych]]. Używane przez: m04 (aproksymacja), m07 (schemat θ), m08 (MES).
#pragma once
#include "mn.h"

// Eliminacja Gaussa z częściowym wyborem elementu głównego (zamiana wierszy na max |a_ik|).
// A i b dostajesz przez wartość, więc możesz je zmieniać. Osobliwa macierz -> std::runtime_error.
Vec gauss(Mat A, Vec b);

// Rozkład LU Doolittle'a (bez wyboru elementu): A = L U, L ma jedynki na przekątnej.
// Zwraca jedną macierz: pod przekątną L, na i nad przekątną U.
Mat rozklad_lu(Mat A);

// Rozwiązanie z gotowego rozkładu: L y = b (w przód), U x = y (wstecz).
Vec rozwiaz_lu(const Mat& LU, const Vec& b);

// Algorytm Thomasa dla układu trójdiagonalnego:
// a[i] x[i-1] + d[i] x[i] + c[i] x[i+1] = b[i]  (a[0] i c[n-1] są pomijane). O(n).
Vec thomas(const Vec& a, const Vec& d, const Vec& c, const Vec& b);

// Gauss–Seidel: x_i <- (b_i - sum_{j != i} a_ij x_j) / a_ii, od razu nowymi wartościami.
// Stop: max |x_nowe - x_stare| < eps. Ustaw iteracje na liczbę wykonanych przebiegów.
Vec gauss_seidel(const Mat& A, const Vec& b, double eps, int maxit, int& iteracje);

// Iloczyn macierz-wektor i norma maksimum residuum ||A x - b||_inf.
Vec mnoz(const Mat& A, const Vec& x);
double residuum(const Mat& A, const Vec& x, const Vec& b);
