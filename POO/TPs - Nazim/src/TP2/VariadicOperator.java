package TP2;
import TP1_Ex4bis.Formula;

abstract class VariadicOperator implements Formula{
	private Formula[] formulas;

	public VariadicOperator(Formula...formulas)
	{
		this.formulas = formulas;
	}

	@Override
	public double asValue()
	{
		double result = 1;
		for(Formula formula:formulas)
			result = ((VariadicOperator) formula).cumulativeValue(result, formula.asValue());

		return result;
	}

	abstract double initialValue();

	abstract double cumulativeValue(double accumulator, double value);

	@Override
	public String asString()
	{
		StringBuilder builder = new StringBuilder();
		builder.append("(");

		for(int i = 0; i < formulas.length; i++)
		{
			if(i > 0)
				builder.append("*");

			builder.append(formulas[i].asString());
		}

		builder.append(")");

		return builder.toString();
	}

	abstract String symbol();
}