package TP8;

public class Variable implements Formula {
    final private String name;
    private double value;

    public Variable(String name, double value) {
        this.name = name;
        this.value = value;
    }
    
    public double getValue() { return value; }
    
    public void set(double value) { this.value = value; }
    
    public String getName() { return name; }

    @Override
    public <R> R accept(VAFormula<R> visitor) {
    	try {
			@SuppressWarnings("unchecked")
			VAVariable<R> variable = (VAVariable<R>)visitor;
    		return variable.visit(this);
    	} catch(ClassCastException e) {
    		return null;
    	}
    }
}
