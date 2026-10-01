---
typ: pojęcie
status: nowe
przedmioty: ["[[AGH Programowanie równoległe]]"]
wymaga: ["[[Zakleszczenie]]", "[[Mutex i zmienna warunkowa]]"]
powiązane: ["[[Pthreads]]", "[[Współbieżność w Javie]]", "[[OpenMP]]", "[[Prawo Amdahla]]"]
tags: [pojęcie, równoległość, współbieżność, systemy operacyjne]
utworzono: 2026-10-01
źródła: ["I. Foster — Designing and Building Parallel Programs (online: mcs.anl.gov/~itf/dbpp)", "cppreference.com — Memory model / Threads and data races (C11)", "man 7 pthreads"]
---
# Wyścig danych

> [!skrot] W 30 sekund
> - **Co to:** dwa wątki sięgają do tej samej zmiennej bez synchronizacji i co najmniej jeden pisze.
> - **Klucz:** `licznik++` to trzy instrukcje (odczyt, dodanie, zapis), więc aktualizacje się gubią. W C to *undefined behavior*.
> - **Kiedy:** każda wspólna zmienna w Pthreads, Javie i OpenMP. Lekarstwo: atomik, mutex albo redukcja.

> [!intuicja] Po ludzku
> **Dwa wątki gonią tę samą zmienną i oba chcą ją ruszyć w tej samej chwili.** Jeśli jeden czyta, a drugi w tym czasie pisze (albo oba piszą) — możesz dostać wynik, którego jako człowiek **nigdy byś nie przewidział**. Kolejność operacji jest przypadkowa jak w wyścigu samochodowym: kto zdąży pierwszy, ten decyduje — a zdarza się, że „przebitka” wpadnie w sam środek i zepsuje wynik.

> [!intuicja] To nie to samo co [[Zakleszczenie]]: tam nikt nic nie robi (wszyscy czekają w kółko), a tu wszyscy **pchają się do tej samej komórki naraz**.

## Definicja
> [!definicja] Wyścig danych (ang. *data race*)
> Wyścig danych to sytuacja, w której co najmniej dwa wątki wykonują **konkurujące dostępy** (ang. *conflicting accesses*) do **tej samej lokacji pamięci**:
> - przynajmniej jeden z nich **pisze**,
> - dostępy nie są **uporządkowane** żadną relacją *happens-before* (synchronizacją),
> - wykonywane są **jednocześnie** (współbieżnie).
>
> Czyli: **czytanie zmiennej, podczas gdy inny wątek ją pisze — to już wyścig.** Nie muszą oba pisać.

> [!definicja] Race condition ≠ data race
> - **Data race** = problem **pamięci** — surowe, niezsynchronizowane dostępy do tych samych bajtów. W C/C++ to **undefined behavior**; formalizują to `C11` i Java Memory Model. Da się wykryć narzędziami (Helgrind, ThreadSanitizer).
> - **Race condition** = szerszy problem **logiczny** — błąd, gdy wynik zależy od chronologii (sprawdzasz saldo, ktoś je zmienia, wypłacasz z nieaktualnego stanu). Może istnieć **nawet bez data race**, gdy dostępy są uporządkowane, ale logika i tak jest zła.
>
> Egzamin uwielbia to rozróżnienie — podaj je dokładnie tak.

## Jak to działa
**Dlaczego wynik jest losowy?** Bo na współczesnym sprzęcie `i++` to nie jedna operacja, a trzy:
```c
// int i = 0; dwa wątki robią: i++
mov %rax, i     // 1. wczytaj i do rejestru
add $1, %rax   // 2. zwiększ rejestr
mov %rax, i     // 3. wyślij z powrotem do pamięci
```
Dwa wątki to po trzy kroki na jedną wspólną zmienną; kolejność wykonania to permutacje tych kroków. Ponieważ nie da się wymusić, który wątek wejdzie w szczelinę między krokiem 1. a 3. drugiego — wynik to **gra losowa**. Czasem 1+1=2 (gdy jeden wątek zdąży wykonać wszystkie trzy kroki, zanim zacznie drugi), a czasem **1+1=1** — oba wątki wczytały `0`, oba zwiększyły lokalnie do `1` i oba zapisały `1`. Ostatni zapis „wygrał” i zgubił tamten krok.

