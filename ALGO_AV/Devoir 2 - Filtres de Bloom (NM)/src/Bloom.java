import java.io.File;
import java.io.FileNotFoundException;
import java.math.BigInteger;
import java.util.ArrayList;
import java.util.BitSet;
import java.util.HashSet;
import java.util.Iterator;
import java.util.Random;
import java.util.Scanner;
import java.util.TreeSet;

public class Bloom<Key> {
	
	private int k; // k functions of dispersions
	private int m ; // Bits length
	private int n; // numberOfKeys
	private float p = (float) 0.01; // probability
	
	private int prime;     // prime number 
	private int pairs[][]; // a and b pairs table
	private Random gen;	  
	
	private  BitSet bitmap;
	private HashSet hashset;
	private TreeSet treeset;
	
	
	
	public Bloom(int numberOfKeys) {
		this.gen = new Random();
		
		this.n = numberOfKeys;	
		this.prime = BigInteger.probablePrime(30, gen).intValue(); // prime number generate
		this.m = (int) Math.ceil((n * Math.log(p)) / Math.log(1.0 / (Math.pow(2.0, Math.log(2.0)))));
		this.k = (int) Math.round(Math.log(2.0) * m / n); 
		 
		this.pairs = new int[k][2];
		
		for(int i = 0; i < k; i++){
			pairs[i][0] = gen.nextInt(prime-2)+1; // a
			pairs[i][1] = gen.nextInt(prime-1);   // b
		} // pairs calculate (one for each (k)function of dispersion)
		
		this.bitmap = new BitSet(m);
	}
	
	public void add(Key key) {
		int hash = key.hashCode();
		
	    for(int i=0; i<k; i++)
	    	bitmap.set(Math.abs((pairs[i][0]*hash+pairs[i][1])%prime%m));
	}
	
	public boolean probablyContains(Key key) {
		int hash = key.hashCode();
			
		for(int i=0; i<k; i++)
			if(!bitmap.get(Math.abs((pairs[i][0]*hash+pairs[i][1])%prime%m)))
				return false;
		
		//System.out.println(key.toString());
		
		return true;
	}
	
	public boolean testHashSet(Key key){
		Person liste = getPersonListFromFile();
		for(int i = 0; i < liste)
		return true; 
	}
	
	public ArrayList<Person> getPersonListFromFile(String file) throws FileNotFoundException {	
		ArrayList<Person> liste = new ArrayList<>();
		
		Scanner buf = new Scanner(new File(file));
		buf.useDelimiter("\n");
		while(buf.hasNext()) liste.add(Person.readPerson(buf));
		buf.close();
		
		return liste;
	}
	
	public static void main(String[] args) throws FileNotFoundException {
		long start = System.nanoTime();
		
		Bloom<Person> filter = new Bloom<Person>(577);
		HashSet<Person> filter2 = new HashSet<Person>();
 		
		// --------------> Suspects <--------------
		ArrayList<Person> suspectsList = filter.getPersonListFromFile("liste_suspects_577.txt");
		Iterator<Person> it1 = suspectsList.iterator();
		while(it1.hasNext()) filter.add(it1.next());
		
		ArrayList<Person> suspectsList2 = filter2.getPersonListFromFile("liste_suspects_577.txt");
		Iterator<Person> it12 = suspectsList2.iterator();
		while(it1.hasNext()) filter2.add(it12.next());
		
		// --------------> Liste globale <--------------
		int compteur = 0;
		ArrayList<Person> globalList = filter.getPersonListFromFile("person_list_100k.txt");
		Iterator<Person> it2 = globalList.iterator();		
		while(it2.hasNext()) if(filter.probablyContains(it2.next())) compteur++;
		System.out.println("Nombre : " + compteur);
		
		long end = System.nanoTime()-start;
		System.out.println("Temps d'éxécution : "+ end + " ns (" + (double)end/1000000000 + " s)");
	}

}