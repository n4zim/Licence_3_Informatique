package TP8;

public class Sum extends VariadicOperator {

    public Sum(Formula... formulas) {
    	super(formulas);
    }

	@Override
    public <R> R accept(VAFormula<R> visitor) {
    	try {
			@SuppressWarnings("unchecked")
			VASum<R> sum = (VASum<R>)visitor;
    		return sum.visit(this);
    	} catch(ClassCastException e) {
    		return null;
    	}
    }
}
