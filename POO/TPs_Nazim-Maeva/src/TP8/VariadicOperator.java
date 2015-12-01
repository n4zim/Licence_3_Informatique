package TP8;

abstract public class VariadicOperator implements Formula {
    final private Formula[] formulas;

    public VariadicOperator(Formula... formulas) { this.formulas = formulas; }

    public Formula[] getFormulas() { return formulas; }
}
