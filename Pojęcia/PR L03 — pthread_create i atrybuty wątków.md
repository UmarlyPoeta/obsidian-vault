---
typ: pojęcie
status: nowe
przedmioty: ["[[AGH Programowanie równoległe]]"]
wymaga: ["[[PR W02 — Wątki i model SPMD]]", "[[PR L02 — fork i clone]]"]
powiązane: ["[[PR L05 — Całka w Pthreads]]", "[[Proces i wątek]]", "[[Wyścig danych]]"]
tags: [pojęcie, równoległość, lab]
utworzono: 2026-10-02
źródła: ["K. Banaś — RR_L03_pthread_create_attr.pdf (instrukcja lab, 2026/27)", "K. Banaś — PRiR_skrypt.pdf, rozdz. 2.5.3", "pomiary własne 2026-10-02: Ryzen 5 5500, WSL2, gcc 13.3"]
---
# PR L03 — pthread_create i atrybuty wątków

> [!skrot] W 30 sekund
> - **Co to:** lab 3 (pt **16.10**): uzupełnić `pthreads_detach_kill.c` (wątki przyłączalne i odłączone, `pthread_cancel`), napisać od zera program z identyfikatorami 0..p−1 i celowo wywołać błąd `&i`; potem struktura in/out (szkielet całki z L05) i atrybuty.
> - **Klucz:** `pthread_join(tid, &wynik); if(wynik == PTHREAD_CANCELED)…` — tylko dla wątku **przyłączalnego**; odłączony sprawdzasz pośrednio (zmienna wspólna + limit czasu).
> - **Kiedy:** przeczytaj [[PR W02 — Wątki i model SPMD]] (kartkówka) i skrypt **2.5.3 „Czas życia wątków”** — instrukcja każe uzupełniać kod „zgodnie z opisem w p. 2.5.3”.

> [!intuicja] Po ludzku
> Wątek przyłączalny to pracownik, który po robocie czeka na podpis szefa (`join`) i dopiero wtedy oddaje identyfikator. Odłączony wychodzi sam i nikomu się nie melduje — szef nie wie, czy skończył, czy go „zabito”, może tylko zgadywać po śladach.

## Kroki laba w skrócie
1. `lab_3/zad_1`: `pthreads_detach_kill.c` — wstawić `pthread_create`, `pthread_join` przed sprawdzeniem `wynik`, `pthread_detach`, sprawdzenie odłączonego, `pthread_attr_init` / `setdetachstate` / `destroy`. **(ocena)**
2. `zad_2`: **nowy** program (od zera) — wątki w pętli dostają ID 0..p−1, wypisują `pthread_self()` i swoje ID; join w osobnej pętli; test dla różnych p. **(ocena)**
3. Wersja z błędem: przekazać `&i` zmiennej sterującej pętli i złapać dwa wątki z tym samym ID; zrzuty obu wersji do sprawozdania.
4. **4.0** — `zad_3`: struktura z polem `in` i `in/out` — najlepiej szkielet całki: tablica struktur `{a, b, wynik}` (double), każdy wątek liczy np. środek swojego odcinka, `main` po join sumuje; ≥ 2 wątki.
5. **5.0** — `zad_4`: wątki z różnymi atrybutami, wypisanie ich w wątku (`pthread_getattr_np`): rozmiar i położenie stosu (duża tablica `int tab[10000000]`, kompilacja `-g`), priorytet, afiniczność (rdzeń). **(ocena)**

## Co jest oceniane
> [!wazne] „(ocena)” i warunki
> - **3.0:** działający `pthreads_detach_kill.c` + program z identyfikatorami i pokazany błąd synchronizacji (kroki 1–6).
> - **4.0:** struktura in/out, ≥ 2 wątki, suma w `main`.
> - **5.0:** atrybuty — stos, priorytet, afiniczność, inne z `man pthread_attr_init`.
> - **Sprawozdanie:** opis, Twoje uzupełnienia kodu, wydruki terminala z identyfikacją osoby: poprawne ID i wersja z błędem; odpowiedzi na „->”.

