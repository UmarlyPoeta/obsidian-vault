---
typ: przedmiot
uczelnia: AGH
semestr: 5
ects: 3
forma: zaliczenie
prowadzący: "Gabriel Rojek"
ocena: "ocena z laboratorium (kolokwia + zadania projektowe); wszystkie laby muszą być zaliczone, obecność obowiązkowa"
tags: [przedmiot, agh]
---
# AGH Wzorce projektowe

> [!definicja] Zaliczenie
> **Zaliczenie, 3 ECTS.** Ocena z laboratorium (kolokwia + zadania projektowe); wszystkie laby muszą być zaliczone, obecność obowiązkowa.
> Prowadzący: Gabriel Rojek. Pełny sylabus: `~/studia/sylabus/semestr5.md`, terminy: `plan przedmiot "Wzorce projektowe"`.
> Laby to jeden projekt zespołowy: UML (przypadki użycia, klasy, sekwencje) → wzorce kreacyjne → strukturalne → czynnościowe → refaktoryzacja pod antywzorce, SOLID, GRASP. Przed labami kolokwium pisemne z przygotowania. Opiera się na Programowaniu obiektowym (sem. 2) i Inżynierii oprogramowania (sem. 4). Literatura: GoF, „Rusz głową! Wzorce projektowe”.

## Tematy
- [[Diagram klas UML]]
- [[Diagram sekwencji UML]]
- [[SOLID]]
- [[GRASP]]
- [[Singleton]]
- [[Metoda wytwórcza]]
- [[Fabryka abstrakcyjna]]
- [[Budowniczy]]
- [[Prototyp]]
- [[Adapter]]
- [[Dekorator]]
- [[Fasada]]
- [[Kompozyt]]
- [[Pełnomocnik]]
- [[Most]]
- [[Obserwator]]
- [[Strategia]]
- [[Polecenie]]
- [[Stan]]
- [[Metoda szablonowa]]
- [[Iterator]]
- [[Łańcuch zobowiązań]]
- [[Antywzorce projektowe]]

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
