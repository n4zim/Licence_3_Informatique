package TP6_Remaster;

public class SumOperator implements Operator{

    @Override
    public String symbol() {
        return "+";
    }

    @Override
    public double initialValue() {
        return 0;
    }

    @Override
    public double cumulativeValue(double a, double b) {
        return a + b;
    }
    
}
