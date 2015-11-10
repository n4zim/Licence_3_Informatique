package TP5_Ex1;

public class Main {
    public static void main(String[] args) {
        testFormula();
        testGeneratePoints();
    }
    
    static public void testFormula(){
        Variable x = new Variable("x", 2.5);
        Variable y = new Variable("y", 4);
        Formula formula = new Sum(x, new Product(y, new Sum(x, y)));
        
        assert formula.asString().equals("(x+(y*(x+y))");
        assert formula.asValue() == 28.5;
        x.set(5);
        assert formula.asValue() == 41;
    }
    
    static public void testGeneratePoints(){
        Variable variable = new Variable("variable", 0);
        Formula formula = new ProductOperator(variable);
        generatePoints(formula, variable, -5, 10, 1.5);
    }
    
    static public void generatePoints(Formula formula, Variable variable, double startValue, double endValue, double step){
        for(double currentValue = startValue; currentValue < endValue; currentValue += step){
            variable.set(currentValue);
            System.out.println(currentValue + " " + formula.asValue());
        }
    }
}
