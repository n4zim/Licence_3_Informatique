package TP1_Ex4bis;

public class AbsoluteValue implements Formula{
    final private Formula value;

    public AbsoluteValue(Formula value) {
        this.value = value;
    }

    @Override
    public double asValue() {
        return Math.abs(value.asValue());
    }

    @Override
    public String asString() {
        return "|" + value.asString() + "|";
    }
    
}