## Na tej maszynie
- Uzupełniony `pthreads_detach_kill.c` (`-O0`) zachowuje się jak opis w skrypcie: wątek 1 „zostal zabity”, wątek 2 „PRAWDOPODOBNIE zostal zabity”, wątek 3 kończy pracę po `pthread_exit` w `main`. Całość ≈ **19 s** — `czasozajmowacz()` (mnożenie macierzy 1000×1000) to ok. **3,6 s** na wywołanie.
- Błąd `&i` (`-O0`, 20 uruchomień): powtórzone ID w **2/20** dla p = 4, **7/20** dla p = 10, **13/20** dla p = 100 → testuj większe p i kilka razy.
- Domyślny stos wątku Pthreads: **8 MiB** (= `ulimit -s` 8192 KiB). `int tab[10000000]` to 40 MB → potrzebny atrybut `pthread_attr_setstacksize` ≥ ~40 MB.

## Odpowiedzi-szkice na pytania „->” (wg skryptu 2.5.3)
> [!zadanie] W jakich dwóch trybach działają wątki Pthreads i czym się różnią?
> > [!odpowiedz]- Szkic
> > **Przyłączalny** (joinable, domyślny wg POSIX) — można czekać na niego `pthread_join`; po zakończeniu część zasobów jest utrzymywana do wywołania `pthread_join`. **Odłączony** (detached) — nie da się na niego czekać, zasoby zwalniane od razu; przydatny, gdy wątek główny nie jest powiązany z pracą potomnego (np. obsługa żądań sieciowych).

> [!zadanie] Kiedy wątek standardowo kończy pracę i jak wymusić zakończenie?
> > [!odpowiedz]- Szkic
> > Przy powrocie z funkcji startowej lub wywołaniu `pthread_exit` w dowolnej procedurze. Wymuszenie: `pthread_cancel` (sygnał anulowania) albo `pthread_kill`. Wątek odbiera anulowanie tylko w **punktach anulowania** (np. `sleep`, `pthread_testcancel`). Dodatkowo `return`/`exit` w `main` kończy cały proces razem ze wszystkimi wątkami — tylko `pthread_exit` w `main` zostawia działające wątki.

> [!zadanie] Jak wątek chroni się przed „zabiciem”?
> > [!odpowiedz]- Szkic
> > `pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL)` — nie przyjmuje anulowania nawet w punktach anulowania; po `PTHREAD_CANCEL_ENABLE` przyjmie je w najbliższym punkcie (np. `pthread_testcancel()`).

> [!zadanie] Jak sprawdzić, czy zabicie się powiodło (przyłączalny vs odłączony)?
> > [!odpowiedz]- Szkic
> > Przyłączalny: `pthread_join(tid, &wynik)` i porównanie `wynik == PTHREAD_CANCELED`. Odłączony: brak `join` — tylko pośrednio, np. zmienna wspólna zmieniana tuż przed końcem pracy i oczekiwanie z limitem czasu (pętla z `sleep`); wniosek „prawdopodobnie”.

> [!zadanie] Jak poprawnie przesłać identyfikator i jaki błąd daje wskaźnik do zwykłego `int`?
> > [!odpowiedz]- Szkic
> > Każdy wątek dostaje adres **własnej** komórki (`indeksy[i]` albo element tablicy struktur), która nie zmienia się do jego odczytu. Przy `&i` wszystkie wątki czytają tę samą komórkę, którą pętla w międzyczasie zwiększa → powtórzone lub pominięte ID (błąd synchronizacji, slajd 7 W02).

> [!zadanie] Jakie atrybuty można ustawić (5.0)?
> > [!odpowiedz]- Szkic
> > Wg skryptu: stan odłączenia, adres i rozmiar stosu, sposób szeregowania (priorytet, polityka); w Linuksie też afiniczność (`pthread_attr_setaffinity_np`), rozmiar obszaru ochronnego stosu. Pełną listę daj z `man pthread_attr_init` na maszynie z labu.

## Plan i szkielet (najpierw sam!)
- `zad_2`: `#define MAX 100`, `pthread_t w[MAX]; int id[MAX];`, `p` z `argv` lub `scanf`; pętla 1: `id[i]=i; pthread_create(…, &id[i])`; pętla 2: `pthread_join`. W wątku `int moj = *(int*)arg;` i `printf` z `pthread_self()` (rzutuj na `unsigned long`, format `%lu`).
- Wersja z błędem: zamiast `&id[i]` daj `&i`, odpal kilka razy dla p = 10, 100; `./prog | sort | uniq -d` pokazuje powtórzenia.

