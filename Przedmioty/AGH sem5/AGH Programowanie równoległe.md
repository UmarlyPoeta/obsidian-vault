---
typ: przedmiot
uczelnia: AGH
semestr: 5
ects: 5
forma: egzamin
prowadzący: "Krzysztof Banaś, Mateusz Sitko"
ocena: "2/3·egzamin + 1/3·laby, obie ≥ 3,0; > 3 nieobecności na labach = brak zaliczenia"
tags: [przedmiot, agh]
---
# AGH Programowanie równoległe

> [!definicja] Zaliczenie
> **Egzamin, 5 ECTS.** 2/3·egzamin + 1/3·laby, obie ≥ 3,0; > 3 nieobecności na labach = brak zaliczenia
> Prowadzący: Krzysztof Banaś, Mateusz Sitko. Pełny sylabus: `~/studia/sylabus/semestr5.md`.
> Opiera się na Systemach operacyjnych (sem. 2).

## Laby i wykłady
Laby: piątki 9:45–11:15, B5 s. 404, dr inż. Mateusz Sitko (gr. 3). Na początku laba możliwa **kartkówka z wykładu do tematu** (3 × ndst = brak zaliczenia, bez poprawy w semestrze). Daty zgodne z `plan przedmiot "równoległe"`.

| Lab | Data | Temat | Wykład do przeczytania | Przygotowanie |
|---|---|---|---|---|
| L01 | 2.10 | Pomiar czasu, Makefile, biblioteka statyczna | — | [[Kompilacja i pomiar czasu wykonania]] |
| L02 | 9.10 | Procesy i wątki — fork, clone | [[PR W01 — Procesy i wątki]] (nagranie od ~35 min) | [[PR L02 — fork i clone]] |
| L03 | 16.10 | Wątki POSIX — create, atrybuty, detach, cancel | [[PR W02 — Wątki i model SPMD]] | [[PR L03 — pthread_create i atrybuty wątków]] |
| L04 | 23.10 | Wzajemne wykluczanie — muteksy, pub | [[PR W03 — Współbieżność]] | [[PR L04 — Muteksy i symulacja pubu]] |
| L05 | 30.10 | Dekompozycja — suma i całka w Pthreads | [[PR W02 — Wątki i model SPMD]], [[PR W03 — Współbieżność]], [[PR W04 — Tworzenie programów równoległych]] | [[PR L05 — Całka w Pthreads]] |
| L06 | 6.11 | Java — synchronizacja (histogram) | [[PR W05 — Wątki w Javie]] | — |
| L07 | 13.11 | Java — pule wątków (Executor, Callable) | [[PR W05 — Wątki w Javie]] | — |
| L08 | 20.11 | Zmienne warunku (czytelnicy–pisarze, bariera, prod.–kons.) | [[PR W06 — Monitory i zmienne warunku]] | — |
| L09 | 27.11 | OpenMP — pętle | W07, W08 | — |
| L10 | 4.12 | OpenMP — zmienne, zależności | W07, W08, W09 | — |
| L11 | 11.12 | MPI — podstawy (sztafeta) | W10 | — |
| L12 | 18.12 | MPI — komunikacja grupowa | W11 | — |
| L13 | 8.01 | Pomiary wydajności | W13 | — |
| L14 | 15.01 | Poprawy, zaliczenia; MPI — rozszerzenia | W12 | — |

Materiały lokalnie: `~/studia/sem5/programowanie_rownolegle/materialy/` (README z tabelą, PDF-y, tekst).

## Tematy
- [[Kompilacja i pomiar czasu wykonania]]
- [[Proces i wątek]]
- [[Wyścig danych]]
- [[Zakleszczenie]]
- [[Mutex i zmienna warunkowa]]
- [[Pthreads]]
- [[Współbieżność w Javie]]
- [[OpenMP]]
- [[MPI]]
- [[Prawo Amdahla]]
- [[Przyspieszenie i efektywność]]
- [[PR W01 — Procesy i wątki]]
- [[PR W02 — Wątki i model SPMD]]
- [[PR W03 — Współbieżność]]
- [[PR W04 — Tworzenie programów równoległych]]
- [[PR W05 — Wątki w Javie]]
- [[PR W06 — Monitory i zmienne warunku]]
- [[PR L02 — fork i clone]]
- [[PR L03 — pthread_create i atrybuty wątków]]
- [[PR L04 — Muteksy i symulacja pubu]]
- [[PR L05 — Całka w Pthreads]]

## Postęp
```dataviewjs
const me = dv.current();
const topics = [...new Set((me.file.outlinks ?? []).map(l => l.path))];
const rows = topics.map(p => { const n = dv.page(p);
  return [n ? n.file.link : dv.fileLink(p.replace(/\.md$/, "")), n?.status ?? "brak notatki"]; });
const icon = {"umiem": "✅", "uczę się": "🟡", "nowe": "🟠", "brak notatki": "⬜"};
const c = s => rows.filter(r => r[1] === s).length;
const done = c("umiem"), all = rows.length || 1;
dv.paragraph(`**Postęp:** ${"▰".repeat(Math.round(10*done/all))}${"▱".repeat(10-Math.round(10*done/all))} ${done}/${rows.length} · ✅ ${done} · 🟡 ${c("uczę się")} · 🟠 ${c("nowe")} · ⬜ ${c("brak notatki")}`);
dv.table(["Pojęcie", "Status"], rows.map(r => [r[0], `${icon[r[1]] ?? ""} ${r[1]}`]));
```
