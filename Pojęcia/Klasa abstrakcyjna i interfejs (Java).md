---
typ: pojęcie
status: nowe
przedmioty: ["[[AGH Aplikacje okienkowe i mobilne]]", "[[AGH Wzorce projektowe]]"]
wymaga: ["[[Java dla programisty C++ (JDK, JRE, JVM, typy)]]"]
powiązane: ["[[AOiM Lab 1 — funkcje (klasa abstrakcyjna i interfejs)]]", "[[Diagram klas UML]]", "[[Trening — Wzorce projektowe w kodzie]]"]
tags: [pojęcie, aoim, java, obiektowość]
utworzono: 2026-10-05
źródła: ["S. Gajoch — instrukcja LAB 1, sekcja „Teoria” (UPeL PAOiM-26)", "dev.java/learn — Interfaces, Inheritance", "J. Bloch — Effective Java, 3rd ed., Item 20: Prefer interfaces to abstract classes"]
---
# Klasa abstrakcyjna i interfejs (Java)

> [!skrot] W 30 sekund
> - **Interfejs** to umowa: „umiem zrobić X” (`Printable`, `Comparable`). Klasa może implementować **wiele** interfejsów. Brak pól instancji.
> - **Klasa abstrakcyjna** to niedokończona klasa bazowa: wspólne pola i kod plus metody `abstract` do uzupełnienia. Dziedziczyć można tylko po **jednej** klasie. Nie da się stworzyć jej obiektu.
> - **Kiedy:** interfejs, gdy opisujesz zdolność wspólną niepowiązanym klasom. Klasa abstrakcyjna, gdy masz rodzinę podobnych obiektów ze wspólnym stanem lub kodem.

> [!intuicja] Po ludzku
> Interfejs to **uprawnienia**: prawo jazdy kategorii B mogą mieć kierowca, lekarz i robot. Klasa abstrakcyjna to **gatunek**: „Pojazd” ma koła i silnik (wspólne pola), umie zatankować (wspólny kod), ale „jak jedzie” zależy od konkretnego pojazdu (metoda abstrakcyjna). Jeden obiekt może mieć wiele uprawnień, ale należy do jednego gatunku.

## Definicja i wzory
> [!definicja] Porównanie
> | | interfejs | klasa abstrakcyjna |
> |---|---|---|
> | słowo kluczowe | `interface`, `implements` | `abstract class`, `extends` |
> | ile naraz | dowolnie wiele | jedna (Java nie ma wielodziedziczenia klas) |
> | pola | tylko stałe `public static final` | dowolne, także `private` i instancji |
> | konstruktor | brak | jest (wywoływany przez `super(...)` z podklasy) |
> | metody | abstrakcyjne (domyślnie `public abstract`), od Javy 8 też `default` i `static`, od Javy 9 `private` | abstrakcyjne i zwykłe, dowolny dostęp |
> | obiekt `new` | nie | nie |
> | relacja | „potrafi” (*can-do*) | „jest” (*is-a*) |
> | odpowiednik w C++ | klasa z samymi metodami czysto wirtualnymi | klasa z co najmniej jedną metodą `= 0` |

> [!definicja] Składnia
> ```java
> public interface Printable {
>     void print();                              // public abstract z automatu
>     default void printTwice() { print(); print(); }   // Java 8+: metoda z ciałem
> }
>
> public abstract class Function {
>     protected static final double EPS = 1e-9;  // wspólna stała i stan
>     public abstract double calculateValue(double x);  // podklasa MUSI zaimplementować
>     public double average(double a, double b) {        // wspólny, gotowy kod
>         return (calculateValue(a) + calculateValue(b)) / 2;
>     }
> }
>
> public class LinearFunction extends Function implements Printable, Comparable<LinearFunction> {
>     private final double a, b;
>     public LinearFunction(double a, double b) { this.a = a; this.b = b; }
>     @Override public double calculateValue(double x) { return a * x + b; }
>     @Override public void print() { System.out.println(a + "x + " + b); }
>     @Override public int compareTo(LinearFunction o) { return Double.compare(a, o.a); }
> }
> ```

> [!wzor] Polimorfizm, czyli po co to wszystko
> ```java
> Function f = new QuadraticFunction(1, -4, 3);   // typ zmiennej: abstrakcja
> f.calculateValue(2);                            // wywoła QuadraticFunction.calculateValue (wiązanie dynamiczne)
> List<Printable> doDruku = List.of(f1, f2, przebieg);   // różne klasy, wspólna zdolność
> for (Printable p : doDruku) p.print();
> ```
> Kod, który używa `Function` albo `Printable`, nie zmienia się, gdy dopisujesz nowe klasy. To jest zasada otwarte/zamknięte z [[AGH Wzorce projektowe|wzorców projektowych]].

