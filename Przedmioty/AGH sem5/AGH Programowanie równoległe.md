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
