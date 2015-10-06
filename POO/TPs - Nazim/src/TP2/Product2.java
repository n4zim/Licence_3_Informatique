package TP2;
import TP1_Ex4bis.Formula;

public class Product2 extends VariadicOperator {
	
	public Product2(Formula...formulas) {
		super(formulas);
	}

	double initialValue()
	{
		return 1;
	}

	double cumulativeValue(double accumulator, double value)
	{
		return accumulator * value;
	}

	String symbol()
	{
		return "*";
	}
}