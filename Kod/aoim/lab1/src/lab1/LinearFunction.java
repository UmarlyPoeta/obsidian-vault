package lab1;

public class LinearFunction extends Function implements Printable {
    private final double a;
    private final double b;

    public LinearFunction(double a, double b) {
        if (Math.abs(a) < EPS) {
            throw new IllegalArgumentException(
                    "Współczynnik a nie może być równy 0 (funkcja stała: brak albo nieskończenie wiele miejsc zerowych)");
        }
        this.a = a;
        this.b = b;
    }

    @Override
    public double calculateValue(double x) {
        return a * x + b;
    }

    @Override
    public double[] calculateZeros() {
        return new double[] {-b / a};
    }

    public boolean isIncreasing() {
        return a > 0;
    }

    @Override
    public String formula() {
        return fmt(a) + "x" + signed(b, "");
    }

    @Override
    public void print() {
        System.out.println("--- f(x) = " + formula());
        System.out.println("Funkcja " + (isIncreasing() ? "rosnąca" : "malejąca"));
        System.out.println("Miejsca zerowe: " + zerosToString(calculateZeros()));
    }
}
