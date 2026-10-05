package lab1;

import java.util.Locale;
import java.util.Scanner;

public class Main {
    public static void main(String[] args) {
        // Bez useLocale Scanner na polskim systemie czyta "0,5", a "0.5" odrzuca.
        ConsoleInput input = new ConsoleInput(new Scanner(System.in).useLocale(Locale.US));
        while (true) {
            int choice = input.readChoice();
            if (choice == 0) {
                break;
            }
            try {
                Function f = input.readFunction(choice);
                FunctionOnInterval interval = input.readInterval(f);
                ((Printable) f).print(); // każda z trzech klas implementuje Printable
                interval.print();
            } catch (IllegalArgumentException e) {
                System.out.println("Błąd: " + e.getMessage());
            }
            System.out.println();
        }
    }
}
