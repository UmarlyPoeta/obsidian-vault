---
typ: pojęcie
status: nowe
przedmioty: ["[[AGH Programowanie równoległe]]"]
wymaga: ["[[PR W01 — Procesy i wątki]]"]
powiązane: ["[[PR L03 — pthread_create i atrybuty wątków]]", "[[PR L05 — Całka w Pthreads]]", "[[PR W04 — Tworzenie programów równoległych]]", "[[Proces i wątek]]", "[[Wyścig danych]]"]
tags: [pojęcie, równoległość, wykład]
utworzono: 2026-10-02
źródła: ["K. Banaś — PR_W02_Watki_SPMD.pdf (wykład 2026/27)", "K. Banaś — PRiR_skrypt.pdf, rozdz. 2.5"]
---
# PR W02 — Wątki i model SPMD

> [!skrot] W 30 sekund
> - **Co to:** wątki **Pthreads** (specyfikacja POSIX) — tworzenie, przesyłanie argumentów, oczekiwanie; klasyfikacja **MPMD / SPMD**.
> - **Klucz:** `pthread_create(&tid, attr, funkcja, arg)` + `pthread_join(tid, &wynik)`; w SPMD każdy wątek dostaje **swój identyfikator 0..p−1** i na jego podstawie wybiera dane, ścieżkę lub iteracje pętli.
> - **Kiedy:** kartkówka przed [[PR L03 — pthread_create i atrybuty wątków]] (16.10), potem [[PR L05 — Całka w Pthreads]].

> [!intuicja] Po ludzku
> SPMD to jedna instrukcja obsługi rozdana całej brygadzie, ale każdy ma na kartce inny numer — „ty bierzesz pierwszą ćwiartkę tablicy, ty drugą”. Numer musi być **na osobnej kartce dla każdego**, a nie na jednej tablicy, którą szef ciągle zmazuje (`&i`).

## Definicje i pojęcia

> [!definicja] Operacje na wątkach Pthreads (slajd 2)
> Specyfikacja **POSIX** (IEEE 1003.1, 1995, 2001). Operacje: określanie **atrybutów** („odłączalność”, adres i rozmiar stosu), **tworzenie i uruchamianie** (`pthread_create`), **porównywanie identyfikatorów** (`pthread_equal`, `pthread_self`), **zabijanie i przesyłanie sygnałów** (`pthread_cancel`, `pthread_kill`), **odłączanie** (`pthread_detach`), **oczekiwanie na zakończenie** (`pthread_join`).

> [!definicja] MPMD — Multiple Program Multiple Data (slajd 5)
> Każdy proces realizuje **różny kod**, związany z **różnymi plikami binarnymi** (np. przez `fork`/`exec`); sprzęt zawsze **MIMD**; odpowiada **dekompozycji funkcjonalnej** (ewentualnie złożonej); konieczna jest **komunikacja**; najczęściej model z **pamięcią rozproszoną** i przesyłaniem komunikatów — zbliża się do programowania rozproszonego.

> [!definicja] SPMD — Single Program Multiple Data (slajd 6)
> Każdy proces (wątek) realizuje **ten sam program** (ten sam plik binarny); model **bardzo uniwersalny, częściej używany niż MPMD**.
> - na **SIMD**: ten sam kod z **synchronizacją sprzętową** (w wariancie **SIMT** — karty graficzne — różnicowanie kodu przez **serializację**),
> - na **MIMD**: wątki wykonują ten sam kod, każdy na **przydzielonej porcji danych**, albo różne funkcje / różne ścieżki przez ten sam kod.

> [!definicja] SPMD w praktyce (slajdy 7–8)
> Do wielu wątków wykonujących ten sam kod przesyła się **różne dane jako argumenty** procedury startowej — najczęściej **identyfikator wątku**. Na jego podstawie wątek może:
> - zlokalizować dane: `dane[ f(my_id) ]`,
> - wybrać ścieżkę: `if(my_id == ...)`,
> - określić iteracje pętli: `for(i=f1(my_id); i<f2(my_id); i+=f3(my_id))`.
>
> **Komunikacja między wątkami odbywa się głównie za pomocą pamięci wspólnej.**

> [!definicja] Zmienne prywatne i wspólne (skrypt 2.4–2.5)
> **Prywatne** wątku = zmienne lokalne i argumenty wykonywanych procedur (na **jego stosie**) — ta sama nazwa w każdym wątku to inna komórka pamięci. **Wspólne** = globalne i statyczne — jedna kopia, zapis widoczny dla wszystkich wątków.

