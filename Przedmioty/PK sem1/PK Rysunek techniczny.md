---
typ: przedmiot
uczelnia: PK
semestr: 1
ects: 3
forma: zaliczenie
prowadzący: "Paweł Romanowicz"
ocena: "średnia ważona ocen formujących (testy, kolokwium, projekty i rysunki)"
tags: [przedmiot, pk]
---
# PK Rysunek techniczny

> [!definicja] Zaliczenie
> **Zaliczenie, 3 ECTS.** średnia ważona ocen formujących (testy, kolokwium, projekty i rysunki)
> Prowadzący: Paweł Romanowicz. Pełny sylabus: `~/studia/sylabus/pk-air.md`.
> Zupełna nowość. Podręcznik prowadzącego: Romanowicz „Rysunek techniczny maszynowy z elementami CAD”.

## Tematy
- [[Rzutowanie prostokątne]]
- [[Aksonometria]]
- [[Wymiarowanie]]
- [[Przekroje i kłady]]
- [[Tolerancje i pasowania]]
- [[Tolerancje kształtu i położenia]]
- [[Chropowatość powierzchni]]
- [[Gwinty na rysunku]]
- [[Rysunek złożeniowy]]

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
