---
typ: pojęcie
status: nowe
przedmioty: ["[[AGH Programowanie równoległe]]"]
wymaga: ["[[PR W02 — Wątki i model SPMD]]", "[[PR W03 — Współbieżność]]"]
powiązane: ["[[PR L05 — Całka w Pthreads]]", "[[PR W05 — Wątki w Javie]]", "[[Przyspieszenie i efektywność]]"]
tags: [pojęcie, równoległość, wykład]
utworzono: 2026-10-02
źródła: ["K. Banaś — PR_W04_Tworzenie.pdf (wykład 2026/27)"]
---
# PR W04 — Tworzenie programów równoległych

> [!skrot] W 30 sekund
> - **Co to:** metodologia — jak z zadania zrobić program równoległy: dekompozycja (funkcjonalna, danych, w dziedzinie problemu), dekompozycja pętli **blokowa / cykliczna**, wzorce (zarządca‑wykonawcy, dziel i rządź, …), równoległość zadań vs pętli, **PCAM**.
> - **Klucz:** blokowo `start = id·⌈N/p⌉`, `koniec = min(N, (id+1)·⌈N/p⌉)`, `skok = 1`; cyklicznie `start = id`, `koniec = N`, `skok = p`.
> - **Kiedy:** kartkówka przed [[PR L05 — Całka w Pthreads]] (30.10) — razem z W02 i W03.

> [!intuicja] Po ludzku
> N kartek, p asystentów. **Blokowo**: każdy bierze stos ⌈N/p⌉ (ostatni może dostać mało albo nic). **Cyklicznie**: rozdajesz jak karty — różnica to najwyżej jedna kartka.

## Definicje i pojęcia

> [!definicja] Dwa kroki o zasadniczym znaczeniu (slajd 2)
> 1. **wykrycie dostępnej współbieżności** (bardziej twórcze),
> 2. **określenie koniecznej synchronizacji lub wymiany komunikatów** (bardziej techniczne).
>
> Wymaganie: **przenośność** — uruchomienie na dowolnej liczbie rdzeni, więc **liczba wątków/procesów staje się parametrem programu**.

> [!definicja] Pięć zadań przy tworzeniu programu równoległego (slajd 3)
> 1. **podział (dekompozycja)** zadania na podzadania,
> 2. **odwzorowanie** zadań na procesy/wątki, a dalej na elementy przetwarzania (węzły, procesory, rdzenie),
> 3. **podział (dystrybucja) danych** między elementy pamięci (z hierarchią pamięci),
> 4. określenie koniecznej **wymiany danych** i odwzorowanie jej na sieć połączeń,
> 5. określenie koniecznej **synchronizacji**.

> [!definicja] Rodzaje podziału (slajd 4)
> - **funkcjonalny** — złożone aplikacje, złożona optymalizacja,
> - **struktury danych** — sortowanie tablic, układy równań,
> - **w dziedzinie problemu** — symulacje w przestrzeni (**podział geometryczny**),
> - **złożony** — cechy funkcjonalnego i danych, np. **przetwarzanie potokowe**.
>
> Slajd 5 (POSIX): funkcjonalna = różne funkcje w `pthread_create`; **prosta dekompozycja pętli (SPMD)** = ta sama funkcja, argument z ID 0..p−1; dekompozycja **zadań** = tyle wątków, ile zadań. **Klasyfikacja jest nieostra.**

> [!definicja] Wzorce programowania równoległego (slajd 10)
> **Zarządca‑wykonawcy** (*manager‑worker, master‑slave*), **dziel i rządź** (*recursive splitting*, np. fork‑join), **zrównoleglenie pętli**, **przetwarzanie potokowe**, **agenci / aktorzy / sterowanie zdarzeniami**, **Map‑Reduce** (mikro‑wzorzec redukcji), mikro‑wzorzec **busy wait**. Wzorce to ogólne wskazówki — program może łączyć wiele z nich.

> [!definicja] Równoległość zadań vs pętli (slajd 13)
> - **task parallelism** — wątek dostaje **zadanie** (najczęściej funkcję); ta sama funkcja na innych danych albo różne funkcje; przydział **statyczny lub dynamiczny**; wykonanie najczęściej **asynchroniczne**,
> - **loop parallelism** — wątek dostaje **pewną liczbę iteracji pętli**, ma **własny indeks iteracji** z podzbioru indeksów; wykonanie najczęściej **synchroniczne**.

