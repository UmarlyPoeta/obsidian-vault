---
typ: pojęcie
status: nowe
przedmioty: ["[[AGH Programowanie równoległe]]"]
wymaga: ["[[PR W03 — Współbieżność]]"]
powiązane: ["[[PR W06 — Monitory i zmienne warunku]]", "[[PR W04 — Tworzenie programów równoległych]]", "[[Współbieżność w Javie]]", "[[Wyścig danych]]"]
tags: [pojęcie, równoległość, wykład]
utworzono: 2026-10-02
źródła: ["K. Banaś — PR_W05_Obiekty_Java.pdf (wykład 2026/27)"]
---
# PR W05 — Wątki w Javie

> [!skrot] W 30 sekund
> - **Co to:** współbieżność w środowiskach obiektowych — **bezpieczeństwo wątkowe** klas, wątki Javy (`Thread`, `Runnable`), metody **synchronizowane** i **zamki monitorowe**, zmienne atomowe, **pule wątków** (`ExecutorService`, `Callable`, `Future`), **work stealing** i `ForkJoinPool`.
> - **Klucz:** z każdym obiektem związany jest **zamek monitorowy**; `synchronized` zajmuje go na czas metody/bloku. Zadania w wykonawcach mają **mniejszy narzut** niż nowe obiekty `Thread`.
> - **Kiedy:** kartkówka przed L06 (6.11, histogram — synchronizacja) i L07 (13.11, pule wątków).

> [!intuicja] Po ludzku
> Każdy obiekt w Javie ma wbudowany „klucz do pokoju”. Metoda `synchronized` to wejście do pokoju z kluczem — inni czekają w kolejce. Pula wątków to stała ekipa pracowników, której podaje się kolejne zlecenia, zamiast zatrudniać człowieka do każdego drobiazgu.

## Definicje i pojęcia

> [!definicja] Model współbieżności Javy (slajd 2)
> Opiera się na: **zarządzaniu wątkami**; **operacjach atomowych** (operacje na referencjach, zmiennych typów wbudowanych, zmiennych `volatile`, specjalne jak `compareAndSet`, `getAndIncrement`); **mechanizmach synchronizacji** (zamki, zmienne warunku); **klasach kontenerowych** do współbieżnego użycia (`BlockingQueue`, `ConcurrentMap`).

> [!definicja] Bezpieczeństwo wątkowe (slajd 3)
> Atrybuty obiektów niosą **te same ryzyka co zmienne globalne** w programach proceduralnych, a metody mogą być wywołane z dowolnych wątków. Wymaganie dla każdej klasy używanej przez inne klasy: **„bezpieczeństwo wątkowe” (thread safety)** — wymaga się go od wszystkich ogólnie dostępnych bibliotek. Rozwiązanie pośrednie, nie zawsze możliwe: **klasy niezmienne (immutable)**.

> [!definicja] Wątki w Javie (slajdy 4–5)
> Wątek jest **obiektem klasy `Thread`** (`java.lang`). Wielowątkowość jest wbudowana w podstawy: klasa **`Object`** ma metody `wait`, `notify`, `notifyAll`. Dwa sposoby uruchomienia:
> 1. przekazać obiekt implementujący **`Runnable`** (metoda `run`) do konstruktora `Thread` i wywołać `start` — **klasa może dziedziczyć po innych klasach**,
> 2. klasa pochodna **`Thread`** przeciążająca `run`, wywołanie `start` — **klasa nie może dziedziczyć po innych klasach**.

> [!definicja] Metody synchronizowane (slajd 10)
> Realizacja synchronizowanej metody przez wątek **uniemożliwia realizację dowolnej synchronizowanej metody tego obiektu** przez inne wątki (te czekają w **kolejce**); **ten sam wątek może wywoływać kolejne** metody synchronizowane; po zakończeniu wszystkich JVM wznawia **pierwszy wątek z kolejki**. **Konstruktory nie mogą być synchronizowane.**

> [!definicja] Zamki monitorowe (slajd 11)
> Synchronizacja opiera się na **wewnętrznych zamkach monitorowych (monitor locks)** — „monitorach”. **Z każdym obiektem związany jest zamek monitorowy**: zajmuje go wątek wchodzący do synchronizowanej metody lub **bloku synchronizowanego**; synchronizowana **metoda statyczna** zajmuje zamek obiektu reprezentującego **klasę**.

