package TP6;

abstract class VariadicOperator implements Formula {
	private Formula[] formulas;
	private Operator operator;

	public VariadicOperator(Operator operator, Formula...formulas) {
		this.operator = operator;
		this.formulas = formulas;
	}

	@Override
	public double asValue() {
		double result = operator.cumulativeValue();
		for(Formula formula:formulas)
			result = operator.cumulativeValue(result, formula.asValue());
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
				builder.append(operator.symbol());

			builder.append(formulas[i].asString());
		}

		builder.append(")");

		return builder.toString();
	}

	abstract String symbol();
}