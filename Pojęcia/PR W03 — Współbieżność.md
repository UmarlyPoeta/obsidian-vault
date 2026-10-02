---
typ: pojęcie
status: nowe
przedmioty: ["[[AGH Programowanie równoległe]]"]
wymaga: ["[[PR W02 — Wątki i model SPMD]]", "[[Wyścig danych]]"]
powiązane: ["[[PR L04 — Muteksy i symulacja pubu]]", "[[PR L05 — Całka w Pthreads]]", "[[PR W06 — Monitory i zmienne warunku]]", "[[Wyścig danych]]", "[[Zakleszczenie]]", "[[Mutex i zmienna warunkowa]]"]
tags: [pojęcie, równoległość, wykład]
utworzono: 2026-10-02
źródła: ["K. Banaś — PR_W03_Wspolbieznosc.pdf (wykład 2026/27)"]
---
# PR W03 — Współbieżność

> [!skrot] W 30 sekund
> - **Co to:** problemy przetwarzania współbieżnego i ich rozwiązania: wyścig, zależności, synchronizacja, **wzajemne wykluczanie** (sekcja krytyczna, zamki, **muteksy** POSIX, semafory), model pamięci, klasyczne problemy (filozofowie).
> - **Klucz:** `pthread_mutex_lock(&m); …sekcja krytyczna…; pthread_mutex_unlock(&m);` — a `pthread_mutex_trylock` wraca **natychmiast** (EBUSY), dając wzorzec **aktywnego czekania**.
> - **Kiedy:** kartkówka przed [[PR L04 — Muteksy i symulacja pubu]] (23.10); potrzebne też w [[PR L05 — Całka w Pthreads]].

> [!intuicja] Po ludzku
> Sekcja krytyczna to łazienka na jedną osobę. Goła zmienna `zamek` to kartka „zajęte”, którą dwie osoby mogą odwrócić naraz. Muteks to prawdziwy zamek: `lock` czeka pod drzwiami, `trylock` szarpie klamką i od razu odchodzi.

## Definicje i pojęcia

> [!definicja] Problemy współbieżności (slajd 2)
> - **wyścig** (*race condition*) — **brak determinizmu wykonania**,
> - **zależności danych** (*data dependence, data race*),
> - **synchronizacja** — sprzętowa (SIMD, komputery macierzowe) lub programowa (**bariera, sekcja krytyczna, operacje atomowe**),
> - **wzajemne wykluczanie** — sekcja krytyczna, **protokoły wejścia i wyjścia** z sekcji krytycznej,
> - cechy rozwiązań: pożądane — **bezpieczeństwo i żywotność, uczciwość**; możliwe błędy — **zakleszczenie, zagłodzenie**.

> [!definicja] Zależności (slajd 3)
> Wzajemne uzależnienie instrukcji **nakładające ograniczenia na kolejność ich realizacji**:
> - **zasobów** — wiele instrukcji korzysta ze wspólnego zasobu (np. pliku),
> - **sterowania** — wykonanie instrukcji zależy od wyniku poprzedzających instrukcji warunkowych,
> - **danych (przepływu)** — instrukcje w bezpośrednim sąsiedztwie czasowym operują na tych samych danych i **choć jedna dokonuje zapisu**.

> [!definicja] Zamek z aktywnym czekaniem (slajdy 4–5)
> `while(zamek != 0){}  zamek = 1;  sekcja_krytyczna();  zamek = 0;` — **aktywne czekanie (busy wait)**. Problemy: **zużywa zasoby komputera**, a procedura **nie jest bezpieczna ani nie zapewnia żywotności** (dwa wątki mogą jednocześnie zobaczyć 0 i wejść). Rozwiązania (slajd 6): bardziej rozbudowane algorytmy, **wsparcie sprzętowe** (rozkazy procesora), **wsparcie systemowe**, odpowiednie **API**.

> [!definicja] Model pamięci (slajdy 7–9)
> Kiedy kompilator może trzymać zmienną w rejestrze, a kiedy musi zapisać do pamięci; spójność kopii w **pamięci podręcznej**; czy wolno **zmieniać kolejność** operacji. Sekwencyjnie to problem **wydajności**; równolegle — **poprawności**: bez wymuszenia zapisu różne wątki widzą **różne wartości tej samej zmiennej**. Wymuszenie: **memory fences, flush**, niejawnie — bariery i protokoły sekcji krytycznych. Modele spójności: **sekwencyjnej** i **osłabionej** (*relaxed*); paradoks: współczesne procesory **nie spełniają** spójności sekwencyjnej.