> [!definicja] Równoległość danych (slajd 14)
> **Węższy sens:** programista określa przydział danych i operację, resztę robi środowisko (np. **HPF**). **Szerszy sens:** podstawą zrównoleglenia jest dekompozycja danych i zasada **„właściciel przeprowadza obliczenia” (owner computes)**. Slajd 15: dekompozycja **danych** (indeks → dane, ten sam kod) vs **sterowania** (indeks → ścieżka w programie lub iteracja).

> [!definicja] PCAM (Foster, slajd 16)
> **P** – partition (podział), **C** – communicate (komunikacja, dane wspólne/prywatne, synchronizacja), **A** – agglomerate (analiza wariantów podziału), **M** – map (odwzorowanie na architekturę). **Pierwsze dwa kroki dają poprawny program, dwa kolejne — jego optymalizację.**

## Prosty przykład sumowania (slajdy 6–7, 12)
```c
int el_na_watek = ceil( (float) N / p );
int moj_start   = el_na_watek*moj_id;       // lub moj_start = moj_id
int moj_koniec  = el_na_watek*(moj_id+1);   // lub moj_koniec = N
if(moj_koniec > N) moj_koniec = N;
int moj_skok    = 1;                        // lub moj_skok = p
for(i=moj_start; i<moj_koniec; i+=moj_skok) moja_suma += A[i];
pthread_mutex_lock(&muteks); suma += moja_suma; pthread_mutex_unlock(&muteks);
```
Ten przykład (slajd 12) powstaje z **podziału w dziedzinie problemu lub danych**, realizuje **statyczną równoległość zadań**, wzorzec **zarządca‑wykonawcy**, model **SPMD**, i jest w istocie **zrównolegleniem pętli**.

## Równoważenie obciążenia (slajd 8)

| przypadek | dekompozycja | 1. wątek | ostatni | ile wątków ma mniej | maks. różnica |
|---|---|---|---|---|---|
| N = 1000100, p = 16 | blokowa | 62507 | 62495 | 1 | 12 (zawsze < p) |
| N = 1000100, p = 16 | cykliczna | 62507 | 62506 | 12 (zawsze < p) | 1 |
| N = 101, p = 32 | blokowa | 4 | 0 | 7 (**6 bezczynnych**) | 4 |
| N = 101, p = 32 | cykliczna | 4 | 3 | 27 | 1 |

Wniosek: przy blokowej z ⌈N/p⌉ różnica może sięgać prawie p, a przy małym N część wątków **nic nie robi**; cykliczna daje różnicę **co najwyżej 1**. Maksymalna liczba iteracji na wątek jest w obu taka sama.

## Na zajęcia
> [!zajecia]
> - **Lab 5:** [[PR L05 — Całka w Pthreads]] — dekompozycja pętli cykliczna i blokowa (z `my_start`, `my_end`, `my_stride`) oraz dekompozycja obszaru całkowania.
> - Łączy się z: [[PR W02 — Wątki i model SPMD]], [[PR W03 — Współbieżność]], [[Przyspieszenie i efektywność]].

## Na kartkówkę
> [!zadanie] 1. Jakie dwa kroki mają zasadnicze znaczenie przy tworzeniu programu równoległego?
> > [!odpowiedz]- Odpowiedź
> > Wykrycie dostępnej współbieżności oraz określenie koniecznej synchronizacji lub wymiany komunikatów.

> [!zadanie] 2. Wymień pięć zadań przy tworzeniu programu równoległego.
> > [!odpowiedz]- Odpowiedź
> > Dekompozycja, odwzorowanie na procesy/wątki i rdzenie, dystrybucja danych, wymiana danych, synchronizacja.

> [!zadanie] 3. Wymień rodzaje dekompozycji z przykładem.
> > [!odpowiedz]- Odpowiedź
> > Funkcjonalna (złożona aplikacja), danych (sortowanie), w dziedzinie problemu (symulacja — podział geometryczny), złożona (potok).

> [!zadanie] 4. Podaj start, koniec i skok dla dekompozycji blokowej i cyklicznej.
> > [!odpowiedz]- Odpowiedź
> > Blokowa: id·⌈N/p⌉, min(N,(id+1)·⌈N/p⌉), 1. Cykliczna: id, N, p.

