package lab1;

import java.util.Locale;

/** Przebieg dowolnej funkcji na przedziale. Korzysta tylko z calculateValue(), bez instanceof. */
public class FunctionOnInterval implements Printable {
    private final Function function;
    private final double start;
    private final double end;
    private final double step;

    public FunctionOnInterval(Function function, double start, double end, double step) {
        if (function == null) {
            throw new IllegalArgumentException("Funkcja nie może być null");
        }
        if (start >= end) {
            throw new IllegalArgumentException("Początek przedziału musi być mniejszy od końca");
        }
        if (step <= 0 || step > end - start) {
            throw new IllegalArgumentException("Krok musi spełniać 0 < krok ≤ długość przedziału");
        }
        this.function = function;
        this.start = start;
        this.end = end;
        this.step = step;
    }

    /** Węzły x0 = start, x0 + krok, …; tolerancja chroni przed zgubieniem końca przez błąd zaokrąglenia. */
    public double[] arguments() {
        int n = (int) Math.floor((end - start) / step + 1e-9) + 1;
        double[] xs = new double[n];
        for (int i = 0; i < n; i++) {
            xs[i] = start + i * step; // mnożenie, nie sumowanie: błąd się nie kumuluje
        }
        return xs;
    }

    public double[] values() {
        double[] xs = arguments();
        double[] ys = new double[xs.length];
        for (int i = 0; i < xs.length; i++) {
            ys[i] = function.calculateValue(xs[i]);
        }
        return ys;
    }

    public double min() {
        double m = Double.POSITIVE_INFINITY;
        for (double y : values()) {
            m = Math.min(m, y);
        }
        return m;
    }

    public double max() {
        double m = Double.NEGATIVE_INFINITY;
        for (double y : values()) {
            m = Math.max(m, y);
        }
        return m;
    }

    /** Metoda trapezów na węzłach z tabeli (pole „ze znakiem”, czyli całka oznaczona). */
    public double trapezoidArea() {
        double[] xs = arguments();
        double[] ys = values();
        double sum = 0;
        for (int i = 0; i + 1 < xs.length; i++) {
            sum += (ys[i] + ys[i + 1]) / 2 * (xs[i + 1] - xs[i]);
        }
        return sum;
    }

    @Override
    public void print() {
        double[] xs = arguments();
        double[] ys = values();
        StringBuilder rowX = new StringBuilder("x:");
        StringBuilder rowY = new StringBuilder("f(x):");
        for (int i = 0; i < xs.length; i++) {
            rowX.append(String.format(Locale.US, " %6.2f", xs[i]));
            rowY.append(String.format(Locale.US, " %6.2f", ys[i]));
        }
        System.out.println(String.format(Locale.US, "--- Przebieg na [%.2f, %.2f], krok %.2f", start, end, step));
        System.out.println(rowX);
        System.out.println(rowY);
        System.out.println(String.format(Locale.US, "Min: %.2f, max: %.2f", min(), max()));
        System.out.println(String.format(Locale.US, "Pole (metoda trapezów): %.2f", trapezoidArea()));
    }
}
