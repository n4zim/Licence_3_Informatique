package TP5_Ex1;

public class Sum implements Formula {
	private Formula[] formulas;

	public Sum(Formula...formulas)
	{
		this.formulas = formulas;
	}

	@Override
	public double asValue()
	{
		double result = 0;
		for(Formula formula:formulas)
			result = ((Sum) formula).cumulativeValue(result, formula.asValue());

		return result;
	}

	public double initialValue()
	{
		return 0;
	}

	public double cumulativeValue(double accumulator, double value)
	{
		return accumulator+value;
	}

	@Override
	public String asString()
	{
		StringBuilder builder = new StringBuilder();
		builder.append("(");

		for(int i = 0; i < formulas.length; i++)
		{
			if(i > 0)
				builder.append("+");

			builder.append(formulas[i].asString());
		}

		builder.append(")");

		return builder.toString();
	}

	public String symbol()
	{
		return "+";
	}
}