---
typ: pojęcie
status: nowe
przedmioty: ["[[AGH Programowanie równoległe]]"]
wymaga: ["[[Proces i wątek]]"]
powiązane: ["[[Wyścig danych]]", "[[Pthreads]]", "[[OpenMP]]", "[[Prawo Amdahla]]"]
tags: [pojęcie, równoległość]
utworzono: 2026-10-01
źródła: ["I. Foster — Designing and Building Parallel Programs (online: mcs.anl.gov/~itf/dbpp), rozdz. 3 o modelowaniu i mierzeniu wydajności", "M. J. Quinn — Parallel Programming in C with MPI and OpenMP", "man 2 clock_gettime, man 3 clock, man 1 time", "Dokumentacja GCC (gcc.gnu.org/onlinedocs) — Optimize Options, Instrumentation Options", "Dokumentacja GDB — Debugging Programs with Multiple Threads"]
---
# Kompilacja i pomiar czasu wykonania

> [!skrot] W 30 sekund
> - **Co to:** jak zbudować program w C z właściwymi flagami i jak rzetelnie zmierzyć, ile trwa — bo cały przedmiot kręci się wokół „o ile szybciej po zrównolegleniu”.
> - **Klucz:** `gcc -O2 -g -Wall -Wextra prog.c -o prog -pthread`; czas mierz **ścienny** (`clock_gettime(CLOCK_MONOTONIC)` / `omp_get_wtime()`), nie `clock()`.
> - **Kiedy:** na każdym labie — przyspieszenie $S(p)=T_1/T_p$ liczysz z czasów, które sam zmierzyłeś.

> [!intuicja] Po ludzku
> Czas ścienny (*wall-clock*) to stoper w ręku: od startu do mety. Czas CPU to suma czasu pracy **wszystkich** robotników. Czterech murarzy stawia ścianę w 1 h — stoper pokaże 1 h, ale płacisz za 4 roboczogodziny. Program równoległy ma być szybszy **na stoperze**; czas CPU zwykle nawet rośnie (narzut wątków).

## Definicja i wzory
> [!definicja] Flagi gcc, które musisz znać
> | flaga | co robi |
> |---|---|
> | `-O0` | bez optymalizacji (domyślne) — do debugowania, **nie do pomiarów** |
> | `-O2` | standardowe optymalizacje — **ustawienie do pomiarów** |
> | `-O3` | agresywniej: wektoryzacja, rozwijanie pętli; nie zawsze szybciej |
> | `-g` | symbole debugowe dla gdb/valgrind (nie spowalnia kodu) |
> | `-Wall -Wextra` | ostrzeżenia — traktuj je poważnie |
> | `-pthread` | biblioteka Pthreads (+ makra) — przy kompilacji i linkowaniu |
> | `-fopenmp` | włącza `#pragma omp` i linkuje libgomp; bez niej pragmy są **ignorowane** |
> | `-lm` | biblioteka matematyczna (`sqrt`, `sin`) — **na końcu**, po plikach `.c` |

> [!definicja] `time ./prog` — trzy liczby
> - **real** — czas ścienny od startu do końca.
> - **user** — czas CPU w trybie użytkownika, **zsumowany po wszystkich wątkach**.
> - **sys** — czas CPU w jądrze (wywołania systemowe: `clone`, `mmap`, I/O).
>
> Program sekwencyjny: `user + sys ≈ real`. Program wielowątkowy: **`user > real`** — to dobry znak, znaczy, że kilka rdzeni liczyło naraz. Gdy `user ≈ p · real`, wątki pracowały cały czas.

> [!wzor] Wielkości, które policzysz z pomiarów
> $$S(p) = \frac{T_1}{T_p} \qquad E(p) = \frac{S(p)}{p}$$
> $T_1$ — czas wersji sekwencyjnej, $T_p$ — na $p$ wątkach/procesach (zawsze czasy ścienne). Szczegóły: [[Prawo Amdahla]].

