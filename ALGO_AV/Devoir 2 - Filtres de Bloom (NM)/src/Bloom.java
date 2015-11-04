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
	
	private BitSet bitmap;	
	
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
	
	/*public boolean testHashSet(Key key){
		ArrayList<Person> liste = getPersonListFromFile("liste_suspects_577.txt");
		for(int i = 0; i < liste)
		return true; 
	}*/
	
	public ArrayList<Person> getPersonListFromFile(String file) throws FileNotFoundException {	
		ArrayList<Person> liste = new ArrayList<>();
		
		Scanner buf = new Scanner(new File(file));
		buf.useDelimiter("\n");
		while(buf.hasNext()) liste.add(Person.readPerson(buf));
		buf.close();
		
		return liste;
	}
	
	public static void main(String[] args) throws FileNotFoundException {		
		Bloom<Person> filter = new Bloom<Person>(577);
		HashSet<Person> filter2 = new HashSet<Person>();
		TreeSet<Person> filter3 = new TreeSet<Person>();
		
		long start = System.nanoTime();
 		
		// --------------> Suspects <--------------
		ArrayList<Person> suspectsList = filter.getPersonListFromFile("liste_suspects_577.txt");
		Iterator<Person> it1 = suspectsList.iterator();
		while(it1.hasNext()) filter.add(it1.next());
		long step1 = System.nanoTime()-start;
		
		/* ------------------------------------- */
		ArrayList<Person> suspectsList2 = new ArrayList<>();
		Scanner buf2 = new Scanner(new File("liste_suspects_577.txt")); buf2.useDelimiter("\n");
		while(buf2.hasNext()) suspectsList2.add(Person.readPerson(buf2)); buf2.close();
		Iterator<Person> it2 = suspectsList2.iterator();
		while(it2.hasNext()) filter2.add(it2.next());
		long step2 = System.nanoTime()-start-step1;
		
		/* ------------------------------------- */
		ArrayList<Person> suspectsList3 = new ArrayList<>();
		Scanner buf3 = new Scanner(new File("liste_suspects_577.txt")); buf3.useDelimiter("\n");
		while(buf3.hasNext()) suspectsList3.add(Person.readPerson(buf3)); buf3.close();
		Iterator<Person> it3 = suspectsList3.iterator();
		while(it3.hasNext()) filter3.add(it3.next());
		long step3 = System.nanoTime()-start-step1-step2;

		// --------------> Liste globale <--------------
		int compteur = 0;
		ArrayList<Person> globalList = filter.getPersonListFromFile("person_list_100k.txt");
		Iterator<Person> it = globalList.iterator();		
		while(it.hasNext()) if(filter.probablyContains(it.next())) compteur++;
		System.out.println("Nombre : " + compteur);

		long end = System.nanoTime()-start;
		System.out.println("Temps d'éxécution global : "+ end + " ns (" + (double)end/1000000000 + " s)");
		System.out.println("------- Bloom : "+ step1 + " ns (" + (double)step1/1000000000 + " s)");
		System.out.println("------- HashSet : "+ step2 + " ns (" + (double)step2/1000000000 + " s)");
		System.out.println("------- TreeSet : "+ step3 + " ns (" + (double)step3/1000000000 + " s)");
	}

}