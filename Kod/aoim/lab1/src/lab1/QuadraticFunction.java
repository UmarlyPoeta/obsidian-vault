package lab1;

public class QuadraticFunction extends Function implements Printable {
    private final double a;
    private final double b;
    private final double c;

    public QuadraticFunction(double a, double b, double c) {
        if (Math.abs(a) < EPS) {
            throw new IllegalArgumentException("Współczynnik a nie może być równy 0 (to nie jest funkcja kwadratowa)");
        }
        this.a = a;
        this.b = b;
        this.c = c;
    }

    @Override
    public double calculateValue(double x) {
        return (a * x + b) * x + c; // schemat Hornera
    }

    public double calculateDelta() {
        return b * b - 4 * a * c;
    }

    /** Wierzchołek paraboli: {p, q}. */
    public double[] calculateVertex() {
        return new double[] {-b / (2 * a), -calculateDelta() / (4 * a)};
    }

    @Override
    public double[] calculateZeros() {
        double delta = calculateDelta();
        if (Math.abs(delta) < EPS) {
            return new double[] {-b / (2 * a)};
        }
        if (delta < 0) {
            return new double[0];
        }
        double s = Math.sqrt(delta);
        double x1 = (-b - s) / (2 * a);
        double x2 = (-b + s) / (2 * a);
        return x1 < x2 ? new double[] {x1, x2} : new double[] {x2, x1};
    }

    @Override
    public String formula() {
        return fmt(a) + "x^2" + signed(b, "x") + signed(c, "");
    }

    @Override
    public void print() {
        double[] v = calculateVertex();
        System.out.println("--- f(x) = " + formula());
        System.out.println("Delta: " + fmt(calculateDelta()) + ", wierzchołek: (" + fmt(v[0]) + ", " + fmt(v[1]) + ")");
        System.out.println("Miejsca zerowe: " + zerosToString(calculateZeros()));
    }
}
