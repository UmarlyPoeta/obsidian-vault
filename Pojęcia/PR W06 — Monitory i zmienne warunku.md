---
typ: pojęcie
status: nowe
przedmioty: ["[[AGH Programowanie równoległe]]"]
wymaga: ["[[PR W03 — Współbieżność]]", "[[PR W05 — Wątki w Javie]]"]
powiązane: ["[[PR L04 — Muteksy i symulacja pubu]]", "[[Mutex i zmienna warunkowa]]", "[[Zakleszczenie]]", "[[Współbieżność w Javie]]"]
tags: [pojęcie, równoległość, wykład]
utworzono: 2026-10-02
źródła: ["K. Banaś — PR_W06_Monitory.pdf (wykład 2026/27)"]
---
# PR W06 — Monitory i zmienne warunku

> [!skrot] W 30 sekund
> - **Co to:** jak czekać na warunek **bez aktywnego czekania** — **zmienne warunku** Pthreads, **monitory** (sekcja krytyczna na poziomie obiektu), obiekt Javy jako monitor (`wait`/`notify`/`notifyAll`), **zamki odczytu/zapisu**.
> - **Klucz:** `lock(m); while(!warunek) pthread_cond_wait(&c, &m); …; unlock(m); pthread_cond_signal(&c2);` — czekanie zawsze **w pętli while**.
> - **Kiedy:** kartkówka przed L08 (20.11: czytelnicy–pisarze, bariera, producent–konsument). Rozwiązuje wadę aktywnego czekania z [[PR L04 — Muteksy i symulacja pubu]].

> [!intuicja] Po ludzku
> Zamiast co chwilę zaglądać do lodówki (busy wait), kładziesz się spać z kartką „obudź mnie, jak będzie mleko” (zmienna warunku). Kto przyniesie mleko, budzi jednego (`signal`) albo wszystkich (`broadcast`). Po obudzeniu i tak sprawdzasz lodówkę jeszcze raz — ktoś mógł być szybszy.

## Definicje i pojęcia

> [!definicja] Producenci i konsumenci (slajd 2)
> Jedna grupa wątków **produkuje** (powiększa zasób), druga **konsumuje** (pomniejsza). Jak zagwarantować przebieg **bez zakleszczeń i zagłodzeń**? Dostęp do zasobu ma tylko jeden wątek, a dodatkowo **istnieją warunki związane z zasobem** (konsument nie konsumuje z pustego).

> [!definicja] Czytelnicy i pisarze (slajd 2)
> Pisarze **modyfikują** zasób, czytelnicy **tylko odczytują**. **Brak dodatkowych warunków** związanych ze stanem zasobu, ale **pisać może jednocześnie tylko jeden** wątek, a **czytać wiele naraz**.

> [!definicja] Zmienne warunku (slajd 3)
> **Zmienne wspólne dla wątków, służące do identyfikacji grupy uśpionych wątków.** `pthread_cond_wait` usypia wątek w miejscu identyfikowanym przez `cond`, do sygnału budzenia; `pthread_cond_signal` budzi **pierwszy w kolejności** wątek czekający na `cond`; `pthread_cond_broadcast` budzi **wszystkie**.

> [!definicja] Monitor (slajdy 7–9)
> W ujęciu klasycznym moduł ze **stanem** (atrybuty) i **zachowaniem** (metody **publiczne** i **prywatne**). Zasada: **sekcja krytyczna na poziomie obiektu** — jeśli wątek rozpoczął dowolną publiczną metodę, żaden inny nie rozpocznie tej samej ani innej publicznej metody; **wewnątrz monitora może być tylko jeden wątek**. Wykluczaniem zarządza **środowisko**: wątek jest wpuszczany albo ustawiany w **kolejce** (jedna kolejka dla całego monitora); po wyjściu wątku wznawiany jest **pierwszy z kolejki**. Chcesz dostęp wyłączny do zasobu — uczyń go **zasobem wewnętrznym monitora**.
> Zmienne warunku (typ `condition`) to wewnętrzne zmienne monitora z operacjami: **`wait(c)`** — uśpienie „na zmiennej c”, **`empty(c)`** — czy ktoś czeka na c, **`signal(c)`** — obudzenie jednego z czekających lub otwarcie wejścia.

> [!definicja] Obiekt Javy jako monitor (slajdy 13–15)
> Każdy obiekt ma `wait()` (**bez argumentu** — jedyną identyfikacją miejsca oczekiwania jest obiekt), `notify()` (jak `signal`), `notifyAll()` (jak `broadcast`), zdefiniowane w **`Object`**. Ponieważ `wait` nie ma identyfikatora miejsca oczekiwania, **należy umieszczać je w pętli `while` sprawdzającej warunek** i budzić **wszystkich** (`notifyAll`). „Standardowe” zmienne warunku (`Condition`) są w `java.util.concurrent`.

