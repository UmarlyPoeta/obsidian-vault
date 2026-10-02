---
typ: przedmiot
uczelnia: AGH
semestr: 5
ects: 3
forma: zaliczenie
prowadzący: "Gabriel Rojek"
ocena: "suma punktów z laboratorium: 9 ocen po 10 pkt (projekt wstępny, 4 kartkówki, 4 studia) = 90 pkt, zaliczenie od 45; obecność obowiązkowa"
tags: [przedmiot, agh]
---
# AGH Wzorce projektowe

> [!definicja] Zaliczenie
> **Zaliczenie, 3 ECTS.** Ocena = punkty z laboratorium: **90 pkt, zaliczenie od 45** (szczegóły w „Przebieg i punkty”); obecność obowiązkowa.
> Prowadzący: Gabriel Rojek. Pełny sylabus: `~/studia/sylabus/semestr5.md`, terminy: `plan przedmiot "Wzorce projektowe"`.
> Laby to jeden projekt zespołowy: UML (przypadki użycia, klasy, sekwencje) → wzorce kreacyjne → strukturalne → czynnościowe → refaktoryzacja pod antywzorce, SOLID, GRASP. Z każdej grupy wzorców i z SOLID/GRASP jest kartkówka na początku zajęć. Opiera się na Programowaniu obiektowym (sem. 2) i Inżynierii oprogramowania (sem. 4). Literatura: GoF, „Rusz głową! Wzorce projektowe”.

## Przebieg i punkty
Projekt: [[WP — Anticheat w grach]] (z Oliwierem Kruczkiem). Narzędzie: Visual Paradigm CE. Źródło: `~/studia/sem5/wzorce_projektowe/materialy/przebieg-i-ocenianie.md`.

| Zaj. | Data (pt 11:30, B5 s. 402) | Zakres | Pkt |
|---|---|---|---|
| 1 | 2.10 | Organizacja, VP CE, zespoły, temat | — |
| 2 | 9.10 | Diagram przypadków użycia | — |
| 3 | 16.10 | Diagram klas | — |
| 4 | 23.10 | Diagramy sekwencji | — |
| 5 | 30.10 | Ocena wstępnego projektu (PU + klasy + sekwencje, bez wzorców) | 10 |
| 6 | 6.11 | Kartkówka: wzorce kreacyjne; wdrożenie 2 wzorców | 10 |
| 7 | 13.11 | Ocena studium kreacyjnych | 10 |
| 8 | 20.11 | Kartkówka: wzorce strukturalne; wdrożenie 2 wzorców | 10 |
| 9 | 27.11 | Ocena studium strukturalnych | 10 |
| 10 | 4.12 | Kartkówka: wzorce czynnościowe; wdrożenie 2 wzorców | 10 |
| 11 | 11.12 | Ocena studium czynnościowych | 10 |
| 12 | 18.12 | Kartkówka: SOLID i GRASP | 10 |
| 13 | 8.01 | Ocena studium pięciu punktów (SOLID / GRASP / antywzorce) | 10 |
| 14 | 15.01 | Uzupełnienia, oceny | — |

> [!wazne] Zasady
> - **Suma 90 pkt, zaliczenie od 45.** Ocena końcowa wynika z punktów (przeliczenie według regulaminu studiów AGH).
> - Kartkówek i zadań **nie da się poprawić**; spóźnienie z oddaniem kosztuje **−2 pkt za każdy tydzień**.
> - Obecność obowiązkowa; kartkówki zaległe z powodu nieobecności pisze się na zaj. 14.
> - Studium wzorca ma 4 części: motywacja, fragment przed, struktura po, współdziałanie (sekwencja).

## Tematy
- [[Diagram przypadków użycia UML]]
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
const topics = [...new Set((me.file.outlinks ?? []).map(l => l.path))].filter(p => dv.page(p)?.typ !== "projekt");
const rows = topics.map(p => { const n = dv.page(p);
  return [n ? n.file.link : dv.fileLink(p.replace(/\.md$/, "")), n?.status ?? "brak notatki"]; });
const icon = {"umiem": "✅", "uczę się": "🟡", "nowe": "🟠", "brak notatki": "⬜"};
const c = s => rows.filter(r => r[1] === s).length;
const done = c("umiem"), all = rows.length || 1;
dv.paragraph(`**Postęp:** ${"▰".repeat(Math.round(10*done/all))}${"▱".repeat(10-Math.round(10*done/all))} ${done}/${rows.length} · ✅ ${done} · 🟡 ${c("uczę się")} · 🟠 ${c("nowe")} · ⬜ ${c("brak notatki")}`);
dv.table(["Pojęcie", "Status"], rows.map(r => [r[0], `${icon[r[1]] ?? ""} ${r[1]}`]));
```
