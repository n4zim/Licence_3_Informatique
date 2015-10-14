package TP1_Ex4bis;

public class SquareRoot implements Formula{
    final private Formula formula;

    public SquareRoot(Formula formula) {
        this.formula = formula;
    }

    @Override
    public double asValue() {
        return Math.sqrt(formula.asValue());
    }

    @Override
    public String asString() {
        return "sqrl(" + formula.asString() + ")";
    }
    
}
