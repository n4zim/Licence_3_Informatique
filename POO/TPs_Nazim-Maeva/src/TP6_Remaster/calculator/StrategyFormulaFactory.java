package TP6_Remaster;

public class StrategyFormulaFactory implements AbstractFormulaFactory{
    final private Operator sumOperator = new SumOperator();
    final private Operator productOperator = new ProductOperator();

    @Override
    public Formula createConstant(double value) {
        return new Constant(value);
    }

    @Override
    public Formula createProduct(Formula... formulas) {
        return new VariadicOperator(productOperator, formulas);
    }

    @Override
    public Formula createSum(Formula... formulas) {
        return new VariadicOperator(sumOperator, formulas);
    }
    
}