> [!zadanie] 5. N = 101, p = 32, dekompozycja blokowa: ile wątków jest bezczynnych?
> > [!odpowiedz]- Odpowiedź
> > ⌈101/32⌉ = 4, więc 25 wątków dostaje po 4, jeden 1, a 6 — zero iteracji.

> [!zadanie] 6. Jaka jest maksymalna różnica liczby iteracji między wątkami przy dekompozycji cyklicznej?
> > [!odpowiedz]- Odpowiedź
> > 1.

> [!zadanie] 7. Wymień wzorce programowania równoległego.
> > [!odpowiedz]- Odpowiedź
> > Zarządca‑wykonawcy, dziel i rządź (fork‑join), zrównoleglenie pętli, potok, agenci/aktorzy/zdarzenia, Map‑Reduce, busy wait.

> [!zadanie] 8. Czym różni się równoległość zadań od równoległości pętli?
> > [!odpowiedz]- Odpowiedź
> > Zadania: wątek dostaje funkcję, przydział statyczny lub dynamiczny, zwykle asynchronicznie. Pętla: wątek dostaje podzbiór iteracji z własnym indeksem, zwykle synchronicznie.

> [!zadanie] 9. Rozwiń PCAM. Które kroki służą poprawności, a które optymalizacji?
> > [!odpowiedz]- Odpowiedź
> > Partition, Communicate, Agglomerate, Map. P i C — poprawność, A i M — optymalizacja.

> [!zadanie] 10. Co oznacza zasada „owner computes”?
> > [!odpowiedz]- Odpowiedź
> > Obliczenia na danych wykonuje wątek/proces, któremu te dane przydzielono (szerszy sens równoległości danych).

## Na egzamin
- Omów pięć zadań tworzenia programu równoległego i metodologię PCAM.
- Porównaj dekompozycję blokową i cykliczną pętli: wzory, równoważenie obciążenia (przykład N = 101, p = 32), przydatność.
- Równoległość zadań vs pętli; dekompozycja danych vs sterowania; równoległość danych w sensie węższym i szerszym.

## Fiszki #fiszki/równoległość
Dwa kluczowe kroki tworzenia programu równoległego::Wykrycie dostępnej współbieżności i określenie koniecznej synchronizacji/komunikacji
Dlaczego liczba wątków ma być parametrem programu?::Przenośność — program ma działać na dowolnej liczbie rdzeni
Rodzaje dekompozycji::Funkcjonalna, danych, w dziedzinie problemu (geometryczna), złożona (np. potok)
Dekompozycja blokowa — start, koniec, skok::id·ceil(N/p), min(N,(id+1)·ceil(N/p)), 1
Dekompozycja cykliczna — start, koniec, skok::id, N, p
Maks. różnica iteracji — dekompozycja cykliczna::1
Maks. różnica iteracji — blokowa z ceil(N/p)::Mniej niż p; przy małym N część wątków bezczynna
Wzorzec zarządca-wykonawcy — inne nazwy::Manager-worker, master-slave
Task parallelism::Wątek dostaje zadanie (funkcję); przydział statyczny lub dynamiczny; zwykle asynchronicznie
Loop parallelism::Wątek dostaje podzbiór iteracji z własnym indeksem; zwykle synchronicznie
Owner computes::Obliczenia na danych wykonuje ich właściciel — ten, komu je przydzielono
PCAM::Partition, Communicate, Agglomerate, Map (Foster)
PCAM — cel kroków::P i C — poprawny program; A i M — optymalizacja
Mikro-wzorzec redukcji::Map-Reduce

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.

## Źródła
- K. Banaś, *Tworzenie programów równoległych* — `~/studia/sem5/programowanie_rownolegle/materialy/wyklady/PR_W04_Tworzenie.pdf` (16 slajdów; tekst: `materialy/tekst/PR_W04_Tworzenie.md`; slajdy 9 i 11 to rysunki — w tekście brak treści).
- Nagranie W04.mp4: https://cloud.wimiip.agh.edu.pl/s/dq6HFZNgtBH5CrM (ze strony https://home.agh.edu.pl/~kbanas/PR/PR.html).
