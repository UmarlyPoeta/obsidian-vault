// Minimalna biblioteka testów: bez zależności, jeden plik.
#pragma once
#include <cmath>
#include <cstdio>
#include <exception>
#include <functional>
#include <string>
#include <vector>
#include "mn.h"

namespace testy {
struct Licznik { int ok = 0, zle = 0, brak = 0; };
inline Licznik& licznik() { static Licznik l; return l; }

struct Przypadek { std::string nazwa; std::function<void()> cialo; };
inline std::vector<Przypadek>& lista() { static std::vector<Przypadek> l; return l; }
struct Rejestr { Rejestr(const char* n, std::function<void()> f) { lista().push_back({n, f}); } };

struct Porazka { std::string opis; };

inline void blisko(double a, double b, double tol, const char* ea, const char* eb, int linia) {
    if (!(std::fabs(a - b) <= tol)) {
        char buf[512];
        std::snprintf(buf, sizeof buf, "linia %d: %s = %.12g, oczekiwano %s = %.12g (tol %.1e)",
                      linia, ea, a, eb, b, tol);
        throw Porazka{buf};
    }
}
inline void prawda(bool w, const char* e, int linia) {
    if (!w) throw Porazka{"linia " + std::to_string(linia) + ": nie spełnione: " + e};
}

inline int uruchom(const char* modul) {
    std::printf("== %s ==\n", modul);
    for (auto& p : lista()) {
        try {
            p.cialo();
            ++licznik().ok;
            std::printf("  [ OK ]  %s\n", p.nazwa.c_str());
        } catch (const BrakImplementacji& e) {
            ++licznik().brak;
            std::printf("  [TODO]  %s  (brak: %s)\n", p.nazwa.c_str(), e.what());
        } catch (const Porazka& e) {
            ++licznik().zle;
            std::printf("  [BŁĄD]  %s\n          %s\n", p.nazwa.c_str(), e.opis.c_str());
        } catch (const std::exception& e) {
            ++licznik().zle;
            std::printf("  [BŁĄD]  %s\n          wyjątek: %s\n", p.nazwa.c_str(), e.what());
        }
    }
    auto& l = licznik();
    std::printf("-- %d OK, %d błędnych, %d do zrobienia\n", l.ok, l.zle, l.brak);
    return l.zle + l.brak == 0 ? 0 : 1;
}
}  // namespace testy

#define SKLEJ2(a, b) a##b
#define SKLEJ(a, b) SKLEJ2(a, b)
#define TEST(nazwa) \
    static void SKLEJ(test_, __LINE__)(); \
    static testy::Rejestr SKLEJ(rej_, __LINE__)(nazwa, SKLEJ(test_, __LINE__)); \
    static void SKLEJ(test_, __LINE__)()
#define BLISKO(a, b, tol) testy::blisko((a), (b), (tol), #a, #b, __LINE__)
#define PRAWDA(w) testy::prawda((w), #w, __LINE__)
#define URUCHOM(modul) int main() { return testy::uruchom(modul); }
