---
typ: przedmiot
uczelnia: PK
semestr: 2
ects: 6
forma: egzamin
prowadzący: "Adam Bednarz"
ocena: "0,4·ćwiczenia + 0,4·egzamin zadaniowy + 0,2·egzamin teoretyczny"
tags: [przedmiot, pk]
---
# PK Matematyka 2

> [!definicja] Zaliczenie
> **Egzamin, 6 ECTS.** 0,4·ćwiczenia + 0,4·egzamin zadaniowy + 0,2·egzamin teoretyczny
> Prowadzący: Adam Bednarz. Pełny sylabus: `~/studia/sylabus/pk-air.md`.
> Nowość względem AGH: transformaty Laplace'a i Fouriera, RRCz. Reszta — Algebra, Analiza II, Metody numeryczne, Statystyczna analiza danych.

## Tematy
- [[Macierze]]
- [[Wyznacznik]]
- [[Układy równań liniowych]]
- [[Odwzorowania liniowe]]
- [[Szeregi funkcyjne]]
- [[Szereg Fouriera]]
- [[Równanie różniczkowe zwyczajne]]
- [[Równanie różniczkowe liniowe II rzędu]]
- [[Transformata Laplace'a]]
- [[Transformata Fouriera]]
- [[Równania różniczkowe cząstkowe]]
- [[Rachunek prawdopodobieństwa]]
- [[Statystyka]]

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
