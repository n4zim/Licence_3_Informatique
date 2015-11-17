package TP6_Remaster;

public class ProductOperator implements Operator{

    @Override
    public String symbol() {
        return "*";
    }

    @Override
    public double initialValue() {
        return 1;
    }

    @Override
    public double cumulativeValue(double a, double b) {
        return a * b;
    }
    
}
