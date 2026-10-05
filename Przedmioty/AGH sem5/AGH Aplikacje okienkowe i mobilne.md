---
typ: przedmiot
uczelnia: AGH
semestr: 5
ects: 4
forma: zaliczenie
prowadzący: "Łukasz Rauch (sylabus), Sandra Gajoch (laboratorium)"
ocena: "suma punktów z 6 programów (6 × 5 pkt), próg 50%"
upel: "Programowanie aplikacji okienkowych i mobilnych 2026 (PAOiM-26)"
tags: [przedmiot, agh]
---
# AGH Aplikacje okienkowe i mobilne

> [!definicja] Zaliczenie
> **Zaliczenie, 4 ECTS.** Prowadzący wg sylabusa: Łukasz Rauch. Pełny sylabus: `~/studia/sylabus/semestr5.md`.
> **Laboratorium:** mgr Sandra Gajoch, `sgajoch@agh.edu.pl`, konsultacje piątek 7:00–8:00, pok. 405 B5. Kurs na UPeL: **PAOiM-26** (hasło grupy przychodzi mailem).
> **Język: Java 11** (JDK), środowisko IntelliJ IDEA Ultimate (licencja studencka na mail `@agh.edu.pl` albo `@metal.agh.edu.pl`), na koniec Android Studio.

## Zasady laboratorium (z prezentacji organizacyjnej)
> [!wazne] Jak zdobywasz punkty
> - **6 programów × 5 pkt = 30 pkt.** Treść zadania jest dostępna przed zajęciami. Program oddajesz **na zajęciach**: pokazujesz, że działa, i odpowiadasz z kodu i teorii.
> - Prowadzący może poprosić o **zmianę albo dopisanie funkcji** na miejscu i zapytać o teorię. Zakres teorii jest podany w każdej instrukcji i **obowiązuje też na kolejnych labach**.
> - Nieznajomość własnego kodu albo niesamodzielna praca = **0 pkt**. Pojedynczych labów **nie da się poprawić**.
> - Spóźnienie: maksymalnie tydzień, potem spada maksimum punktów. Nieoddanie programu = 0 pkt, a **2 nieoddane programy = ocena 2,0**. Usprawiedliwiona (mailem) nieobecność nie obniża punktów.
> - **Po zaliczeniu na zajęciach** w ciągu **7 dni** wrzucasz źródła na UPeL, inaczej tracisz punkty. Tylko kody źródłowe i potrzebne pliki, bez katalogu projektu i plików binarnych.
> - Nazwa archiwum (wielkość liter ma znaczenie, polskie znaki jak w dziekanacie): **`LABx_GRy_NAZWISKO_IMIĘ.zip`**, np. `LAB1_GR4_GŻEGŻÓŁKA_ŁUCJAN.zip`. Dozwolone: zip, rar, 7zip, tar.gz.
> - Brak zaliczenia w terminie oznacza poprawkę w sesji: **1,5 h na napisanie programu**.

| % punktów | ocena |
|---|---|
| ≥ 90% | 5,0 |
| ≥ 80% | 4,5 |
| ≥ 70% | 4,0 |
| ≥ 60% | 3,5 |
| ≥ 50% | 3,0 |
| < 50% | 2,0 |

## Plan labów
| Lab | Temat projektu | Zagadnienia, które dochodzą |
|---|---|---|
| 1 | Klasa abstrakcyjna, dziedziczenie | interfejs, klasa abstrakcyjna, wyjątki → [[AOiM Lab 1 — funkcje (klasa abstrakcyjna i interfejs)]] |
| 2 | Kolekcje | listy, mapy, sety |
| 3 | Interfejs użytkownika | Swing lub JavaFX, własne klasy wyjątków |
| 4 | Hibernate | baza danych, testy jednostkowe, pokrycie kodu |
| 5 | Spring Boot | aplikacja klient–serwer |
| 6 | Aplikacja mobilna | Android |

W prezentacji pojawiają się też: **zapis do pliku** (serializacja, CSV) oraz **refleksje i adnotacje**. Prezentacja nie przypisuje ich jednoznacznie do konkretnych labów, więc trzymaj się instrukcji danego labu.

Materiały z UPeL: [[Zajęcia organizacyjne — zasady (Gajoch).pdf]], [[Lab 1 — klasa abstrakcyjna i interfejs (funkcje).pdf]].

## Tematy
- [[Java dla programisty C++ (JDK, JRE, JVM, typy)]]
- [[Klasa abstrakcyjna i interfejs (Java)]]
- [[AOiM Lab 1 — funkcje (klasa abstrakcyjna i interfejs)]]
- [[GUI desktopowe]]
- [[Gniazda sieciowe]]
- [[Serwer stanowy i bezstanowy]]
- [[Aplikacja mobilna]]
- [[Sensory w smartfonie]]

## Literatura (z prezentacji)
- *Java Notes for Professionals*: [goalkicker.com/JavaBook](http://goalkicker.com/JavaBook/) (darmowa).
- B. Eckel, *Thinking in Java* (podstawy).
- H. Schildt, *Java. The Complete Reference* (pl. *Java. Kompendium programisty*).
- [dev.java/learn](https://dev.java/learn/): oficjalny kurs Oracle.
- Konwencja nazewnicza na zajęciach: [Google Java Style Guide](https://google.github.io/styleguide/javaguide.html).

## Postęp
```dataviewjs
const me = dv.current();
const topics = [...new Set((me.file.outlinks ?? []).map(l => l.path))].filter(p => !p.endsWith(".pdf"));
const rows = topics.map(p => { const n = dv.page(p);
  return [n ? n.file.link : dv.fileLink(p.replace(/\.md$/, "")), n?.status ?? "brak notatki"]; });
const icon = {"umiem": "✅", "uczę się": "🟡", "nowe": "🟠", "brak notatki": "⬜"};
const c = s => rows.filter(r => r[1] === s).length;
const done = c("umiem"), all = rows.length || 1;
dv.paragraph(`**Postęp:** ${"▰".repeat(Math.round(10*done/all))}${"▱".repeat(10-Math.round(10*done/all))} ${done}/${rows.length} · ✅ ${done} · 🟡 ${c("uczę się")} · 🟠 ${c("nowe")} · ⬜ ${c("brak notatki")}`);
dv.table(["Pojęcie", "Status"], rows.map(r => [r[0], `${icon[r[1]] ?? ""} ${r[1]}`]));
```
