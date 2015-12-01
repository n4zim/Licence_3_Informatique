package TP8;

public class PlainPrinter implements VAVariable<String>,
										VAFormula<String>,
										VAProduct<String>,
										VASum<String> {

	public PlainPrinter() {
		
	}
	
	@Override public String visit(Variable variable) { return variable.getName(); }
	
	@Override public String visit(Sum sum) { return visit(sum, '+'); }

	@Override public String visit(Product product) { return visit(product, '*'); }
	
	public String visit(VariadicOperator operator, char symbol) {
		StringBuilder builder = new StringBuilder("(");
		Formula[] formulas = operator.getFormulas();
		for(int i = 0; i < formulas.length; i++) {
			if(i > 0) builder.append(symbol);
			builder.append(formulas[i].accept(this));
		}
		builder.append(')');
		return builder.toString();
	}
}
