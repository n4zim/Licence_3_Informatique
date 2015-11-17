package TP6_Remaster;

public class VariadicOperator implements Formula {
    final private Operator operator;
    final private Formula[] formulas;

    public VariadicOperator(Operator operator, Formula... formulas) {
        this.operator = operator;
        this.formulas = formulas;
    }

    @Override
    public double asValue() {
        double value = operator.initialValue();
        
        for (Formula formula : formulas) {
            value = operator.cumulativeValue(value, formula.asValue());
        }
        
        return value;
    }

    @Override
    public String asString() {
        StringBuilder sb = new StringBuilder();
        
        sb.append('(');
        
        boolean first = true;
        for (Formula formula : formulas) {
            if(!first)
                sb.append(operator.symbol());
            else
                first = false;
            
            sb.append(formula.asString());
        }
        
        sb.append(')');
        
        return sb.toString();        
    }
}
