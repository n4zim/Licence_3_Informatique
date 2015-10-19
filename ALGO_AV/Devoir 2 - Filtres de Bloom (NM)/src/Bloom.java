import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.BitSet;
import java.util.Random;
import java.util.Scanner;

public class Bloom<Key> {
	
	private Random gen;
	private  BitSet bitmap;
	private int numberOfKeys;
	private double bitsElements; 
	private int bitSize; 
	private int k; // nombre de bits à 1
	
	public Bloom(int k, double m, int n ){
		this.numberOfKeys = 0;
		this.bitsElements = m;
		this.k = k; 
		this.bitSize = (int)Math.ceil(m * n);
		this.bitmap = new BitSet(bitSize);
	}
	
	public void add(Key key) {
	}
	
	public boolean probablyContains(Key key) {
		return false;
	}
	

	public static void main(String[] args) throws FileNotFoundException {
		File nameOfSuspects = new File(args[0]);
		ArrayList<Person> listOfSuspects = new ArrayList<>();
		Scanner bufPositive = new Scanner(nameOfSuspects);
		bufPositive.useDelimiter("\n");
		while (bufPositive.hasNext()) {
			listOfSuspects.add(Person.readPerson(bufPositive));
		}
		bufPositive.close();
		int numberOfKeysMain = numberOfKeys;
		Bloom<Person> filter = new Bloom<Person>(numberOfKeysMain, 1, 2);
	}

}