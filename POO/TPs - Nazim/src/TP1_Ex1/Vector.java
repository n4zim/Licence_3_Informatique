package TP1_Ex1;

public class Vector {
	private int[] array; 
	private int size; 
	
	public Vector( int initialcapacity){
		this.size = 0;
		this.array = new int[initialcapacity]; 
	}
	
	public void ensureCapacity(int capacity){
		int oldCapacity = array.length;
		if(capacity < oldCapacity) return;
		int newCapacity = Math.max(capacity, (2*oldCapacity));
		int[] newArray = new int[newCapacity];
		for(int i = 0; i < size; i++){
			newArray[i] = array[i];
		}
		array = newArray; 
		
	}
	
	public void resize(int size){
		ensureCapacity(size);
		this.size = size;
	}
	
	public int size(){
		return this.size;
	}
	
	public boolean isEmpty(){
		return this.size == 0;
	}
	
	public void add(int elmt){
		ensureCapacity(size+1);
		array[size] = elmt;
		size++;
	}
	
	public void set(int index, int elmt){
		if(index < size) return;
		array[index] = elmt;
	}
	
	public int get(int index){
		return array[index];
	}

	public static void main(String[] args) {
		Vector vect = new Vector(10);
		vect.ensureCapacity(15);
		System.out.println("IsEmpty : "+vect.isEmpty()+"\n");
		System.out.println("Size : "+vect.size()+"\n");
		vect.resize(1);
		System.out.println("Size (after resize(1)) : "+vect.size()+"\n");
		vect.add(17);
		System.out.println("Element 5 : "+vect.get(5)+"\n");
		vect.set(vect.size(), 10);
		System.out.println("Last element ("+vect.size()+":10) : "+vect.get(vect.size())+"\n");
		
		
	}

}
