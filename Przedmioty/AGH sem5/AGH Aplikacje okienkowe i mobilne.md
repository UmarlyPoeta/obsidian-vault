---
typ: przedmiot
uczelnia: AGH
semestr: 5
ects: 4
forma: zaliczenie
prowadzący: "Łukasz Rauch"
ocena: "średnia z ocen za każde warsztaty"
tags: [przedmiot, agh]
---
# AGH Aplikacje okienkowe i mobilne

> [!definicja] Zaliczenie
> **Zaliczenie, 4 ECTS.** średnia z ocen za każde warsztaty
> Prowadzący: Łukasz Rauch. Pełny sylabus: `~/studia/sylabus/semestr5.md`.


## Tematy
- [[GUI desktopowe]]
- [[Gniazda sieciowe]]
- [[Serwer stanowy i bezstanowy]]
- [[Aplikacja mobilna]]
- [[Sensory w smartfonie]]

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
