---
typ: przedmiot
uczelnia: PK
semestr: 2
ects: 5
forma: egzamin
prowadzący: "Marek Kozień"
ocena: "średnia ważona: test (wykład), kolokwia (ćwiczenia), projekty (lab. komputerowe)"
tags: [przedmiot, pk]
---
# PK Mechanika ogólna

> [!definicja] Zaliczenie
> **Egzamin, 5 ECTS.** średnia ważona: test (wykład), kolokwia (ćwiczenia), projekty (lab. komputerowe)
> Prowadzący: Marek Kozień. Pełny sylabus: `~/studia/sylabus/pk-air.md`.
> Częściowo z Fizyki I na AGH; statyka i tarcie to nowość.

## Tematy
- [[Więzy i reakcje]]
- [[Tarcie]]
- [[Moment siły]]
- [[Redukcja układu sił]]
- [[Warunki równowagi]]
- [[Kinematyka punktu]]
- [[Ruch płaski bryły]]
- [[Zasady dynamiki Newtona]]
- [[Moment bezwładności]]
- [[Twierdzenie Steinera]]
- [[Zasada równoważności energii kinetycznej i pracy]]
- [[Zasada pędu i krętu]]
- [[Drgania mechaniczne]]

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
