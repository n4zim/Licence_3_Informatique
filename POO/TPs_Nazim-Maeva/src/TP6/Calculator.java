package TP6; // Copied

import java.util.Stack;

public class Calculator {
	
	private static FormulaFactory formulaFactory = new FormulaFactory();

	public static void main(String[] args) {
		String[] OSEFdesArgs = {"2","5","*","3","4","7","+","*","+"};
		Formula formula = analyse(OSEFdesArgs);
		System.out.println(formula.asValue());
	}
	
	public static Formula analyse(String[] args) {
		Stack<Formula> stack = new Stack<>();
		for(String token : args) analyseToken(stack, token);
		
		if(stack.size() != 1) throw new IllegalArgumentException("Incorrect formula");
		
		return stack.pop();
	}
	
	private static void analyseToken(Stack<Formula> stack, String token) {
		switch(token) {
			case "+": analyseSum(stack); return;
			case "*": analyseProduct(stack); return;
			default: analyseInt(stack, token);
		}
	}

	private static void analyseInt(Stack<Formula> stack, String token) {
		int value = Integer.parseInt(token);
		stack.push(new Constant(value));
	}

	private static void analyseProduct(Stack<Formula> stack) {
		Formula right = stack.pop();
		Formula left = stack.pop();
		
		Formula formula = formulaFactory.createProduct(left, right);
		stack.push(formula);
	}

	private static void analyseSum(Stack<Formula> stack) {
		Formula right = stack.pop();
		Formula left = stack.pop();
		
		Formula formula = formulaFactory.createSum(left, right);
		stack.push(formula);
	}
}
