---
typ: pojęcie
status: nowe
przedmioty: ["[[AGH Programowanie równoległe]]"]
wymaga: ["[[PR W01 — Procesy i wątki]]", "[[Kompilacja i pomiar czasu wykonania]]"]
powiązane: ["[[Proces i wątek]]", "[[Wyścig danych]]", "[[PR L03 — pthread_create i atrybuty wątków]]"]
tags: [pojęcie, równoległość, lab]
utworzono: 2026-10-02
źródła: ["K. Banaś — RR_L02_fork_clone.pdf (instrukcja lab, 2026/27)", "K. Banaś — PR_W01_Procesy_watki.pdf (wykład 2026/27)", "pomiary własne 2026-10-02: Ryzen 5 5500, WSL2, gcc 13.3"]
---
# PR L02 — fork i clone

> [!skrot] W 30 sekund
> - **Co to:** lab 2 (pt **9.10**, B5 s. 404): zmierzyć czas tworzenia 1000 procesów (`fork`) i 1000 wątków (`clone`), potem dwa wątki `clone` zwiększające zmienną globalną i lokalną.
> - **Klucz:** na tej maszynie proces ≈ **210–245 µs**, wątek ≈ **60–76 µs** → wątek ok. **3×** tańszy; `-O3` nic nie zmienia (czas siedzi w jądrze).
> - **Kiedy:** przed zajęciami przeczytaj [[PR W01 — Procesy i wątki]] (możliwa kartkówka) i obejrzyj nagranie W01 od ok. 35 min.

> [!intuicja] Po ludzku
> Mierzysz, ile kosztuje „zatrudnienie” nowego procesu i nowego wątku, i przeliczasz to na „ile rachunków procesor zrobiłby w tym czasie”. Potem sprawdzasz, że dwóch pracowników dopisujących do jednej kartki gubi wpisy, a każdy we własnym notesie — nie.

## Przed zajęciami
1. Notatka [[PR W01 — Procesy i wątki]] + nagranie W01 od ~35 min (tworzenie wątków).
2. Zasady pomiaru czasu (`materialy/ogolne/pomiar_czasu.pdf`): co najmniej **3 pomiary**, odrzucić odstające, średnia — albo **minimum**.
3. Biblioteka `libpomiar_czasu.a` z L01 w katalogu `pomiar_czasu` **obok** `lab_2` (Makefile ma `-I../pomiar_czasu -L../pomiar_czasu`). Twoje repo `lab_1` ma `pomiar_czasu.c/.h`, ale bez `.a` — w razie czego `libpomiar_czasu.tgz` i `make`.

## Kroki laba w skrócie
1. `lab_2/`, rozpakuj `fork_clone.tgz`, `make fork clone`.
2. **Krok 4 (ocena):** wstaw `inicjuj_czas()` przed pętlą i `drukuj_czas()` po pętli w `fork.c` i `clone.c`; zmierz 1000 procesów i 1000 wątków dla `OPT = -g -DDEBUG -O0` i `-O3` (zmiana Makefile → `make clean; make`). W `clone.c` wypisz raz `zmienna_globalna` po pętli, żeby kompilator nie wyrzucił pracy. Opcjonalnie osobno czas `exec`.
3. **Krok 5 (ocena):** nowy program na bazie `clone.c`, **bez pętli tworzenia** — dwa wątki uruchomione **jeden po drugim**, czekanie na oba dopiero potem. W pętli ≥ 100 000 iteracji zwiększają zmienną globalną i lokalną (przekazaną jako argument); wypisanie w wątkach i w `main`.
4. **Na 4.0:** wątek (`clone.c`) i proces (`fork.c`) uruchamiają przez `exec` własny program wypisujący np. imię i nazwisko; wersja zaawansowana z `execv` i argumentami (ostatni element `argv` = `NULL`).
5. **Na 5.0:** `clone` działający jak `fork`; wpływ rozmiaru stosu (duża tablica lokalna w wątku, `ulimit -s`); brak `CLONE_FILES` a otwieranie plików.

