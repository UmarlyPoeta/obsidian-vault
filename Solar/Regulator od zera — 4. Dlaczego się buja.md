---
typ: solar-nauka
pakiet: "[[regulator]]"
tags: [solar, regulator, oscylacje, nauka]
zaktualizowano: 2026-10-05
---
# Regulator od zera — 4. Dlaczego łódź się buja

> [!skrot] W 30 sekund
> - Łódź „pływa wężykiem” przez **trzy rzeczy naraz**: schodkową trasę z plannera, za krótki lookahead (≤ 1 m, a przy błędzie jeszcze krótszy) i wzmocnienie ×15000, które robi z regulatora włącznik.
> - **Każda z nich osobno** byłaby do przeżycia. Razem tworzą pętlę: przestrzelenie → większy błąd → ostrzejsza reakcja → większe przestrzelenie.
> - **Naprawa w skrócie:** prostsza trasa (line-of-sight), dłuższy i stały lookahead (3–6 m), mniejsze wzmocnienie (×5), ogranicznik narastania ciągu.
>
> Seria: [[Regulator od zera — 1. Co to jest i Simulink]] → [[Regulator od zera — 2. Matematyka]] → [[Regulator od zera — 3. Logika rozmyta]] → **4. Dlaczego się buja**
> Wersja techniczna z kodem poprawek: [[Regulator — TODO, błędy i oscylacje]].

## 1. Analogia: jazda rowerem, patrząc pod przednie koło

Jak jedziesz rowerem i patrzysz **metr przed siebie**, każdy kamyczek i każda krzywa linia na asfalcie powoduje szarpnięcie kierownicą. Jak patrzysz **10 metrów przed siebie**, jedziesz płynnie. Nasz regulator patrzy metr przed siebie, a jak się przestraszy (duży błąd), to patrzy jeszcze bliżej.

## 2. Błędne koło

```mermaid
flowchart TB
  A["🧱 Trasa z A* ma schodki<br/>(zmiana kierunku co 45°)"] --> B["🥕 Marchewka jest 1 m przed łodzią,<br/>więc wpada na każdy schodek"]
  B --> C["📐 Duży błąd kierunku α"]
  C --> D["🔻 Lookahead się kurczy<br/>(L = 1 → 0.3 → 0.1 m)"]
  D --> E["🌀 ω_pp = 2·sinα / L rośnie<br/>→ od razu limit 1 rad/s"]
  E --> F["💥 Fuzzy ×15000<br/>→ pełny ciąg różnicowy ±1000"]
  F --> G["🚤 Łódź obraca się za mocno<br/>(bezwładność!)"]
  G --> H["↩️ Przestrzelenie na drugą stronę"]
  H --> C
  R["🔄 Co ~0.5 s planner liczy trasę od nowa<br/>i schodki przeskakują"] --> A
  classDef bad fill:#f8dcdc,stroke:#d62728,color:#000
  class D,E,F bad
```

Czerwone klocki to miejsca w samym regulatorze. Pierwszy klocek (schodki) to planner.

## 3. Problem 1: trasa ze schodkami

Planner liczy trasę algorytmem A* na siatce kratek. Z każdej kratki można iść tylko w 8 kierunkach (co 45°). Trasa pod kątem np. 30° wychodzi jako schodki. Potem planner wrzuca do `/path_points` **każde** miejsce zmiany kierunku.

![[11_sciezka_schodki_vs_los.png]]

Po prawej propozycja „line-of-sight” (przycinanie po linii widzenia): z każdego punktu skacz do najdalszego punktu, do którego da się dojechać prosto bez wjeżdżania w przeszkodę. Zostają tylko punkty naprawdę potrzebne.

```mermaid
flowchart LR
  A["punkt i"] --> B{"czy da się prosto<br/>do punktu j<br/>(najdalszego)?"}
  B -->|"tak"| C["dodaj j, i = j"]
  B -->|"nie"| D["j = j − 1"] --> B
  C --> E{"koniec trasy?"}
  E -->|"nie"| A
  E -->|"tak"| F["gotowe:<br/>kilka punktów zamiast kilkunastu"]
```

