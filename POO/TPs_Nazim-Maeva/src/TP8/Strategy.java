package TP8;

public interface Strategy {
	double initialValue();
	double cumulativeValue(double acc, double v);
}