**Co robić:**
- Jeśli problem to **kilka linii** (`i++`, `count++`, `sum += x`) — nie synchronizuj ręcznie, użyj **operacji atomowych** (`std::atomic<int>`, `AtomicInteger` w Javie). Mutex na jednej zmiennej to nadmiar i koszt.
- Jeśli to **większy blok** (stan musi być spójny przez wiele kroków) — atomik nie wystarczy; użyj [[Mutex i zmienna warunkowa|mutexu]] / sekcji krytycznej.

**Typowe scenariusze w labach:**
- **Licznik zdarzeń** (`hits++`, `count++`) — najprostszy data race.
- **Suma redukcyjna / akumulator** (`sum += lokalna`) — każdy wątek dodaje swój fragment; bez atomika albo **redukcji** dostajesz „zgubione aktualizacje”. Klasyka w [[OpenMP|OpenMPie]]: `#pragma omp parallel for reduction(+:sum)`.
- **Wspólny bufor / kolejka** (jeden wątek pisze, drugi czyta) — wyścig pojawia się w parach: pisanie–czytanie tej samej komórki; dochodzi też ryzyko [[Zakleszczenie|zakleszczenia]] przy zapełnionym buforze.

## Jak rozpoznać data race — krok po kroku
1. **Znajdź wspólne zmienne** — dostępne z więcej niż jednego wątku (globalne, pola statyczne, wspólne obiekty).
2. **Sprawdź, czy są uporządkowane** — czy między dostępami jest relacja **happens-before** (mutex na tym samym locku, `join`, `atomic` z właściwą kolejnością)? Jeśli nie ma porządku i występuje **zapis** — to data race.
3. **Sprawdź językowo** — w C/C++ to **undefined behavior** (kompilator może zrobić wszystko); w Javie jest „well-defined”, ale trzeba `synchronized` / `volatile` / `Atomic*`.
4. **Zweryfikuj narzędziem** — `gcc -fsanitize=thread` (ThreadSanitizer) albo `valgrind --tool=helgrind`. Na labach Linuxa to się liczy do przygotowania; w logu TSan pojawi się `WARNING: data race`.

## Przykłady
> [!przyklad]- Przykład 1 — licznik w C, dwa wątki
> ```c
> #include <pthread.h>
> #include <stdio.h>
> int licznik = 0;
>
> void* worker(void* arg) {
>     for (int i = 0; i < 100000; i++) licznik++;
>     return NULL;
> }
>
> int main(void) {
>     pthread_t t1, t2;
>     pthread_create(&t1, NULL, worker, NULL);
>     pthread_create(&t2, NULL, worker, NULL);
>     pthread_join(t1, NULL);
>     pthread_join(t2, NULL);
>     printf("%d\n", licznik);   // "oczekiwane": 200000
> }
> ```
> Gdyby wątki wykonywały się idealnie szeregowo, byłoby `200000`. W rzeczywistości niektóre `++` się nakładają i wynik to **liczba losowa niższa niż 200000** (np. `123456`). To klasyczny data race: dwa wątki, wspólna `licznik`, zapis bez synchronizacji.

> [!przyklad]- Przykład 2 — Java: int vs AtomicInteger
> ```java
> class Counter {
>     int value = 0;
>     void inc() { value++; }          // data race (bez synchronized / AtomicInteger)
> }
> ```
> ```java
> class Counter {
>     AtomicInteger value = new AtomicInteger(0);
>     void inc() { value.incrementAndGet(); }  // atomowa operacja — bezpieczne
> }
> ```
> Z `int` dostajesz klasyczny wyścig; z `AtomicInteger` operacja jest niepodzielna i każdy wątek widzi spójną wartość — poprawność bez ręcznego locka.

