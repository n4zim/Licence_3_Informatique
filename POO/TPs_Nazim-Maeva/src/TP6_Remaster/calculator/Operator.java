package TP6_Remaster;

public interface Operator {
    public String symbol();
    public double initialValue();
    public double cumulativeValue(double a, double b);
}
