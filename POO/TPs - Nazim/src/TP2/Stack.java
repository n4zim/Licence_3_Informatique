package TP2;

@SuppressWarnings("rawtypes")
public class Stack {
    final static public int INITIAL_CAPACITY = 16;
    
    
	final private Vector vector = new Vector(INITIAL_CAPACITY);
    
 
	public void push(int value){
        vector.add(value);
    }
    
    public int peek(){
        int lastIndex = ((Stack) vector).size() - 1;
        return (int) vector.get(lastIndex);
    }
    
    public int pop(){
        int value = peek();
        vector.resize(size() - 1);
        return value;
    }
    
    public int size(){
        return vector.size();
    }

    public boolean isEmpty() {
        return vector.isEmpty();
    }
}