## Co jest oceniane
> [!wazne] „(ocena)” w instrukcji
> - **3.0:** krok 4 (pomiary fork/clone, `-O0` i `-O3`) + krok 5 (dwa wątki, zmienne) — warunek zaliczenia: kroki 1–6.
> - **4.0:** `exec` z procesu i wątku (własny program).
> - **5.0:** `clone` jak `fork`, eksperyment ze stosem, pliki bez współdzielenia.
> - **Sprawozdanie** (tydzień): cel, kroki, **własny kod przy krokach**, **zrzuty z pomiarami z identyfikacją osoby** (np. login w prompt), wnioski z porównaniem z L01 (czas operacji arytmetycznej i wejścia/wyjścia) i o zachowaniu zmiennych; odpowiedzi na pytania „->”.

## Pomiary na tej maszynie
Ryzen 5 5500 (6 rdzeni / 12 wątków), WSL2, gcc 13.3, 2026-10-02. **W labie (B5 s. 404, serwery) wyniki będą inne — liczy się Twój pomiar.**

| program (1000×) | `-g -DDEBUG -O0` | `-O3` | na 1 sztukę |
|---|---|---|---|
| `fork` + `exit` + `wait` | 0,204–0,245 s (mediana 0,245) | 0,203–0,239 s (mediana 0,239) | ≈ 210–245 µs |
| `clone` + `waitpid` | 0,060–0,076 s (mediana 0,076) | 0,060–0,075 s (mediana 0,075) | ≈ 60–76 µs |

- Stosunek proces/wątek ≈ **3–3,5×**. Pierwsze uruchomienia wolniejsze (rozgrzewka), rozrzut do ~2× przy obciążonym systemie → bierz minimum lub medianę.
- „czas CPU” z biblioteki wychodzi **0,000** — liczy tylko `ru_utime` procesu (czas użytkownika), a praca dzieje się w jądrze (sys) i w potomkach. Do narzutu bierz **czas zegarowy** (tak każe instrukcja).
- Raz wyszło **−0,399 s** czasu zegarowego: `gettimeofday` nie jest monotoniczny (WSL2 korygował zegar). Taki wynik odrzuć.
- Dla porównania: jedna iteracja `a = 1.000001*a + 0.000001` (2 działania, zależne od poprzedniej) ≈ **1,5 ns** z `-O3`, **3,7 ns** z `-O0`; `printf("%d ")` ≈ **0,2 µs** do pseudoterminala, 0,05 µs do pliku (prawdziwe okno terminala jest wolniejsze — weź swoją liczbę z L01).
- Krok 5 (dwa wątki `clone`, `CLONE_VM`): 2 × 100 000 iteracji → zmienna globalna **145–181 tys.** zamiast 200 tys.; 2 × 10⁸ → **120–138 mln** zamiast 200 mln; zmienna lokalna funkcji wątku zawsze **dokładnie 100 000**.

## Odpowiedzi-szkice na pytania „->”
> [!zadanie] Relatywny narzut wątek vs proces?
> > [!odpowiedz]- Szkic
> > Tu: wątek ~65 µs, proces ~220 µs → wątek ok. 3× tańszy. Przyczyna (W01, skrypt 2.4): wątek ma mniej struktur danych (dzieli przestrzeń adresową, pliki), proces wymaga kopii bloku kontrolnego i przestrzeni adresowej (nawet leniwej).

> [!zadanie] Ile operacji arytmetycznych i wejścia/wyjścia w czasie tworzenia wątku?
> > [!odpowiedz]- Szkic
> > 65 µs / 1,5 ns ≈ 4·10⁴ iteracji (≈ 9·10⁴ działań) przy `-O3`, ≈ 1,8·10⁴ przy `-O0`; proces ≈ 1,4·10⁵ iteracji. Wejście/wyjście: 65 µs / 0,2 µs ≈ 300 wydruków do bufora terminala; przy wolnym terminalu rzędu dziesiątek. Wniosek: wątek opłaca się tylko, gdy ma do zrobienia co najmniej dziesiątki tysięcy operacji.

> [!zadanie] Czy optymalizacja wpływa na czas tworzenia? Dlaczego?
> > [!odpowiedz]- Szkic
> > Nie (mediany różnią się o kilka %, w granicach rozrzutu). Prawie cały czas to wywołania systemowe (`fork`/`clone`, `wait`) wykonywane w jądrze — kodu jądra nie kompilujemy naszymi flagami; nasz kod to pusta pętla i jedno `++`.

