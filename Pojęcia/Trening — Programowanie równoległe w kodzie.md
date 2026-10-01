---
typ: pojęcie
status: nowe
przedmioty: ["[[AGH Programowanie równoległe]]"]
wymaga: ["[[Kompilacja i pomiar czasu wykonania]]", "[[Proces i wątek]]"]
powiązane: ["[[Wyścig danych]]", "[[Pthreads]]", "[[OpenMP]]", "[[Przyspieszenie i efektywność]]", "[[Prawo Amdahla]]", "[[Współbieżność w Javie]]"]
tags: [pojęcie, równoległość, kod]
utworzono: 2026-10-01
źródła: ["I. Foster — Designing and Building Parallel Programs (online: mcs.anl.gov/~itf/dbpp), rozdz. 3 — Quantitative Basis for Design", "M. J. Quinn — Parallel Programming in C with MPI and OpenMP (rozdz. o analizie wydajności: Amdahl, Karp–Flatt; rozdz. o OpenMP)", "A. Grama i in. — Introduction to Parallel Computing (rozdz. 5 — Analytical Modeling of Parallel Programs)", "man 2 fork, man 2 waitpid, man 3 execv, man 2 pipe, man 7 pthreads, man 3 pthread_create, man 2 clock_gettime", "OpenMP API Reference Guide (openmp.org)"]
---
# Trening — Programowanie równoległe w kodzie

> [!skrot] W 30 sekund
> - **Co to:** 8 zadań do samodzielnego napisania (pomiar czasu → fork → execv → Pthreads → wyścig → π w Pthreads → π w OpenMP → wątki w Javie) z automatycznym sprawdzaczem.
> - **Klucz:** `cd ~/studia/sem5/programowanie_rownolegle/trening && make test` — każdy test to OK albo FAIL. Rozwiązania w `rozwiazania/`, ale **dopiero po własnej próbie**.
> - **Kiedy:** wieczór przed labem 1 (01–05) i przed kolejnymi labami (06 Pthreads, 07 OpenMP, 08 Java).

> [!intuicja] Po ludzku
> Teorię masz w [[Kompilacja i pomiar czasu wykonania]], [[Proces i wątek]] i [[Wyścig danych]]. Ta notatka to **siłownia**: te same pojęcia, ale palcami na klawiaturze. `fork` to druga, osobna kuchnia (dania podaje się przez okienko — pipe); wątki to kilku kucharzy w jednej kuchni, którzy potrafią sięgnąć po tę samą miskę naraz.

## Definicja i wzory

> [!definicja] Przyspieszenie i efektywność
> $T_1$ — czas najlepszej wersji sekwencyjnej, $T_p$ — czas na $p$ wątkach/procesach (zawsze czas **ścienny**).
> $$S(p) = \frac{T_1}{T_p} \qquad E(p) = \frac{S(p)}{p}$$
> Idealnie $S(p) = p$, $E(p) = 1$; w praktyce $E < 1$ (narzut, synchronizacja, nierówny podział). Więcej: [[Przyspieszenie i efektywność]].

> [!wzor] Prawo Amdahla
> Jeśli ułamek $f$ czasu programu jest sekwencyjny (nie da się go zrównoleglić), to
> $$S(p) \le \frac{1}{f + \frac{1-f}{p}} \xrightarrow{p\to\infty} \frac{1}{f}$$
> Przy $f = 0{,}1$ nawet nieskończenie wiele rdzeni da najwyżej $S = 10$. Z pomiaru da się odwrotnie oszacować „efektywną część sekwencyjną” (metryka Karpa–Flatta): $e = \frac{1/S(p) - 1/p}{1 - 1/p}$. Szczegóły: [[Prawo Amdahla]].

