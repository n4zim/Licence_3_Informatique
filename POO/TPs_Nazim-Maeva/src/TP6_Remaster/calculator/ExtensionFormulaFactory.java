package TP6_Remaster;

public class ExtensionFormulaFactory implements AbstractFormulaFactory{

    @Override
    public Formula createConstant(double value) {
        return new Constant(value);
    }

    @Override
    public Formula createProduct(Formula... formulas) {
        return new Product(formulas[0], formulas[1]);
    }

    @Override
    public Formula createSum(Formula... formulas) {
        return new Sum(formulas[0], formulas[1]);
    }
    
}
