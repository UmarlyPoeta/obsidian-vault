---
typ: pojęcie
status: nowe
przedmioty: ["[[AGH Programowanie równoległe]]"]
wymaga: ["[[PR W04 — Tworzenie programów równoległych]]", "[[PR W02 — Wątki i model SPMD]]", "[[PR W03 — Współbieżność]]"]
powiązane: ["[[PR L03 — pthread_create i atrybuty wątków]]", "[[Przyspieszenie i efektywność]]", "[[Kompilacja i pomiar czasu wykonania]]", "[[Wyścig danych]]"]
tags: [pojęcie, równoległość, lab]
utworzono: 2026-10-02
źródła: ["K. Banaś — RR_L05_pthreads_calka.pdf (instrukcja lab, 2026/27)", "K. Banaś — PR_W04_Tworzenie.pdf (wykład 2026/27)", "pomiary własne 2026-10-02: Ryzen 5 5500, WSL2, gcc 13.3"]
---
# PR L05 — Całka w Pthreads

> [!skrot] W 30 sekund
> - **Co to:** lab 5 (pt **30.10**): analiza `pthreads_suma.c` (redukcja z muteksem i z tablicą sum), potem całka z sin(x) na [0, π] metodą trapezów — wersja sekwencyjna, **zrównoleglenie pętli** (cyklicznie, potem blokowo) i **dekompozycja obszaru** (struktura na wątek).
> - **Klucz:** pętla: `start = id, koniec = N, skok = l_w` (cyklicznie) albo bloki ⌈N/l_w⌉; wynik pętli ma być **identyczny** z sekwencyjnym (~10⁻¹²), dekompozycja obszaru może się **nieznacznie różnić** (inne `dx_adjust` w podprzedziałach).
> - **Kiedy:** kartkówka z **W02, W03, W04** — szczególnie [[PR W04 — Tworzenie programów równoległych]] (dekompozycja blokowa/cykliczna, równoważenie obciążenia).

> [!intuicja] Po ludzku
> Pole pod sinusem kroisz na N pasków. Zrównoleglenie pętli: wątki rozdają sobie numery pasków (co l_w‑ty albo kolejnymi blokami) — paski są dokładnie te same co w wersji sekwencyjnej. Dekompozycja obszaru: każdy wątek dostaje kawałek osi x i sam kroi go na paski — szerokość pasków u każdego może wyjść troszkę inna.

## Kroki laba w skrócie
1. `zad_1`: `pthreads_suma.tgz` — dwa sposoby redukcji: zmienna globalna + **sekcja krytyczna** vs **tablica wyników cząstkowych** sumowana przez wątek główny. Uruchom i porównaj czas sekwencyjny i 2 wątków dla tablic **1000** i **100 000 000**. **(ocena)**
2. `zad2`: `pthreads_calka.tgz` (3 pliki: `obliczanie_calki.c` z `main` i `calka_sekw`, `dekompozycja_petli.c`, `dekompozycja_obszaru.c`).
3. Wersja sekwencyjna dla dx = 0,1; 0,001; 0,00001; 0,000001 — zbieżność do 2,0 (dla dx = 10⁻⁶, N = 3141593 błąd < 10⁻¹²). **(ocena)**
4. `calka_zrownoleglenie_petli` + `calka_fragment_petli_w`: wątki dostają **tylko ID**, reszta w zmiennych globalnych (`a_global`, `dx_global`, `N_global`, …), redukcja jak w `pthreads_suma.c`; zmienne `my_start`, `my_end`, `my_stride`; najpierw **cyklicznie** **(ocena)**, potem **blokowo** **(ocena)**. N i dx identyczne jak w sekwencyjnej.
5. **4.0:** `calka_dekompozycja_obszaru` + `calka_podobszar_w`: struktura z lewym i prawym krańcem podprzedziału, `dx` i polem na wynik; wątek woła `calka_sekw(a_local, b_local, dx)`; bez muteksów. **(ocena)**
6. **5.0:** wynik przez `pthread_exit` + `pthread_join` (dynamiczna alokacja, zwolnienie w `main`); optymalizacja `calka_sekw` (każda wartość funkcji raz, mnożenie przez dx poza sumą).