Dodatkowo w A* jest błąd: kara za skręt miała zniechęcać do schodków, ale przez dzielenie całkowite (`/5` zamiast `/5.0`) zawsze wynosi 0.

## 4. Problem 2: za krótki lookahead

![[03_lookahead_omega.png]]

Lewy wykres: im większy błąd, tym krótszy lookahead. Prawy: ω_ref jest w limicie już od ~18° błędu. Czyli przy prawie każdym skręcie regulator „krzyczy” na maksa.

Zabawkowa symulacja: ten sam prosty model łodzi (z opóźnieniem w obrocie, bo prawdziwa łódź też nie obraca się natychmiast), ta sama schodkowa trasa, dwa ustawienia:

![[10_symulacja_schodki.png]]

![[10b_symulacja_omega.png]]

- **Czerwona** (jak teraz): łódź wiernie odtwarza każdy schodek, a prędkość obrotu skacze lewo-prawo-lewo-prawo co ~2 s.
- **Zielona** (L = 4 m): łódź jedzie po „uśrednionej” linii, prędkość obrotu prawie zero.

> [!wazne] To tylko ilustracja
> Model łodzi w symulacji jest wymyślony (prosty model kinematyczny z opóźnieniem τ = 0.8 s, bez fuzzy, bez wody i wiatru). Pokazuje mechanizm, nie prawdziwe liczby. Prawdziwe strojenie trzeba robić w [[l4sim]] i na wodzie.

## 5. Problem 3: włącznik zamiast pokrętła

![[09_ciag_vs_blad.png]]

Przy ×15 ciąg różnicowy jest pełny już przy błędzie obrotu ~0.2 rad/s. Łódź ma bezwładność: zanim zacznie się obracać, regulator już daje full, a zanim przestanie, już przestrzeliła. To klasyczny przepis na oscylacje.

```mermaid
flowchart LR
  subgraph TERAZ["Teraz: włącznik"]
    t1["błąd mały"] --> t2["−1000 / +1000"]
    t3["błąd duży"] --> t2
  end
  subgraph LEPIEJ["Po zmianie: pokrętło"]
    l1["błąd mały"] --> l2["−150 / +150"]
    l3["błąd duży"] --> l4["−870 / +870"]
  end
```

## 6. Co zmienić (od najłatwiejszego)

```mermaid
flowchart TB
  subgraph CPP["Bez Simulinka (C++)"]
    a1["Ogranicznik narastania ciągu<br/>w SendControl()<br/>max ±50 na krok 20 ms"]
    a2["Zagęszczenie trasy w PathCallback<br/>punkt co 0.5 m"]
    a3["end_heading w radianach"]
  end
  subgraph PLAN["Planner (l2dp)"]
    b1["Line-of-sight: wywal schodki"]
    b2["Naprawić karę za skręt w A*"]
    b3["Nie przeliczać trasy od zera,<br/>jeśli stara jest dobra"]
  end
  subgraph SIM["Simulink (trzeba .slx)"]
    c1["Lookahead stały 3–6 m,<br/>NIE zależny od α"]
    c2["Wzmocnienie 15 → 5"]
    c3["MaxAngularVelocity 1 → 0.4"]
  end
  CPP --> PLAN --> SIM
```

| Zmiana | Trudność | Zysk | Gdzie |
|---|---|---|---|
| line-of-sight w plannerze | średnia | ⭐⭐⭐ | `l2dp/planner/src/planner.cpp` |
| lookahead 3–6 m, niezależny od α | wymaga `.slx` | ⭐⭐⭐ | model Simulink, blok `Delay` / Pure Pursuit |
| wzmocnienie 15 → 5 | wymaga `.slx` | ⭐⭐ | `S6/Gain`, `S6/Gain1` |
| ogranicznik narastania ciągu | łatwa | ⭐ (łagodzi objawy) | `regulator.cpp`, `SendControl` |
| end_heading w radianach | łatwa | ⭐ (koniec trasy) | `regulator.cpp`, `PathCallback` |

Kod gotowych poprawek: [[Regulator — TODO, błędy i oscylacje]].

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.
