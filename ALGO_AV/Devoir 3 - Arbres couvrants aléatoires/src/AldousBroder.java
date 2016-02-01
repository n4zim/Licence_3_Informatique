import java.util.ArrayList;
import java.util.BitSet;
import java.util.List;
import java.util.concurrent.ThreadLocalRandom;

public class AldousBroder {
	Graph graph;
	ArrayList<Edge> tree;
	BitSet reached;
	
	public AldousBroder(Graph graph) {
		this.graph = graph;
		this.tree = new ArrayList<>();
		this.reached = new BitSet(graph.order+1);
	}
	
	private void main(boolean debugDisplay) {
		int current = (int)(Math.random()*this.graph.order+1);
		List<Arc> neighbours;
		Arc currentArc;
		reached.set(current);
		
		while(tree.size() < graph.order) {
			neighbours = this.graph.outNeighbours(current);
			currentArc = neighbours.get(ThreadLocalRandom.current().nextInt(0, neighbours.size()));
			
			if(debugDisplay) System.out.print(currentArc.getSource() +" -> "+ currentArc.getDest()+"  ");
			
			if(!reached.get(currentArc.getDest())) {
				if(debugDisplay) System.out.println("\tDécouverte X");
				tree.add(currentArc.support);
				reached.set(currentArc.getDest());
			} else if(debugDisplay) System.out.println("\tDéjà passé O");
			
			current = currentArc.getDest();
		}
	}	
	
	public static ArrayList<Edge> generateTree(Graph graph, boolean debugDisplay) {
		AldousBroder algo = new AldousBroder(graph);
		if(debugDisplay) System.out.println("-------- Début de la recherche --------");
		algo.main(debugDisplay);
		if(debugDisplay) System.out.println("-------- Fin de la recherche --------");
		return algo.tree;
	}
}
