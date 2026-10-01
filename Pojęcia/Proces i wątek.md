---
typ: pojęcie
status: nowe
przedmioty: ["[[AGH Programowanie równoległe]]"]
wymaga: []
powiązane: ["[[Wyścig danych]]", "[[Kompilacja i pomiar czasu wykonania]]", "[[Pthreads]]", "[[Mutex i zmienna warunkowa]]", "[[OpenMP]]"]
tags: [pojęcie, równoległość]
utworzono: 2026-10-01
źródła: ["I. Foster — Designing and Building Parallel Programs (online: mcs.anl.gov/~itf/dbpp)", "A. Grama i in. — Introduction to Parallel Computing, rozdz. 7 (Pthreads)", "man 2 fork, man 2 wait, man 3 exec, man 2 clone, man 3 pthread_create, man 3 pthread_join"]
---
# Proces i wątek

> [!skrot] W 30 sekund
> - **Co to:** proces = program w trakcie wykonania z **własną** przestrzenią adresową; wątek = niezależny ciąg wykonania **wewnątrz** procesu, dzielący z innymi wątkami pamięć.
> - **Klucz:** po `fork()` dziecko ma **kopię** zmiennych (zmiana nie wraca do rodzica); wątki z `pthread_create` widzą **te same** zmienne globalne i stertę → komunikacja za darmo, ale [[Wyścig danych]].
> - **Kiedy:** procesy — izolacja, uruchamianie innych programów, MPI; wątki — równoległe liczenie na wspólnych danych (Pthreads, Java, OpenMP).

> [!intuicja] Po ludzku
> Proces to osobne mieszkanie: własna kuchnia, własna lodówka. `fork()` to sklonowanie mieszkania razem z zawartością lodówki — od tej chwili każdy je swoje. Wątki to współlokatorzy w **jednym** mieszkaniu: każdy ma swój notes (stos), ale lodówka (sterta, globalne) jest wspólna. Wygodnie — i łatwo o to, że dwóch naraz weźmie ostatnie mleko.

## Definicja i wzory
> [!definicja] Co wątki współdzielą, a co mają prywatne
> | wspólne dla wątków procesu | prywatne dla każdego wątku |
> |---|---|
> | kod programu | **stos** (zmienne lokalne, adresy powrotu) |
> | zmienne globalne i `static` | **rejestry**, w tym licznik rozkazów (PC/RIP) i wskaźnik stosu |
> | **sterta** (`malloc`) | identyfikator wątku (TID) |
> | otwarte **deskryptory plików**, katalog roboczy | maska sygnałów, `errno` |
> | PID, użytkownik, obsługa sygnałów | zmienne `_Thread_local` / `__thread` |
>
> Procesy nie dzielą **niczego** z powyższej lewej kolumny (poza kodem tylko do odczytu i deskryptorami odziedziczonymi przy `fork`). Komunikują się przez potoki, gniazda, pamięć dzieloną, pliki — albo przez MPI.

> [!definicja] `fork()` — wartość zwracana
> - `< 0` — błąd (dziecko nie powstało),
> - `0` — jesteś w **dziecku**,
> - `> 0` — jesteś w **rodzicu**, wartość = PID dziecka.
>
> Jedno wywołanie, **dwa powroty**. Dziecko zaczyna od instrukcji po `fork()`, z kopią całej pamięci rodzica.

> [!definicja] Copy-on-write (COW)
> `fork()` nie kopiuje fizycznie pamięci. Rodzic i dziecko dostają te same strony oznaczone tylko-do-odczytu; dopiero **pierwszy zapis** do strony powoduje skopiowanie jej (4 KiB). Dlatego `fork()` jest tani, a semantycznie nadal każdy ma **swoją kopię**.

> [!wazne] Wspólny mechanizm w Linuksie: `clone()`
> Jądro Linuksa nie rozróżnia „procesów” i „wątków” — ma **zadania** (tasks). Oba powstają przez `clone()`/`clone3()`, różnią się flagami: wątek dostaje `CLONE_VM | CLONE_FILES | CLONE_SIGHAND | CLONE_THREAD …` (wspólna pamięć, deskryptory, ten sam PID), proces z `fork()` — bez nich. Widać to w `strace -f` (patrz przykład 3).

