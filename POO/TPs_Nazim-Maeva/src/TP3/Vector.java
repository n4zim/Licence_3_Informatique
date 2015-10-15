package TP3;

import java.util.Arrays;
import java.util.Iterator;

import TP3.Grid.GridIterator;

public class Vector<T extends Comparable<T>> implements Iterable<T> {
	private Object [] array;
	private int size;

	public Vector(int initialCapacity) {
		array = new Object[initialCapacity];
		size = 0;
		
		VectorIterator it = new VectorIterator();
	}

    private void ensureCapacity(int capacity) {
        if(capacity < array.length)
            return;
        
        int newCapacity = Math.max(capacity, array.length * 2);
        array = Arrays.copyOf(array, newCapacity);
    }
	
	public void add(T element) {
		ensureCapacity(size + 1);
		
		array[size] = element;
		size++;
	}

	public void addAll(Iterable <? extends T> it) {
		for(T e : it) add(e);
	}

	@SuppressWarnings("unchecked")
	public T get(int index) {
		if(index >= size) return null;
		return (T)array[index];
	}
	
	public void set(int index, T element){
		array[index] = element;
	}

	public int size() { return this.size; }
	
	public void resize(int size) {
		ensureCapacity(size);
		for(int i = size; i < this.size; i++) array[i] = null;
		this.size = size;
	}

	public boolean isEmpty(){
		if(array.length == 0){
			return true;
		}
		return false;
		
	}
	
	public boolean isSorted() {
		for(int i=1; i<size(); i++) {
			if(get(i-1).compareTo(get(i)) > 0) {
				return false;
			}
		}
		
		return true;
	}
	
	private class VectorIterator implements Iterator<T>{
		private int current = 0;
			
		@Override
		public boolean hasNext() {
			return current < size();
		}

		@SuppressWarnings("unchecked")
		@Override
		public T next() {
			return (T) array[++current];
		}
		
	}

	@Override
	public String toString() {
		StringBuilder builder = new StringBuilder();
		builder.append('[');
		for(int i = 0; i < size; i++) {
			if(i > 0) builder.append(",");
			builder.append(array[i].toString());
		}
		builder.append("]");
		return builder.toString();
	}
	
	public static void main(String[] args) {
		Vector<String> vect = new Vector<String>(2);
		vect.add("Test");
		
		vect.toString();
	}

	@Override
	public Iterator<T> iterator() { return new VectorIterator(); }

}
