import java.io.File;
import java.io.FileNotFoundException;
import java.util.Collection;
import java.util.Hashtable;
import java.util.LinkedList;
import java.util.Map;
import java.util.Queue;
import java.util.Scanner;
import java.util.Set;

public class Trie {

	private Node root;
	Collection<Node> nodeTries = new LinkedList<Node>();

	Trie(String filename) throws FileNotFoundException {
		root = new Node(null, '.');
		
		Iterable<String> dictionary;
		dictionary = BuildDic(filename);
		for(String word: dictionary) addWord(word);
	}
	
	public String toString() {			
		return "ROOT : "+root.toString()
			+"\nLOOKUP : "+nodeTries.toString();
	}
	
	@SuppressWarnings("unused")
	private class Node {
		char value;
		boolean isInDico;
		Node father;
		Map<Character,Node> ChildrenTable;
		
		Node(Node father, char value) {
			 this.value = value;
			 this.father = father;
			 this.isInDico = false;
			 ChildrenTable = new Hashtable<Character,Node>();
		}
		
		public void showAllDico() {
			Set<Character> children = this.ChildrenTable.keySet();
			System.out.println(children);
		}
		
		public String toString() {
			return "Value : "+value
					+"\tisInDico : "+isInDico
					+"\tFather : "+father;
		}
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
				Node children = new Node(curentNode.ChildrenTable.get(currentValue), currentValue);
				curentNode.ChildrenTable.put(currentValue, children);
				curentNode = children;
			}
		
			if((wordLength-1) == i) curentNode.isInDico = true;
		}
	}
	
	public void lookup() {
		Queue<Node> nodeQueue = new LinkedList<Node>();
		nodeQueue.add(root);
		
		Node currentNode;
		
		while(nodeQueue.isEmpty()) {
			currentNode = nodeQueue.remove();
			
			for(@SuppressWarnings("rawtypes") Map.Entry mapEntry : currentNode.ChildrenTable.entrySet()) {
				nodeQueue.add(currentNode.ChildrenTable.get(mapEntry.getKey()));
			}
			
			((LinkedList<Node>) nodeTries).addLast(currentNode);
			
			System.out.println(currentNode.value+"-----------\t");
		}
	}
	
	private static Iterable<String> BuildDic(String filename) throws FileNotFoundException {
		Scanner scanner = new Scanner(new File(filename));
		LinkedList<String> dictionary = new LinkedList<>();
		
		while(scanner.hasNext()) dictionary.add(scanner.next());
		scanner.close();
		
		return dictionary;
	}
		
	public static void main(String[] args) throws FileNotFoundException {
		Trie trie  = new Trie("1256-0.txt");
		//trie.root.showAllDico();
		trie.lookup();
		System.out.println(trie.toString());
	}
	
}
