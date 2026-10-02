---
typ: pojęcie
status: nowe
przedmioty: ["[[AGH Programowanie równoległe]]"]
wymaga: ["[[PR W03 — Współbieżność]]", "[[PR L03 — pthread_create i atrybuty wątków]]"]
powiązane: ["[[PR W06 — Monitory i zmienne warunku]]", "[[Wyścig danych]]", "[[Mutex i zmienna warunkowa]]", "[[Zakleszczenie]]"]
tags: [pojęcie, równoległość, lab]
utworzono: 2026-10-02
źródła: ["K. Banaś — RR_L04_pthread_mutex_pub.pdf (instrukcja lab, 2026/27)", "K. Banaś — PR_W03_Wspolbieznosc.pdf (wykład 2026/27)", "pomiary własne 2026-10-02: Ryzen 5 5500, WSL2, gcc 13.3"]
---
# PR L04 — Muteksy i symulacja pubu

> [!skrot] W 30 sekund
> - **Co to:** lab 4 (pt **23.10**): symulacja pubu — klienci to wątki, zasób to `l_kf` nierozróżnialnych kufli; najpierw doprowadzić do błędu, potem naprawić **muteksem**, potem dodać **sprawdzanie dostępności z aktywnym czekaniem**; na wyższe oceny `trylock` i rozróżnialne krany.
> - **Klucz:** reprezentacja pubu = **jedna zmienna globalna z liczbą wolnych kufli**; pobranie/oddanie pod `pthread_mutex_lock`; gdy klientów więcej niż kufli — pętla `do { lock; if(wolne>0){wolne--; sukces=1;} unlock; } while(!sukces);`.
> - **Kiedy:** przeczytaj [[PR W03 — Współbieżność]] (kartkówka: muteksy, `trylock`, busy wait, sekcja krytyczna).

> [!intuicja] Po ludzku
> Kufle na ladzie i licznik na tablicy kredą. Dwóch klientów naraz czyta „5”, obaj piszą „4” — jeden kufel „zniknął” z rachunku. Muteks to barman, który pozwala podejść do lady tylko jednemu. Ale gdy kufli brak, klient bez zmiennych warunku może tylko co chwilę wracać i sprawdzać — czyli **aktywnie czekać**.

## Kroki laba w skrócie
1. `lab_4/`, start od `pub_sym_1.c` (wczytuje liczbę klientów i kufli, `ILE_MUSZE_WYPIC 3`, krany „nieskończone”).
2. Zaprojektuj reprezentację pubu (wskazówka z instrukcji: **aktualna liczba dostępnych kufli jako zmienna globalna**) i operacje pobrania/oddania kufla.
3. Dwa sprawdzenia błędów: **(a)** w `main` przed zamknięciem — czy liczba kufli na końcu = na początku; **(b)** w wątku zaraz po pobraniu — czy nie pobrano kufla, gdy wolnych nie było. Kod obu do sprawozdania.
4. Kufli > klientów, **bez** synchronizacji → doprowadzić do błędu (a): `-O0 -g`, zakomentować `printf` i większość `sleep`, duże `ILE_MUSZE_WYPIC` (333+), `usleep(1)` między pobraniem a oddaniem.
5. Naprawa **tylko `pthread_mutex_lock`** — trzy uruchomienia do oceny: (1) kufli > klientów, wersja niebezpieczna — błąd (a); (2) kufli > klientów, z muteksem — poprawnie; (3) **klientów > kufli**, z muteksem — błąd (b): kufle pobierane, choć ich nie ma. **(ocena)**
6. Sprawdzanie dostępności + aktywne czekanie (wzorzec `do{…}while(success==0)` z instrukcji); test dla różnych liczb kufli i klientów. **(ocena)** Na koniec przywróć `printf` i `sleep`.
7. **4.0:** wariant z `trylock` — zamiast czekać w `lock`, wątek robi `wykonana_praca++` i drukuje ją przy wyjściu.
8. **5.0:** `pub_sym_2.c` — mała liczba **rozróżnialnych kranów**, `trylock` do wyboru wolnego kranu **(ocena)**; potem rozróżnialne kufle („piję piwo marki %s z kufla %s”).

