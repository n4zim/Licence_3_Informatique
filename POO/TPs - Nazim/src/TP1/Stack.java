package TP1;

public class Stack {
    final static public int INITIAL_CAPACITY = 16;
    
    final private Vector vector = new Vector(INITIAL_CAPACITY);
    
    public void push(int value){
        vector.add(value);
    }
    
    public int peek(){
        int lastIndex = vector.size() - 1;
        return vector.get(lastIndex);
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
