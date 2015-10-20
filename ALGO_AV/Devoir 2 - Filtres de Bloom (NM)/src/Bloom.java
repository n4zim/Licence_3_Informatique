import java.io.File;
import java.io.FileNotFoundException;
import java.math.BigInteger;
import java.nio.charset.Charset;
import java.util.ArrayList;
import java.util.BitSet;
import java.util.Random;
import java.util.Scanner;

public class Bloom<Key> {
	
	private int k; // k functions of dispersions
	private int m ; // Bits length
	private int n; // numberOfKeys
	private int p = (int) 0.01; // probability
	
	private int prime;     // prime number 
	private int pairs[][]; // a and b pairs table
	private Random gen;	  
	
	private  BitSet bitmap;
	
	public Bloom(int numberOfKeys) {
		this.n = numberOfKeys;	
		this.m = (int) Math.ceil((n * Math.log(p)) / Math.log(1.0 / (Math.pow(2.0, Math.log(2.0)))));
		this.k = (int) Math.round(Math.log(2.0) * m / n); 
		this.prime = BigInteger.probablePrime(30, gen).intValue(); // prime number generate 
		this.pairs = new int[k][2];
		
		for(int i = 0; i < k; i++){
			pairs[i][0] = gen.nextInt(prime-2)+1; // a
			pairs[i][1] = gen.nextInt(prime-1);   // b
		} // pairs calculate (one for each (k)function of dispersion)
		
		this.bitmap = new BitSet(m);
	}
	
	public void add(Key key) {
		//int keyhash = key.hashCode();
	    //bitmap.set(Math.abs(keyhash % m), true);
		
	}
	
	public boolean probablyContains(Key key) {
		return false;
	}
	

	public static void main(String[] args) throws FileNotFoundException {
	// --------------> Person file reader <-------------- 
		File nameOfSuspects = new File(args[0]);
		ArrayList<Person> listOfSuspects = new ArrayList<>();
		Scanner bufPositive = new Scanner(nameOfSuspects);
		bufPositive.useDelimiter("\n");
		while (bufPositive.hasNext()) {
			listOfSuspects.add(Person.readPerson(bufPositive));
		}
		bufPositive.close();
	// --------------------> END <----------------------
		
		Bloom<Person> filter = new Bloom<Person>(577);
	}

}