---
typ: pojęcie
status: nowe
przedmioty: ["[[AGH Programowanie równoległe]]"]
wymaga: ["[[Proces i wątek]]"]
powiązane: ["[[PR L02 — fork i clone]]", "[[PR W02 — Wątki i model SPMD]]", "[[Proces i wątek]]", "[[Kompilacja i pomiar czasu wykonania]]", "[[Wyścig danych]]"]
tags: [pojęcie, równoległość, wykład]
utworzono: 2026-10-02
źródła: ["K. Banaś — PR_W01_Procesy_watki.pdf (wykład 2026/27)", "K. Banaś — PRiR_skrypt.pdf, rozdz. 1 i 2.2–2.4"]
---
# PR W01 — Procesy i wątki

> [!skrot] W 30 sekund
> - **Co to:** wstęp — po co równoległość, Flynn, poziomy równoległości, współbieżność vs równoległość, **proces** (`fork`/`execv`/`wait`) i **wątek** (`clone`).
> - **Klucz:** proces = ciąg(i) rozkazów + stos(y) + **przestrzeń adresowa** + kontekst; wątek = ciąg rozkazów + **stos** + tylko *niektóre* elementy kontekstu procesu. Wątki wprowadzono dla **łatwości komunikacji przez pamięć wspólną** i **wydajności przełączania kontekstu** (slajd 13).
> - **Kiedy:** kartkówka przed [[PR L02 — fork i clone]] (9.10). Nagranie: od ok. 35 min — tworzenie wątków.

> [!intuicja] Po ludzku
> Współbieżność: czasy trwania się nakładają (może być w przeplocie na jednym rdzeniu). Równoległość: naprawdę w tej samej chwili. Proces to osobne mieszkanie, wątki to lokatorzy jednego mieszkania, każdy z własnym notesem (stosem).

## Definicje i pojęcia (dosłownie jak u Banasia)

> [!definicja] Obliczenia równoległe (slajd 6)
> Dwa lub więcej procesów (wątków) **jednocześnie współpracuje (komunikując się wzajemnie) w celu rozwiązania pojedynczego zadania**. Przetwarzanie równoległe to **jedyny sposób wykorzystania pełnej mocy współczesnego sprzętu**. Po co (slajdy 3–5): lepsze wykorzystanie sprzętu, większa maksymalna moc obliczeniowa.

> [!definicja] Przetwarzanie współbieżne (slajd 10)
> Realizacja wielu programów (procesów) tak, że **ich trwanie od momentu rozpoczęcia do momentu zakończenia może się na siebie nakładać**. Pojawiło się z wielozadaniowymi systemami operacyjnymi (Multics 1965) i **nie wymusza równoległości**.
> Skrypt (1.2): **równoległe** — niektóre rozkazy wątków współbieżnych realizowane **w tym samym czasie**; **rozproszone** — wątki na **różnych komputerach połączonych siecią**; **przeplot** — jeden procesor na przemian wykonuje fragmenty wielu wątków.

> [!definicja] Klasyfikacja Flynna (slajd 7)
> Zwielokrotnienie strumieni **rozkazów** i **danych**:
> - **SISD** — oryginalna architektura von Neumanna,
> - **SIMD** — jeden strumień rozkazów, wiele strumieni danych,
> - **MISD** — wiele strumieni rozkazów, jeden danych; najbliżej jest **przetwarzanie potokowe**,
> - **MIMD** — wiele strumieni rozkazów i danych; współczesne systemy jako całość to MIMD (pamięć wspólna lub rozproszona), elementy to SISD/SIMD.

> [!definicja] Rodzaje przetwarzania równoległego (slajd 8)
> Równoległość na poziomie:
> 1. **pojedynczego wątku** — rozkazy wektorowe (kompilator), **ILP**: potokowość, superskalarność;
> 2. **zadań** (*task level*), **pętli** (*loop level*) — wątki i procesy: podział zadania na podzadania i ich przydział wątkom/procesom, a tych rdzeniom;
> 3. **programów** (*job level*) — niezależne programy w systemie operacyjnym, równoważenie obciążenia w klastrach.

> [!definicja] Warianty implementacji (slajd 9)
> Zrównoleglenie **niejawne** (procesor superskalarny, kompilator), języki **równoległości danych**, **pamięć wspólna**, **przesyłanie komunikatów**, **offload** na akcelerator, modele **hybrydowe**.

