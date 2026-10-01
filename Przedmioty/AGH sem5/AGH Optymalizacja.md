---
typ: przedmiot
uczelnia: AGH
semestr: 5
ects: 5
forma: egzamin
prowadzący: "Jan Kusiak (wykład), Dorota Byrska-Wójcik (projekt)"
ocena: "średnia arytmetyczna: ćwiczenia (projekt) i egzamin"
tags: [przedmiot, agh]
---
# AGH Optymalizacja

> [!definicja] Zaliczenie
> **Egzamin, 5 ECTS.** Średnia arytmetyczna: ćwiczenia (projekt) i egzamin.
> Prowadzący: Jan Kusiak (wykład), Dorota Byrska-Wójcik (projekt). Pełny sylabus: `~/studia/sylabus/semestr5.md`, terminy: `plan przedmiot "Optymalizacja"`.
> Projekt w zespole: własny program albo Matlab, sprawozdanie + prezentacja. Opiera się na Analizie mat. I–II i Metodach numerycznych (sem. 3). Podręcznik prowadzącego: Kusiak, Danielewska-Tułecka, Oprocha „Optymalizacja. Wybrane metody z przykładami zastosowań” (PWN 2009).

## Tematy
- [[Funkcja celu i warunki optymalności]]
- [[Metoda złotego podziału]]
- [[Metoda Fibonacciego]]
- [[Metoda Hooke'a-Jeevesa]]
- [[Metoda Rosenbrocka]]
- [[Metoda Powella]]
- [[Metoda simpleksu Neldera-Meada]]
- [[Metoda najszybszego spadku]]
- [[Metoda gradientów sprzężonych]]
- [[Metoda Newtona (optymalizacja)]]
- [[Metody quasi-newtonowskie]]
- [[Mnożniki Lagrange'a]]
- [[Metoda funkcji kary]]
- [[Metoda Monte Carlo]]
- [[Algorytm genetyczny]]
- [[Optymalizacja rojem cząstek]]
- [[Algorytm mrówkowy]]
- [[Symulowane wyżarzanie]]
- [[Optymalność Pareto]]
- [[Metamodelowanie]]
- [[Programowanie liniowe i metoda sympleks]]

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
