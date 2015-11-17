package TP6_Remaster;

public class Sum implements Formula{
    final private Formula firstOperand;
    final private Formula secondOperand;

    public Sum(Formula firstOperand, Formula secondOperand) {
        this.firstOperand = firstOperand;
        this.secondOperand = secondOperand;
    }

    @Override
    public double asValue() {
        return firstOperand.asValue() + secondOperand.asValue();
    }

    @Override
    public String asString() {
        return "(" + firstOperand.asString() + "+" + secondOperand.asString() + ")";
    }
    
}
