package TP1_Ex4bis;

public class Maximum implements Formula{
    final private Formula[] formulas;

    public Maximum(Formula... formulas) {
        this.formulas = formulas;
    }

    @Override
    public double asValue() {
        double maxValue = Double.MIN_EXPONENT;
        
        for(Formula formula : formulas){
            double currentValue = formula.asValue();
            
            if(currentValue > maxValue)
                maxValue = currentValue;
        }
        
        return maxValue;
    }

    @Override
    public String asString() {
        StringBuilder sb = new StringBuilder();
        
        sb.append("Max(");
        
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
