---
typ: przedmiot
uczelnia: {{uczelnia}}
semestr: {{semestr}}
ects:
forma:
tags: [przedmiot]
---
# {{tytul}}

## Tematy
- [[…]]

## Postęp
```dataview
TABLE WITHOUT ID file.link AS "Pojęcie", status AS "Status", length(file.outlinks) AS "Linki"
FROM "Pojęcia"
WHERE contains(przedmioty, this.file.link)
SORT choice(status = "umiem", 3, choice(status = "uczę się", 2, 1)) ASC, file.name ASC
```
