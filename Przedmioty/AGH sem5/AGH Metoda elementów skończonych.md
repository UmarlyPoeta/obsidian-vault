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

## Tematy
- [[Kurs — Metody numeryczne]] (powtórka podstaw: m00–m08)
- [[Funkcje kształtu]]
- [[Macierz sztywności]]
- [[Przekształcenie izoparametryczne]]
- [[Jakobian]]
- [[Kwadratury Gaussa]]
- [[Metoda Galerkina]]
- [[Nieustalony przepływ ciepła]]
- [[Schematy całkowania w czasie]]

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
