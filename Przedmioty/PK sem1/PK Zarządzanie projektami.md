---
typ: przedmiot
uczelnia: PK
semestr: 1
ects: 2
forma: zaliczenie
prowadzący: "Elżbieta Malec"
ocena: "średnia (wykład, projekt, seminarium)"
tags: [przedmiot, pk]
---
# PK Zarządzanie projektami

> [!definicja] Zaliczenie
> **Zaliczenie, 2 ECTS.** średnia (wykład, projekt, seminarium)
> Prowadzący: Elżbieta Malec. Pełny sylabus: `~/studia/sylabus/pk-air.md`.


## Tematy
- [[Cykl życia projektu]]
- [[Trójkąt projektowy]]
- [[Wykres Gantta]]
- [[Metoda ścieżki krytycznej]]
- [[Scrum]]
- [[Ocena opłacalności inwestycji]]

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
