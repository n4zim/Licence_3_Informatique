package TP6_Remaster;

public interface AbstractFormulaFactory {
    public Formula createConstant(double value);
    public Formula createProduct(Formula... formulas);
    public Formula createSum(Formula... formulas);
}