## Co jest oceniane
> [!wazne] „(ocena)” i warunki
> - **3.0:** krok 1 (suma: 1000 vs 10⁸, wnioski), krok 3 (zbieżność), krok 4 (cyklicznie i blokowo, wynik = sekwencyjny) — warunek: kroki 1–5.
> - **4.0:** dekompozycja obszaru z przekazaniem wyniku w strukturze.
> - **5.0:** `pthread_exit`/`pthread_join` z wynikiem, optymalizacja całkowania.
> - **Zawsze** (instrukcja): sprawdzaj, czy wynik równoległy = sekwencyjny, i czy jest **skrócenie czasu**; jeśli nie — wyjaśnij dlaczego.

## Na tej maszynie
> [!blad] Dołączona `libpomiar_czasu.a` się nie linkuje
> `ld: failed to set dynamic section sizes: bad value` (skompilowana w innym systemie). Rozwiązanie z instrukcji: wstaw **swoją** bibliotekę z L01 (`suma/lib/` i katalog `calka/`).

- `pthreads_suma` (tablica 10⁸ `double`, `-O3`, 3 uruchomienia): sekwencyjnie **0,081–0,146 s**; 2 wątki z muteksem **0,054–0,070 s**; 2 wątki z tablicą sum **0,042–0,073 s** → przyspieszenie ~1,5–2×.
- Ta sama dla tablicy **1000**: sekwencyjnie **~1–2 µs**; 2 wątki **0,5 ms** (muteks) i **0,13–0,2 ms** (tablica) → wersja równoległa **100–500× wolniejsza** — narzut tworzenia wątków.
- `calka_sekw`: dx = 0,1 → N = 32, wynik 1,998393…; dx = 0,001 → 1,99999983…; dx = 10⁻⁵ → 1,99999999998…; dx = 10⁻⁶ → N = 3 141 593, **1,999999999999758** (błąd 2,4·10⁻¹³), czas ≈ **0,068 s**.

## Odpowiedzi-szkice na pytania „->”
> [!zadanie] Prosty wzorzec zrównoleglenia pętli, czasy, narzut z muteksem i bez?
> > [!odpowiedz]- Szkic
> > Wątek z ID wylicza swój podzbiór iteracji (start, koniec, skok — blokowo w `pthreads_suma.c`), sumuje do zmiennej **prywatnej**, a na końcu raz dodaje ją do sumy wspólnej pod muteksem albo wpisuje do `tablica[id]`. Dla 10⁸ elementów zysk ~2× z 2 wątków; dla 1000 — strata, bo tworzenie wątku (dziesiątki µs, por. [[PR L02 — fork i clone]]) jest droższe niż całe obliczenie. Muteks przy jednym `lock` na wątek kosztuje mało; widać go dopiero przy małym zadaniu.

> [!zadanie] Dlaczego wynik pętli równoległej jest (nieomal) identyczny z sekwencyjnym? Wady?
> > [!odpowiedz]- Szkic
> > Te same N, `dx_adjust` i punkty x — liczone są dokładnie te same trapezy; różni się tylko **kolejność sumowania** (dodawanie zmiennoprzecinkowe nie jest łączne) → różnica rzędu 10⁻¹⁵–10⁻¹². Wady: wiele **zmiennych globalnych** (a, b, dx, N, l_w, wynik) — kiepska inżynieria, ryzyko wyścigów; redukcja wymaga muteksu.

> [!zadanie] Kod dekompozycji obszaru i skąd różnica wyniku?
> > [!odpowiedz]- Szkic
> > Wątek dostaje strukturę `{a_local, b_local, dx, wynik}` i liczy `calka_sekw(a_local, b_local, dx)`. Każdy podprzedział ma własne `N_local = ceil((b_local−a_local)/dx)` i `dx_adjust_local` — **różne od globalnego `dx_adjust` i między wątkami** → inne trapezy, różnica rzędu błędu metody. Bezpieczniejsze jest przekazywanie przez **strukturę** (brak globalnych, każdy wątek pisze tylko do swojego pola).

## Plan i szkielet (najpierw sam!)
1. W `calka_zrownoleglenie_petli`: ustaw globalne (`a_global`, `dx_global = dx_adjust`, `N_global`, `l_w_global`), zainicjuj muteks, `pthread_t w[l_w]`, `int id[l_w]`, create z `&id[i]`, join, zwróć `calka_global`.
2. W wątku: skopiuj globalne do lokalnych, ustaw `my_start/my_end/my_stride`, pętla (wnętrze przeniesione 1:1 z `calka_sekw`), na końcu `lock; calka_global += calka; unlock;` i `return NULL` (szablon nic nie zwraca!).
3. Odkomentuj w `main` wczytywanie `l_w` (domyślnie 0 → żaden wątek).
4. Test: 1, 2, 3, 4, 7 wątków; N niepodzielne przez l_w.

