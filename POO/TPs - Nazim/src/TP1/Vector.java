package TP1;
import java.util.Arrays;

public class Vector {
    final static public int START_INDEX = 0;
    final static public int EMPTY_VALUE = 0;
    
    private int[] data;
    private int size;

    public Vector(int initialCapacity) {
        data = new int[initialCapacity];
    }
    
    private int capacity(){
        return data.length;
    }
    
    private void ensureCapacity(int capacity){
        if(capacity < capacity())
            return;
        
        int newCapacity = Math.max(capacity, capacity() * 2);
        data = Arrays.copyOf(data, newCapacity);
    }
    
    public void resize(int size){
        if(size < 0)
            return;
        
        ensureCapacity(size);
        
        for(int i = size; i < this.size; ++i){
            set(i, EMPTY_VALUE);
        }
    }
    
    public int size(){
        return size;
    }
    
    public boolean isEmpty(){
        return size == 0;
    }
    
    public void add(int value){
        resize(size + 1);
        set(size - 1, value);
    }
    
    public void set(int index, int value){
        if(!checkIndex(index))
            return;
        
        data[index] = value;
    }
    
    private boolean checkIndex(int index){
        return index >= 0 && index < size;
    }
    
    public int get(int index){
        if(!checkIndex(index))
            return EMPTY_VALUE;
        
        return data[index];
    }
}
