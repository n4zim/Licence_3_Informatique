package TP1_Ex4bis;

public class Minimum implements Formula{
    final private Formula[] formulas;

    public Minimum(Formula... formulas) {
        this.formulas = formulas;
    }

    @Override
    public double asValue() {
        double minValue = Double.MAX_VALUE;
        
        for(Formula formula : formulas){
            double currentValue = formula.asValue();
            
            if(currentValue < minValue)
                minValue = currentValue;
        }
        
        return minValue;
    }

    @Override
    public String asString() {
        StringBuilder sb = new StringBuilder();
        
        sb.append("Min(");
        
        boolean isFirst = false;
        
        for(Formula formula : formulas){
            if(isFirst){
                isFirst = false;
            }else{
                sb.append(",");
            }
            sb.append(formula.asString());
        }
        
        sb.append(")");
        
        return sb.toString();
    }
    
    
}