> [!zadanie] Czy wartości zmiennych odpowiadają liczbie operacji? Globalne vs lokalne?
> > [!odpowiedz]- Szkic
> > Globalna (wspólna dzięki `CLONE_VM`): mniej niż 2·N — `++` to odczyt–dodanie–zapis, przeplot gubi aktualizacje ([[Wyścig danych]]). Lokalna w funkcji wątku: dokładnie N — każdy wątek ma swoją na własnym stosie. Lokalna `main` przekazana wskaźnikiem: zachowuje się jak wspólna. Dla porównania po `fork` potomek zmienia swoją kopię, rodzic widzi 0.

> [!zadanie] Rozmiar stosu nowego wątku (5.0)?
> > [!odpowiedz]- Szkic
> > W `clone` — sam alokujesz (`ROZMIAR_STOSU` = 64 KiB) i podajesz koniec obszaru. Test: tablica lokalna w funkcji wątku — u mnie 15 000 `int` (60 KB) działa, 17 000 (68 KB) już nie: wątek ginie od SIGSEGV, a `main` **działa dalej bez komunikatu** (sprawdź `WIFSIGNALED` w statusie `waitpid`). Domyślnie tu: `ulimit -s` = 8192 KiB, wątki Pthreads też dostają 8 MiB.

## Plan i szkielet kroku 5 (najpierw sam!)
1. Dwa stosy (`malloc` × 2) — **każdy wątek musi mieć własny**.
2. `int lokalna = 0;` w `main`, `clone(funkcja, stos1 + ROZMIAR, flagi, &lokalna)`, zaraz drugie `clone(...)`.
3. Dopiero potem dwa `waitpid(pid, NULL, __WCLONE)`.
4. W funkcji: rzutowanie argumentu, zmienna `moja_lokalna`, pętla `ITER` razy `++` na trzech zmiennych, jeden wydruk.
5. Policz oczekiwane wartości (N i 2N) i porównaj; uruchom kilka razy, potem z większym `ITER`.

> [!kod]- Ściąga: pełny kod kroku 5 (sprawdzony na tej maszynie)
> ```c
> #define _GNU_SOURCE
> #include <stdlib.h>
> #include <stdio.h>
> #include <sched.h>
> #include <sys/wait.h>
>
> #define ROZMIAR_STOSU (1024*64)
> #define ITER 100000
>
> int zmienna_globalna = 0;
>
> int funkcja_watku(void *argument) {
>   int *lokalna_main = (int *)argument;   // zmienna z main, przez wskaźnik
>   int moja_lokalna = 0;                  // na stosie wątku
>   for (int i = 0; i < ITER; i++) {
>     zmienna_globalna++; (*lokalna_main)++; moja_lokalna++;
>   }
>   printf("watek: globalna=%d, lokalna_main=%d, moja_lokalna=%d\n",
>          zmienna_globalna, *lokalna_main, moja_lokalna);
>   return 0;
> }
>
> int main(void) {
>   int lokalna = 0;
>   printf("main: start\n");   // ważne — patrz pułapki
>   void *stos1 = malloc(ROZMIAR_STOSU), *stos2 = malloc(ROZMIAR_STOSU);
>   int flagi = CLONE_FS | CLONE_FILES | CLONE_SIGHAND | CLONE_VM;
>   pid_t p1 = clone(funkcja_watku, (char *)stos1 + ROZMIAR_STOSU, flagi, &lokalna);
>   pid_t p2 = clone(funkcja_watku, (char *)stos2 + ROZMIAR_STOSU, flagi, &lokalna);
>   waitpid(p1, NULL, __WCLONE);
>   waitpid(p2, NULL, __WCLONE);
>   printf("main: globalna=%d, lokalna=%d (oczekiwane %d)\n",
>          zmienna_globalna, lokalna, 2 * ITER);
>   free(stos1); free(stos2);
>   return 0;
> }
> ```
> `clone` jak `fork` (5.0): flagi tylko `SIGCHLD` (bez `CLONE_VM` itd.) i zwykłe `waitpid(pid, NULL, 0)`.

## Pułapki
> [!blad] Jeden stos dla dwóch równoległych wątków
> W `clone.c` jeden stos wystarcza, bo wątki idą po kolei. W kroku 5 oba działają naraz — wspólny stos = losowe awarie.

