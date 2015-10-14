package TP1_Ex2;
import TP1_Ex1.Vector;

public class Stack {
	private Vector vector = new Vector(0);
		
	public void push(int value) {
		vector.add(value);
	}
	
	public int peek() {
		if(vector.size() == 0) return 0;
		return vector.get(vector.size()-1);
	}
	
	public int pop() {
		if(vector.size() == 0) return 0;
		int first = vector.get(vector.size()-1);
		vector.resize(vector.size()-1);
		return first;
	}
	
	public int size() {
		return vector.size();
	}
	
	public boolean isEmpty() {
		if(vector.size() == 0) return true;
		return false;
	}
	
	public static void main(String[] args) {
		Stack stack = new Stack();
		System.out.println("Size : "+stack.size());
		stack.push(23);
		System.out.println("Size : "+stack.size());
		System.out.println("Peek : "+stack.peek());
		System.out.println("Size : "+stack.size());
		System.out.println("Pop : "+stack.pop());
		System.out.println("Size : "+stack.size());
	}

}
