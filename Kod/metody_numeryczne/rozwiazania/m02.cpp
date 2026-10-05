#include "../m02.h"
#include <utility>

Mat macierz_rozszerzona(const Mat& A, const Vec& b) {
    Mat Ab = A;
    for (size_t i = 0; i < A.size(); ++i) Ab[i].push_back(b[i]);
    return Ab;
}

void postepowanie_proste(Mat& Ab) {
    int n = (int)Ab.size();
    for (int k = 0; k < n - 1; ++k) {
        if (std::fabs(Ab[k][k]) < 1e-12) throw std::runtime_error("zero na przekątnej w wierszu " + std::to_string(k));
        for (int i = k + 1; i < n; ++i) {
            double m = Ab[i][k] / Ab[k][k];
            for (int j = k; j <= n; ++j) Ab[i][j] -= m * Ab[k][j];
        }
    }
    if (std::fabs(Ab[n - 1][n - 1]) < 1e-12) throw std::runtime_error("zero na przekątnej w ostatnim wierszu");
}

Vec postepowanie_odwrotne(const Mat& Ab) {
    int n = (int)Ab.size();
    Vec x(n);
    for (int i = n - 1; i >= 0; --i) {
        double s = Ab[i][n];
        for (int k = i + 1; k < n; ++k) s -= Ab[i][k] * x[k];
        x[i] = s / Ab[i][i];
    }
    return x;
}

Vec gauss_bez_wyboru(const Mat& A, const Vec& b) {
    Mat Ab = macierz_rozszerzona(A, b);
    postepowanie_proste(Ab);
    return postepowanie_odwrotne(Ab);
}

Vec gauss_crout(Mat A, Vec b, std::vector<int>& kol) {
    int n = (int)b.size();
    kol.resize(n);
    for (int j = 0; j < n; ++j) kol[j] = j;
    for (int k = 0; k < n; ++k) {
        int p = k;                                       // największy |a_kj| w wierszu k
        for (int j = k + 1; j < n; ++j)
            if (std::fabs(A[k][kol[j]]) > std::fabs(A[k][kol[p]])) p = j;
        std::swap(kol[k], kol[p]);                       // zamiana kolumn tylko w wektorze
        double piv = A[k][kol[k]];
        if (std::fabs(piv) < 1e-300) throw std::runtime_error("gauss_crout: wiersz zerowy");
        for (int i = k + 1; i < n; ++i) {
            double m = A[i][kol[k]] / piv;
            for (int j = k; j < n; ++j) A[i][kol[j]] -= m * A[k][kol[j]];
            b[i] -= m * b[k];
        }
    }
    Vec x(n);
    for (int i = n - 1; i >= 0; --i) {                   // x[kol[i]] to niewiadoma z kolumny i
        double s = b[i];
        for (int j = i + 1; j < n; ++j) s -= A[i][kol[j]] * x[kol[j]];
        x[kol[i]] = s / A[i][kol[i]];
    }
    return x;
}

Vec gauss(Mat A, Vec b) {
    int n = (int)b.size();
    for (int k = 0; k < n; ++k) {
        int p = k;                                       // wybór elementu głównego
        for (int i = k + 1; i < n; ++i)
            if (std::fabs(A[i][k]) > std::fabs(A[p][k])) p = i;
        if (std::fabs(A[p][k]) < 1e-300) throw std::runtime_error("gauss: macierz osobliwa");
        std::swap(A[k], A[p]);
        std::swap(b[k], b[p]);
        for (int i = k + 1; i < n; ++i) {               // eliminacja w dół
            double m = A[i][k] / A[k][k];
            for (int j = k; j < n; ++j) A[i][j] -= m * A[k][j];
            b[i] -= m * b[k];
        }
    }
    Vec x(n);
    for (int i = n - 1; i >= 0; --i) {                   // podstawianie wstecz
        double s = b[i];
        for (int j = i + 1; j < n; ++j) s -= A[i][j] * x[j];
        x[i] = s / A[i][i];
    }
    return x;
}

Mat rozklad_lu(Mat A) {
    int n = (int)A.size();
    for (int k = 0; k < n; ++k) {
        if (A[k][k] == 0) throw std::runtime_error("rozklad_lu: zero na przekątnej");
        for (int i = k + 1; i < n; ++i) {
            A[i][k] /= A[k][k];                          // l_ik
            for (int j = k + 1; j < n; ++j) A[i][j] -= A[i][k] * A[k][j];
        }
    }
    return A;
}

Vec lu_przod(const Mat& LU, const Vec& b) {
    int n = (int)b.size();
    Vec y(n);
    for (int i = 0; i < n; ++i) {
        y[i] = b[i];
        for (int j = 0; j < i; ++j) y[i] -= LU[i][j] * y[j];
    }
    return y;
}