> [!blad] Pierwszy `printf` w dwóch wątkach `clone` naraz
> Glibc nie wie o wątkach z gołego `clone` (nie zakłada blokad), a pierwszy `printf` alokuje bufor `stdout`. U mnie bez `printf` w `main` przed `clone` program padał w **8 na 30** uruchomień (`double free or corruption`, segfault). Jeden wydruk w `main` przed `clone` — 0 na 50.

> [!blad] Za krótka pętla / `-O3`
> Przy 100 000 iteracjach i `-O3` pierwszy wątek często kończy, zanim drugi wystartuje → wynik dokładnie 200 000 i błędny wniosek „nie ma wyścigu”. Zwiększ `ITER` (10⁷–10⁸) lub kompiluj `-O0`.

> [!blad] `waitpid` bez `__WCLONE`
> Wątek z `clone` bez `SIGCHLD` nie jest „zwykłym dzieckiem” — bez `__WCLONE` `waitpid` od razu zwraca błąd i `main` nie czeka.

## Na zajęcia
> [!zajecia]
> - **Pokaż prowadzącemu:** tabelę czasów (fork/clone × O0/O3, kilka powtórzeń), przeliczenie na 1 proces/wątek i na liczbę operacji, wydruk kroku 5 z wartościami zmiennych i wyjaśnieniem.
> - **Kartkówka (W01):** definicje współbieżne/równoległe, Flynn, proces vs wątek, `fork`/`exec`/`wait`, `clone`.
> - Łączy się z: [[PR W01 — Procesy i wątki]], [[Proces i wątek]], [[Wyścig danych]].

## Fiszki #fiszki/równoległość
Czas utworzenia procesu (fork) na moim Ryzenie w WSL2::Ok. 210–245 µs (1000 forków ≈ 0,2–0,25 s)
Czas utworzenia wątku (clone) na moim Ryzenie w WSL2::Ok. 60–76 µs (1000 clone ≈ 0,06–0,075 s)
Ile razy wątek tańszy od procesu (mój pomiar)?::Ok. 3–3,5 razy
Czy -O3 przyspiesza fork/clone?::Nie — czas to wywołania systemowe w jądrze
Dlaczego czas CPU z libpomiar_czasu wychodzi 0 przy fork?::Liczy tylko czas użytkownika procesu, a praca jest w jądrze i potomkach
Zmienna globalna zwiększana przez 2 wątki clone(CLONE_VM)::Mniej niż 2N — wyścig, gubione aktualizacje
Zmienna lokalna funkcji wątku po N iteracjach::Dokładnie N — każdy wątek ma ją na własnym stosie
Ile stosów dla dwóch równoległych wątków clone?::Dwa — każdy wątek własny
Flaga waitpid dla wątków z clone bez SIGCHLD::__WCLONE
Jak zrobić clone działające jak fork?::Flagi tylko SIGCHLD, bez CLONE_VM/FILES/FS/SIGHAND
Domyślny rozmiar stosu (ulimit -s) na mojej maszynie::8192 KiB (wątki Pthreads też 8 MiB)
Ostatni element tablicy argv dla execv::NULL

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.

## Źródła
- Instrukcja: `~/studia/sem5/programowanie_rownolegle/materialy/laby/RR_L02_fork_clone.pdf` (tekst: `materialy/tekst/RR_L02_fork_clone.md`); kod `laby/fork_clone.tgz`, `laby/libpomiar_czasu.tgz`.
- Zasady pomiaru czasu: `~/studia/sem5/programowanie_rownolegle/materialy/ogolne/pomiar_czasu.pdf`; regulamin i wzór sprawozdania: `materialy/ogolne/regulamin_lab.pdf`, `materialy/ogolne/wzorcowe_sprawozdanie.pdf`.
- Wykład W01 i nagranie: [[PR W01 — Procesy i wątki]] (W01.mp4: https://cloud.wimiip.agh.edu.pl/s/9KyQtbFD5KqAfjY).
- Pomiary: kopie `fork.c`/`clone.c` z dopisanym pomiarem czasu, uruchomione 15–20 razy; kod pomocniczy w scratchpadzie sesji (`/tmp/claude-1000/…/scratchpad/pr_lab/m/`), nie w Twoim repo.