> [!definicja] Ściąga API (szczegóły w `man`)
> | wywołanie | co robi / na co uważać |
> |---|---|
> | `fork()` | 0 w dziecku, PID dziecka w rodzicu, −1 błąd |
> | `waitpid(pid, &st, 0)` | `WIFEXITED`/`WEXITSTATUS` (8 bitów!), `WIFSIGNALED`/`WTERMSIG` |
> | `execv(sciezka, argv)` | `argv` kończy `NULL`; wraca tylko przy błędzie; bez `$PATH` |
> | `pipe(fd)` | `fd[0]` czytanie, `fd[1]` pisanie; zamykaj nieużywane końce |
> | `pthread_create(&t, NULL, f, arg)` | `void *f(void *)`; zwraca 0 albo kod błędu |
> | `pthread_join(t, &ret)` | czeka i odbiera `void *` zwrócone przez `f` |
> | `pthread_mutex_lock/unlock(&m)` | sekcja krytyczna |
> | `atomic_fetch_add(&x, 1)` | `<stdatomic.h>`, niepodzielne `+=` |
> | `#pragma omp parallel for reduction(+:s)` | dzieli pętlę, `s` prywatne, sumowane na końcu |
> | `num_threads(p)`, `omp_get_wtime()` | liczba wątków; czas ścienny |

## Jak to zrobić — krok po kroku
1. `cd ~/studia/sem5/programowanie_rownolegle/trening`, przeczytaj `README.md`.
2. Otwórz `zadania/01_pomiar.c` — nagłówek mówi, co zrobić i który `man` czytać. Szukaj `TODO`.
3. Pisz **sam**, z `man` i notatkami — bez `rozwiazania/`.
4. `make test` (albo `./sprawdz.sh 01`). Na starcie `4 OK, 23 FAIL` — tak ma być.
5. FAIL → przeczytaj opis po myślniku, uruchom program ręcznie.
6. Wszystko OK → **dopiero teraz** porównaj z `rozwiazania/`.
7. Odpowiedz na „Do przemyślenia” z nagłówka i przećwicz `strace -f`, `gdb` (`info threads`), `make tsan` z README.

> [!wazne] Kolejność i co na który lab
> **Lab 1:** 01–05. **Kolejne:** 06 Pthreads, 07 OpenMP, 08 Java. MPI nie ma na tym WSL (instalacja wymaga sudo) — w README tylko zapowiedź.

## Przykład
Zadanie 06 rozwiązane i zmierzone na tej maszynie (Ryzen 5 5500: 6 rdzeni × 2 SMT, $N = 4\cdot10^8$, $T_1 = 0{,}532$ s):

| p | $T_p$ [s] | $S(p)$ | $E(p)$ |
|---:|---:|---:|---:|
| 1 | 0,533 | 1,00 | 1,00 |
| 2 | 0,258 | 2,06 | 1,03 |
| 4 | 0,161 | 3,30 | 0,83 |
| 8 | 0,111 | 4,82 | 0,60 |

Jak to czytać krok po kroku:
1. $S(2) = 0{,}532/0{,}258 = 2{,}06$ — ponad 2 to szum/turbo; „prawie idealnie”.
2. $E(4) = 3{,}30/4 = 0{,}83$ — przy wielu aktywnych rdzeniach spada taktowanie turbo.
3. $p = 8 > 6$ rdzeni fizycznych — wątki dzielą rdzeń (SMT), więc $E(8) = 0{,}60$.
4. Karp–Flatt: $e = (1/4{,}82 - 1/8)/(1 - 1/8) \approx 0{,}094$ — to sprzęt, nie kod sekwencyjny.

**Liczby zależą od sprzętu** — w sprawozdaniu podaj CPU, flagi, $N$, liczbę powtórzeń.

> [!kod]- Rozwiązanie — rdzeń zadania 02 (fork + pipe)
> ```c
> for (int k = 0; k < n; k++) {
>     pipe(rury[k]);
>     pid_t pid = fork();
>     if (pid == 0) {                       /* dziecko */
>         close(rury[k][0]);
>         long od = (long)((long long)k * m / n) + 1, do_ = (long)((long long)(k + 1) * m / n);
>         long long s = 0;
>         for (long i = od; i <= do_; i++) s += i;
>         write(rury[k][1], &s, sizeof s);
>         _exit(k);
>     }
>     pidy[k] = pid;
>     close(rury[k][1]);                    /* rodzic tylko czyta */
> }
> for (int k = 0; k < n; k++) {
>     long long czesc; int st;
>     read(rury[k][0], &czesc, sizeof czesc);
>     waitpid(pidy[k], &st, 0);             /* WEXITSTATUS(st) == k */
>     suma += czesc;
> }
> ```

