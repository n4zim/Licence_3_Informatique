package TP8;

public class Product extends VariadicOperator {

	public Product(Formula...formulas) {
		super(formulas);
	}
	
    public <R> R accept(VAFormula<R> visitor) {
    	try {
			@SuppressWarnings("unchecked")
			VAProduct<R> product = (VAProduct<R>)visitor;
    		return product.visit(this);
    	} catch(ClassCastException e) {
    		return null;
    	}
    }
}
