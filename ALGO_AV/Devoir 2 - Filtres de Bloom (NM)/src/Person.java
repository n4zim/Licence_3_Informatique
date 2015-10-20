import java.util.Scanner;

public class Person {
	String nom; 
	String prenom; 
	String ville;
	
	public Person(String prenom, String nom, String ville) {
		this.prenom = prenom;
		this.nom = nom; 
		this.ville = ville;
	}
	
	@Override
	public int hashCode() {
		final int prime = 31;
		int result = 1;
		result = prime * result + ((nom == null) ? 0 : nom.hashCode());
		result = prime * result + ((prenom == null) ? 0 : prenom.hashCode());
		result = prime * result + ((ville == null) ? 0 : ville.hashCode());
		return result;
	}
	
	@Override
	public boolean equals(Object obj) {
		if(this == obj) return true;
		if(obj == null) return false;
		
		if(getClass() != obj.getClass()) return false;
		
		Person other = (Person) obj;
		if (nom == null) {
			if (other.nom != null) return false;
		} else if (!nom.equals(other.nom)) return false;
		
		if (prenom == null) {
			if (other.prenom != null) return false;
		} else if (!prenom.equals(other.prenom)) return false;
		
		if (ville == null) {
			if (other.ville != null) return false;
		} else if (!ville.equals(other.ville)) return false;
		
		return true;
	} 
	
	public void personCompare(/* args */){
		// TODO
	} 

	@Override
	public String toString() { return "Person [nom=" + nom + ", prenom=" + prenom + ", ville=" + ville + "]"; }
	
	public static Person readPerson(Scanner bufPositive){
		String prenom = bufPositive.next();
		String nom = bufPositive.next();
		String ville = bufPositive.next(); 
		
		Person person = new Person(prenom, nom, ville);
		return person;
	}
	
	
	
}