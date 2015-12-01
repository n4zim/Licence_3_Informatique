package TP8;

public class SquareRoot extends VariadicOperator {

    public SquareRoot(Formula... formulas) {
    	super(formulas);
    }

	@Override
    public <R> R accept(VAFormula<R> visitor) {
    	try {
			@SuppressWarnings("unchecked")
			VASquareRoot<R> sr = (VASquareRoot<R>)visitor;
    		return sr.visit(this);
    	} catch(ClassCastException e) {
    		return null;
    	}
    }
}
