package TP3;

import java.util.Iterator;

public class Grid<T> implements Iterable<T> {
	
	private T[][] elements;
	
	public Grid(T[][] elements) { this.elements = elements; }
	
	public T get(int line, int column) { return elements[line][column]; }
	
	public int nbLines() { return elements.length; }
	
	public int nbColumns() { return elements[0].length; }

	@Override
	public Iterator<T> iterator() { return new GridIterator(this); }
	
	private class GridIterator implements Iterator<T> {
		private Grid<T> grid;
		private int line, column;
		
		public GridIterator(Grid<T> grid) {
			this.grid = grid;
			line = 0;
			column = 0;
		}

		@Override
		public boolean hasNext() { return line < grid.nbLines(); }

		@Override
		public T next() {
			T next = grid.get(line, column);
			if(++column >= grid.nbColumns()) {
				line++;
				column = 0;
			}
			return next;
		}
		
	}
	
	public static void main(String[] args) {
		String[][] elements = { {"A", "C", "E", "G" }, {"B", "D", "F", "H"} };
		Grid<String> grid = new Grid<String>(elements);
		
		for(int line = 0; line < grid.nbLines(); line++)
			for(int column = 0; column < grid.nbColumns(); column++)
				System.out.print(grid.get(line, column) + " ");
		
		System.out.println("\n");
		
		for(String element : grid) System.out.print(element + " ");
	}

}
