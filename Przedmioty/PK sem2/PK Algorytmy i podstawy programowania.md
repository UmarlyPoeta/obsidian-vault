---
typ: przedmiot
uczelnia: PK
semestr: 2
ects: 3
forma: zaliczenie
prowadzący: "Krzysztof Wójcik"
ocena: "0,3·kolokwium + 0,7·projekt (każda ocena pozytywna)"
tags: [przedmiot, pk]
---
# PK Algorytmy i podstawy programowania

> [!definicja] Zaliczenie
> **Zaliczenie, 3 ECTS.** 0,3·kolokwium + 0,7·projekt (każda ocena pozytywna)
> Prowadzący: Krzysztof Wójcik. Pełny sylabus: `~/studia/sylabus/pk-air.md`.
> Na AGH: Podstawy programowania + ASD — kandydat do przepisania.

## Tematy
- [[Złożoność obliczeniowa]]
- [[Wskaźniki w C]]
- [[Pamięć dynamiczna w C]]
- [[Algorytmy sortowania]]
- [[Lista jednokierunkowa]]
- [[Drzewo BST]]
- [[Metoda Eulera]]

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