> [!definicja] Proces (slajd 11)
> Proces (jednowątkowy / wielowątkowy) to:
> - ciąg rozkazów (wątek główny) / ciągi rozkazów (wątek główny i inne wątki),
> - stos (wątku głównego) / **odrębne stosy dla każdego wątku**,
> - **przestrzeń adresowa**,
> - dodatkowe elementy tworzące m.in. **kontekst procesu**.

> [!definicja] Wątek (slajd 13)
> Ciąg rozkazów, stos i **niektóre z elementów tworzących kontekst procesu**. Przyczyna wprowadzenia: **łatwość komunikacji przy użyciu pamięci wspólnej i wydajność przełączania kontekstu**. Skrypt: wątek **nie ma własnej przestrzeni adresowej**; jego zmienne prywatne to zmienne lokalne i argumenty procedur (na jego stosie).

## API z wykładu (slajdy 11–13)
```c
pid_t fork( void );
int   execv( const char *filename, char *const argv[] );
pid_t wait( pid_t pid, int *status );   // tak na slajdzie; w POSIX: wait(int*), waitpid(pid, int*, opcje)
int   clone( int (*fn)(void *), void *child_stack, int flags, void *arg );   // Linux
```
Slajd 12: `pid = fork(); if(pid==0){ execl(...); } else { wait(&stan); }` + uwaga **„zachowanie zmiennych (w tym globalnych)”** — potomek ma **kopię** zmiennych. `fork` zwraca **0 w potomnym**, **PID potomka w nadrzędnym**. Po udanym `exec` proces „znika” z kodu — wraca tylko przy błędzie. `wait` to pierwszy mechanizm **synchronizacji** (skrypt 2.3).

`clone` (slajd 13) daje wątek, gdy flagi każą dzielić zasoby (`CLONE_VM | CLONE_FS | CLONE_FILES | CLONE_SIGHAND` jak w `clone.c`). Stos alokujesz sam i podajesz jego **koniec** (rośnie w dół).

## Na zajęcia
> [!zajecia]
> - **Lab:** [[PR L02 — fork i clone]] — czas tworzenia 1000 procesów i wątków, dwa wątki i zmienne globalne/lokalne.
> - **Nagranie:** W01.mp4 (link w Źródłach) — tworzenie wątków **od ok. 35 minuty**.
> - Łączy się z: [[Proces i wątek]], [[Wyścig danych]], [[PR W02 — Wątki i model SPMD]].

## Na kartkówkę
> [!zadanie] 1. Zdefiniuj obliczenia równoległe.
> > [!odpowiedz]- Odpowiedź
> > Dwa lub więcej procesów (wątków) jednocześnie współpracuje, komunikując się wzajemnie, w celu rozwiązania pojedynczego zadania.

> [!zadanie] 2. Czym różni się przetwarzanie współbieżne od równoległego?
> > [!odpowiedz]- Odpowiedź
> > Współbieżne: czasy trwania się nakładają (może być przeplot na jednym rdzeniu). Równoległe: część rozkazów w tej samej chwili (wiele rdzeni). Każde równoległe jest współbieżne, nie odwrotnie.

> [!zadanie] 3. Wymień klasy Flynna. Co jest najbliższe MISD?
> > [!odpowiedz]- Odpowiedź
> > SISD, SIMD, MISD, MIMD. Najbliżej MISD jest przetwarzanie potokowe.

> [!zadanie] 4. Do której klasy Flynna należy współczesny komputer wielordzeniowy?
> > [!odpowiedz]- Odpowiedź
> > Jako całość MIMD (z pamięcią wspólną lub rozproszoną); pojedyncze elementy odpowiadają SISD i SIMD.

> [!zadanie] 5. Co to ILP i jakie mechanizmy ją realizują?
> > [!odpowiedz]- Odpowiedź
> > Równoległość na poziomie pojedynczego rozkazu — potokowość i superskalarność procesora (w ramach jednego wątku).

> [!zadanie] 6. Z czego składa się proces, a z czego wątek?
> > [!odpowiedz]- Odpowiedź
> > Proces: ciąg(i) rozkazów, stos(y), przestrzeń adresowa, kontekst. Wątek: ciąg rozkazów, stos, niektóre elementy kontekstu procesu.

