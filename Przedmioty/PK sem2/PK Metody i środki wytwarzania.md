---
typ: przedmiot
uczelnia: PK
semestr: 2
ects: 3
forma: zaliczenie
prowadzący: "Małgorzata Kowalczyk"
ocena: "średnia z wykładu i labów (progi 50/60/70/80/90%)"
tags: [przedmiot, pk]
---
# PK Metody i środki wytwarzania

> [!definicja] Zaliczenie
> **Zaliczenie, 3 ECTS.** średnia z wykładu i labów (progi 50/60/70/80/90%)
> Prowadzący: Małgorzata Kowalczyk. Pełny sylabus: `~/studia/sylabus/pk-air.md`.
> Nowość (na AGH była przeróbka plastyczna, nie skrawanie).

## Tematy
- [[Toczenie]]
- [[Frezowanie]]
- [[Wiercenie]]
- [[Szlifowanie]]
- [[Parametry skrawania]]
- [[Zużycie ostrza]]
- [[Obróbka elektroerozyjna]]
- [[Warstwa wierzchnia]]

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
