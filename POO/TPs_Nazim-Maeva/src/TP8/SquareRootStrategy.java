package TP8;

public class SquareRootStrategy implements Strategy {
	final static Strategy instance = new SquareRootStrategy();
	
	@Override public double initialValue() { return 1; }

	@Override public double cumulativeValue(double acc, double v) { return Math.sqrt(acc); }

}