> [!wazne] Trzy zegary w kodzie
> - `clock_gettime(CLOCK_MONOTONIC, &ts)` — czas ścienny, nie cofa się przy zmianie zegara systemowego. **Domyślny wybór.**
> - `clock()` — czas CPU **całego procesu** (suma wątków). W programie wielowątkowym pokaże ~$p$ razy więcej niż stoper — pułapka.
> - `omp_get_wtime()` — czas ścienny w OpenMP (`#include <omp.h>`, `-fopenmp`).

## Jak to zrobić — krok po kroku
1. **Sprawdź maszynę:** `nproc` (liczba procesorów logicznych), `lscpu` — patrz na `Core(s) per socket` i `Thread(s) per core`. Np. 6 rdzeni × 2 (SMT/Hyper-Threading) = 12 logicznych. Przyspieszenie zwykle przestaje rosnąć po liczbie **fizycznych** rdzeni.
2. **Kompiluj z `-O2`** (i `-g`, nie przeszkadza). Wersję sekwencyjną i równoległą — **tymi samymi flagami**.
3. **Mierz tylko obliczenia:** `t0` tuż przed pętlą, `t1` tuż po. Bez `printf`, alokacji i wczytywania danych w środku.
4. **Użyj wyniku** (wypisz go po pomiarze) — inaczej `-O2` może uznać pętlę za martwy kod i ją usunąć.
5. **Powtórz 3–5 razy**, pierwszy przebieg traktuj jako rozgrzewkę (cache, page faulty, CPU podkręca zegar). Podaj minimum lub medianę.
6. **Dobierz rozmiar problemu** tak, by czas był rzędu 0,1–10 s — milisekundy to szum.
7. Zapisz w sprawozdaniu: CPU, liczba rdzeni, flagi, rozmiar, liczba powtórzeń.

**Prosty Makefile** (na każdy lab warto mieć gotowy):

```make
CC      = gcc
CFLAGS  = -O2 -g -Wall -Wextra
LDLIBS  = -pthread -lm

PROGS = petla licznik

all: $(PROGS)

%: %.c
	$(CC) $(CFLAGS) -o $@ $< $(LDLIBS)

clean:
	rm -f $(PROGS)

.PHONY: all clean
```

(Wcięcia przed `$(CC)` i `rm` to **tabulator**, nie spacje.) Dla OpenMP dopisz `-fopenmp` do `CFLAGS` i `LDLIBS`.

**Śledzenie wykonania — ściąga:**

```bash
gcc -g -O0 -pthread prog.c -o prog        # -O0 do debugowania: zmienne nie znikają
gdb ./prog
#   (gdb) break praca        # pułapka na funkcji
#   (gdb) run                # start
#   (gdb) bt                 # stos wywołań (backtrace)
#   (gdb) info threads       # lista wątków, * = bieżący
#   (gdb) thread 2           # przełącz na wątek 2
#   (gdb) next / step / print x / continue
strace -f ./prog                          # wywołania systemowe, -f = też dzieci i wątki
strace -f -e trace=clone,clone3,wait4 ./prog   # tylko tworzenie i czekanie
ltrace ./prog                             # wywołania funkcji bibliotecznych (libc)
valgrind ./prog                           # błędy pamięci (memcheck)
valgrind --tool=helgrind ./prog           # wyścigi w Pthreads
gcc -g -fsanitize=address prog.c -o prog  # ASan: przepełnienia, use-after-free
gcc -g -fsanitize=thread -pthread prog.c -o prog   # TSan: wyścigi danych
perf stat ./prog                          # cykle, instrukcje, cache-misses, task-clock
```

## Przykład
**Zadanie:** zmierz czas pętli sumującej $\sum 1/i^2$ (dąży do $\pi^2/6$) przy `-O0`, `-O2`, `-O3`, oboma zegarami.

