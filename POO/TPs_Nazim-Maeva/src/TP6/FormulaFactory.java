package TP6; // Copied

public class FormulaFactory {

	public FormulaFactory() {
		
	}
	
	public Formula createConstant(double value) {
		return new Constant(value);
	}
	
	public Formula createProduct(Formula...formulas) {
		return new Product(formulas);
	}
	
	public Formula createSum(Formula...formulas) {
		return new Sum(formulas);
	}
}
