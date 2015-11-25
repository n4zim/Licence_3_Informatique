import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;

public class Graph {
	public int order;
	private HashSet<Integer> vertices;

	private HashMap<Integer,List<Edge>> neighbours;
	private HashMap<Integer,List<Arc>> inNeighbours;
	private HashMap<Integer,List<Arc>> outNeighbours;
	
	public Graph(int upperBound) {
		order = upperBound;
		
		vertices =  new HashSet<>();
		neighbours = new HashMap<>();
		inNeighbours = new HashMap<>();
		outNeighbours = new HashMap<>();
	}
		
	public void addVertex(int indexVertex) {
		if(!vertices.contains(indexVertex)) {
			vertices.add(indexVertex);
			neighbours.put(indexVertex, new ArrayList<>());
			outNeighbours.put(indexVertex, new ArrayList<>());
			inNeighbours.put(indexVertex, new ArrayList<>());
		}
	}
	
	public void addArc(Arc arc) {
		inNeighbours.get(arc.getDest()).add(arc);
		outNeighbours.get(arc.getSource()).add(arc);

	}
	
	public void addEdge(Edge e) {
		addVertex(e.dest);
		neighbours.get(e.dest).add(e);
		addVertex(e.source);
		neighbours.get(e.source).add(e);
		
		addArc(new Arc(e,false));
		addArc(new Arc(e,true));
	}
	
	public List<Edge> neighbours(int vertex) { return neighbours.get(vertex); }
	
	public List<Arc> inNeighbours(int vertex) { return inNeighbours.get(vertex); }
	
	public List<Arc> outNeighbours(int vertex) { return outNeighbours.get(vertex); }

	public int degree(int vertex) {
		return neighbours.get(vertex).size();
	}
	
	public int inDegree(int vertex) {
		return inNeighbours.get(vertex).size();
	}
	
	public int outDegree(int vertex) {
		return outNeighbours.get(vertex).size();
	}
}