```c
#include <stdio.h>
#include <time.h>

#define N 200000000L

static double teraz(void) {            /* czas ścienny w sekundach */
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return t.tv_sec + t.tv_nsec * 1e-9;
}

int main(void) {
    double suma = 0.0;
    double t0 = teraz();
    clock_t c0 = clock();

    for (long i = 1; i <= N; i++)
        suma += 1.0 / ((double)i * i);  /* szereg -> pi^2/6 */

    double t1 = teraz();
    clock_t c1 = clock();

    printf("suma  = %.12f\n", suma);    /* wypisz wynik: inaczej -O2 może wyrzucić pętlę */
    printf("sciana: %.3f s\n", t1 - t0);
    printf("CPU   : %.3f s\n", (double)(c1 - c0) / CLOCKS_PER_SEC);
    return 0;
}
```

```bash
for o in O0 O2 O3; do gcc -$o -Wall -Wextra -o petla_$o petla.c && echo "== -$o" && ./petla_$o; done
```

Prawdziwe wyjście (AMD Ryzen 5 5500, WSL2 — **twoje liczby będą inne**):

```text
== -O0
suma  = 1.644934057835
sciana: 0.724 s
CPU   : 0.724 s
== -O2
suma  = 1.644934057835
sciana: 0.305 s
CPU   : 0.305 s
== -O3
suma  = 1.644934057835
sciana: 0.246 s
CPU   : 0.246 s
```

**Interpretacja:**
1. `-O0` → `-O2`: ok. 2,5× szybciej. Przy `-O0` zmienne `suma` i `i` lądują w pamięci po każdym obrocie, przy `-O2` siedzą w rejestrach.
2. `-O2` ≈ `-O3` — w powtórzeniach różnica skakała w obie strony (0,24–0,30 s). Kompilator **nie może** zwektoryzować sumy `double`, bo zmieniłoby to kolejność dodawania, a dodawanie zmiennoprzecinkowe nie jest łączne. Dopiero `-ffast-math` mu na to pozwala.
3. Ściana = CPU, bo program ma jeden wątek. Wyniki skakały między przebiegami o ±30% → dlatego **powtarzasz** pomiary.

> [!przyklad]- Przykład 2 — pułapka `clock()` w programie wielowątkowym
> Ta sama suma, ale w OpenMP na 4 wątkach:
> ```c
> #include <stdio.h>
> #include <time.h>
> #include <omp.h>
>
> #define N 400000000L
>
> int main(void) {
>     double suma = 0.0;
>     double w0 = omp_get_wtime();
>     clock_t c0 = clock();
>
>     #pragma omp parallel for reduction(+:suma)
>     for (long i = 1; i <= N; i++)
>         suma += 1.0 / ((double)i * i);
>
>     double w1 = omp_get_wtime();
>     clock_t c1 = clock();
>     printf("watki : %d\n", omp_get_max_threads());
>     printf("suma  = %.12f\n", suma);
>     printf("sciana: %.3f s (omp_get_wtime)\n", w1 - w0);
>     printf("clock : %.3f s (suma CPU wszystkich watkow!)\n",
>            (double)(c1 - c0) / CLOCKS_PER_SEC);
>     return 0;
> }
> ```
> ```bash
> gcc -O2 -fopenmp -Wall -Wextra -o omp_czas omp_czas.c
> OMP_NUM_THREADS=4 bash -c 'time ./omp_czas'
> ```
> ```text
> watki : 4
> suma  = 1.644934065335
> sciana: 0.171 s (omp_get_wtime)
> clock : 0.642 s (suma CPU wszystkich watkow!)
>
> real	0m0.176s
> user	0m0.621s
> sys	0m0.028s
> ```
> `clock()` ≈ 4 × stoper, `user` ≈ 3,5 × `real` — cztery rdzenie liczyły równolegle. Gdybyś liczył przyspieszenie z `clock()`, wyszłoby, że wersja równoległa jest **wolniejsza**. Ostatnie cyfry sumy różnią się od wersji sekwencyjnej — inna kolejność dodawania (nieprzemienność zaokrągleń, nie błąd).

