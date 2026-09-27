---
typ: przedmiot
uczelnia: PK
semestr: 2
ects: 3
forma: zaliczenie
prowadzący: "Piotr Cyklis"
ocena: "0,1·test + 0,5·ćwiczenia + 0,4·laby (100% obecności na labach)"
tags: [przedmiot, pk]
---
# PK Termodynamika techniczna

> [!definicja] Zaliczenie
> **Zaliczenie, 3 ECTS.** 0,1·test + 0,5·ćwiczenia + 0,4·laby (100% obecności na labach)
> Prowadzący: Piotr Cyklis. Pełny sylabus: `~/studia/sylabus/pk-air.md`.
> Częściowo z Fizyki I i SMOC II na AGH.

## Tematy
- [[Równanie stanu gazu doskonałego]]
- [[Pierwsza zasada termodynamiki]]
- [[Entalpia]]
- [[Entropia]]
- [[Przemiany charakterystyczne gazu]]
- [[Obieg Carnota]]
- [[Obieg parowy]]
- [[Powietrze wilgotne]]

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
