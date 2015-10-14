import java.io.File;
import java.io.FileNotFoundException;
import java.util.Collection;
import java.util.Hashtable;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.Map;
import java.util.Queue;
import java.util.Scanner;
import java.util.Set;

public class Trie {

	private Node root;
	private Collection<Node> nodeTries = new LinkedList<Node>();

	Trie(String filename) throws FileNotFoundException {
		root = new Node(null, '.');
		root.isInDico = true;
		
		Iterable<String> dictionary;
		dictionary = BuildDic(filename);
		for(String word: dictionary) addWord(word);
	}
	
	public String toString() {			
		String output = "ROOT : "+root.toString()+"\nLOOKUP : ";
		Iterator<Node> it = nodeTries.iterator();
		while(it.hasNext()) { output += it.next()+" - "; }
		
		return output;
	}
	
	@SuppressWarnings("unused")
	private class Node {
		char value;
		boolean isInDico;
		Node father;
		Map<Character,Node> ChildrenTable;
		Node suffix, acceptingSuffix;
		
		Node(Node father, char value) {
			 this.value = value;
			 this.father = father;
			 this.isInDico = false;
			 ChildrenTable = new Hashtable<Character,Node>();
			 suffix = null;
			 acceptingSuffix = null;
		}
		
		public void showAllDico() {
			Set<Character> children = this.ChildrenTable.keySet();
			System.out.println(children);
		}
		
		public String toString() {
			/*return "Value : "+value
					+"\tisInDico : "+isInDico
					+"\tFather : "+father;*/
			return Trie.this.toString(this);
		}
		
		public Iterator<Node> iterator() { return nodeTries.iterator(); }
	}
	
	public void addWord(String word) {
		int wordLength = word.length();
		char currentValue;
		Node curentNode = root;
		for(int i=0; i < wordLength; i++) {
			currentValue = word.charAt(i);
			if(curentNode.ChildrenTable.containsKey(currentValue)) {
				curentNode = curentNode.ChildrenTable.get(currentValue);
			
			} else {
				Node children = new Node(curentNode, currentValue);
				curentNode.ChildrenTable.put(currentValue, children);
				curentNode = children;
			}
		
			if((wordLength-1) == i) curentNode.isInDico = true;
		}
	}
	
	private void checkSuffixes() {
		Queue<Node> nodeQueue = new LinkedList<Node>();
		nodeQueue.add(root);
		Node currentNode, nextNode;
		
		while(!nodeQueue.isEmpty()) {
			currentNode = nodeQueue.remove();
			
			for(Map.Entry<Character, Node> mapEntry : currentNode.ChildrenTable.entrySet())
				nodeQueue.add(currentNode.ChildrenTable.get(mapEntry.getKey()));
		
			if(root != currentNode) ((LinkedList<Node>) nodeTries).addLast(currentNode);
			//System.out.println(currentNode.value+"-----------\t");
		}
		
		Iterator<Node> it = nodeTries.iterator();
		root.suffix = root.acceptingSuffix = root;
		
		// Pour chaque mot cherché dans l'arbre
		while(it.hasNext()) {
			nextNode = it.next();
			if(root.ChildrenTable.containsValue(nextNode)) nextNode.suffix = root;
			
			else if(nextNode.father.suffix.ChildrenTable.containsKey(nextNode.value))
				nextNode.suffix = nextNode.father.suffix.ChildrenTable.get(nextNode.value);
				
			else nextNode.suffix = nextNode.father.suffix;

			
			if(nextNode.suffix.isInDico) nextNode.acceptingSuffix = nextNode.suffix;
				else nextNode.acceptingSuffix = nextNode.suffix.acceptingSuffix;
		}
	}

	private static int cpt = 0;
	
	private Node charSearch(Node currentNode, char character, int index) {
		if(currentNode.ChildrenTable.containsKey(character)) {
			currentNode = currentNode.ChildrenTable.get(character);
			if(currentNode.isInDico) System.out.println(++cpt + "\t\t" + index + "\t\t\"" + currentNode.toString() + "\"");
		
		} else currentNode = currentNode.acceptingSuffix;

		return currentNode;
	}
	
	public void wordSearch(String filename) throws FileNotFoundException {
		int index = 0;
		Node currentNode = root;
				
		Scanner buf = new Scanner(new File(filename));
		buf.useDelimiter("");
				
		while(buf.hasNext()) currentNode = charSearch(currentNode, buf.next().charAt(0), ++index);
		buf.close();
		
		System.out.println("\nIl y a dans le texte " + cpt + " correspondances du fichier \"" + filename + "\"");
	}
		
	private static Iterable<String> BuildDic(String filename) throws FileNotFoundException {
		Scanner scanner = new Scanner(new File(filename));
		LinkedList<String> dictionary = new LinkedList<>();
		
		while(scanner.hasNext()) dictionary.add(scanner.next());
		scanner.close();
		
		return dictionary;
	}
	
	public String toString(Node node) {
		if(root != node) return toString(node.father) + ":" + node.value; 
			else return "ROOT";
	}
		
	public static void main(String[] args) throws FileNotFoundException {
		Trie trie  = new Trie("cyranoDict.txt");
		//trie.root.showAllDico();
		trie.checkSuffixes();
		//System.out.println(trie.toString());
		trie.wordSearch("1256-0.txt");
	}
	
}