> [!przyklad]- Przykład 3 — gdb na programie z wątkami
> ```bash
> gcc -g -O0 -pthread -o licznik licznik.c
> gdb -q -batch -ex "break praca" -ex "run" -ex "bt" -ex "info threads" ./licznik
> ```
> ```text
> Thread 2 "licznik" hit Breakpoint 1, praca (arg=0x0) at licznik.c:14
> #0  praca (arg=0x0) at licznik.c:14
> #1  ... in start_thread (arg=<optimized out>) at ./nptl/pthread_create.c:447
> #2  ... in clone3 () at ../sysdeps/unix/sysv/linux/x86_64/clone3.S:78
>   Id   Target Id                                    Frame
>   1    Thread 0x7ffff7fac740 (LWP 5614) "licznik" clone3 () ...
> * 2    Thread 0x7ffff7bff6c0 (LWP 5621) "licznik" praca (arg=0x0) at licznik.c:14
> ```
> Widać, że wątek startuje z `start_thread` (glibc) wywołanego przez `clone3` — wątki w Linuksie to zadania jądra (LWP) ze wspólną pamięcią. Program `licznik.c` jest w [[Proces i wątek]].

## Typowe błędy
> [!blad] Pomiar przy `-O0`
> Mierzysz wtedy głównie złe zarządzanie rejestrami, nie algorytm. Przyspieszenia z `-O0` bywają sztucznie piękne, bo wersja sekwencyjna jest sztucznie wolna.

> [!blad] `clock()` jako stoper programu wielowątkowego
> Zwraca sumę czasu CPU wszystkich wątków. Do przyspieszenia — wyłącznie czas ścienny.

> [!blad] `-lm` / `-pthread` w złym miejscu albo wcale
> `gcc -lm prog.c` może dać `undefined reference to 'sqrt'` — biblioteki po plikach źródłowych. Bez `-fopenmp` program z pragmami skompiluje się bez błędu, tylko będzie **jednowątkowy**.

> [!blad] Wynik obliczeń nieużyty
> Kompilator z `-O2` usunie pętlę, której wynik nigdzie nie trafia, i zmierzysz 0,000 s.

> [!blad] `printf` w mierzonym fragmencie i jeden pomiar
> I/O jest wolne i serializuje wątki. Pojedynczy pomiar na laptopie/WSL może odbiegać o kilkadziesiąt procent.

> [!blad] TSan: `FATAL: ThreadSanitizer: unexpected memory mapping`
> Na nowych jądrach (u ciebie WSL2 6.18) losowość adresów jest za duża dla TSan. Obejście: `setarch -R ./prog` (wyłącza ASLR dla tego uruchomienia). Sprawdzone — wtedy raportuje wyścig.

## Na zajęcia i egzamin
> [!zajecia]
> - **Przygotowanie do lab 1 (przewidywanie na bazie sylabusa: „kompilatory i śledzenie wykonania”):** umieć bez zaglądania skompilować program z `-O2 -g -Wall`, napisać Makefile, uruchomić `time`, wyjaśnić real/user/sys, zmierzyć fragment kodu `clock_gettime`, postawić breakpoint w gdb i przełączać wątki, puścić `strace -f`. Sprawdź `nproc`/`lscpu` na sprzęcie w sali.
> - **Typowe pytania:** czym różni się czas ścienny od czasu CPU; dlaczego `user > real`; jak zdefiniować przyspieszenie i efektywność; czemu mierzymy przy włączonej optymalizacji; czemu przyspieszenie na 12 logicznych CPU nie jest 12×.
> - Łączy się z: [[Proces i wątek]], [[Wyścig danych]], [[Prawo Amdahla]], [[OpenMP]].

## Zadania
> [!zadanie] Zadanie 1
> `time ./prog` daje `real 2.0s, user 7.6s, sys 0.1s`. Ile mniej więcej wątków pracowało i co to mówi o programie?
> > [!odpowiedz]- Odpowiedź
> > `user/real ≈ 3,8` → ok. 4 wątki zajęte prawie cały czas. Program jest wielowątkowy i dobrze zrównoleglony (mało czekania).

> [!zadanie] Zadanie 2
> Wersja sekwencyjna: 8,0 s, na 4 wątkach: 2,5 s. Policz $S$ i $E$.
> > [!odpowiedz]- Odpowiedź
> > $S = 8/2{,}5 = 3{,}2$, $E = 3{,}2/4 = 0{,}8$ (80%).