> [!definicja] Typy muteksów i wskazówki (slajd 13)
> **Jednorazowy** (`PTHREAD_MUTEX_NORMAL`, `PTHREAD_MUTEX_DEFAULT`) i **wielokrotnego wejścia** (*reentrant*, `PTHREAD_MUTEX_RECURSIVE` — licznik zamknięć przez wątek). Wskazówki: eliminować zamki zmianą algorytmu, a jeśli się nie da — **minimalizować liczbę i częstotliwość**; zakładać zamki **we właściwej kolejności** w każdym wątku (zakleszczenie); dla każdego zamknięcia umieścić otwarcie.

> [!definicja] Semafor (slajd 16)
> Konstrukcja teoretyczna rozwiązująca wzajemne wykluczanie: zmienna globalna z dwiema **niepodzielnymi, wykluczającymi się** operacjami **P** (*probeer, wait*) i **V** (*verhoog, signal*), zwykle w jądrze. `P(s){ if(s>0) s--; else uśpij_wątek(); }`, `V(s){ if(ktoś_śpi()) obudź_wątek(); else s++; }`. Implementacja V decyduje o **uczciwości** (np. FIFO); `s` = liczba dostępów do zasobu (semafor **binarny**).

> [!definicja] Współbieżne struktury danych (slajd 14)
> Struktury, do których dostęp jest **bezpieczny z poziomu wielu współbieżnie wykonywanych wątków** — alternatywa dla sekcji krytycznej.

## API z wykładu (slajdy 10–12)
```c
int pthread_mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t *mutexattr);
int pthread_mutex_lock(pthread_mutex_t *mutex);     // 0 = sukces
int pthread_mutex_trylock(pthread_mutex_t *mutex);  // wolny -> jak lock; zajęty -> natychmiast EBUSY
int pthread_mutex_unlock(pthread_mutex_t *mutex);

while( pthread_mutex_trylock(&muteks) != 0 ){ }   // spin lock — efekt jak lock
```
**Odpowiedzialność za poprawne użycie muteksów (bezpieczeństwo i żywotność) spoczywa na programiście.** Przy `lock` o zachowaniu czekającego wątku decyduje **implementacja** (np. uśpienie), przy `trylock` — **programista**. Slajd 12: pętla `while(work_done==0)` — w każdej iteracji wątek próbuje `trylock`; jak się nie uda, robi **pracę poza sekcją krytyczną** (flagi `work_inside`, `work_outside`).

## Filozofowie (slajdy 15, 17–18)
Pięciu filozofów, między talerzami po widelcu, do jedzenia potrzeba dwóch. Wersja prosta z 5 semaforami binarnymi (`wait(widelec[i]); wait(widelec[(i+1)%5]);`) **dopuszcza blokadę** — gdy wszyscy naraz podniosą lewy widelec. Wersja poprawna: dodatkowy **semafor poczwórny `pozwolenie`** (wartość 4) — przy stole sięga po widelce najwyżej czterech, więc ktoś zawsze zje.

## Na zajęcia
> [!zajecia]
> - **Lab 4:** [[PR L04 — Muteksy i symulacja pubu]] — kufle jako zasób, muteks, aktywne czekanie, `trylock`.
> - **Lab 5:** redukcja sumy pod muteksem w [[PR L05 — Całka w Pthreads]].
> - Łączy się z: [[Wyścig danych]], [[Zakleszczenie]], [[PR W06 — Monitory i zmienne warunku]].

## Na kartkówkę
> [!zadanie] 1. Wymień problemy współbieżności ze slajdu 2.
> > [!odpowiedz]- Odpowiedź
> > Wyścig, zależności danych, synchronizacja, wzajemne wykluczanie (sekcja krytyczna, protokoły wejścia/wyjścia).

> [!zadanie] 2. Pożądane cechy rozwiązań synchronizacji i możliwe błędy?
> > [!odpowiedz]- Odpowiedź
> > Pożądane: bezpieczeństwo, żywotność, uczciwość. Błędy: zakleszczenie, zagłodzenie.

> [!zadanie] 3. Wymień rodzaje zależności.
> > [!odpowiedz]- Odpowiedź
> > Zasobów, sterowania, danych (przepływu) — te ostatnie, gdy instrukcje operują na tych samych danych i choć jedna pisze.

> [!zadanie] 4. Dlaczego zamek `while(zamek!=0){} zamek=1;` jest zły?
> > [!odpowiedz]- Odpowiedź
> > Aktywne czekanie zużywa zasoby, a procedura nie jest bezpieczna (sprawdzenie i ustawienie nie są niepodzielne) ani nie zapewnia żywotności.

