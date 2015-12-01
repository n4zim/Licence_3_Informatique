package TP8;

public class SumStrategy implements Strategy {
	final static Strategy instance = new SumStrategy();
	
	@Override public double initialValue() { return 0; }

	@Override public double cumulativeValue(double acc, double v) { return acc+v; }
}
