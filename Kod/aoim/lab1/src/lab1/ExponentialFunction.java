package lab1;

public class ExponentialFunction extends Function implements Printable {
    private final double a;
    private final double b;

    public ExponentialFunction(double a, double b) {
        if (a <= 0 || Math.abs(a - 1) < EPS) {
            throw new IllegalArgumentException("Podstawa a musi spełniać a > 0 i a ≠ 1");
        }
        this.a = a;
        this.b = b;
    }

    @Override
    public double calculateValue(double x) {
        return Math.pow(a, x) + b;
    }

    /** a^x = -b ma rozwiązanie tylko dla b < 0: x = ln(-b) / ln(a). */
    @Override
    public double[] calculateZeros() {
        if (b >= 0) {
            return new double[0];
        }
        return new double[] {Math.log(-b) / Math.log(a)};
    }

    public boolean isIncreasing() {
        return a > 1;
    }

    @Override
    public String formula() {
        return fmt(a) + "^x" + signed(b, "");
    }

    @Override
    public void print() {
        System.out.println("--- f(x) = " + formula());
        System.out.println("Funkcja " + (isIncreasing() ? "rosnąca" : "malejąca") + ", asymptota pozioma y = " + fmt(b));
        System.out.println("Miejsca zerowe: " + zerosToString(calculateZeros()));
    }
}