> [!definicja] Blok synchronizowany i Lock (slajdy 13–14)
> `synchronized(ref){…}` — zajmuje zamek obiektu `ref` (najczęściej `this`), synchronizując tylko część kodu. Osobne obiekty `lock1`, `lock2` służące tylko do synchronizacji pełnią rolę **zbliżoną do muteksów**; brakuje im jednak **trylock**, dlatego `java.util.concurrent` daje interfejs **`Lock`** i implementacje, np. **`ReentrantLock`**.

> [!definicja] Pule wątków i wykonawcy (slajdy 16–19)
> Dla **masowej równoległości**: **pule wątków (thread pool)** i **interfejsy wykonawców (executor)**. Pula (`ThreadPoolExecutor`) zarządza zadaniami **`Runnable`** lub **`Callable`** (uruchamianie, zatrzymywanie, stan). **Uruchamianie zadań w wykonawcach ma mniejszy narzut niż nowych wątków `Thread`.** `Runnable` nie zwraca wyniku, **`Callable`** zwraca; wyniki i stan przekazują obiekty **`Future`** (parametryzowane typem wyniku: sprawdzenie zakończenia, oczekiwanie i pobranie wyniku `get`, anulowanie).

> [!definicja] Work stealing (slajd 22)
> „Kradzież pracy” — decyzja o przydziale zadania podejmowana jest **tylko, gdy jakiś wątek nie ma nic do zrobienia**. Realizuje ją **`ForkJoinPool`** — wzorzec **fork‑join (recursive splitting)**; zadania `RecursiveAction` (bez wyniku) i `RecursiveTask<T>` (z wynikiem), metoda `compute`, w niej `fork()` i `join()`.

## API z wykładu
```java
void start();  void run();  static void sleep(long millis);
void join();  void join(long millis);  void interrupt();
static boolean interrupted();  boolean isAlive();  boolean isInterrupted();   // slajdy 7, 9

public synchronized void inc1(){ c1++; }               // slajd 12
synchronized(lock1){ c1++; }                           // slajd 14
AtomicInteger c1 = new AtomicInteger(0); c1.incrementAndGet();   // slajd 15

ExecutorService ex = Executors.newFixedThreadPool(NTHREADS);   // slajd 18
ex.execute(runnable);  ex.shutdown();  while(!ex.isTerminated()){}
Future<String> f = ex.submit(callable);  f.get();      // slajd 21
new ForkJoinPool().invoke(task) / execute(task) / submit(task);   // slajd 25
```
`interrupt()` **ustawia sygnalizator przerwania**; przerwanie następuje, gdy wątek jest w metodzie przerywalnej (`sleep`, `join`, `wait`) albo sam sprawdzi `Thread.interrupted()` i zareaguje (np. `InterruptedException`). `shutdown()` — wykonawca **przestaje przyjmować nowe** zadania.

> [!blad] Uwaga na slajd 9
> W przykładzie jest `t.interrupted()` — to metoda **statyczna** sprawdzająca **bieżący** wątek. Do przerwania wątku `t` służy `t.interrupt()` (slajd 7).

## Na zajęcia
> [!zajecia]
> - **L06 (6.11):** synchronizacja w Javie (histogram, `Histogram_test.java`, `Obraz.java`) — `synchronized`, bloki, zamki.
> - **L07 (13.11):** `Executor`, `Callable`, `Future` (`Calka_callable.java`), ForkJoin.
> - Łączy się z: [[PR W03 — Współbieżność]], [[PR W06 — Monitory i zmienne warunku]], [[Współbieżność w Javie]].

## Na kartkówkę
> [!zadanie] 1. Co to jest bezpieczeństwo wątkowe klasy?
> > [!odpowiedz]- Odpowiedź
> > Obiekty klasy mogą być poprawnie używane przez wiele współbieżnych wątków; wymagane od ogólnie dostępnych bibliotek. Alternatywa: klasy niezmienne.

> [!zadanie] 2. Dwa sposoby uruchomienia wątku w Javie i ich różnica?
> > [!odpowiedz]- Odpowiedź
> > `Runnable` przekazany do `Thread` + `start` (klasa może dziedziczyć po innych) albo klasa pochodna `Thread` z `run` (nie może dziedziczyć po innych).

> [!zadanie] 3. Czym różni się `start()` od `run()`?
> > [!odpowiedz]- Odpowiedź
> > `start` to polecenie dla JVM uruchomienia metody `run` (slajd 7) — w nowym wątku; samo wywołanie `run()` to zwykłe wywołanie metody w bieżącym wątku (dopowiedzenie, nie ze slajdu).

