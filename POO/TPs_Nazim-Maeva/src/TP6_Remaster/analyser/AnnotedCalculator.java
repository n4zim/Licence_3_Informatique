/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package fr.quatrevieux.l3.cpoo.tp6.analyzer;

import fr.quatrevieux.l3.cpoo.tp1.ex4.Formula;
import fr.quatrevieux.l3.cpoo.tp6.formula.FormulaFactory;
import java.lang.reflect.Method;
import java.util.HashMap;
import java.util.Map;
import java.util.Stack;

/**
 *
 * @author q13000412
 */
public class AnnotedCalculator extends AbstractCalculator{
    final private FormulaFactory factory;
    final private Map<String, Analyzer> analyzers = new HashMap<>();
    private Analyzer defaultAnalyzer;

    public AnnotedCalculator(FormulaFactory factory) {
        this.factory = factory;
        
        for(Method method : getClass().getMethods()){
            if(method.isAnnotationPresent(DefaultAnalyzer.class)){
                defaultAnalyzer = analyzerFromMethod(method);
            }
        }
    }
    
    private Analyzer analyzerFromMethod(Method method){
        return (formulas, token) -> {
            try{
                return (Formula)method.invoke(this, formulas, token);
            }catch(Exception e){
                e.printStackTrace();
                return null;
            }
        };
    }

    @Override
    void analyzeToken(Stack<Formula> formulas, String token) {
        
    }
    
    
}
