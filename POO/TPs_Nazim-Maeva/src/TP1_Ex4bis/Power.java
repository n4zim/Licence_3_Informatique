package TP1_Ex4bis;

public class Power implements Formula{
    final private Formula formula;
    final private int power;

    public Power(Formula formula, int power) {
        this.formula = formula;
        this.power = power;
    }

    @Override
    public double asValue() {
        return Math.pow(formula.asValue(), power);
    }

    @Override
    public String asString() {
        return "(" + formula.asString() + ")^" + power;
    }
    
}
