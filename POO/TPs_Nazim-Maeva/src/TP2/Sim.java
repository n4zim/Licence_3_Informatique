package TP2;
import TP1_Ex4bis.Formula;

public class Sim extends VariadicOperator {
	public Sim(Formula...formulas) {
		super(formulas);
	}

	double initialValue() {
		return 0;
	}

	double cumulativeValue(double accumulator, double value)
	{
		return accumulator + value;
	}

	String symbol()
	{
		return "+";
	}
}
