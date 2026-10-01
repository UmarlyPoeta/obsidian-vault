---
typ: start
cssclasses: [start]
---
# 🧠 Wiedza — AGH IT sem. 5 + PK AiR rok 1

> [!intuicja] Jak z tego korzystać
> **Głosem albo w terminalu z Albertem:**
> - *„ej albert, wytłumacz mi transformatę Laplace'a”* — robi lekcję-notatkę w `Pojęcia/`, linkuje do przedmiotów i wymagań, potem pyta cię kontrolnie.
> - *„rozwiń notatkę Liczby zespolone”* — zamienia zalążek w pełną lekcję.
> - *„przepytaj mnie z Laplace'a”* / *„co mam do powtórki?”* / *„czego mi brakuje z matmy na PK?”*
> - *„umiem już Laplace'a”* — zmienia status na ✅.
>
> **W Obsidianie:** `Ctrl+P → Spaced Repetition: Review flashcards` — powtórka fiszek. `Ctrl+G` — graf wiedzy (🟠 nowe, 🟡 uczę się, 🟢 umiem, 🟣 przedmioty, blade kropki = tematy bez notatki). Kliknięcie w niebieski link bez notatki tworzy ją od razu.
>
> - *„przygotuj mnie na jutro”*: co będzie na ćwiczeniach i labach, minimum teorii, pytania na wejściówkę.
> - *„pogadajmy po angielsku”*: rozmowa po angielsku z poprawkami. Słówka są w [[Słówka]], zasady w [[Angielski PK]].
>
> **Terminal:** `wiedza status`, `wiedza luki pk`, `wiedza powtorka`, `wiedza przygotuj jutro`, `wiedza sprawdz --wszystko`, `plan jutro`, `angielski on`.

## Najbliższe zajęcia
```dataview
TABLE WITHOUT ID file.link AS "Przygotowanie", data AS "Dzień"
FROM "Przygotowanie"
WHERE date(data) >= date(today)
SORT data ASC
LIMIT 3
```
*„ej albert, przygotuj mnie na jutro”* robi taką notatkę z planu i sylabusa. Liczą się tylko ćwiczenia i laby.

## Przedmioty
```dataviewjs
const icon = {"umiem": "✅", "uczę się": "🟡", "nowe": "🟠"};
for (const [uni, sem] of [["PK", 1], ["PK", 2], ["AGH", 5]]) {
  const subs = dv.pages('"Przedmioty"').where(p => p.uczelnia === uni && p.semestr === sem).sort(p => p.forma === "egzamin" ? 0 : 1);
  if (!subs.length) continue;
  dv.header(3, `${uni} — semestr ${sem}`);
  dv.table(["Przedmiot", "Forma", "ECTS", "Postęp"], subs.map(s => {
    const t = [...new Set((s.file.outlinks ?? []).map(l => l.path))].map(p => dv.page(p)?.status ?? "brak");
    const done = t.filter(x => x === "umiem").length, n = t.length || 1;
    const bar = "▰".repeat(Math.round(8 * done / n)) + "▱".repeat(8 - Math.round(8 * done / n));
    return [s.file.link, s.forma === "egzamin" ? "**egzamin**" : "zal.", s.ects,
            `${bar} ${done}/${t.length} · 🟡${t.filter(x => x === "uczę się").length} 🟠${t.filter(x => x === "nowe").length} ⬜${t.filter(x => x === "brak").length}`];
  }));
}
```

## W trakcie nauki
```dataview
TABLE WITHOUT ID file.link AS "Pojęcie", status AS "Status", przedmioty AS "Przedmioty"
FROM "Pojęcia"
WHERE status = "uczę się" OR (status = "nowe" AND !contains(tags, "zalążek"))
SORT file.mtime DESC
LIMIT 12
```

## Zalążki do rozwinięcia
```dataview
LIST
FROM "Pojęcia" AND #zalążek
SORT file.name ASC
```

## Ostatnio zmieniane
```dataview
TABLE WITHOUT ID file.link AS "Notatka", dateformat(file.mtime, "dd.MM HH:mm") AS "Zmiana"
FROM "Pojęcia"
SORT file.mtime DESC
LIMIT 8
```