## API z wykładu (slajdy 2–4, 7)
```c
int pthread_create(pthread_t *thread, pthread_attr_t *attr,
                   void * (*start_routine)(void *), void * arg);
int pthread_join(pthread_t th, void **thread_return);

// SPMD — slajd 7: osobna komórka na identyfikator każdego wątku
for(i=0;i<p;i++) indeksy[i]=i;
for(i=0;i<p;i++) pthread_create(&watki[i], NULL, funkcja_w, (void *) &indeksy[i]);
for(i=0;i<p;i++) pthread_join(watki[i], NULL);
```
Slajd 4 — dwa sposoby odebrania argumentu w wątku:
- **kopiowanie zawartości** do struktury lokalnej: `struktura s_lok = *((struktura *)arg_wsk);` — dalej pracujesz na zmiennej **prywatnej**,
- **kopiowanie wskaźnika** z rzutowaniem: `struktura *s_wsk = (struktura *)arg_wsk;` — dostęp do zmiennych **nie‑lokalnych**, widocznych też dla innych wątków.

Skrypt 2.5.1: utworzenie wątku w Pthreads jest **zawsze połączone z jego uruchomieniem**; argument jest **jeden** (wskaźnik `void*`), więc wiele danych przekazuje się przez **strukturę**. Drugi argument `pthread_join` odbiera wskaźnik zwracany przez `return` lub `pthread_exit` — **nie może to być adres zmiennej lokalnej** wątku. Kompilacja: `-pthread` (lub `-lpthread`); w Linuksie `pthread_create` korzysta z `clone`.

> [!blad] Przekazanie `&i` (slajd 7: „błąd synchronizacji, kiedy przekazujemy &i!”)
> Wszystkie wątki dostają adres **tej samej** zmiennej sterującej pętli; zanim wątek ją odczyta, pętla już ją zmieniła → dwa wątki z tym samym ID. Lekarstwo: tablica `indeksy[]` (albo struktura per wątek).

## Na zajęcia
> [!zajecia]
> - **Lab 3:** [[PR L03 — pthread_create i atrybuty wątków]] — identyfikatory 0..p−1, celowe wywołanie błędu z `&i`, struktura in/out (szkielet całki), atrybuty.
> - **Lab 5:** [[PR L05 — Całka w Pthreads]] — wzorzec SPMD w praktyce.
> - Łączy się z: [[PR W01 — Procesy i wątki]], [[PR W04 — Tworzenie programów równoległych]], [[Wyścig danych]].

## Na kartkówkę
> [!zadanie] 1. Jaki standard opisuje wątki Pthreads i jakie operacje udostępnia?
> > [!odpowiedz]- Odpowiedź
> > POSIX (IEEE 1003.1). Atrybuty, tworzenie (`pthread_create`), porównywanie ID (`pthread_equal`, `pthread_self`), zabijanie/sygnały (`pthread_cancel`, `pthread_kill`), odłączanie (`pthread_detach`), oczekiwanie (`pthread_join`).

> [!zadanie] 2. Podaj sygnaturę `pthread_create` i opisz argumenty.
> > [!odpowiedz]- Odpowiedź
> > `int pthread_create(pthread_t *thread, pthread_attr_t *attr, void*(*start_routine)(void*), void *arg)` — zwracany ID, atrybuty (NULL = domyślne), funkcja startowa, jeden wskaźnik na argumenty.

> [!zadanie] 3. Co robi `pthread_join` i co jest jej drugim argumentem?
> > [!odpowiedz]- Odpowiedź
> > Czeka na zakończenie wskazanego wątku; drugi argument (`void**`) odbiera wskaźnik zwrócony przez wątek (`return`/`pthread_exit`), zwykle NULL.

> [!zadanie] 4. Rozwiń MPMD i SPMD. Który jest częściej używany?
> > [!odpowiedz]- Odpowiedź
> > Multiple Program Multiple Data / Single Program Multiple Data. SPMD — uniwersalny, częściej używany.

> [!zadanie] 5. Na jakiej architekturze sprzętu działa MPMD i jakiej dekompozycji odpowiada?
> > [!odpowiedz]- Odpowiedź
> > Zawsze MIMD; dekompozycji funkcjonalnej (ewentualnie złożonej); zwykle pamięć rozproszona i komunikaty.

> [!zadanie] 6. Co wątek SPMD może zrobić na podstawie swojego identyfikatora?
> > [!odpowiedz]- Odpowiedź
> > Zlokalizować swoje dane, wybrać ścieżkę wykonania (if), określić swoje iteracje pętli (start, koniec, skok).

