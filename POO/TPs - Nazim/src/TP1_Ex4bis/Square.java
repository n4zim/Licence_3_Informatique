package TP1_Ex4bis;

public class Square implements Formula{
    final private Formula formula;

    public Square(Formula formula) {
        this.formula = formula;
    }

    @Override
    public double asValue() {
        double value = formula.asValue();
        return value * value;
    }

    @Override
    public String asString() {
        return "(" + formula.asString() + ")^2";
    }
    
}