> [!definicja] Zamki odczytu/zapisu (slajd 16)
> Gotowe API do problemu czytelników i pisarzy (POSIX 2001): `pthread_rwlock_init`, `_destroy`, `_rdlock` (zamknięcie do odczytu; jest `_tryrdlock`), `_wrlock` (do zapisu; `_trywrlock`), `_unlock`.

## API z wykładu (slajdy 3–6)
```c
int pthread_cond_init(pthread_cond_t *cond, pthread_condattr_t *cond_attr);
int pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex);
int pthread_cond_signal(pthread_cond_t *cond);
int pthread_cond_broadcast(pthread_cond_t *cond);

// producent (slajd 5); konsument symetrycznie z nie_pusty / nie_pelny
pthread_mutex_lock(&muteks);
while( zasob_pelny(fifo) ) pthread_cond_wait(&nie_pelny, &muteks);
zasob_wstaw(fifo, ...);
pthread_mutex_unlock(&muteks);
pthread_cond_signal(&nie_pusty);
```
`pthread_cond_wait` dostaje **muteks**: czekający wątek go zwalnia na czas snu, a po obudzeniu wraca z muteksem zamkniętym (tego slajd nie mówi wprost — wg `man pthread_cond_wait`) — dlatego warunek sprawdza się pod muteksem i **w pętli**, jak na slajdach 5–6 i 15.

## Czytelnia — monitor czytelników i pisarzy (slajdy 10–12)
Procedury `chcę_pisać / koniec_pisania / chcę_czytać / koniec_czytania` w monitorze `Czytelnia` z licznikami `liczba_czyt`, `liczba_pisz` i zmiennymi warunku `czytelnicy`, `pisarze`:
- **chcę_pisać:** jeśli `liczba_czyt + liczba_pisz > 0` → `wait(pisarze)`; potem `liczba_pisz++`.
- **koniec_pisania:** `liczba_pisz--`; jeśli czekają czytelnicy → `signal(czytelnicy)`, wpp `signal(pisarze)` — **uczciwe względem czytelników**.
- **chcę_czytać:** jeśli `liczba_pisz > 0` **lub czekają pisarze** → `wait(czytelnicy)`; `liczba_czyt++`; `signal(czytelnicy)` — **czytelnicy budzą się kolejno nawzajem**; uczciwe względem pisarzy.
- **koniec_czytania:** `liczba_czyt--`; jeśli `liczba_czyt == 0` → `signal(pisarze)` — budzi tylko pisarzy.

## Na zajęcia
> [!zajecia]
> - **L08 (20.11):** czytelnicy–pisarze (`CzytPis_Pthreads.tgz`), bariera (`bariera.tgz`), producent–konsument (`ProdKons.tgz`).
> - Pub z [[PR L04 — Muteksy i symulacja pubu]] da się przepisać tak, by klient **spał** na zmiennej warunku zamiast aktywnie czekać.
> - Łączy się z: [[PR W03 — Współbieżność]], [[PR W05 — Wątki w Javie]], [[Mutex i zmienna warunkowa]].

## Na kartkówkę
> [!zadanie] 1. Czym różni się problem producentów i konsumentów od czytelników i pisarzy?
> > [!odpowiedz]- Odpowiedź
> > Producenci–konsumenci: dodatkowe warunki zależne od stanu zasobu (nie konsumuj z pustego). Czytelnicy–pisarze: brak takich warunków, ale pisze jeden, czytać może wielu naraz.

> [!zadanie] 2. Czym jest zmienna warunku?
> > [!odpowiedz]- Odpowiedź
> > Zmienną wspólną wątków służącą do identyfikacji grupy uśpionych wątków.

> [!zadanie] 3. Różnica `pthread_cond_signal` i `pthread_cond_broadcast`?
> > [!odpowiedz]- Odpowiedź
> > Signal budzi pierwszy czekający wątek, broadcast — wszystkie.

> [!zadanie] 4. Dlaczego `pthread_cond_wait` przyjmuje muteks?
> > [!odpowiedz]- Odpowiedź
> > Zwalnia go na czas snu (inni mogą zmienić stan) i po obudzeniu zamyka ponownie, więc warunek sprawdza się pod ochroną muteksu.

