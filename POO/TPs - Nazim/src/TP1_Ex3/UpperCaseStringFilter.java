package TP1_Ex3;

public class UpperCaseStringFilter implements StringFilter {
	
	public String filter(String str) {
		return str.toUpperCase();
	}

	public static void main(String[] args) {
		String test = "Ceci est un test";
		UpperCaseStringFilter ucf = new UpperCaseStringFilter();
		
		System.out.println(test+" -> "+ucf.filter(test));
	}
	
}