> [!zadanie] 7. Dlaczego wprowadzono wątki?
> > [!odpowiedz]- Odpowiedź
> > Łatwość komunikacji przy użyciu pamięci wspólnej i wydajność przełączania kontekstu.

> [!zadanie] 8. Co zwraca `fork()` w procesie nadrzędnym, a co w potomnym?
> > [!odpowiedz]- Odpowiedź
> > W nadrzędnym PID potomka, w potomnym 0 (−1 przy błędzie).

> [!zadanie] 9. Proces potomny po `fork` zwiększa zmienną globalną. Co widzi proces nadrzędny po `wait`?
> > [!odpowiedz]- Odpowiedź
> > Starą wartość — potomek ma własną kopię przestrzeni adresowej (kopia zmiennych globalnych).

> [!zadanie] 10. Co robi `execv` i kiedy z niej wracamy?
> > [!odpowiedz]- Odpowiedź
> > Wiąże proces z nowym plikiem wykonywalnym (podmienia kod); wraca tylko przy błędzie.

> [!zadanie] 11. Jaką funkcją Linux tworzy wątki „na niskim poziomie” i co trzeba jej podać?
> > [!odpowiedz]- Odpowiedź
> > `clone(fn, child_stack, flags, arg)` — funkcję, koniec zaalokowanego stosu, flagi współdzielenia (np. CLONE_VM), argument.

## Na egzamin
- Zdefiniuj przetwarzanie współbieżne, równoległe i rozproszone; pokaż relacje między nimi (które jest szczególnym przypadkiem którego).
- Omów klasyfikację Flynna i umieść w niej współczesny procesor wielordzeniowy z rozkazami wektorowymi.
- Omów poziomy równoległości i kto za każdy odpowiada (sprzęt, kompilator, programista, system).
- Porównaj proces i wątek: co jest wspólne, co prywatne; dlaczego tworzenie i przełączanie wątków jest tańsze.
- Opisz działanie `fork` + `exec` + `wait` na przykładzie; co dzieje się ze zmiennymi globalnymi.

## Fiszki #fiszki/równoległość
Obliczenia równoległe — definicja Banasia::Dwa lub więcej procesów (wątków) jednocześnie współpracuje, komunikując się, by rozwiązać pojedyncze zadanie
Przetwarzanie współbieżne — definicja::Wiele programów, których czasy trwania (od rozpoczęcia do zakończenia) mogą się nakładać; nie wymusza równoległości
Równoległe a współbieżne — relacja::Każde wykonanie równoległe jest współbieżne, nie każde współbieżne jest równoległe
Przetwarzanie rozproszone::Wątki programu wykonywane na różnych komputerach połączonych siecią
Klasy Flynna::SISD, SIMD, MISD, MIMD
Co jest najbliższe MISD?::Przetwarzanie potokowe
ILP — co to?::Równoległość na poziomie pojedynczego rozkazu: potokowość, superskalarność
Elementy procesu::Ciąg(i) rozkazów, stos(y), przestrzeń adresowa, elementy kontekstu procesu
Elementy wątku::Ciąg rozkazów, stos, niektóre elementy kontekstu procesu
Przyczyna wprowadzenia wątków::Łatwość komunikacji przez pamięć wspólną i wydajność przełączania kontekstu
Wartość zwracana przez fork::0 w procesie potomnym, PID potomka w nadrzędnym, -1 przy błędzie
Zmienna globalna po fork::Każdy proces ma własną kopię — zmiana w potomnym nie jest widoczna w nadrzędnym
Sygnatura clone::int clone(int (*fn)(void*), void *child_stack, int flags, void *arg)

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.

## Źródła
- K. Banaś, *Procesy i wątki* — `~/studia/sem5/programowanie_rownolegle/materialy/wyklady/PR_W01_Procesy_watki.pdf` (13 slajdów; tekst: `materialy/tekst/PR_W01_Procesy_watki.md`).
- Nagranie W01.mp4: https://cloud.wimiip.agh.edu.pl/s/9KyQtbFD5KqAfjY (link ze strony https://home.agh.edu.pl/~kbanas/PR/PR.html); tworzenie wątków od ok. 35 min (wg instrukcji L02).
- K. Banaś, skrypt *Programowanie równoległe i rozproszone*, rozdz. 1 (definicje, test) i 2.2–2.4 — `~/studia/sem5/programowanie_rownolegle/materialy/ogolne/PRiR_skrypt.pdf`.