**Koszt:** utworzenie wątku to dziesiątki mikrosekund (stos + struktura w jądrze). `fork()` musi skopiować tablice stron i deskryptorów — zwykle kilka–kilkanaście razy drożej, a przełączanie między procesami dodatkowo zmienia przestrzeń adresową (TLB). Dlatego do drobnoziarnistej równoległości używa się wątków.

## Jak to zrobić — krok po kroku
**Procesy:**
1. `pid_t pid = fork();` — sprawdź `pid < 0` (błąd).
2. `if (pid == 0) { … praca dziecka …; exit(0); }` — **zawsze** kończ dziecko `exit`, inaczej poleci dalej kodem rodzica (np. sam zrobi kolejne `fork`).
3. Opcjonalnie w dziecku `execv("/ścieżka", argv)` — zastępuje cały program innym; wraca tylko przy błędzie.
4. Rodzic: `waitpid(pid, &status, 0)` lub `wait(NULL)` dla każdego dziecka; `WIFEXITED(status)`, `WEXITSTATUS(status)` dają kod wyjścia. Bez tego zostają **zombie**.

**Wątki (Pthreads, `#include <pthread.h>`, kompilacja z `-pthread`):**
1. `pthread_t t;`
2. `pthread_create(&t, NULL, funkcja, arg);` — `funkcja` ma sygnaturę `void *funkcja(void *arg)`.
3. Argument: wskaźnik na **osobną** pamięć dla każdego wątku (element tablicy, `malloc`), nie na zmienną pętli.
4. Wynik: zapisz do tej struktury albo `return wskaźnik;` i odbierz `pthread_join(t, &wynik)`. Nigdy nie zwracaj adresu zmiennej lokalnej wątku — stos znika po jego zakończeniu.
5. `pthread_join(t, NULL);` — czekaj na każdy wątek przed użyciem wyników.

## Przykład
**Zadanie:** zmienna globalna `licznik = 0`. Najpierw 4 procesy (fork), potem 4 wątki — każdy zwiększa licznik milion razy. Co wypisze rodzic?

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>

#define ILE 4            /* liczba procesów / wątków */
#define KROKI 1000000    /* ile razy każdy zwiększa licznik */

int licznik = 0;         /* zmienna GLOBALNA */

void *praca(void *arg) {
    (void)arg;
    for (int i = 0; i < KROKI; i++)
        licznik++;       /* bez synchronizacji! */
    return NULL;
}

int main(void) {
    /* --- część 1: procesy --- */
    for (int p = 0; p < ILE; p++) {
        pid_t pid = fork();
        if (pid < 0) { perror("fork"); exit(1); }
        if (pid == 0) {                         /* dziecko */
            praca(NULL);
            printf("dziecko %d (pid %d): licznik = %d\n", p, getpid(), licznik);
            exit(0);
        }
    }
    for (int p = 0; p < ILE; p++)
        wait(NULL);                             /* rodzic czeka na dzieci */
    printf("RODZIC po fork:      licznik = %d\n\n", licznik);

    /* --- część 2: wątki --- */
    pthread_t t[ILE];
    for (int i = 0; i < ILE; i++)
        pthread_create(&t[i], NULL, praca, NULL);
    for (int i = 0; i < ILE; i++)
        pthread_join(t[i], NULL);
    printf("po watkach:          licznik = %d (oczekiwane %d)\n", licznik, ILE * KROKI);
    return 0;
}
```

```bash
gcc -O0 -Wall -Wextra -pthread -o licznik licznik.c && ./licznik
```

Prawdziwe wyjście (liczby po wątkach i kolejność dzieci **zmieniają się przy każdym uruchomieniu**):

```text
dziecko 1 (pid 5453): licznik = 1000000
dziecko 0 (pid 5452): licznik = 1000000
dziecko 2 (pid 5454): licznik = 1000000
dziecko 3 (pid 5455): licznik = 1000000
RODZIC po fork:      licznik = 0

