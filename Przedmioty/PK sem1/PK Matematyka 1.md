---
typ: przedmiot
uczelnia: PK
semestr: 1
ects: 6
forma: egzamin
prowadzący: "Adam Bednarz"
ocena: "0,4·ćwiczenia + 0,4·egzamin zadaniowy + 0,2·egzamin teoretyczny (każda część ≥ 3,0; ćwiczenia ≥ 50% punktów)"
tags: [przedmiot, pk]
---
# PK Matematyka 1

> [!definicja] Zaliczenie
> **Egzamin, 6 ECTS.** 0,4·ćwiczenia + 0,4·egzamin zadaniowy + 0,2·egzamin teoretyczny (każda część ≥ 3,0; ćwiczenia ≥ 50% punktów)
> Prowadzący: Adam Bednarz. Pełny sylabus: `~/studia/sylabus/pk-air.md`.
> Na AGH: Analiza I–II i Algebra — większość już znasz, traktuj jako powtórkę.

## Tematy
- [[Logika matematyczna]]
- [[Teoria zbiorów]]
- [[Algebra Boole'a]]
- [[Funkcje elementarne]]
- [[Ciągi liczbowe]]
- [[Szeregi liczbowe]]
- [[Granica funkcji]]
- [[Pochodna]]
- [[Funkcje wielu zmiennych]]
- [[Pochodne cząstkowe]]
- [[Całka nieoznaczona]]
- [[Całka oznaczona]]
- [[Całka niewłaściwa]]
- [[Liczby zespolone]]
- [[Rachunek wektorowy]]
- [[Geometria analityczna]]

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
