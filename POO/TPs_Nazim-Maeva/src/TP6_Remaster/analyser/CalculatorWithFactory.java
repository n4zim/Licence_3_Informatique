/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package fr.quatrevieux.l3.cpoo.tp6.analyzer;

import fr.quatrevieux.l3.cpoo.tp1.ex4.Formula;
import fr.quatrevieux.l3.cpoo.tp1.ex4.Product;
import fr.quatrevieux.l3.cpoo.tp1.ex4.Sum;
import fr.quatrevieux.l3.cpoo.tp6.formula.Constant;
import fr.quatrevieux.l3.cpoo.tp6.formula.FormulaFactory;
import java.util.Stack;

/**
 *
 * @author q13000412
 */
public class CalculatorWithFactory extends AbstractCalculator{
    final private FormulaFactory factory;

    public CalculatorWithFactory(FormulaFactory factory) {
        this.factory = factory;
    }

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
    
    private void analyzeSum(Stack<Formula> formulas){
        formulas.push(factory.createSum(formulas.pop(), formulas.pop()));
    }
    
    private void analyzeProduct(Stack<Formula> formulas){
        formulas.push(factory.createProduct(formulas.pop(), formulas.pop()));
    }
    
    private void analyseConstant(Stack<Formula> formulas, String token){
        formulas.push(new Constant(Double.valueOf(token)));
    }
}
