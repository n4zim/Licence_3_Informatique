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
public interface Analyzer {
    public Formula analyze(Stack<Formula> formulas, String token);
}
