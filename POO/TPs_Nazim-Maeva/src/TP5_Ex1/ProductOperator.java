package TP5_Ex1;

public class ProductOperator implements Operator {
	public String symbol() { return "*"; }
	public double initialValue() { return 1; }
	public double cumulativeValue(double a, double v) { return a*v; }
}