## Co jest oceniane
> [!wazne] „(ocena)” i warunki
> - **3.0:** trzy uruchomienia z kroku 5 + działające aktywne czekanie (krok 6); sprawdzenia (a) i (b) w kodzie.
> - **4.0:** `trylock` + licznik wykonanej pracy, odpowiedzi o jej zależności od proporcji klientów do kufli.
> - **5.0:** rozróżnialne krany (`pub_sym_2.c`), potem kufle.
> - **Sprawozdanie:** wszystkie kolejne modyfikacje kodu (łącznie ze sprawdzeniami), wydruki dla różnych parametrów, odpowiedzi na „->” w punktach.

## Na tej maszynie
Uproszczony test (10 klientów, 20 kufli, `wolne--; usleep(1); wolne++;` bez muteksu, `-O0`): przy `ILE = 3` zawsze **20 → 20** (błąd nie wychodzi!), przy 333 → **22–26**, przy 3000 → **151–268**, z `-O3` nawet **283–996**. Liczba kufli **rośnie** — giną zmniejszenia. Wniosek: z domyślnym `ILE_MUSZE_WYPIC 3` i `sleep` błędu nie zobaczysz.

## Odpowiedzi-szkice na pytania „->”
> [!zadanie] Najprostsza reprezentacja przy kuflach > klientów?
> > [!odpowiedz]- Szkic
> > Jedna zmienna globalna `int l_wolnych_kufli` + muteks; pobranie = `l_wolnych_kufli--`, oddanie = `++`, oba w sekcji krytycznej.

> [!zadanie] Jak połączyć bezpieczeństwo ze sprawdzeniem dostępności? Wada samych muteksów?
> > [!odpowiedz]- Szkic
> > Sprawdzenie warunku `wolne > 0` i zmniejszenie muszą być **w tej samej sekcji krytycznej** (inaczej ktoś zabierze kufel między sprawdzeniem a pobraniem). Gdy się nie uda — `unlock` i ponowienie próby w pętli, dając innym szansę oddać kufel. Wada: **aktywne czekanie** — procesor bez przerwy sprawdza warunek, marnując zasoby, jeśli nie ma nic sensownego w `do_something_else_or_nothing()`. Ten fragment wykonuje się **poza** sekcją krytyczną (po `unlock`).

> [!zadanie] Schemat wątku z `trylock` (4.0)?
> > [!odpowiedz]- Szkic
> > Jak slajd 12 W03: w pętli `if(pthread_mutex_trylock(&m)==0){ sprawdź i ewentualnie pobierz; unlock; } else { wykonana_praca++; }`. Wątek nie usypia w `lock`, tylko od razu robi coś innego. `trylock` **nie usuwa** pętli ponownego sprawdzania warunku — zmienia tylko sposób wejścia do sekcji krytycznej; ilość pracy zależy od liczby klientów i proporcji klienci/kufle (więcej rywalizacji → więcej nieudanych prób → więcej „pracy”).

> [!zadanie] Reprezentacja rozróżnialnych kranów (5.0)?
> > [!odpowiedz]- Szkic
> > Tablica kranów (np. struktura z nazwą piwa) i **osobny muteks na każdy kran**; wątek przechodzi po kranach `trylock` aż trafi na wolny, nalewa, odblokowuje. Kufle mogą zostać licznikiem z poprzedniej wersji.

## Plan i szkielet (najpierw sam!)
1. Globalnie: `int l_wolnych_kufli;` i `pthread_mutex_t muteks_kufle;` (init w `main`), zapamiętaj początkową liczbę.
2. Wątek: w pętli picia — pobranie (sekcja krytyczna), sprawdzenie (b), nalewanie/picie (`usleep`, poza sekcją!), oddanie (sekcja krytyczna).
3. `main` po `join`: sprawdzenie (a) i komunikat o błędzie lub OK.
4. Wersja z dostępnością: pobranie w pętli `do…while(success==0)`.

