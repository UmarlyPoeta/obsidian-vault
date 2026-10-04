---
typ: materiały
przedmioty: ["[[AGH Metoda elementów skończonych]]"]
powiązane: ["[[MES — kurs dr. Kustry (mapa labów)]]"]
tags: [materiały, mes]
utworzono: 2026-10-05
---
# MES — źródła i podręczniki

> [!skrot] Co tu jest
> - **W repo (offline):** wszystkie instrukcje i dane testowe dr. Kustry, dwa pełne darmowe podręczniki (licencja CC BY), referencyjny solver i parser w C++.
> - **Linki:** polskie skrypty z innych uczelni, darmowe książki i kursy, repozytoria studentów AGH z tego samego kursu.
> - **Kolejność:** najpierw materiały prowadzącego (to z nich jest kolokwium), potem Milenin, a dopiero potem reszta, kiedy coś jest niejasne.

## 1. Materiały prowadzącego (pobrane do repo)
Folder `Załączniki/MES/`. Oryginały są na [home.agh.edu.pl/~pkustra/MES.html](https://home.agh.edu.pl/~pkustra/MES.html). Kopie pochodzą z publicznego repozytorium studenta z roku 2025/26 (patrz punkt 5), bo strona AGH była niedostępna z miejsca pobierania. Na zajęciach sprawdź, czy prowadzący nie wrzucił nowszych wersji.

| Plik | Lab | Co zawiera |
|---|---|---|
| [[00 Zasady zaliczenia 2025-26.pdf]] | 1 | zasady kursu, ocena, kolokwium ustne |
| [[01 MES zajęcia 1 — funkcje kształtu, interpolacja, siatka.pdf]] | 1 | analiza problemu, interpolacja, praca domowa (struktury) |
| [[01 Wstęp — ustalony przepływ ciepła (do druku).pdf]] | 1 | **do wydrukowania**: funkcjonał, $[H]$, $\{P\}$, pręt na 2 elementach |
| [[02 Tablica kwadratur Gaussa.png]] | 2 | punkty i wagi Gaussa |
| [[04 Całkowanie Gaussa — materiały do zajęć.pdf]] | 2 | prostokąty, Gauss 1D/2D, praca domowa |
| [[03 Jakobian.pdf]] | 3 | wyprowadzenie jakobianu 1D i 2D |
| [[03.1 Całkowanie z jakobianem przekształcenia 1D-2D.pdf]] | 3 | całkowanie na $[a,b]$, przykład 49,5 |
| [[05 Całkowanie macierzy H.pdf]] | 4–5 | $[H]$ krok po kroku w punktach Gaussa |
| [[06 Agregacja H.pdf]] | 6 | agregacja 1D i 2D |
| [[07 Macierz Hbc.pdf]] | 7 | warunek konwekcji, punkty na bokach |
| [[08 Wektor P.pdf]] | 8 | wektor obciążeń |
| [[09 Rozwiązanie niestacjonarne.pdf]] | 9 | schemat jawny i niejawny |
| [[09.1 Całkowanie macierzy C.pdf]] | 9 | macierz pojemności $[C]$ |
| [[10 Wzory (ściąga).pdf]] | — | wszystkie wzory na jednej stronie |
| [[11 Tworzenie siatek w ABAQUS.pdf]] | 10–11 | własne siatki do problemu rzeczywistego |

**Dane i wyniki:** siatki [[Test1_4_4.txt]], [[Test2_4_4_MixGrid.txt]], [[Test3_31_31_kwadrat.txt]] oraz folder `Kustra — wyniki testowe/` (jakobian, $dN/dx$, $H$, $H_{bc}$, $P$, $H$ globalna, [[TestCase — rozwiązanie niestacjonarne (min, max T).pdf|TestCase z min/max T]]).

**Mój kod do sprawdzania** (`Załączniki/MES/kod/`): `mes2d.py` (pełny solver, zgodny z wynikami prowadzącego), `siatka.cpp` (parser z labu 1), `wyniki referencyjne.txt`.

## 2. Podręczniki pobrane do repo (darmowe, legalne)
- **H.P. Langtangen, K.-A. Mardal, *Introduction to Numerical Methods for Variational Problems*, Springer 2019, open access CC BY 4.0.** [[Langtangen, Mardal — Introduction to Numerical Methods for Variational Problems (2019, CC BY).pdf]]
  Najlepsze darmowe wprowadzenie do MES „od zera” z kodem w Pythonie. Pod kurs: rozdz. 4 (funkcje bazowe, 4.5 całkowanie numeryczne, 4.6 elementy 2D), rozdz. 6 (MES dla równań, 6.3 warunki z pochodną = konwekcja), rozdz. 7 (Euler w przód i wstecz dla dyfuzji).
- **H.P. Langtangen, S. Linge, *Finite Difference Computing with PDEs*, Springer 2017, open access CC BY 4.0.** [[Langtangen, Linge — Finite Difference Computing with PDEs (2017, CC BY).pdf]]
  Rozdział 3 „Diffusion equations”: stabilność schematów, reguła θ (3.2.4), dyfuzja w 2D. Przydaje się do sekcji sprawozdania o kroku czasowym.

## 3. Polskie materiały (linki)
- A. Milenin, *Podstawy metody elementów skończonych*, Wydawnictwa AGH 2010. **Podstawowa książka kursu** (z niej są „Wstęp” i „Rozwiązanie niestacjonarne”). Biblioteka AGH; nie jest darmowa w sieci.
- A. Milenin, materiały MES (AGH): [MileninGL7-8_pl.pdf](http://ww1.metal.agh.edu.pl/~milenin/Dydaktyka/MES/MileninGL7-8_pl.pdf)
- P. Kustra, materiały dydaktyczne (inne przedmioty, m.in. ABAQUS i metody numeryczne): [home.agh.edu.pl/~pkustra/matdydakt.html](https://home.agh.edu.pl/~pkustra/matdydakt.html)
- M. Głowacki (AGH), *Model wymiany ciepła* — ten sam układ $[H]$, $[C]$, $\{P\}$ w innym ujęciu: [ModelCieplny_2.pdf](https://home.agh.edu.pl/~glowacki/docs/matwykl/ProjKompl/ModelCieplny_2.pdf)
- G. Krzesiński (PW MEiL), *MES — notatki do wykładów, cz. 1*: [MES1_notatki do wykładów_czesc_1.pdf](https://www.meil.pw.edu.pl/pl/content/download/21826/116038/file/MES1_notatki%20do%20wyk%C5%82ad%C3%B3w_czesc_1.pdf)
- S. Milewski (PK), *Metody komputerowe — skrypt*: [MK_skrypt_2020.pdf](https://www.cce.pk.edu.pl/~slawek/Met_Komp_2017/MK_skrypt_2020.pdf). Przyda się też na PK.
- ZUT Szczecin, *MES — wstęp* (wykład): [MES-Wetep.pdf](https://mleus.zut.edu.pl/fileadmin/MES_wyklady/MES-Wetep.pdf)

## 4. Darmowe książki i kursy po angielsku (linki)
- O.C. Zienkiewicz, R.L. Taylor, J.Z. Zhu, *The Finite Element Method: Its Basis and Fundamentals* — klasyka (płatna, w bibliotece).
- K.-J. Bathe, *Finite Element Procedures*, 2nd ed. — autor udostępnia PDF za darmo na swojej stronie w MIT: [FEP_2nd_Edition_4th_Printing.pdf](https://web.mit.edu/kjb/www/Books/FEP_2nd_Edition_4th_Printing.pdf)
- C. Felippa, *Introduction to Finite Element Methods* (University of Colorado), notatki rozdział po rozdziale: [IFEM](https://www.colorado.edu/engineering/CAS/courses.d/IFEM.d/)
- C. Clason, *Introduction to Finite Element Methods* (arXiv, notatki matematyczne): [arXiv:1709.08618](https://arxiv.org/abs/1709.08618)
- D. Ham, *Finite elements: analysis and implementation* (Imperial College, kurs z kodem w Pythonie): [finite-element.github.io](https://finite-element.github.io/)
- MIT OpenCourseWare 2.092, *Finite Element Analysis of Solids and Fluids I* (K.-J. Bathe, wykłady wideo i notatki): [ocw.mit.edu](https://ocw.mit.edu/courses/2-092-finite-element-analysis-of-solids-and-fluids-i-fall-2009/)

> [!wazne] Linki z punktów 3 i 4
> Nie dało się ich sprawdzić z miejsca, w którym robiłem notatki (sieć przepuszczała tylko GitHub). Zostały znalezione wyszukiwarką albo są to znane adresy. Jeśli któryś nie działa, poszukaj tytułu w Google.

## 5. Repozytoria studentów AGH z tego samego kursu
Do porównania wyników i podglądania struktury. **Nie kopiuj kodu**: kolokwium ustne jest z twojego programu.
- [AndrzejJanaszek/fem-lab-agh](https://github.com/AndrzejJanaszek/fem-lab-agh) — rok 2025/26, C++, eksport do ParaView (`.vtu`), problem rzeczywisty: radiator procesora. Stąd są kopie materiałów prowadzącego.
- [adamczykpiotr-AGH/AGH_WIMiIP_Metoda_Elementow_Skonczonych](https://github.com/adamczykpiotr-AGH/AGH_WIMiIP_Metoda_Elementow_Skonczonych) — C++, klasy `Node`, `Element`, `GlobalData`, `Grid`, `Jacobian`, `Surface`, `UniversalElement`, ze sprawozdaniem.
- [ptutak/MES](https://github.com/ptutak/MES) — sprawozdanie z problemem rzeczywistym (ściana, zmiany dobowe) i tablicami materiałowymi.
- [Explicite/Temp1D](https://github.com/Explicite/Temp1D) — wariant 1D osiowosymetryczny (starsza edycja).
- [vonProteus/NagrzewanieIChlodzenieCialaMES2D](https://github.com/vonProteus/NagrzewanieIChlodzenieCialaMES2D) — nagrzewanie i chłodzenie 2D (2011/12).

## 6. Narzędzia
- **ParaView** ([paraview.org](https://www.paraview.org)) — wizualizacja pól temperatury z plików `.vtu` do sprawozdania.
- **scikit-fem** ([github.com/kinnala/scikit-fem](https://github.com/kinnala/scikit-fem)) — lekka biblioteka MES w Pythonie. Pozwala niezależnie sprawdzić wyniki problemu rzeczywistego.
- **ABAQUS Student Edition** — generowanie siatek (instrukcja prowadzącego wyżej).
