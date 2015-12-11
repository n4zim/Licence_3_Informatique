import java.util.ArrayList;
import java.util.BitSet;
import java.util.Collections;
import java.util.List;
import java.util.Stack;

public class RandomWay {
	Graph graph;
	Stack<Arc> frontier;
	ArrayList<Arc> tree;
	BitSet reached;
	
	private void push(int vertex) {
		List<Arc> neighbours = new ArrayList<>(graph.outNeighbours(vertex));
		Collections.shuffle(neighbours);
        for(int i=0; i < neighbours.size(); i++) frontier.push(neighbours.get(i));
	}
	
	private void explore(Arc nextArc) {
		if(reached.get(nextArc.getDest())) return;
		reached.set(nextArc.getDest());
		tree.add(nextArc);
		push(nextArc.getDest());
	}

	private void main(int startingVertex) {
		reached.set(startingVertex);
		push(startingVertex);
		while(!frontier.isEmpty()) explore(frontier.pop());
	}
	
	private RandomWay(Graph graph) {
		this.graph = graph;
		this.frontier = new Stack<>();
		this.tree = new ArrayList<>();
		this.reached = new BitSet(graph.order);
	}
	
	public static ArrayList<Arc> generateTree(Graph graph) {
		RandomWay algo = new RandomWay(graph);		
		algo.main((int)(Math.random()*graph.order+1));
		return algo.tree;
	}
	
}
