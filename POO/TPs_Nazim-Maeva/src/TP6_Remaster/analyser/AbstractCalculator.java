/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package fr.quatrevieux.l3.cpoo.tp6.analyzer;

import fr.quatrevieux.l3.cpoo.tp1.ex4.Formula;
import java.util.Stack;

/**
 *
 * @author q13000412
 */
abstract class AbstractCalculator implements Calculator{

    @Override
    public Formula analyze(String[] tokens) {
        Stack<Formula> formulas = new Stack<>();
        
        for (String token : tokens) {
            analyzeToken(formulas, token);
        }
        
        return formulas.pop();
    }
    
    abstract void analyzeToken(Stack<Formula> formulas, String token);
}
