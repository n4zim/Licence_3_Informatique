import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.ThreadLocalRandom;

public class AldousBroder {
	Graph graph;
	ArrayList<Arc> tree;
	//Queue<Edge> queue;
	//BitSet reached;
	
	public AldousBroder(Graph graph) {		
		this.graph = graph;
		this.tree = new ArrayList<>();
		//this.queue = new LinkedList<>();
		//this.reached = new BitSet(graph.order);
	}
	
	private boolean checkNextNeighbour(int sommetActuel) {
		/*System.out.print(nextEdge.source +" -> "+ nextEdge.dest);
		if(!reached.get(nextEdge.getDest())) {
			System.out.println("\tDéjà passé O");
			//push(nextEdge.getDest());
			tree.add(nextEdge);
			//queue.poll();
		} else {
			System.out.println("\tDécouverte X");
			reached.set(nextEdge.getDest());
		}*/
		
		int nextEdge;
		List<Edge> neighbours = this.graph.neighbours(sommetActuel);
		nextEdge = ThreadLocalRandom.current().nextInt(0, neighbours.size());
		Edge currentEdge = neighbours.get(nextEdge);
		
		System.out.print(currentEdge.source +" -> "+ currentEdge.dest);
		
		Arc newArc;
		if(currentEdge.getSource() == sommetActuel) {
			System.out.println("\tDéjà passé O");
			newArc = new Arc(currentEdge, false);
			if(!this.tree.contains(newArc)) this.tree.add(newArc);
		
		} else {
			System.out.println("\tDécouverte X");
			newArc = new Arc(currentEdge, true);
			if(!this.tree.contains(newArc)) this.tree.add(newArc);
		}
		
		sommetActuel = nextEdge;
		
		if(this.tree.size() >= (this.graph.order-1)) return false;
		return true;
	}
	
	private void main(int premierSommet) {
		//reached.set(vertex);
		//push(vertex);
		boolean loop = true;
		System.out.println("-------- Début de la recherche --------");
		while(loop) loop = checkNextNeighbour(premierSommet);
		System.out.println("-------- Fin de la recherche --------");
		/*List<Edge> neighbours = this.graph.neighbours(vertex);
		vertex = neighbours.get(ThreadLocalRandom.current().nextInt(0, neighbours.size())).getDest();
		checkNextNeighbour(neighbours.get(vertex));*/
	}

	public static ArrayList<Arc> generateTree(Graph graph) {
		AldousBroder algo = new AldousBroder(graph);
		
		int premierSommet = (int)(Math.random()*graph.order);
		System.out.println("Premier sommet : "+premierSommet);
		algo.main(premierSommet);
		
		return algo.tree;
	}
	
}
