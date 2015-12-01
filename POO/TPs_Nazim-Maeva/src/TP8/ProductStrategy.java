package TP8;

public class ProductStrategy implements Strategy {
	final static Strategy instance = new ProductStrategy();
	
	@Override public double initialValue() { return 1; }

	@Override public double cumulativeValue(double acc, double v) { return acc*v; }

}