> [!przyklad]- Przykład 3 — OpenMP: redukcja bez wyścigu
> ```c
> long sum = 0;
> #pragma omp parallel for reduction(+:sum)
> for (long i = 0; i < N; i++) sum += i;
> ```
> Bez `reduction` każdy wątek pisałby do **tej samej** `sum` → data race. Z `reduction(+:sum)` każdy wątek ma **lokalną** kopię, a na końcu wyniki się łączą. Szybciej i bezpieczniej niż ręczny mutex.

## Typowe błędy
> [!blad] „Przecież `i++` jest proste — musi działać”
> `i++` to **trzy operacje maszynowe** (czytaj, zwiększ, zapisz), a nie jedna — między nie wpełza drugi wątek. Prosty kod nie znaczy atomowy.

> [!blad] Mylenie data race z race condition
> Data race = dwa „gołe” dostępy do tej samej pamięci bez happens-before. Race condition = szerszy błąd logiczny zależny od chronologii; może istnieć nawet, gdy dane są zsynchronizowane. Egzamin patrzy na to rozróżnienie.

> [!blad] Synchronizowanie wszystkiego mutexem „na wszelki wypadek”
> Mutex na każdej zmiennej to koszt na każde wejście + ryzyko [[Zakleszczenie|zakleszczenia]] (błędna kolejność locków). Atomik, redukcja w OpenMP albo jawny `join` wystarczają i są szybsze.

> [!blad] „`volatile` mnie uchroni”
> `volatile` tylko mówi kompilatorowi „nie optymalizuj tego dostępu” — **nie** daje atomowości ani spójności między wątkami. To dają dopiero `std::atomic` (z odpowiednim `memory_order`) czy `synchronized`/`Atomic*` w Javie. `volatile` do synchronizacji to fałszywe bezpieczeństwo.

## Gdzie ci się przyda
> [!przyda]
> - [[AGH Programowanie równoległe]] — temat wykładu „bezpieczeństwo i żywotność (wyścigi, zakleszczenia)”, **pewniak na egzaminie**.
> - W labach: [[Pthreads]] (licznik, sekcje krytyczne), [[Współbieżność w Javie]] (`synchronized`, `AtomicInteger`), [[OpenMP]] (`critical`, `reduction`, `atomic`), a w [[MPI]] — przy dzieleniu wspólnych danych między procesami.
> - Łączy się z [[Prawo Amdahla|prawem Amdahla]]: poprawne, ale wolne przez locki sekcje potrafią zjeść przyspieszenie — stąd redukcje i atomiki zamiast grubego mutexu.

## Zadania
> [!zadanie] Zadanie 1 — rozpoznaj: data race czy nie?
> Dwa wątki robią `tab[i] = f(a);`, gdzie `f` nie ma skutków ubocznych, każde `i` jest inne (wątek dostaje swój indeks). Czy to data race?
> > [!odpowiedz]- Odpowiedź
> > **Nie.** Dostępy dotyczą **różnych lokacji** (różne `tab[i]`). Konflikt byłby tylko, gdyby dwa wątki sięgały **tej samej komórki** z zapisem. Różne indeksy → różne bajty → brak wyścigu.

> [!zadanie] Zadanie 2 — przewidź wynik
> `int x = 0;` Wątek A robi `x = 1;`, wątek B robi `printf("%d", x);`. Czy B może wydrukować 0? Co to jest?
> > [!odpowiedz]- Odpowiedź
> > **Tak, może wydrukować 0** — B może przeczytać, zanim A zapisze. To **data race** (czytanie i pisanie bez happens-before; w C11 undefined behavior). Ani 0, ani 1 nie jest wynikiem gwarantowanym — wynik jest nieokreślony.