> [!zadanie] 5. Czym różni się `pthread_mutex_trylock` od `pthread_mutex_lock`?
> > [!odpowiedz]- Odpowiedź
> > Przy zajętym muteksie `trylock` wraca natychmiast (EBUSY), a `lock` czeka wewnątrz funkcji (np. usypiając wątek).

> [!zadanie] 6. Jakie są typy muteksów POSIX?
> > [!odpowiedz]- Odpowiedź
> > Jednorazowy (NORMAL, DEFAULT) i wielokrotnego wejścia (RECURSIVE, z licznikiem zamknięć).

> [!zadanie] 7. Zdefiniuj operacje P i V semafora.
> > [!odpowiedz]- Odpowiedź
> > P: jeśli s>0 to s−−, inaczej uśpij wątek. V: jeśli ktoś śpi — obudź go, inaczej s++. Obie niepodzielne.

> [!zadanie] 8. Kiedy proste rozwiązanie problemu filozofów się blokuje i jak to naprawić?
> > [!odpowiedz]- Odpowiedź
> > Gdy wszyscy jednocześnie podniosą lewy widelec. Naprawa: semafor `pozwolenie` z wartością 4.

> [!zadanie] 9. Czemu model pamięci jest problemem poprawności w programach równoległych?
> > [!odpowiedz]- Odpowiedź
> > Bez wymuszenia zapisu (fence/flush, synchronizacja) różne wątki mogą widzieć różne wartości tej samej zmiennej wspólnej.

> [!zadanie] 10. Czy współczesne procesory zapewniają spójność sekwencyjną?
> > [!odpowiedz]- Odpowiedź
> > Nie — przez optymalizacje kompilatora i procesora nie spełniają prostych modeli spójności (paradoks ze slajdu 9).

## Na egzamin
- Omów problemy współbieżności i cechy poprawnych rozwiązań; podaj przykład zakleszczenia i zagłodzenia.
- Dlaczego naiwny zamek z aktywnym czekaniem nie działa i jak rozwiązuje się protokoły wejścia/wyjścia?
- Porównaj `lock` i `trylock`; opisz wzorzec z pracą poza sekcją krytyczną.
- Czym jest model pamięci i spójność sekwencyjna vs osłabiona; jak wymusza się zapis.
- Semafory: definicja, uczciwość; rozwiązanie problemu filozofów z blokadą i bez.

## Fiszki #fiszki/równoległość
Wyścig (race condition) — definicja Banasia::Brak determinizmu wykonania
Pożądane cechy rozwiązań synchronizacji::Bezpieczeństwo, żywotność, uczciwość
Możliwe błędy synchronizacji::Zakleszczenie i zagłodzenie
Rodzaje zależności::Zasobów, sterowania, danych (przepływu)
Zależność danych::Instrukcje w bliskim sąsiedztwie czasowym operują na tych samych danych i choć jedna zapisuje
Dwa problemy zamka z aktywnym czekaniem::Zużywa zasoby komputera; nie jest bezpieczny ani nie zapewnia żywotności
Co zwraca pthread_mutex_trylock dla zajętego muteksu?::Wraca natychmiast z EBUSY (0 tylko gdy zamknął)
Typy muteksów POSIX::Jednorazowy (NORMAL/DEFAULT) i wielokrotnego wejścia (RECURSIVE)
Kto odpowiada za poprawne użycie muteksów?::Programista (bezpieczeństwo i żywotność)
Operacja P semafora::Jeśli s>0 to s--, inaczej uśpij wątek
Operacja V semafora::Jeśli ktoś śpi — obudź wątek, inaczej s++
Od czego zależy uczciwość semafora?::Od implementacji V (np. kolejka FIFO)
Poprawne rozwiązanie filozofów::Dodatkowy semafor pozwolenie z wartością 4
Modele spójności pamięci::Sekwencyjnej i osłabionej (relaxed)

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.

## Źródła
- K. Banaś, *Przetwarzanie współbieżne — problemy i rozwiązania* — `~/studia/sem5/programowanie_rownolegle/materialy/wyklady/PR_W03_Wspolbieznosc.pdf` (18 slajdów; tekst: `materialy/tekst/PR_W03_Wspolbieznosc.md`).
- Nagranie W03.mp4: https://cloud.wimiip.agh.edu.pl/s/PKMF9HR9iXjZtCk (ze strony https://home.agh.edu.pl/~kbanas/PR/PR.html).
