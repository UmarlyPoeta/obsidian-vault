---
typ: przedmiot
uczelnia: AGH
semestr: 5
ects: 5
forma: egzamin
prowadzący: "Jan Kusiak (wykład), Dorota Byrska-Wójcik i Mateusz Janik (ćwiczenia projektowe)"
ocena: "0,4·ćwiczenia + 0,6·egzamin (UPeL 2026/27)"
upel: "Optymalizacja IT/IO (course/view.php?id=282)"
tags: [przedmiot, agh]
---
# AGH Optymalizacja

> [!definicja] Zaliczenie
> **Egzamin, 5 ECTS.** Wg zasad na UPeL (2026/27): **ocena końcowa = 0,4·ćwiczenia + 0,6·egzamin**. Sylabus mówił o średniej arytmetycznej, ale obowiązuje to, co ogłosił prowadzący.
> Prowadzący: prof. Jan Kusiak (wykład, `kusiak@agh.edu.pl`), dr Dorota Byrska-Wójcik (`byrska@agh.edu.pl`) i mgr inż. Mateusz Janik (`mjanik@agh.edu.pl`) (ćwiczenia projektowe). Pełny sylabus: `~/studia/sylabus/semestr5.md`, terminy: `plan przedmiot "Optymalizacja"`.
> **Wykład:** czwartki 9:45–11:15, B-4 s. 122. **Konsultacje:** Kusiak czw. 11:30–12:30 (wg slajdów; na UPeL podano 12:00–13:00), B-4 p. 207, Byrska-Wójcik wt. 11:30–12:30, B5 p. 710, Janik czw. 13:00–14:00, B5 p. 511 (po umówieniu mailem/Teams).
> Opiera się na Analizie mat. I–II i [[AGH Metody numeryczne|Metodach numerycznych]]. Podręcznik prowadzącego: Kusiak, Danielewska-Tułecka, Oprocha „Optymalizacja. Wybrane metody z przykładami zastosowań” (PWN 2009).

## Zasady (UPeL 2026/27)
> [!wazne] Ćwiczenia projektowe
> - **6 ćwiczeń, każde na dwóch zajęciach**, z każdego **sprawozdanie oceniane 0–100 pkt**. Termin: dzień kolejnych zajęć, **−20 pkt za każdy dzień spóźnienia**. Identyczne (lub bardzo podobne) sprawozdania dostają 0 pkt **oba**.
> - Wszystkie ćwiczenia robisz w jednym projekcie C++ dr. Sztangreta, rozbudowywanym o kolejne metody: [[Framework optymalizacji (matrix, solution) w C++]]. Lab 0 (wprowadzający): [[Optymalizacja Lab 0 — Monte Carlo i wahadło]].
> - **Obecność obowiązkowa.** 2 nieusprawiedliwione nieobecności wolno, każda kolejna obniża ocenę z ćwiczeń o stopień, ponad 4 (albo ponad połowa zajęć) = jak nieuczęszczanie. Zwolnienie lekarskie do tygodnia po jego końcu (ksero lub wydruk z IKP). Nieobecność nie zwalnia ze sprawozdania.
> - Za mało punktów: dodatkowe zadanie w terminie poprawkowym (tylko jeśli chodziłeś na zajęcia).

> [!wazne] Egzamin i ocena końcowa
> - Do egzaminu tylko z zaliczonymi ćwiczeniami. 3 podejścia (1 podstawowe + 2 poprawkowe). Usprawiedliwienie nieobecności do 7 dni.
> - **Egzamin zerowy** (jeśli będzie, na wniosek roku): tylko dla ocen z ćwiczeń **4,5 lub 5,0**.
> - Ocena końcowa pozytywna tylko przy pozytywnych ćwiczeniach i egzaminie. Wzór $0{,}4 \cdot \text{ćw} + 0{,}6 \cdot \text{egz}$, przy czym ocena 2,0 wchodzi z wagą 0,1 (ćwiczenia) i 0,15 (egzamin). Przy obu pozytywnych ocena końcowa nie spada poniżej 3,0. Niepodejście = 2,0.

## Wykłady
- [[Optymalizacja W01 — wprowadzenie i klasyfikacja problemów]] (1.10.2026): po co optymalizacja, sformułowanie zadania, problem soków (programowanie liniowe), belka (Pareto), rzut oszczepem (złoty podział), komiwojażer (NP), Monte Carlo, klasyfikacje.
- Materiały: [[W01 Wprowadzenie (Kusiak 2026-27).pdf]].

## Tematy
- [[Optymalizacja W01 — wprowadzenie i klasyfikacja problemów]]
- [[Optymalizacja Lab 0 — Monte Carlo i wahadło]]
- [[Framework optymalizacji (matrix, solution) w C++]]
- [[Funkcja celu i warunki optymalności]]
- [[Metoda złotego podziału]]
- [[Metoda Fibonacciego]]
- [[Metoda Hooke'a-Jeevesa]]
- [[Metoda Rosenbrocka]]
- [[Metoda Powella]]
- [[Metoda simpleksu Neldera-Meada]]
- [[Metoda najszybszego spadku]]
- [[Metoda gradientów sprzężonych]]
- [[Metoda Newtona (optymalizacja)]]
- [[Metody quasi-newtonowskie]]
- [[Mnożniki Lagrange'a]]
- [[Metoda funkcji kary]]
- [[Metoda Monte Carlo]]
- [[Algorytm genetyczny]]
- [[Optymalizacja rojem cząstek]]
- [[Algorytm mrówkowy]]
- [[Symulowane wyżarzanie]]
- [[Optymalność Pareto]]
- [[Metamodelowanie]]
- [[Programowanie liniowe i metoda sympleks]]

## Postęp
```dataviewjs
const me = dv.current();
const topics = [...new Set((me.file.outlinks ?? []).map(l => l.path))].filter(p => !p.endsWith(".pdf") && !p.includes("Metody numeryczne"));
const rows = topics.map(p => { const n = dv.page(p);
  return [n ? n.file.link : dv.fileLink(p.replace(/\.md$/, "")), n?.status ?? "brak notatki"]; });
const icon = {"umiem": "✅", "uczę się": "🟡", "nowe": "🟠", "brak notatki": "⬜"};
const c = s => rows.filter(r => r[1] === s).length;
const done = c("umiem"), all = rows.length || 1;
dv.paragraph(`**Postęp:** ${"▰".repeat(Math.round(10*done/all))}${"▱".repeat(10-Math.round(10*done/all))} ${done}/${rows.length} · ✅ ${done} · 🟡 ${c("uczę się")} · 🟠 ${c("nowe")} · ⬜ ${c("brak notatki")}`);
dv.table(["Pojęcie", "Status"], rows.map(r => [r[0], `${icon[r[1]] ?? ""} ${r[1]}`]));
```
