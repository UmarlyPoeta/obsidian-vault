package lab1;

import java.util.Locale;

public abstract class Function {
    protected static final double EPS = 1e-9;

    public abstract double calculateValue(double x);

    /** Miejsca zerowe rosnąco; pusta tablica, gdy ich brak. */
    public abstract double[] calculateZeros();

    /** Wzór funkcji do wypisania, np. "1.00x^2 - 4.00x + 3.00". */
    public abstract String formula();

    protected static String fmt(double v) {
        return String.format(Locale.US, "%.2f", v);
    }

    /** Dokleja składnik ze znakiem: " + 3.00" albo " - 4.00x". */
    protected static String signed(double v, String suffix) {
        return (v < 0 ? " - " : " + ") + fmt(Math.abs(v)) + suffix;
    }

    protected static String zerosToString(double[] zeros) {
        if (zeros.length == 0) {
            return "brak";
        }
        StringBuilder sb = new StringBuilder();
        for (int i = 0; i < zeros.length; i++) {
            if (i > 0) {
                sb.append(", ");
            }
            sb.append(fmt(zeros[i]));
        }
        return sb.toString();
    }
}