> [!kod]- Rozwiązanie — rdzeń zadania 03 (fork + execv)
> ```c
> pid_t pid = fork();
> if (pid == 0) {
>     execv(argv[1], argv + 1);   /* wraca tylko przy błędzie */
>     perror("execv");
>     _exit(127);
> }
> int st;
> waitpid(pid, &st, 0);
> if (WIFEXITED(st))        printf("kod=%d\n", WEXITSTATUS(st));
> else if (WIFSIGNALED(st)) printf("sygnal=%d\n", WTERMSIG(st));
> ```

> [!kod]- Rozwiązanie — rdzeń zadań 04/06 (Pthreads) i 07 (OpenMP)
> ```c
> typedef struct { long od, do_; double h, wynik; } arg_t;
>
> void *praca(void *p) {
>     arg_t *a = p;
>     double s = 0.0;                       /* suma lokalna: bez wyścigu */
>     for (long i = a->od; i < a->do_; i++) s += f((i + 0.5) * a->h);
>     a->wynik = s * a->h;
>     return NULL;
> }
> /* main: arg_t a[P]; pthread_t t[P];  — osobna struktura dla każdego wątku */
> for (int k = 0; k < p; k++) {
>     a[k] = (arg_t){ k * n / p, (k + 1) * n / p, h, 0.0 };
>     pthread_create(&t[k], NULL, praca, &a[k]);
> }
> for (int k = 0; k < p; k++) { pthread_join(t[k], NULL); pi += a[k].wynik; }
>
> /* 07 — to samo w OpenMP: */
> #pragma omp parallel for reduction(+:s) num_threads(p)
> for (long i = 0; i < n; i++) s += f((i + 0.5) * h);
> ```

## Typowe błędy
> [!blad] `&i` jako argument wątku
> Wszystkie wątki widzą tę samą zmienną pętli. Z `-DPULAPKA` (zadanie 04) przy 8 wątkach wyszło `id=1..8` — bez zera. Każdy wątek dostaje **własną** komórkę `args[k]`.

> [!blad] Suma przez kod wyjścia procesu
> `WEXITSTATUS` ma 8 bitów — wynik obetnie się modulo 256. Liczby przesyłaj pipe'em.

> [!blad] Zapomniany `close` końca pipe'a
> Bez zamknięcia końca do pisania czytanie do EOF wisi w nieskończoność.

> [!blad] `printf` przed `fork` bez `fflush`
> Bufor stdout kopiuje się do dziecka — tekst wychodzi dwa razy (zwłaszcza przy przekierowaniu).

> [!blad] Pomiar z `-O0` albo `clock()`
> `-O0` jest kilka razy wolniejsze (zadanie 01: 0,85 s vs 0,12 s). `clock()` sumuje CPU wszystkich wątków — nie pokaże przyspieszenia.

> [!blad] Pragma bez `-fopenmp`
> Kompiluje się, wynik dobry, przyspieszenia brak — pragmy zignorowane.

> [!blad] ThreadSanitizer na WSL
> Bez `setarch $(uname -m) -R ./prog` kończy się `FATAL: unexpected memory mapping` — to nie Twój błąd.

## Na zajęcia i egzamin
> [!zajecia]
> - **Laby:** z pamięci `fork` + `waitpid` + kod wyjścia, `execv` z obsługą błędu, `pthread_create`/`join` ze strukturą argumentów, naprawa wyścigu i pomiar czasu; $S$, $E$ i czemu $E<1$; `strace -f`.
> - **Sprawozdanie:** tabela $T_p$, $S$, $E$ + `lscpu`, flagi, powtórzenia, komentarz (rdzenie fizyczne vs logiczne, Amdahl).
> - **Egzamin:** wzory $S$, $E$, Amdahl i granica $1/f$; różnica proces/wątek; czym jest wyścig i jak go usunąć; co robi `reduction`.
> - Łączy się z: [[Wyścig danych]], [[Pthreads]], [[OpenMP]], [[Współbieżność w Javie]], [[Prawo Amdahla]].

