---
typ: przedmiot
uczelnia: PK
semestr: 2
ects: 5
forma: zaliczenie
prowadzący: "Marek Kowalski"
ocena: "0,5·ćwiczenia (2 kolokwia) + 0,5·laby (test + sprawozdania)"
tags: [przedmiot, pk]
---
# PK Elektrotechnika i miernictwo

> [!definicja] Zaliczenie
> **Zaliczenie, 5 ECTS.** 0,5·ćwiczenia (2 kolokwia) + 0,5·laby (test + sprawozdania)
> Prowadzący: Marek Kowalski. Pełny sylabus: `~/studia/sylabus/pk-air.md`.
> Nowość (na AGH tylko elektromagnetyzm z Fizyki II).

## Tematy
- [[Prawa Kirchhoffa]]
- [[Metoda prądów oczkowych]]
- [[Metoda potencjałów węzłowych]]
- [[Twierdzenie Thevenina i Nortona]]
- [[Obwód magnetyczny]]
- [[Transformator]]
- [[Metoda symboliczna]]
- [[Rezonans elektryczny]]
- [[Moc czynna, bierna i pozorna]]
- [[Układy trójfazowe]]
- [[Prostowniki]]
- [[Liczby zespolone]]
- [[Stany nieustalone w obwodach]]

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