po watkach:          licznik = 1463208 (oczekiwane 4000000)
```

**Interpretacja krok po kroku:**
1. Każde dziecko dostało **własną kopię** `licznik` (= 0) i doliczyło do 1 000 000 — u siebie. Kolejność dzieci jest dowolna (decyduje planista).
2. Rodzic wciąż ma `0` — zmiany w dzieciach nie wracają. To klasyczne pytanie z wejściówek.
3. Wątki mają **jedną wspólną** zmienną. `licznik++` to trzy kroki: odczyt → +1 → zapis. Dwa wątki odczytują tę samą wartość, oba zapisują +1 i jedna inkrementacja ginie. Wynik ≈ 1,4 mln zamiast 4 mln — [[Wyścig danych]]. Naprawa: mutex, `atomic` lub sumy lokalne łączone na końcu.
4. Ciekawostka: z `-O2` wynik często wychodzi poprawne 4 000 000 — kompilator zamienia pętlę na jedno `licznik += 1000000`, więc okno wyścigu maleje. Błąd **dalej jest**, tylko rzadziej widać (w testach u mnie raz 4 000 000, raz 1 391 807). „U mnie działa” nie dowodzi poprawności.

> [!przyklad]- Przykład 2 — przekazanie argumentu i zwrot wyniku z wątku
> Każdy wątek dostaje **swoją** strukturę (element tablicy) i do niej zapisuje wynik.
> ```c
> #include <stdio.h>
> #include <pthread.h>
>
> #define W 4
>
> typedef struct { int id; long od, doo; long wynik; } Zadanie;
>
> void *suma(void *arg) {
>     Zadanie *z = arg;                 /* każdy wątek ma SWOJĄ strukturę */
>     z->wynik = 0;
>     for (long i = z->od; i < z->doo; i++)
>         z->wynik += i;
>     return NULL;
> }
>
> int main(void) {
>     pthread_t t[W];
>     Zadanie z[W];                     /* tablica, nie jedna zmienna! */
>     long n = 1000000;
>     for (int i = 0; i < W; i++) {
>         z[i] = (Zadanie){ .id = i, .od = i * n / W, .doo = (i + 1) * n / W };
>         pthread_create(&t[i], NULL, suma, &z[i]);
>     }
>     long razem = 0;
>     for (int i = 0; i < W; i++) {
>         pthread_join(t[i], NULL);
>         printf("watek %d: [%ld, %ld) -> %ld\n", z[i].id, z[i].od, z[i].doo, z[i].wynik);
>         razem += z[i].wynik;
>     }
>     printf("razem = %ld (wzor: %ld)\n", razem, n * (n - 1) / 2);
>     return 0;
> }
> ```
> ```text
> watek 0: [0, 250000) -> 31249875000
> watek 1: [250000, 500000) -> 93749875000
> watek 2: [500000, 750000) -> 156249875000
> watek 3: [750000, 1000000) -> 218749875000
> razem = 499999500000 (wzor: 499999500000)
> ```
> Brak wyścigu: każdy wątek pisze tylko do swojego `z[i]`, a główny czyta dopiero po `pthread_join`.

> [!przyklad]- Przykład 3 — pułapka: wskaźnik na zmienną pętli
> ```c
> #include <stdio.h>
> #include <pthread.h>
>
> void *pokaz(void *arg) {
>     printf("dostalem %d\n", *(int *)arg);
>     return NULL;
> }
>
> int main(void) {
>     pthread_t t[4];
>     for (int i = 0; i < 4; i++)
>         pthread_create(&t[i], NULL, pokaz, &i);   /* BŁĄD: adres zmiennej pętli */
>     for (int i = 0; i < 4; i++)
>         pthread_join(t[i], NULL);
>     return 0;
> }
> ```
> Trzy uruchomienia:
> ```text
> dostalem 3 dostalem 4 dostalem 3 dostalem 4
> dostalem 1 dostalem 2 dostalem 4 dostalem 4
> dostalem 3 dostalem 4 dostalem 4 dostalem 4
> ```
> Wszystkie wątki dostają **ten sam adres**; zanim wątek odczyta wartość, pętla już ją zmieniła (a po wyjściu z pętli `i` w ogóle nie istnieje — zachowanie niezdefiniowane). Naprawa: `int id[4]; id[i] = i; … &id[i]` jak w przykładzie 2.

> [!przyklad]- Przykład 4 — `fork` + `execv` + `waitpid`, i co widać w `strace`
> ```c
> #include <stdio.h>
> #include <stdlib.h>
> #include <unistd.h>
> #include <sys/wait.h>
>
> int main(void) {
>     pid_t pid = fork();
>     if (pid == 0) {                                  /* dziecko */
>         char *argv[] = { "ls", "-l", "/etc/hostname", NULL };
>         execv("/bin/ls", argv);                      /* zastępuje program */
>         perror("execv");                             /* tu tylko przy błędzie */
>         exit(127);
>     }
>     int status;
>     waitpid(pid, &status, 0);
>     if (WIFEXITED(status))
>         printf("rodzic: dziecko %d skonczylo z kodem %d\n", pid, WEXITSTATUS(status));
>     return 0;
> }
> ```
> ```text
> -rw-r--r-- 1 root root 16 Oct  1 16:12 /etc/hostname
> rodzic: dziecko 5560 skonczylo z kodem 0
> ```
> A tak `strace -f -e trace=clone,clone3 ./licznik` pokazuje różnicę proces/wątek (skrócone):
> ```text
> clone(child_stack=NULL, flags=CLONE_CHILD_CLEARTID|CLONE_CHILD_SETTID|SIGCHLD)        <- fork()
> clone3({flags=CLONE_VM|CLONE_FS|CLONE_FILES|CLONE_SIGHAND|CLONE_THREAD|...})         <- pthread_create
> ```
> To ten sam mechanizm jądra; wątek to `clone` z flagą „dziel pamięć” (`CLONE_VM`).

## Typowe błędy
> [!blad] „Dziecko zmieniło zmienną globalną, więc rodzic ją widzi”
> Nie. Po `fork()` to osobne kopie (COW). Do przekazania danych potrzebny jest potok, pamięć dzielona (`mmap`/`shm_open`) albo kod wyjścia.

> [!blad] Brak `exit()` w dziecku
> W pętli z `fork()` dziecko bez `exit` też wykona kolejne obroty i kolejne `fork` — zamiast 4 procesów masz $2^4 = 16$.

> [!blad] Brak `wait`/`pthread_join`
> Rodzic kończy się przed dziećmi (sieroty, zombie), a `main` kończący się `return` zabija wszystkie wątki — wyniki nie zdążą powstać.

> [!blad] `&i` jako argument wątku
> Patrz przykład 3. Każdy wątek potrzebuje własnej komórki pamięci.

> [!blad] Zwracanie adresu zmiennej lokalnej z funkcji wątku
> `return &wynik;` gdzie `wynik` jest lokalny — stos wątku znika po jego zakończeniu. Zwracaj `malloc` albo pisz do struktury od wywołującego.

## Na zajęcia i egzamin
> [!zajecia]
> - **Przygotowanie (przewidywanie na bazie sylabusa — lab 1 „kompilatory i śledzenie wykonania”, lab 2 Pthreads):** umieć napisać z pamięci szkielet `fork`+`waitpid` i `pthread_create`+`pthread_join`, przewidzieć wynik programu z licznikiem po `fork`, policzyć, ile procesów powstanie z `fork()` w pętli, pokazać wątki w gdb (`info threads`) i w `strace -f`.
> - **Typowe pytania egzaminacyjne:** różnica proces/wątek (co współdzielą); co zwraca `fork`; co to copy-on-write; dlaczego tworzenie wątku jest tańsze; jak OS wspiera wątki (Linux: `clone`, model 1:1); na czym polega wyścig przy `licznik++`.
> - Łączy się z: [[Wyścig danych]], [[Kompilacja i pomiar czasu wykonania]], [[Pthreads]], [[Mutex i zmienna warunkowa]].

## Zadania
> [!zadanie] Zadanie 1
> Ile razy zostanie wypisane „hej”?
> ```c
> fork(); fork(); fork(); printf("hej\n");
> ```
> > [!odpowiedz]- Odpowiedź
> > Każdy `fork` podwaja liczbę procesów: $2^3 = 8$ razy.

> [!zadanie] Zadanie 2
> `int x = 5;` potem `if (fork() == 0) { x = 10; exit(0); } wait(NULL); printf("%d", x);` — co wypisze rodzic?
> > [!odpowiedz]- Odpowiedź
> > `5`. Dziecko zmieniło swoją kopię; przestrzenie adresowe są rozdzielone.

> [!zadanie] Zadanie 3
> To samo, ale `x = 10` robi wątek utworzony `pthread_create`, a główny wypisuje po `pthread_join`. Co teraz?
> > [!odpowiedz]- Odpowiedź
> > `10`. Wątki dzielą zmienne globalne; `pthread_join` gwarantuje, że zapis wątku jest widoczny.

> [!zadanie] Zadanie 4
> Wymień dwa elementy, które każdy wątek ma prywatne, i dwa współdzielone.
> > [!odpowiedz]- Odpowiedź
> > Prywatne: stos, rejestry (w tym licznik rozkazów). Współdzielone: sterta, zmienne globalne (też deskryptory plików, kod).

> [!zadanie] Zadanie 5
> Jak poprawić program z licznikiem, żeby wątki dały 4 000 000 i nadal liczyły równolegle?
> > [!odpowiedz]- Odpowiedź
> > Najlepiej: każdy wątek liczy we **własnej** zmiennej lokalnej, a na końcu jeden raz dodaje do globalnej pod mutexem (lub atomowo). Mutex przy każdej inkrementacji też działa, ale serializuje pracę i jest wolny.

## Fiszki #fiszki/równoległość
Proces a wątek — główna różnica::Proces ma własną przestrzeń adresową; wątki jednego procesu dzielą pamięć (sterta, globalne), mają własny stos i rejestry
Co wątki współdzielą?::Kod, zmienne globalne i static, stertę, deskryptory plików, PID
Co wątek ma prywatne?::Stos (zmienne lokalne), rejestry w tym licznik rozkazów, TID, errno
Co zwraca `fork()`?::0 w dziecku, PID dziecka w rodzicu, -1 przy błędzie
Zmienna globalna zmieniona w dziecku po fork — czy rodzic widzi zmianę?::Nie — dziecko ma własną kopię przestrzeni adresowej
Co to copy-on-write?::Po fork strony są wspólne tylko do odczytu; kopiowane dopiero przy pierwszym zapisie
Do czego `execv` i kiedy wraca?::Zastępuje program procesu innym plikiem wykonywalnym; wraca tylko przy błędzie
Po co `wait`/`waitpid`?::Czekanie na zakończenie dziecka i odebranie kodu wyjścia (WEXITSTATUS); zapobiega zombie
Funkcje do tworzenia i czekania na wątek w Pthreads::`pthread_create(&t, NULL, f, arg)` i `pthread_join(t, &wynik)`; kompilacja z `-pthread`
Pułapka przy argumencie `pthread_create` w pętli::Przekazanie `&i` — wszystkie wątki widzą tę samą, zmieniającą się zmienną; daj każdemu osobną komórkę
Jak Linux tworzy procesy i wątki?::Oba przez `clone()`; wątek dostaje flagi dzielenia pamięci i plików (CLONE_VM, CLONE_THREAD…)
Czemu `licznik++` w wielu wątkach gubi wyniki?::To odczyt–dodaj–zapis; przeplecione wątki nadpisują sobie zapisy — wyścig danych

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.

## Źródła
- I. Foster, *Designing and Building Parallel Programs* (online: mcs.anl.gov/~itf/dbpp) — model zadań i kanałów, pamięć wspólna vs rozproszona.
- A. Grama, A. Gupta, G. Karypis, V. Kumar, *Introduction to Parallel Computing* — rozdz. 7: programowanie z pamięcią wspólną, Pthreads.
- M. J. Quinn, *Parallel Programming in C with MPI and OpenMP* — procesy (MPI) vs wątki (OpenMP).
- `man 2 fork`, `man 2 wait`, `man 3 exec`, `man 2 clone`, `man 3 pthread_create`, `man 3 pthread_join`, `man 7 pthreads`.
- Wszystkie programy i wyjścia sprawdzone 2026-10-01 na gcc 13.3, AMD Ryzen 5 5500, WSL2.
