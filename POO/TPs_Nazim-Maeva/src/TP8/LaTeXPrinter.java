package TP8;

public class LaTeXPrinter implements VAVariable<String>,
										VAFormula<String>,
										VAProduct<String>,
										VASum<String> {

	public LaTeXPrinter() {
		
	}
	
	@Override public String visit(Variable variable) { return variable.getName(); }
	
	@Override
	public String visit(Sum sum) {
		return null;
	}

	@Override
	public String visit(Product product) {
		return null;
	}
	
	public String visit(VariadicOperator operator, String symbol) {
		return symbol;
	}
}
