package TP2;

import java.util.Arrays;

public class Vector<T> {
	private Object [] array;
	private int size;

	public Vector(int initialCapacity) {
		array = new Object[initialCapacity];
		size = 0;
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

	@SuppressWarnings("unchecked")
	public T get(int index) {
		if(index >= size) return null;
		return (T)array[index];
	}

	public void resize(int size) {
		ensureCapacity(size);

		for(int i = size; i < this.size; i++)
			array[i] = null;
		this.size = size;
	}

	@Override
	public String toString() {
		StringBuilder builder = new StringBuilder();
		builder.append('[');
		for(int i = 0; i < size; i++)
		{
			if(i>0)
				builder.append(",");
			builder.append(array[i].toString());
		}

		builder.append("]");

		return builder.toString();
	}
}