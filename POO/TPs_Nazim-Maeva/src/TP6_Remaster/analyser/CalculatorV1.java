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
import java.util.Stack;

/**
 *
 * @author q13000412
 */
public class CalculatorV1 extends AbstractCalculator{

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
        formulas.push(new Sum(formulas.pop(), formulas.pop()));
    }
    
    private void analyzeProduct(Stack<Formula> formulas){
        formulas.push(new Product(formulas.pop(), formulas.pop()));
    }
    
    private void analyseConstant(Stack<Formula> formulas, String token){
        formulas.push(new Constant(Double.valueOf(token)));
    }
}
