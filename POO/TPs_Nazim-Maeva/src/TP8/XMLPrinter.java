package TP8;

public class XMLPrinter implements VAVariable<String>,
									VAFormula<String>,
									VAProduct<String>,
									VASum<String> {
	private int depth = 0;
	
	public XMLPrinter() {}
	
	@Override public String visit(Variable variable) {
		return "<var>"+variable.getName()+"</var>\n";
	}
	
	@Override public String visit(Sum sum) { return visit(sum, "sum"); }

	@Override public String visit(Product product) { return visit(product, "product"); }
	
	public String visit(VariadicOperator operator, String tag) {
		StringBuilder builder = new StringBuilder("<"+tag+">\n");
		Formula[] formulas = operator.getFormulas();
		depth++;
		for(int i = 0; i < formulas.length; i++) {
			builder.append(indent()+formulas[i].accept(this));
			if(i > 0) depth--;
		}
		builder.append(indent()+"</"+tag+">\n");
		return builder.toString();
	}
	
	private String indent() {
		StringBuilder builder = new StringBuilder();
		for(int i = 0; i < depth; i++) builder.append("\t");
		return builder.toString();
	}
}