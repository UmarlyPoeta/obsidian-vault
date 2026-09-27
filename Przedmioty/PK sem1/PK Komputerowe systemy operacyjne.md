---
typ: przedmiot
uczelnia: PK
semestr: 1
ects: 2
forma: zaliczenie
prowadzący: "Maciej Dudek"
ocena: "0,3·test z wykładu + 0,7·średnia sprawozdań (każde ≥ 51%)"
tags: [przedmiot, pk]
---
# PK Komputerowe systemy operacyjne

> [!definicja] Zaliczenie
> **Zaliczenie, 2 ECTS.** 0,3·test z wykładu + 0,7·średnia sprawozdań (każde ≥ 51%)
> Prowadzący: Maciej Dudek. Pełny sylabus: `~/studia/sylabus/pk-air.md`.
> Na AGH: Systemy operacyjne (sem. 2) — kandydat do przepisania.

## Tematy
- [[Proces i wątek]]
- [[Planowanie procesów]]
- [[Pamięć wirtualna]]
- [[System plików]]
- [[Potoki i przekierowania]]
- [[Skrypty bash]]

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