## Zadania
> [!zadanie] Zadanie 1 — Amdahl
> 5% programu jest sekwencyjne. Jakie maksymalne przyspieszenie na 8 rdzeniach, a jakie na nieskończenie wielu?
> > [!odpowiedz]- Odpowiedź
> > $S(8) = 1/(0{,}05 + 0{,}95/8) = 1/0{,}169 \approx 5{,}9$. Granica: $1/0{,}05 = 20$.

> [!zadanie] Zadanie 2 — efektywność z pomiaru
> $T_1 = 12$ s, $T_6 = 2{,}5$ s. Policz $S$ i $E$. Czy to dobry wynik?
> > [!odpowiedz]- Odpowiedź
> > $S = 12/2{,}5 = 4{,}8$, $E = 4{,}8/6 = 0{,}8$. Dobry — 80% wykorzystania rdzeni to typowy wynik dla prostej pętli.

> [!zadanie] Zadanie 3 — ile procesów?
> Ile procesów (łącznie z pierwszym) powstanie po `fork(); fork(); fork();`?
> > [!odpowiedz]- Odpowiedź
> > $2^3 = 8$. Każdy `fork` podwaja liczbę procesów, które go wykonują.

> [!zadanie] Zadanie 4 — w kodzie
> Zrób 01–05 w treningu (same OK), potem 06: przy jakim $p$ przyspieszenie przestaje rosnąć i dlaczego?
> > [!odpowiedz]- Odpowiedź
> > Na Ryzenie 5 5500 rośnie wyraźnie do 6 wątków (rdzenie fizyczne), potem słabo (SMT); $E$ spada już od 4 przez niższe turbo.

## Fiszki #fiszki/równoległość
Wzór na przyspieszenie S(p)::S(p) = T1 / Tp, oba czasy ścienne, T1 z najlepszej wersji sekwencyjnej
Wzór na efektywność E(p)::E(p) = S(p) / p, idealnie 1
Prawo Amdahla — wzór::S(p) = 1 / (f + (1 - f)/p), gdzie f to część sekwencyjna
Granica przyspieszenia z prawa Amdahla::1/f, np. f = 0,1 daje najwyżej 10
Ile bitów ma kod wyjścia procesu odczytany przez WEXITSTATUS?::8 bitów, zakres 0–255
Kiedy execv wraca do wywołującego?::Tylko przy błędzie; po sukcesie kod procesu zostaje zastąpiony
Jak przekazać argument do wątku w pętli bez pułapki?::Osobna komórka dla każdego wątku, np. tablica struktur args[k], a nie adres licznika pętli
Jak wątek zwraca wynik do main?::return wskaźnika na trwałą pamięć (struktura argumentów albo malloc), odbiór w pthread_join(t, &ret)
Najtańsza poprawna wersja wspólnego licznika w wielu wątkach::Liczenie w zmiennej lokalnej i jedno dodanie na końcu (w OpenMP reduction)
Co się stanie z pragmą OpenMP bez -fopenmp?::Zostanie zignorowana; program działa sekwencyjnie
Jak uruchomić ThreadSanitizer na tym WSL?::gcc -fsanitize=thread -g, potem setarch $(uname -m) -R ./prog
Co pokazuje strace -f przy fork i przy pthread_create?::Oba to clone; wątek ma flagi CLONE_VM i CLONE_THREAD, proces nie

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.

## Źródła
- I. Foster, *Designing and Building Parallel Programs* (mcs.anl.gov/~itf/dbpp), rozdz. 3.
- M. J. Quinn, *Parallel Programming in C with MPI and OpenMP* — Amdahl, Karp–Flatt, OpenMP.
- A. Grama i in., *Introduction to Parallel Computing*, rozdz. 5.
- `man 2 fork`, `man 2 waitpid`, `man 3 execv`, `man 2 pipe`, `man 7 pthreads`, `man 2 clock_gettime`.
- OpenMP API Reference Guide (openmp.org).
- Kod i wyniki: `~/studia/sem5/programowanie_rownolegle/trening/` (README — pełne tabele z tej maszyny).
