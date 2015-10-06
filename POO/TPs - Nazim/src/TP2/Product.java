package TP2;
import TP1_Ex4bis.Formula;

public class Product implements Formula {
	private Formula[] formulas;

	public Product(Formula...formulas) {
		this.formulas = formulas;
	}

	@Override
	public double asValue() {
		double result = 1;
		for(Formula formula:formulas)
			result = ((Product) formula).cumulativeValue(result, formula.asValue());

		return result;
	}

	public double initialValue() {
		return 1;
	}

	public double cumulativeValue(double accumulator, double value) {
		return accumulator*value;
	}

	@Override
	public String asString() {
		StringBuilder builder = new StringBuilder();
		builder.append("(");

		for(int i = 0; i < formulas.length; i++) {
			if(i > 0) builder.append("*");
			builder.append(formulas[i].asString());
		}

		builder.append(")");

		return builder.toString();
	}

	public String symbol() {
		return "*";
	}
}