Vec lu_wstecz(const Mat& LU, const Vec& y) {
    int n = (int)y.size();
    Vec x(n);
    for (int i = n - 1; i >= 0; --i) {
        x[i] = y[i];
        for (int j = i + 1; j < n; ++j) x[i] -= LU[i][j] * x[j];
        x[i] /= LU[i][i];
    }
    return x;
}

Vec rozwiaz_lu(const Mat& LU, const Vec& b) { return lu_wstecz(LU, lu_przod(LU, b)); }

void rozdziel_lu(const Mat& LU, Mat& L, Mat& U) {
    size_t n = LU.size();
    L.assign(n, Vec(n, 0.0));
    U.assign(n, Vec(n, 0.0));
    for (size_t i = 0; i < n; ++i)
        for (size_t j = 0; j < n; ++j) {
            if (j < i) L[i][j] = LU[i][j];
            else U[i][j] = LU[i][j];
            if (i == j) L[i][j] = 1;
        }
}

bool diagonalnie_dominujaca(const Mat& A) {
    bool ostro = false;
    for (size_t i = 0; i < A.size(); ++i) {
        double s = 0;
        for (size_t j = 0; j < A.size(); ++j)
            if (j != i) s += std::fabs(A[i][j]);
        if (std::fabs(A[i][i]) < s) return false;
        if (std::fabs(A[i][i]) > s) ostro = true;
    }
    return ostro;
}

static Vec krok_jacobiego(const Mat& A, const Vec& b, const Vec& x) {
    size_t n = b.size();
    Vec nowy(n);
    for (size_t i = 0; i < n; ++i) {
        double s = b[i];
        for (size_t j = 0; j < n; ++j)
            if (j != i) s -= A[i][j] * x[j];          // (L + U) x ze STAREGO wektora
        nowy[i] = s / A[i][i];                         // D^-1
    }
    return nowy;
}

Vec jacobi(const Mat& A, const Vec& b, double eps, int maxit, int& iteracje) {
    Vec x(b.size(), 0.0);
    for (iteracje = 1; iteracje <= maxit; ++iteracje) {
        Vec nowy = krok_jacobiego(A, b, x);
        double zmiana = 0;
        for (size_t i = 0; i < x.size(); ++i) zmiana = std::max(zmiana, std::fabs(nowy[i] - x[i]));
        x = nowy;
        if (zmiana < eps) return x;
    }
    iteracje = maxit;
    return x;
}

Vec jacobi_n(const Mat& A, const Vec& b, int n) {
    Vec x(b.size(), 0.0);
    for (int i = 0; i < n; ++i) x = krok_jacobiego(A, b, x);
    return x;
}

Vec thomas(const Vec& a, const Vec& d, const Vec& c, const Vec& b) {
    int n = (int)d.size();
    Vec cp(n), bp(n), x(n);
    cp[0] = c[0] / d[0];
    bp[0] = b[0] / d[0];
    for (int i = 1; i < n; ++i) {
        double m = d[i] - a[i] * cp[i - 1];
        cp[i] = i < n - 1 ? c[i] / m : 0;
        bp[i] = (b[i] - a[i] * bp[i - 1]) / m;
    }
    x[n - 1] = bp[n - 1];
    for (int i = n - 2; i >= 0; --i) x[i] = bp[i] - cp[i] * x[i + 1];
    return x;
}

Vec gauss_seidel(const Mat& A, const Vec& b, double eps, int maxit, int& iteracje) {
    int n = (int)b.size();
    Vec x(n, 0.0);
    for (iteracje = 1; iteracje <= maxit; ++iteracje) {
        double zmiana = 0;
        for (int i = 0; i < n; ++i) {
            double s = b[i];
            for (int j = 0; j < n; ++j)
                if (j != i) s -= A[i][j] * x[j];
            double nowe = s / A[i][i];
            zmiana = std::max(zmiana, std::fabs(nowe - x[i]));
            x[i] = nowe;
        }
        if (zmiana < eps) return x;
    }
    iteracje = maxit;
    return x;
}

Vec mnoz(const Mat& A, const Vec& x) {
    Vec y(A.size(), 0.0);
    for (size_t i = 0; i < A.size(); ++i)
        for (size_t j = 0; j < x.size(); ++j) y[i] += A[i][j] * x[j];
    return y;
}

double residuum(const Mat& A, const Vec& x, const Vec& b) {
    Vec r = mnoz(A, x);
    double m = 0;
    for (size_t i = 0; i < b.size(); ++i) m = std::max(m, std::fabs(r[i] - b[i]));
    return m;
}