> [!kod]- Ściąga: brakujące linie w pthreads_detach_kill.c (sprawdzone)
> ```c
> pthread_create(&tid, NULL, zadanie_watku, NULL);        // wątki 1 i 2
> pthread_join(tid, &wynik);                              // przed if(wynik == PTHREAD_CANCELED)
> pthread_detach(tid);                                    // wątek 2
> for(i=0;i<10;i++){ sleep(1); if(zmienna_wspolna!=0) break; }   // „czy zabity?”
> if(zmienna_wspolna==0) printf("...PRAWDOPODOBNIE zostal zabity\n");
> else                   printf("...PRAWDOPODOBNIE NIE zostal zabity\n");
> pthread_attr_init(&attr);                               // wątek 3
> pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
> pthread_attr_destroy(&attr);                            // po pthread_create
> ```

## Pułapki
> [!blad] Sprawdzanie `wynik` bez `pthread_join`
> W szablonie `wynik` jest niezainicjowany — bez `join` porównanie z `PTHREAD_CANCELED` to czytanie śmieci.

> [!blad] `exit(0)` zamiast `pthread_exit(NULL)` na końcu `main`
> Zabija odłączony wątek nr 3 — nigdy nie wypisze „zmiana wartosci zmiennej wspolnej”. To pytanie z komentarza w kodzie.

> [!blad] `pthread_join` na wątku odłączonym
> Niedozwolone (POSIX: zachowanie nieokreślone; glibc zwykle zwraca `EINVAL`) — stąd pośrednie sprawdzanie przez zmienną wspólną.

> [!blad] „U mnie działa” przy `&i`
> Błąd jest losowy (2/20 przy 4 wątkach) — do sprawozdania potrzebujesz zrzutu z wystąpieniem, więc powtarzaj z większym p.

## Na zajęcia
> [!zajecia]
> - **Pokaż prowadzącemu:** przebieg `pthreads_detach_kill` z komentarzem, program z poprawnymi ID dla kilku p, wersję z powtórzonym ID.
> - **Kartkówka (W02):** sygnatury `pthread_create`/`pthread_join`, MPMD/SPMD, przekazywanie argumentów, błąd `&i`.
> - Łączy się z: [[PR W02 — Wątki i model SPMD]], [[PR L05 — Całka w Pthreads]] (struktura in/out to szkielet dekompozycji obszaru).

## Fiszki #fiszki/równoległość
Dwa tryby wątków Pthreads::Przyłączalny (joinable, domyślny) i odłączony (detached)
Przyłączalny vs odłączony — zasoby::Przyłączalny trzyma część zasobów do pthread_join; odłączony zwalnia od razu
Jak utworzyć wątek od razu odłączony?::pthread_attr_init, pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED), pthread_create z &attr
Kiedy wątek odbiera sygnał anulowania?::W punktach anulowania (np. sleep, pthread_testcancel)
Jak wątek blokuje anulowanie?::pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL)
Jak sprawdzić, że przyłączalny wątek został anulowany?::pthread_join(tid, &wynik) i wynik == PTHREAD_CANCELED
Jak zakończyć main, nie zabijając wątków?::pthread_exit(NULL) zamiast return/exit
Skutek exit w dowolnym wątku::Kończy cały proces ze wszystkimi wątkami
Poprawne przesłanie ID do wątku::Adres osobnej komórki dla każdego wątku (np. id[i]), nie &i
Funkcja zwracająca systemowy ID wątku::pthread_self()
Odczyt atrybutów działającego wątku w Linuksie::pthread_getattr_np
Domyślny stos wątku Pthreads na mojej maszynie::8 MiB (= ulimit -s)

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.

## Źródła
- Instrukcja: `~/studia/sem5/programowanie_rownolegle/materialy/laby/RR_L03_pthread_create_attr.pdf` (tekst: `materialy/tekst/RR_L03_pthread_create_attr.md`); kod `laby/pthreads_detach_kill.c`.
- Skrypt PRiR, rozdz. 2.5.1–2.5.3 (kod 2.4 to pełny wzór programu z wątkami 1–3): `~/studia/sem5/programowanie_rownolegle/materialy/ogolne/PRiR_skrypt.pdf`.
- Wykład: [[PR W02 — Wątki i model SPMD]] (W02.mp4: https://cloud.wimiip.agh.edu.pl/s/ACojSszxX57oz3Q).
