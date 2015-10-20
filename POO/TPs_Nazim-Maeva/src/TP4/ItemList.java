package TDTP4;

import java.util.ArrayList;
import java.util.List;

import TP4_Ex3.HTMLListFormat;

public class ItemList {
	private List<String> items;
	
	public enum Type { HTML, LaTex }
	
	public ItemList() { items = new ArrayList<String>(); }
	
	public void add(String item) { items.add(item); }
	
	public void printHTML() {
		System.out.println("<ul>");
		for(String i : items) System.out.println("<li>"+i+"</li>");
		System.out.println("</ul>");
	}

	public void printLaTex() {
		System.out.println("\\begin{itemize}");
		for(String i : items) System.out.println("\\item "+i);
		System.out.println("\\end{itemize}");
	}
	
	public void print(ListFormat format) {
		System.out.println(format.getTop());
        for(String i : items) System.out.println(format.getPre() + i + format.getPost());
        System.out.println(format.getBottom());
	}
	
	public static void main(String[] args) {
		ItemList list = new ItemList();
		list.add("Maison");
		list.add("Immeuble");
		list.add("Hutte");
		list.printHTML();
		System.out.println("---");
		list.printLaTex();
		
		System.out.println("\n");
		
		// Nouvelle méthode avec l'interface
		list.print(new HTMLListFormat());
		System.out.println("---");
		list.print(new LaTexListFormat());
	}

}
