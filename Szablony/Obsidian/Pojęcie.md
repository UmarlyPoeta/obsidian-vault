<%*
// Nazwa: jeśli notatka powstała jako „Bez nazwy”, zapytaj.
let title = tp.file.title;
if (/^(Bez nazwy|Untitled)/.test(title)) {
  const t = await tp.system.prompt("Nazwa pojęcia");
  if (t) { title = t.trim(); await tp.file.rename(title); }
}
// Przedmiot: wybór z Przedmioty/
const subs = app.vault.getMarkdownFiles().filter(f => f.path.startsWith("Przedmioty/")).map(f => f.basename).sort();
const subj = await tp.system.suggester(["— bez przedmiotu —", ...subs], [null, ...subs], false, "Przedmiot");
const przedmioty = subj ? `["[[${subj}]]"]` : "[]";
// Talia fiszek: istniejące #fiszki/… albo nowa
const decks = Object.keys(app.metadataCache.getTags()).filter(t => t.startsWith("#fiszki/")).map(t => t.slice(8)).sort();
let deck = await tp.system.suggester([...decks, "➕ nowa talia…"], [...decks, "+"], false, "Talia fiszek");
if (deck === "+") deck = (await tp.system.prompt("Nazwa talii (np. automatyka)"))?.trim().toLowerCase().replace(/\s+/g, "-");
deck = deck || "temat";
-%>
---
typ: pojęcie
status: nowe
przedmioty: <% przedmioty %>
wymaga: []
powiązane: []
tags: [pojęcie]
utworzono: <% tp.date.now("YYYY-MM-DD") %>
źródła: []
---
# <% title %>

> [!skrot] W 30 sekund
> - **Co to:** <% tp.file.cursor() %>
> - **Klucz:** najważniejszy wzór / reguła / komenda.
> - **Kiedy:** do czego tego używasz.

> [!intuicja] Po ludzku
> 2–4 zdania i jedna analogia z życia.

## Definicja i wzory
> [!definicja]
> …

## Jak to zrobić — krok po kroku
1. …

## Przykład
Jeden przykład rozwiązany do końca, z każdym krokiem. Kolejne zwinięte:
> [!przyklad]- Przykład 2
> …

## Typowe błędy
> [!blad] …

## Na zajęcia i egzamin
> [!zajecia]
> - **Ćwiczenia/laby:** co konkretnie musisz umieć zrobić.
> - **Egzamin/kolokwium:** typowe pytanie.
> - Łączy się z: [[…]]

## Zadania
> [!zadanie] Zadanie 1
> …
> > [!odpowiedz]- Odpowiedź
> > …

## Fiszki #fiszki/<% deck %>
Pytanie::Odpowiedź

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.

## Źródła
- …