> [!zadanie] Zadanie 3
> Kolega zmierzył `clock()` i twierdzi, że wersja na 8 wątkach trwa 9 s, a sekwencyjna 8 s. Co jest nie tak?
> > [!odpowiedz]- Odpowiedź
> > `clock()` sumuje czas CPU wszystkich wątków. Wersja równoległa zużyła trochę więcej CPU (narzut), ale stoper mógł pokazać np. 1,3 s. Trzeba mierzyć `clock_gettime(CLOCK_MONOTONIC)` lub `omp_get_wtime()`.

> [!zadanie] Zadanie 4
> Po dodaniu `-O2` pomiar pętli spadł do 0,000 s. Dlaczego i jak to naprawić?
> > [!odpowiedz]- Odpowiedź
> > Wynik pętli nie był używany, więc kompilator usunął ją jako martwy kod. Wypisz wynik po pomiarze (albo zapisz do zmiennej globalnej / `volatile`).

## Fiszki #fiszki/równoległość
Co znaczą real, user i sys w `time`?::real — czas ścienny; user — czas CPU w trybie użytkownika (suma wątków); sys — czas CPU w jądrze
Co znaczy `user > real`?::Kilka rdzeni pracowało naraz — program wielowątkowy; `user/real` ≈ liczba aktywnych wątków
Czym mierzyć czas ścienny w C na Linuksie?::`clock_gettime(CLOCK_MONOTONIC, &ts)`, w OpenMP `omp_get_wtime()`
Pułapka `clock()` w programie wielowątkowym?::Zwraca czas CPU całego procesu (sumę wątków), więc rośnie z liczbą wątków zamiast maleć
Wzór na przyspieszenie i efektywność::$S(p)=T_1/T_p$, $E(p)=S(p)/p$, z czasów ściennych
Jaką optymalizacją kompilować do pomiarów?::`-O2` (ta sama dla wersji sekwencyjnej i równoległej); `-O0` tylko do debugowania
Co się stanie bez `-fopenmp`?::Pragmy `#pragma omp` są ignorowane — program kompiluje się, ale działa na jednym wątku
Czym różnią się `strace` i `ltrace`?::strace śledzi wywołania systemowe (jądro), ltrace wywołania funkcji bibliotek dzielonych; `-f` śledzi też dzieci/wątki
Komendy gdb do wątków::`info threads` — lista; `thread N` — przełącz; `bt` — stos bieżącego wątku
Jak wykryć wyścig danych narzędziem?::`gcc -g -fsanitize=thread -pthread` (TSan) albo `valgrind --tool=helgrind`
`nproc` = 12, `lscpu`: 6 rdzeni × 2 wątki — ile realnego przyspieszenia?::Zwykle do ok. 6× (rdzenie fizyczne); SMT dodaje niewiele przy obliczeniach
Zasady rzetelnego pomiaru::-O2, mierz tylko obliczenia (bez printf), użyj wyniku, rozgrzewka, 3–5 powtórzeń, min lub mediana, czas rzędu sekund

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.

## Źródła
- I. Foster, *Designing and Building Parallel Programs* (online: mcs.anl.gov/~itf/dbpp) — rozdz. 3: modele wydajności, przyspieszenie, efektywność, eksperymentalne badanie wydajności.
- M. J. Quinn, *Parallel Programming in C with MPI and OpenMP* — rozdz. o analizie wydajności i pomiarze czasu.
- `man 2 clock_gettime`, `man 3 clock`, `man 1 time`, `man 1 strace`, `man 1 ltrace`.
- Dokumentacja GCC: *Options That Control Optimization*, *Program Instrumentation Options* (`-fsanitize`).
- Dokumentacja GDB: *Debugging Programs with Multiple Threads*.
- Wszystkie programy i wyjścia sprawdzone 2026-10-01 na gcc 13.3, AMD Ryzen 5 5500, WSL2.