> [!zadanie] 7. Dlaczego nie wolno przekazać wątkom `&i` zmiennej sterującej pętli?
> > [!odpowiedz]- Odpowiedź
> > Wszystkie dostają ten sam adres, a wartość zmienia się, zanim wątek ją odczyta — błąd synchronizacji (powtórzone ID). Trzeba osobnej komórki dla każdego wątku.

> [!zadanie] 8. Jak przekazać wątkowi kilka wartości?
> > [!odpowiedz]- Odpowiedź
> > Przez wskaźnik do struktury (rzutowanej z/na `void*`), osobnej dla każdego wątku.

> [!zadanie] 9. Czym różni się `s_lok = *(struktura*)arg` od `s_wsk = (struktura*)arg`?
> > [!odpowiedz]- Odpowiedź
> > Pierwsze kopiuje zawartość do zmiennej prywatnej (na stosie wątku); drugie kopiuje tylko wskaźnik — wątek pracuje na zmiennych nie‑lokalnych, wspólnych.

> [!zadanie] 10. Którymi zmiennymi wątki się komunikują, a które są prywatne?
> > [!odpowiedz]- Odpowiedź
> > Komunikacja: pamięć wspólna (globalne, statyczne, sterta przez wskaźniki). Prywatne: zmienne lokalne i argumenty procedur na stosie wątku.

> [!zadanie] 11. Czym jest SIMT?
> > [!odpowiedz]- Odpowiedź
> > Wariant SPMD na kartach graficznych — zróżnicowanie wykonywanego kodu realizowane przez serializację.

## Na egzamin
- Scharakteryzuj modele MPMD i SPMD (kod, architektura, dekompozycja, komunikacja); podaj przykład realizacji każdego.
- Opisz, jak w Pthreads realizuje się SPMD: przekazywanie identyfikatorów, wyznaczanie danych i iteracji; jaki błąd pojawia się przy przekazaniu `&i` i dlaczego.
- Omów sposoby przekazywania argumentów do wątków i zwracania wyników (zmienne wspólne, struktura, `pthread_exit` + `pthread_join`).
- Wyjaśnij pojęcia zmiennej prywatnej i wspólnej wątku i powiąż je z organizacją pamięci procesu (stos, dane statyczne, sterta).

## Fiszki #fiszki/równoległość
Pthreads — jaki standard?::POSIX (IEEE 1003.1, 1995, 2001)
Sygnatura pthread_create::int pthread_create(pthread_t *thread, pthread_attr_t *attr, void*(*start_routine)(void*), void *arg)
Sygnatura pthread_join::int pthread_join(pthread_t th, void **thread_return)
Funkcje do porównywania identyfikatorów wątków::pthread_equal i pthread_self
Funkcje do zabijania wątków i sygnałów::pthread_cancel i pthread_kill
MPMD — cechy::Różny kod (różne pliki binarne), sprzęt MIMD, dekompozycja funkcjonalna, pamięć rozproszona, konieczna komunikacja
SPMD — cechy::Ten sam program (plik binarny) dla wszystkich wątków/procesów, każdy na swoich danych; model uniwersalny
SPMD na kartach graficznych::SIMT — zróżnicowanie kodu przez serializację
Do czego wątek SPMD używa swojego ID?::Wybór danych, wybór ścieżki wykonania, wyznaczenie swoich iteracji pętli
Błąd przy pthread_create(..., &i) w pętli::Wszystkie wątki czytają tę samą zmieniającą się komórkę — powtórzone identyfikatory
Główny sposób komunikacji wątków::Pamięć wspólna (zmienne globalne/statyczne, sterta)
Zmienne prywatne wątku systemowego::Zmienne lokalne i argumenty procedur — na prywatnym stosie wątku
Czego nie wolno zwracać z funkcji wątku?::Adresu zmiennej lokalnej — znika razem z zakończeniem wątku

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.

## Źródła
- K. Banaś, *Przetwarzanie wielowątkowe* — `~/studia/sem5/programowanie_rownolegle/materialy/wyklady/PR_W02_Watki_SPMD.pdf` (8 slajdów; tekst: `materialy/tekst/PR_W02_Watki_SPMD.md`).
- Nagranie W02.mp4: https://cloud.wimiip.agh.edu.pl/s/ACojSszxX57oz3Q (ze strony https://home.agh.edu.pl/~kbanas/PR/PR.html).
- K. Banaś, skrypt *Programowanie równoległe i rozproszone*, rozdz. 2.4–2.5.2 — `~/studia/sem5/programowanie_rownolegle/materialy/ogolne/PRiR_skrypt.pdf`.