> [!zadanie] 5. Dlaczego warunek sprawdza się w `while`, a nie `if`?
> > [!odpowiedz]- Odpowiedź
> > Po obudzeniu warunek mógł znów przestać być spełniony (inny wątek był szybszy, budzimy wielu); w Javie `wait` dodatkowo nie identyfikuje miejsca oczekiwania.

> [!zadanie] 6. Podaj podstawową zasadę działania monitora.
> > [!odpowiedz]- Odpowiedź
> > Sekcja krytyczna na poziomie obiektu — wewnątrz monitora (w dowolnej publicznej metodzie) może być tylko jeden wątek.

> [!zadanie] 7. Ile kolejek ma monitor dla wejścia i kogo wznawia po wyjściu wątku?
> > [!odpowiedz]- Odpowiedź
> > Jedną, dla wszystkich metod; wznawia pierwszy wątek z kolejki.

> [!zadanie] 8. Wymień operacje na zmiennych warunku monitora.
> > [!odpowiedz]- Odpowiedź
> > `wait(c)`, `empty(c)`, `signal(c)`.

> [!zadanie] 9. Jakie są odpowiedniki signal i broadcast w Javie i gdzie są zdefiniowane?
> > [!odpowiedz]- Odpowiedź
> > `notify()` i `notifyAll()`, w klasie `Object`.

> [!zadanie] 10. Dlaczego w `koniec_pisania` budzi się najpierw czytelników?
> > [!odpowiedz]- Odpowiedź
> > Uczciwość względem czytelników — nie dopuszcza do ich zagłodzenia przez kolejnych pisarzy.

> [!zadanie] 11. Jakie API POSIX rozwiązuje problem czytelników i pisarzy?
> > [!odpowiedz]- Odpowiedź
> > Zamki odczytu/zapisu: `pthread_rwlock_rdlock`, `_wrlock`, `_unlock` (+ wersje try, init, destroy).

## Na egzamin
- Omów problem producentów i konsumentów i jego rozwiązanie z muteksem i dwiema zmiennymi warunku.
- Zdefiniuj monitor, opisz wpuszczanie wątków i rolę zmiennych warunku; porównaj z obiektem Javy (`synchronized`, `wait`, `notifyAll`).
- Przeanalizuj monitor `Czytelnia`: bezpieczeństwo, uczciwość wobec czytelników i pisarzy, brak zagłodzenia.
- Porównaj aktywne czekanie (muteks + pętla) z czekaniem na zmiennej warunku.

## Fiszki #fiszki/równoległość
Zmienna warunku — definicja::Zmienna wspólna wątków służąca do identyfikacji grupy uśpionych wątków
pthread_cond_signal::Budzi pierwszy w kolejności wątek oczekujący na danej zmiennej warunku
pthread_cond_broadcast::Budzi wszystkie wątki oczekujące na danej zmiennej warunku
Sygnatura pthread_cond_wait::int pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex)
Co dzieje się z muteksem w pthread_cond_wait?::Zwalniany na czas snu, ponownie zamknięty po obudzeniu
Czemu cond_wait w pętli while?::Po obudzeniu warunek mógł znów nie być spełniony
Problem producentów i konsumentów — istota::Wyłączny dostęp do zasobu plus warunki zależne od jego stanu (pusty/pełny)
Problem czytelników i pisarzy — istota::Pisze jeden naraz, czytać może wielu; brak warunków zależnych od stanu
Monitor — zasada::Sekcja krytyczna na poziomie obiektu: w monitorze tylko jeden wątek
Operacje zmiennej warunku monitora::wait(c), empty(c), signal(c)
Ile kolejek wejścia ma monitor?::Jedną, dla wszystkich jego metod
Odpowiedniki signal/broadcast w Javie::notify() i notifyAll() z klasy Object
Dlaczego w Javie zaleca się notifyAll?::wait nie ma identyfikatora miejsca oczekiwania — budzimy wszystkich, a oni sprawdzają warunek w while
Zamki odczytu/zapisu POSIX::pthread_rwlock_rdlock, pthread_rwlock_wrlock, pthread_rwlock_unlock

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.

## Źródła
- K. Banaś, *Monitory i zmienne warunku* — `~/studia/sem5/programowanie_rownolegle/materialy/wyklady/PR_W06_Monitory.pdf` (16 slajdów; tekst: `materialy/tekst/PR_W06_Monitory.md`).
- Nagranie W06.mp4: https://cloud.wimiip.agh.edu.pl/s/ZsxaF5xL6YiiCPW (ze strony https://home.agh.edu.pl/~kbanas/PR/PR.html).