> [!zadanie] Zadanie 3 — napisz poprawnie (C, Pthreads)
> Masz `long licznik` wspólny dla 4 wątków, każdy robi `licznik++` 100000 razy. Popraw najmniejszym kosztem wydajności i wyjaśnij, dlaczego.
> > [!odpowiedz]- Odpowiedź
> > Użyj **operacji atomowej**: `atomic_long licznik = 0;` + `atomic_fetch_add(&licznik, 1);` (nagłówek `<stdatomic.h>`). To daje niepodzielny `++` bez blokowania całego bloku — minimalny koszt, zero wyścigu. (Dla większego bloku stanu użyłbyś mutexu; w OpenMP wystarczy `reduction(+:licznik)`).

> [!zadanie] Zadanie 4 — data race vs race condition
> Oba wątki robią na wspólnym `saldo`: `if (saldo >= 100) wypłac(50);`, bez synchronizacji. Czy to data race? Co może pójść nie tak?
> > [!odpowiedz]- Odpowiedź
> > `saldo` to wspólna zmienna z czytaniem (`if`) i zapisem (`wypłac`) — **tak, to data race**. Ale nawet po `synchronized` logika by się wywalała (**race condition**): oba wątki przechodzą `if`, zanim któryś zmieni `saldo`, i oba „wypłacają” 50, choć środków starczało na jedną wypłatę. To pokazuje, że mutex nie naprawia złej logiki.

> [!zadanie] Zadanie 5 — ThreadSanitizer
> Jakim poleceniem wykryjesz data race w `prog.c` i co zobaczysz w wyjściu?
> > [!odpowiedz]- Odpowiedź
> > `gcc -fsanitize=thread -O1 -g prog.c -o prog -lpthread`, potem `./prog`. Przy wyścigu TSan wypisze `WARNING: ThreadSanitizer: data race` ze stosami obu wątków i wskazaniem lokacji pamięci. (Alternatywnie: `valgrind --tool=helgrind ./prog`).

## Fiszki #fiszki/równoległość
Definicja wyścigu danych (data race)::≥ 2 wątki, ta sama komórka pamięci, co najmniej jeden zapis, brak synchronizacji (happens-before)
Czy czytanie zmiennej, gdy inny wątek ją pisze, to już data race?::Tak — czytaj vs pisz na tej samej lokacji bez synchronizacji = data race
Data race vs race condition::Data race to niezsynchronizowany dostęp do pamięci (w C: UB); race condition to błąd logiki zależny od kolejności, może wystąpić bez data race
Dlaczego `i++` nie jest atomowe?::Bo to 3 operacje maszynowe (czytaj, zwiększ rejestr, zapisz) — między nie może wpełznąć inny wątek
Czy `volatile` zabezpiecza przed data race?::Nie — volatile to tylko „nie optymalizuj dostępu”; atomowość daje dopiero atomic_int z <stdatomic.h> albo synchronized w Javie
Najlepszy sposób na `count++` na jednej zmiennej::Operacja atomowa (atomic_int w C11, AtomicInteger w Javie), bo mutex na jednej zmiennej to nadmiar i koszt
Co robi `reduction(+:sum)` w OpenMP::Każdy wątek ma lokalną kopię sumy (zero wyścigu), na końcu wyniki się sumują — szybciej i bezpieczniej niż mutex
Jak zadeklarować atomowy licznik w C11::atomic_long licznik = 0; potem atomic_fetch_add(&licznik, 1)
Jak wykryć data race narzędziem::gcc -fsanitize=thread (ThreadSanitizer: WARNING: data race) albo valgrind --tool=helgrind
Kiedy atomik nie wystarczy i trzeba mutexu?::Gdy stan musi być spójny przez kilka kroków (np. kolejka, saldo z if) — na takim bloku atomik to za mało

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.

## Źródła
- I. Foster, *Designing and Building Parallel Programs* (online: mcs.anl.gov/~itf/dbpp) — literatura wykładu.
- cppreference.com: *Memory model* oraz *Threads and data races* (definicja wyścigu w C11).
- `man 7 pthreads`, dokumentacja ThreadSanitizer (`-fsanitize=thread`).