> [!kod]- Ściąga: start/koniec/skok
> ```c
> // cyklicznie
> int my_start = my_id, my_end = N, my_stride = l_w;
> // blokowo
> int na_watek = (N + l_w - 1) / l_w;        // ceil(N / l_w)
> int my_start = my_id * na_watek;
> int my_end   = my_start + na_watek; if (my_end > N) my_end = N;
> int my_stride = 1;
> ```

## Pułapki
> [!blad] `l_w = 0` w `main`
> Wczytywanie liczby wątków jest zakomentowane — bez zmiany funkcje równoległe nie utworzą żadnego wątku i zwrócą 0.

> [!blad] Inne `dx` w wersji pętli
> W pętli równoległej używaj `dx_adjust` (jak `calka_sekw`), nie surowego `dx` — inaczej N·dx ≠ b−a i wynik się rozjedzie.

> [!blad] Blok `j = ceil(N/p)` bez obcięcia końca
> Ostatni wątek wyjdzie poza N (`pthreads_suma.c` w ogóle przerywa program, gdy N nie dzieli się przez p). Obetnij `my_end` do N.

> [!blad] Brak `return` w funkcji wątku
> Szablon `calka_fragment_petli_w` kończy się bez `return` — dopisz `return NULL;` (inaczej UB, ostrzeżenie kompilatora).

## Na zajęcia
> [!zajecia]
> - **Pokaż prowadzącemu:** czasy sumy dla 1000 i 10⁸, tabelę zbieżności całki, wynik pętli cyklicznej i blokowej równy sekwencyjnemu, czasy dla kilku liczb wątków.
> - **Kartkówka (W02–W04):** SPMD i ID wątków, muteks, dekompozycja blokowa/cykliczna i tabela równoważenia obciążenia, wzorce, PCAM.
> - Łączy się z: [[PR W04 — Tworzenie programów równoległych]], [[PR L03 — pthread_create i atrybuty wątków]] (struktura in/out), [[Przyspieszenie i efektywność]].

## Fiszki #fiszki/równoległość
Dwa sposoby redukcji wyników wątków w pthreads_suma::Zmienna globalna w sekcji krytycznej albo tablica wyników cząstkowych sumowana przez wątek główny
Kiedy wersja równoległa sumy się nie opłaca?::Dla małego zadania (np. 1000 elementów) — narzut tworzenia wątków przewyższa obliczenia
Dekompozycja cykliczna pętli — my_start, my_end, my_stride::my_id, N, l_w
Dekompozycja blokowa pętli — my_start, my_end, my_stride::my_id·ceil(N/l_w), min(N, start+ceil(N/l_w)), 1
Dlaczego zrównoleglenie pętli daje wynik jak sekwencyjny?::Te same trapezy (N, dx_adjust); różni się tylko kolejność sumowania
Dlaczego dekompozycja obszaru daje nieco inny wynik?::Każdy podprzedział ma własne N_local i dx_adjust_local
Bezpieczniejszy sposób przekazywania danych do wątków w całce::Struktura jako argument wątku zamiast zmiennych globalnych
Wynik całki z sin na [0, π] dla dx = 1e-6 (moja maszyna)::1,999999999999758 (N = 3141593, błąd ok. 2e-13)
Co zrobić, gdy dołączona libpomiar_czasu.a się nie linkuje?::Wstawić własną bibliotekę zbudowaną na L01
Zwrot wyniku przez pthread_exit — o czym pamiętać?::Zmienna alokowana dynamicznie, rzutowanie w join, zwolnienie pamięci w main

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.

## Źródła
- Instrukcja: `~/studia/sem5/programowanie_rownolegle/materialy/laby/RR_L05_pthreads_calka.pdf` (tekst: `materialy/tekst/RR_L05_pthreads_calka.md`); kod `laby/pthreads_suma.tgz`, `laby/pthreads_calka.tgz`.
- Wykłady: [[PR W02 — Wątki i model SPMD]], [[PR W03 — Współbieżność]], [[PR W04 — Tworzenie programów równoległych]] (W04.mp4: https://cloud.wimiip.agh.edu.pl/s/dq6HFZNgtBH5CrM).
- Pomiary: oryginalne `pthreads_suma.c` (plus kopia z ROZMIAR 1000) i `obliczanie_calki.c` z własną biblioteką, scratchpad sesji — nie w Twoim repo.