> [!kod]- Ściąga: pobranie kufla z aktywnym czekaniem (wzorzec z instrukcji, uzupełniony)
> ```c
> int success = 0;
> do {
>   pthread_mutex_lock(&muteks_kufle);
>   if (l_wolnych_kufli > 0) { l_wolnych_kufli--; success = 1; }
>   pthread_mutex_unlock(&muteks_kufle);
>   if (success == 0) {
>     /* do_something_else_or_nothing(); — POZA sekcją krytyczną */
>   }
> } while (success == 0);
> /* ... nalewanie i picie ... */
> pthread_mutex_lock(&muteks_kufle);
> l_wolnych_kufli++;
> pthread_mutex_unlock(&muteks_kufle);
> ```

## Pułapki
> [!blad] „Błąd się nie pojawia, więc nie ma wyścigu”
> Przy 3 piwach i `printf`/`sleep` w środku okno wyścigu jest mikroskopijne — instrukcja wprost każe zwiększyć `ILE_MUSZE_WYPIC`, wyłączyć napisy i kompilować `-O0 -g`.

> [!blad] Sprawdzenie dostępności poza muteksem
> `if(wolne>0){ lock; wolne--; unlock; }` — między `if` a `lock` inny klient zabiera ostatni kufel → ujemna liczba.

> [!blad] Czekanie z zamkniętym muteksem
> Pętla „czekaj, aż będzie kufel” **wewnątrz** sekcji krytycznej = nikt nie odda kufla = zakleszczenie. Między próbami trzeba `unlock`.

> [!blad] `sleep` (picie) w sekcji krytycznej
> Poprawne, ale serializuje cały pub — klienci piją po kolei.

## Na zajęcia
> [!zajecia]
> - **Pokaż prowadzącemu:** trzy uruchomienia z kroku 5 (z wydrukami sprawdzeń), potem działające aktywne czekanie dla klientów > kufli.
> - **Kartkówka (W03):** problemy współbieżności, zamek z busy wait, muteks POSIX (`lock`/`trylock`/`unlock`), typy muteksów, semafory, filozofowie.
> - Następny krok teoretyczny: [[PR W06 — Monitory i zmienne warunku]] — jak czekać na kufel **śpiąc**.

## Fiszki #fiszki/równoległość
Najprostsza reprezentacja pubu z nierozróżnialnymi kuflami::Globalna liczba wolnych kufli chroniona muteksem
Dwa sprawdzenia poprawności pubu::Kufli na końcu = na początku (w main); pobrano kufel, gdy wolnych nie było (w wątku)
Jak wywołać błąd wyścigu w pubie?::-O0 -g, bez printf/sleep, duże ILE_MUSZE_WYPIC, usleep(1) między pobraniem a oddaniem
Klientów więcej niż kufli, tylko muteks — skutek::Klienci pobierają kufle, których nie ma (licznik ujemny)
Gdzie sprawdzać warunek dostępności kufla?::W tej samej sekcji krytycznej co pobranie
Wada rozwiązania z samymi muteksami::Aktywne czekanie — ciągłe sprawdzanie warunku marnuje procesor
Czy do_something_else jest w sekcji krytycznej?::Nie — po unlock, poza sekcją
Co zmienia trylock w pubie?::Wątek nie czeka w lock, tylko od razu robi inną pracę; pętla sprawdzania zostaje
Reprezentacja rozróżnialnych kranów::Tablica kranów i osobny muteks na kran, wybór wolnego przez trylock
Co się stanie, gdy klient czeka na kufel z zamkniętym muteksem?::Zakleszczenie — nikt nie odda kufla

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.

## Źródła
- Instrukcja: `~/studia/sem5/programowanie_rownolegle/materialy/laby/RR_L04_pthread_mutex_pub.pdf` (tekst: `materialy/tekst/RR_L04_pthread_mutex_pub.md`); kod `laby/pub_sym_1.c`, `laby/pub_sym_2.c`.
- Wykład: [[PR W03 — Współbieżność]] (W03.mp4: https://cloud.wimiip.agh.edu.pl/s/PKMF9HR9iXjZtCk).
