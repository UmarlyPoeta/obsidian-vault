<%*
const uczelnia = await tp.system.suggester(["AGH", "PK"], ["AGH", "PK"], false, "Uczelnia") ?? "";
const semestr = (await tp.system.prompt("Semestr (liczba)")) ?? "";
const forma = await tp.system.suggester(["egzamin", "zaliczenie"], ["egzamin", "zaliczenie"], false, "Forma") ?? "";
-%>
---
typ: przedmiot
uczelnia: <% uczelnia %>
semestr: <% semestr %>
ects:
forma: <% forma %>
tags: [przedmiot]
---
# <% tp.file.title %>

## Tematy
- <% tp.file.cursor() %>

## Postęp
```dataview
TABLE WITHOUT ID file.link AS "Pojęcie", status AS "Status", length(file.outlinks) AS "Linki"
FROM "Pojęcia"
WHERE contains(przedmioty, this.file.link)
SORT choice(status = "umiem", 3, choice(status = "uczę się", 2, 1)) ASC, file.name ASC
```
