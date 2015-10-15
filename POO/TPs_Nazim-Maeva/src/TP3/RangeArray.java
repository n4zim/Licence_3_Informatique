package TP3;

import java.util.Iterator;

public class RangeArray<T> implements Iterable<T> {
	
	private final int indexMin;
	private final Object[] array;
	
	public RangeArray(int indexMin, int indexMax) {
		this.indexMin = indexMin;
		this.array = new Object[indexMax - indexMin + 1];		
	}

	public int rangeSize() { return this.array.length; }
	public int getIndexMin() { return this.indexMin; }
	public int getIndexMax() { return this.indexMin + this.rangeSize() - 1; }
		
	public void set(int index, T element) { array[this.arrayIndex(index)] = element; }
	
	@SuppressWarnings("unchecked")
	public T get(int index) { return (T)array[arrayIndex(index)]; }
	
	private int arrayIndex(int index) { return index - this.indexMin; }
	
	@Override
	public Iterator<T> iterator() { return new RangeArrayIterator(); }
	
	private class RangeArrayIterator implements Iterator<T> {
		private int nextIndex;
		
		RangeArrayIterator() { nextIndex = getIndexMin(); }
		
		@Override
		public boolean hasNext() { return nextIndex <= getIndexMax(); }
		
		@Override
		public T next() {
			T next = get(nextIndex);
			nextIndex++;
			return next;
		}
	}
	
	public static void main(String[] args) {
		RangeArray<String> habitations = new RangeArray<String>(3, 5);
		habitations.set(3, "Maison");
		habitations.set(4, "Immeuble");
		habitations.set(5, "Hutte");
		
		for(int index = habitations.getIndexMin(); index <= habitations.getIndexMax(); index++)
			System.out.print(habitations.get(index).length() + " ");
		
		System.out.println("\n");
		
		for(String habitation : habitations) System.out.print(habitation + " ");
	}
}