## Jak to zrobić — krok po kroku (jak wybrać)
1. Czy obiekty mają **wspólny stan albo kod**? Tak, więc klasa abstrakcyjna.
2. Czy to **zdolność**, którą mogą mieć niepowiązane klasy, albo klasa już po czymś dziedziczy? Tak, więc interfejs.
3. W razie wątpliwości zacznij od interfejsu (*Effective Java*, Item 20). Wspólny kod możesz dodać przez metodę `default` albo klasę abstrakcyjną implementującą ten interfejs („szkieletowa implementacja”, np. `AbstractList` implementuje `List`).

## Przykład
> [!przyklad] Lab 1 AOiM
> `Printable` jest interfejsem, bo drukować umieją zarówno funkcje, jak i `FunctionOnInterval`, który funkcją nie jest. `Function` jest klasą abstrakcyjną, bo trzy funkcje to jedna rodzina ze wspólnymi pomocnikami (tolerancja `EPS`, formatowanie liczb). `FunctionOnInterval` przechowuje pole typu `Function` i woła `calculateValue()`, nie wiedząc, którą funkcję dostał. Pełny kod: [[AOiM Lab 1 — funkcje (klasa abstrakcyjna i interfejs)]].

## Typowe błędy
> [!blad] Najczęstsze potknięcia
> - `new Function()` dla klasy abstrakcyjnej = błąd kompilacji (ale klasa anonimowa `new Function() { … }` jest dozwolona).
> - Zapomniana implementacja metody abstrakcyjnej w podklasie: kompilator każe zrobić podklasę też `abstract`.
> - Pole instancji w interfejsie: każde pole interfejsu jest `public static final`, więc to stała, a nie stan obiektu.
> - Zawężenie dostępu przy implementacji: metoda interfejsu jest `public`, więc w klasie też musi być `public`.
> - Brak `@Override`: literówka w nazwie tworzy nową metodę zamiast nadpisać. Adnotacja wyłapuje to przy kompilacji.
> - `instanceof` + rzutowanie na konkretne klasy tam, gdzie wystarczy wywołać metodę abstrakcyjną: to łamie polimorfizm.

## Na zajęcia i egzamin
> [!zajecia]
> - **Pytania na odbiorze:** różnice interfejsu i klasy abstrakcyjnej (tabela wyżej), dlaczego Java nie ma wielodziedziczenia klas (problem diamentu), co to `default` w interfejsie, czy klasa abstrakcyjna może mieć konstruktor (tak), czy może nie mieć żadnej metody abstrakcyjnej (tak, wtedy po prostu nie da się utworzyć jej obiektu).
> - Łączy się z: [[Diagram klas UML]] (`<<interface>>`, linia przerywana z trójkątem = realizacja; ciągła z trójkątem = dziedziczenie), [[Trening — Wzorce projektowe w kodzie]].

## Zadania
> [!zadanie] Zadanie 1
> Masz klasy `Dog`, `Robot` i `Car`. Wszystkie mają metodę `move()`, a `Dog` i `Cat` mają też wspólne pole `name` i metodę `eat()`. Zaprojektuj hierarchię.
> > [!odpowiedz]- Odpowiedź
> > `interface Movable { void move(); }` implementowany przez `Dog`, `Robot`, `Car`. `abstract class Animal { protected String name; void eat() {…} abstract String sound(); }`, a `Dog` i `Cat` dziedziczą po `Animal`. `Dog extends Animal implements Movable`.

> [!zadanie] Zadanie 2
> Czy poniższy kod się skompiluje? `interface A { int X = 5; } … A.X = 6;`
> > [!odpowiedz]- Odpowiedź
> > Nie. `X` jest niejawnie `public static final`, więc nie można go zmienić.

## Fiszki #fiszki/aoim
Ile interfejsów może implementować klasa w Javie?::Dowolnie wiele
Po ilu klasach może dziedziczyć klasa w Javie?::Po jednej
Czy klasa abstrakcyjna może mieć konstruktor?::Tak, woła się go przez `super(...)` z podklasy
Jakie pola może mieć interfejs?::Tylko stałe `public static final`
Co to metoda `default` w interfejsie?::Metoda z ciałem w interfejsie (Java 8+), dziedziczona przez implementujące klasy
Relacja klasy abstrakcyjnej i interfejsu w jednym zdaniu?::Klasa abstrakcyjna mówi, czym obiekt „jest”, interfejs, co „potrafi”

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.

## Źródła
- Instrukcja LAB 1, „Teoria” pkt 5–6: [[Lab 1 — klasa abstrakcyjna i interfejs (funkcje).pdf]].
- [dev.java/learn](https://dev.java/learn/): „Interfaces”, „Inheritance”.
- J. Bloch, *Effective Java*, 3rd ed., Addison-Wesley 2018, Item 20.
