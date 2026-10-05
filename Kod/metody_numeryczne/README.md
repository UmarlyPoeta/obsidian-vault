# Kurs metod numerycznych w C++

Opis, plan i kolejność modułów: notatka `Pojęcia/Kurs — Metody numeryczne.md` w vaulcie.

```bash
./sprawdz.sh 01                    # testy twojej wersji modułu m01 (zadania/m01.cpp)
./sprawdz.sh 04 --zaleznosci-wzor  # twój m04, zależności (gauss z m02) z rozwiązań
./sprawdz.sh 06 --wzor             # testy rozwiązań wzorcowych
./sprawdz.sh wszystko              # wszystkie moduły (twoje wersje); dodaj --wzor dla wzorców
```

| Plik | Rola |
|---|---|
| `mn.h` | wspólne typy: `Vec`, `Mat`, `F1`, `F2`, `FN`, `Wynik`, makro `TODO` |
| `test.h` | mini biblioteka testów (`TEST`, `BLISKO`, `PRAWDA`) |
| `mNN.h` | specyfikacja modułu — nie zmieniasz |
| `zadania/mNN.cpp` | tu piszesz |
| `testy/test_mNN.cpp` | testy (w tym przykłady z instrukcji labów i liczby z plików dr. Kustry) |
| `rozwiazania/mNN.cpp` | wzorzec — po własnej próbie |

Moduły: m00 arytmetyka, m01 równania nieliniowe, m02 układy liniowe, m03 interpolacja i funkcje kształtu,
m04 aproksymacja, m05 różniczkowanie, m06 całkowanie, m07 RRZ i schemat θ, m08 MES 1D,
m09 element 4-węzłowy MES, m10 optymalizacja.