> [!zadanie] 4. Co gwarantuje metoda `synchronized`?
> > [!odpowiedz]- Odpowiedź
> > Gdy jeden wątek ją wykonuje, inne nie wykonają żadnej synchronizowanej metody tego obiektu (czekają w kolejce); ten sam wątek może wchodzić ponownie.

> [!zadanie] 5. Który zamek zajmuje synchronizowana metoda statyczna?
> > [!odpowiedz]- Odpowiedź
> > Zamek obiektu reprezentującego klasę.

> [!zadanie] 6. Czy konstruktor może być `synchronized`?
> > [!odpowiedz]- Odpowiedź
> > Nie.

> [!zadanie] 7. Czego brakuje blokom `synchronized` w porównaniu z muteksem i co to rozwiązuje?
> > [!odpowiedz]- Odpowiedź
> > Brak trylock — rozwiązuje to interfejs `Lock` (np. `ReentrantLock`) z `java.util.concurrent`.

> [!zadanie] 8. Jak działa `interrupt()`?
> > [!odpowiedz]- Odpowiedź
> > Ustawia sygnalizator przerwania; wątek zostaje przerwany w metodzie przerywalnej (sleep, join, wait) albo sam sprawdza `interrupted()` i reaguje.

> [!zadanie] 9. Czym różni się `Runnable` od `Callable`? Do czego `Future`?
> > [!odpowiedz]- Odpowiedź
> > `Callable` zwraca wynik (`call`), `Runnable` nie. `Future` pozwala sprawdzić zakończenie, poczekać i pobrać wynik (`get`), anulować zadanie.

> [!zadanie] 10. Dlaczego używa się pul wątków?
> > [!odpowiedz]- Odpowiedź
> > Uruchamianie zadań w wykonawcach ma mniejszy narzut niż tworzenie nowych wątków — dla masowej równoległości.

> [!zadanie] 11. Na czym polega work stealing i gdzie jest w Javie?
> > [!odpowiedz]- Odpowiedź
> > Przydział zadania tylko wtedy, gdy jakiś wątek nie ma pracy; `ForkJoinPool` (fork‑join).

## Na egzamin
- Omów model współbieżności Javy: wątki, operacje atomowe, synchronizacja, kontenery współbieżne.
- Wyjaśnij zamki monitorowe: metody i bloki synchronizowane, metody statyczne, wielokrotne wejście, porównanie z muteksami Pthreads.
- Porównaj tworzenie wątków `Thread` z pulą wątków; `Runnable`/`Callable`/`Future`; fork‑join i work stealing.

## Fiszki #fiszki/równoległość
Thread safety::Bezpieczeństwo wątkowe — poprawne działanie obiektów klasy przy użyciu przez wiele współbieżnych wątków
Rozwiązanie pośrednie zamiast synchronizacji klasy::Klasy niezmienne (immutable)
Które metody Object służą współbieżności?::wait, notify, notifyAll
Runnable vs dziedziczenie po Thread::Runnable pozwala dziedziczyć po innej klasie; podklasa Thread już nie
Metoda synchronized — zasada::Inne wątki nie wykonają żadnej synchronizowanej metody tego obiektu; ten sam wątek może wejść ponownie
Z czym związany jest zamek monitorowy?::Z każdym obiektem; dla metody statycznej — z obiektem reprezentującym klasę
Czy konstruktor może być synchronized?::Nie
Interfejs Javy z trylock::Lock (np. ReentrantLock) z java.util.concurrent
Klasa z atomowym incrementAndGet::AtomicInteger
Co robi interrupt()?::Ustawia sygnalizator przerwania wątku
Runnable a Callable::Callable zwraca wynik przez call, Runnable nie zwraca
Do czego służy Future?::Sprawdzanie zakończenia, oczekiwanie i pobranie wyniku (get), anulowanie zadania
Po co pula wątków?::Mniejszy narzut uruchamiania zadań niż tworzenie nowych wątków
Work stealing::Przydział zadania tylko, gdy wątek nie ma nic do zrobienia; ForkJoinPool
executor.shutdown()::Wykonawca przestaje przyjmować nowe zadania

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.

## Źródła
- K. Banaś, *Współbieżność i równoległość w środowiskach obiektowych* — `~/studia/sem5/programowanie_rownolegle/materialy/wyklady/PR_W05_Obiekty_Java.pdf` (25 slajdów; tekst: `materialy/tekst/PR_W05_Obiekty_Java.md`; slajd 17 to rysunek).
- Nagranie W05.mp4: https://cloud.wimiip.agh.edu.pl/s/RCCicrjxzjFioRb (ze strony https://home.agh.edu.pl/~kbanas/PR/PR.html).
