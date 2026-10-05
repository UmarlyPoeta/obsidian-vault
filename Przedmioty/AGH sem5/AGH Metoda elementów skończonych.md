---
typ: przedmiot
uczelnia: AGH
semestr: 5
ects: 5
forma: egzamin
prowadzący: "Andriy Milenin, Piotr Kustra"
ocena: "0,4·zaliczenie + 0,6·egzamin"
tags: [przedmiot, agh]
---
# AGH Metoda elementów skończonych

> [!definicja] Zaliczenie
> **Egzamin, 5 ECTS.** 0,4·zaliczenie + 0,6·egzamin
> Prowadzący: Andriy Milenin, Piotr Kustra. Pełny sylabus: `~/studia/sylabus/semestr5.md`.
> Opiera się na Metodach numerycznych (sem. 3) i SMOC I/II.

## Laboratorium (dr Kustra)
- [[MES — kurs dr. Kustry (mapa labów)]]: plan 14 labów, zasady zaliczenia, materiały prowadzącego, siatki testowe i wyniki do porównania. Stamtąd jest też link do listy podręczników.
- **UPeL 2026/27** („Metoda Elementów Skończonych — ITE 2026/2027”, stan z 2026-10-05): zasady zaliczenia na nowy rok ([[00 Zasady zaliczenia 2026-27.pdf]]: kod i sprawozdanie **tylko przez UPEL**, maile nie są brane pod uwagę), „Wstęp” i „Zajęcia 1” (bez zmian względem kopii w vaulcie), opis siatek testowych z wynikami ABAQUS ([[12 Siatki MES — opis plików testowych.pdf]]). Siatka Test4 (trapez) jest opisana, ale pliku nie ma. Szczegóły w mapie labów.

## Tematy
- [[Kurs — Metody numeryczne]] (powtórka podstaw: m00–m08)
- [[Funkcje kształtu]]
- [[Siatka MES i wczytywanie danych]]
- [[Metoda Galerkina]]
- [[Całkowanie numeryczne i kwadratury Gaussa|Kwadratury Gaussa]]
- [[Jakobian]] (przekształcenie izoparametryczne)
- [[Macierz sztywności]] ($H$, $H_{bc}$, $P$, agregacja)
- [[Nieustalony przepływ ciepła]]
- [[Schematy całkowania w czasie]]
- [[MES 1D — od różnic skończonych do macierzy sztywności]]

## Postęp
```dataviewjs
const me = dv.current();
const topics = [...new Set((me.file.outlinks ?? []).map(l => l.path))].filter(p => !p.endsWith(".pdf"));
const rows = topics.map(p => { const n = dv.page(p);
  return [n ? n.file.link : dv.fileLink(p.replace(/\.md$/, "")), n?.status ?? "brak notatki"]; });
const icon = {"umiem": "✅", "uczę się": "🟡", "nowe": "🟠", "brak notatki": "⬜"};
const c = s => rows.filter(r => r[1] === s).length;
const done = c("umiem"), all = rows.length || 1;
dv.paragraph(`**Postęp:** ${"▰".repeat(Math.round(10*done/all))}${"▱".repeat(10-Math.round(10*done/all))} ${done}/${rows.length} · ✅ ${done} · 🟡 ${c("uczę się")} · 🟠 ${c("nowe")} · ⬜ ${c("brak notatki")}`);
dv.table(["Pojęcie", "Status"], rows.map(r => [r[0], `${icon[r[1]] ?? ""} ${r[1]}`]));
```
