import java.util.ArrayList;
import java.util.BitSet;
import java.util.LinkedList;
import java.util.Queue;

public class AldousBroder {
	
	Graph graph;
	Queue<Edge> queue;
	ArrayList<Edge> tree;
	BitSet reached;
	
	public AldousBroder(Graph graph) {		
		this.graph = graph;
		this.tree = new ArrayList<>();
		this.queue = new LinkedList<>();
		this.reached = new BitSet(graph.order);
	}
	
	private void push(int vertex) {
		for(Edge edge : graph.neighbours(vertex)) queue.offer(edge);
	}
	
	private void explore(Edge nextEdge) {
		if(reached.get(nextEdge.getDest())) return;
		reached.set(nextEdge.getDest());
		tree.add(nextEdge);
		push(nextEdge.getDest());
	}
	
	private void main(int startingVertex) {
		reached.set(startingVertex);
		push(startingVertex);
		while(!queue.isEmpty()) explore(queue.poll());
	}

	public static ArrayList<Edge> generateTree(Graph graph) {
		AldousBroder algo = new AldousBroder(graph);
		
		int sommetActuel = 0; //(int)(Math.random()*graph.order);
		System.out.println("Premier sommet : "+sommetActuel);
		
		algo.main(sommetActuel);
		return algo.tree;
	}
	
}
