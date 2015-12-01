package TP8;

public class Main {
	public static void main(String[] args) {
		Variable a = new Variable("a", 2);
		Variable b = new Variable("b", 2);
		Variable c = new Variable("c", 3);
		
		Formula f1 = new Product(new Sum(new Product(c,b),c), new Sum(a,b)); // (b+c)*(a+b)=20
		
		switch(2) { // Modifier le nombre ici pour les tests
		
		case 1:
			PlainPrinter plain = new PlainPrinter();
			System.out.println(f1.accept(plain));
			break;
		
		case 2:
			XMLPrinter xml = new XMLPrinter();
			System.out.println(f1.accept(xml));
			break;
		
		case 3:
			Evaluator ev = new Evaluator();
			System.out.println(f1.accept(ev));
			break;
		
		default:
			System.out.println("Indiquer un nombre valide dans le Main du programme.");
			break;
		}
	}
}
