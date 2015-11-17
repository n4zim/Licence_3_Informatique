/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package fr.quatrevieux.l3.cpoo.tp6.analyzer;

import fr.quatrevieux.l3.cpoo.tp1.ex4.Formula;
import fr.quatrevieux.l3.cpoo.tp1.ex4.Product;
import fr.quatrevieux.l3.cpoo.tp1.ex4.Sum;
import fr.quatrevieux.l3.cpoo.tp5.ex1.Operator;
import fr.quatrevieux.l3.cpoo.tp5.ex1.ProductOperator;
import fr.quatrevieux.l3.cpoo.tp5.ex1.SumOperator;
import fr.quatrevieux.l3.cpoo.tp5.ex1.VariadicOperator;
import fr.quatrevieux.l3.cpoo.tp6.formula.Constant;
import java.util.Stack;

/**
 *
 * @author q13000412
 */
public class CalculatorStrategy extends AbstractCalculator{

    @Override
    void analyzeToken(Stack<Formula> formulas, String token) {
        switch(token){
            case "*":
                analyzeProduct(formulas);
                break;
            case "+":
                analyzeSum(formulas);
                break;
            default:
                analyseConstant(formulas, token);
        }
    }
    
    private void analyzeOperator(Stack<Formula> formulas, Operator operator){
        VariadicOperator variadicOperator = new VariadicOperator(operator, formulas.pop(), formulas.pop());
        formulas.push(variadicOperator);
    }
    
    private void analyzeSum(Stack<Formula> formulas){
        analyzeOperator(formulas, new SumOperator());
    }
    
    private void analyzeProduct(Stack<Formula> formulas){
        analyzeOperator(formulas, new ProductOperator());
    }
    
    private void analyseConstant(Stack<Formula> formulas, String token){
        formulas.push(new Constant(Double.valueOf(token)));
    }
}
