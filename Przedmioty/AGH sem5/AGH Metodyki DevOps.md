---
typ: przedmiot
uczelnia: AGH
semestr: 5
ects: 5
forma: egzamin
prowadzący: "Kazimierz Michalik"
ocena: "0,45·laby + 0,55·egzamin (+0,5 za aktywność)"
tags: [przedmiot, agh]
---
# AGH Metodyki DevOps

> [!definicja] Zaliczenie
> **Egzamin, 5 ECTS.** 0,45·laby + 0,55·egzamin (+0,5 za aktywność)
> Prowadzący: Kazimierz Michalik. Pełny sylabus: `~/studia/sylabus/semestr5.md`.


## Tematy
- [[Strategie gałęzi w Git]]
- [[Git hooks]]
- [[Submoduły Git]]
- [[Konteneryzacja]]
- [[Ansible]]
- [[CI/CD]]
- [[Infrastruktura jako kod]]
- [[SLA i monitoring]]

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
