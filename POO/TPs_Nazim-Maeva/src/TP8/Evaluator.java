package TP8;

public class Evaluator implements VAVariable<Double>,
										VAFormula<Double>,
										VAProduct<Double>,
										VASum<Double>,
										VASquareRoot<Double> {

	public Evaluator() {}
	
	@Override public Double visit(Variable variable) { return variable.getValue(); }
	
	@Override public Double visit(Sum sum) { return visit(sum, SumStrategy.instance); }
	
	@Override public Double visit(Product product) { return visit(product, ProductStrategy.instance); }
	
	@Override public Double visit(SquareRoot sr) { return visit(sr, ProductStrategy.instance); }
	
	public Double visit(VariadicOperator operator, Strategy strategy) {
		double result = strategy.initialValue();
    	for(Formula f : operator.getFormulas()) result = strategy.cumulativeValue(result, f.accept(this));
        return result;
	}
}
