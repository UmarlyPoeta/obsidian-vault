---
typ: dzień
data: {{date}}
---
# {{date:dddd, D MMMM YYYY}}

## Plan na dziś
- [ ] 

## Czego się dziś nauczyłem
- 

## Ruszone notatki
```dataview
LIST
FROM "Pojęcia" OR "Projekty" OR "Solar" OR "Przedmioty"
WHERE file.mday = this.file.day OR file.cday = this.file.day
SORT file.mtime DESC
```

## Pomodoro
