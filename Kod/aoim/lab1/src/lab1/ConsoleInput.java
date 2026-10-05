package lab1;

import java.util.InputMismatchException;
import java.util.Scanner;

/** Cała komunikacja z użytkownikiem. Logika (klasy funkcji) nie wie, że istnieje konsola. */
public class ConsoleInput {
    private final Scanner scanner;

    public ConsoleInput(Scanner scanner) {
        this.scanner = scanner;
    }

    public int readChoice() {
        System.out.println("1 – Liniowa  2 – Kwadratowa  3 – Wykładnicza  0 – Wyjście");
        System.out.print("> ");
        if (!scanner.hasNext()) {
            return 0; // koniec danych (np. Ctrl+D) traktujemy jak wyjście
        }
        return (int) readDouble("");
    }

    public double readDouble(String prompt) {
        while (true) {
            System.out.print(prompt);
            try {
                return scanner.nextDouble();
            } catch (InputMismatchException e) {
                System.out.println("To nie jest liczba, spróbuj jeszcze raz.");
                scanner.nextLine(); // wyrzuć błędny token
            }
        }
    }

    public Function readFunction(int choice) {
        switch (choice) {
            case 1:
                return new LinearFunction(readDouble("a: "), readDouble("b: "));
            case 2:
                return new QuadraticFunction(readDouble("a: "), readDouble("b: "), readDouble("c: "));
            case 3:
                return new ExponentialFunction(readDouble("a: "), readDouble("b: "));
            default:
                throw new IllegalArgumentException("Nieznana opcja: " + choice);
        }
    }

    public FunctionOnInterval readInterval(Function f) {
        double start = readDouble("Początek przedziału: ");
        double end = readDouble("Koniec przedziału: ");
        double step = readDouble("Krok: ");
        return new FunctionOnInterval(f, start, end, step);
    }
}
