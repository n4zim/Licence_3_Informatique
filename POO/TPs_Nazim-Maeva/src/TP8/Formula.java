package TP8;

public interface Formula {
    public <R> R accept(VAFormula<R> visitor);
}
