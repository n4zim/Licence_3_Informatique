import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;


public class Graph implements Iterable<Edge> {

	int upperBound;
	List<Arc> arcs;
	boolean isOriented;
	
	public Graph(int upperBound) {
		this.upperBound = upperBound;
		arcs = new ArrayList<Arc>();
		isOriented = false;
	}
	
	public void addVertex(int indexVertex) {
		
	}
	
	public void addArc(Arc arc) {
		
	}
	
	public void addEdge(Edge e) {
		
	}
	
	public List<Edge> neighbours(int vertex) {
		return null;
	}
	
	public List<Arc> inNeighbours(int vertex) {
		return null;
	}
	
	public List<Arc> outNeighbours(int vertex) {
		return null;
	}

	public int degree(int vertex) {
		return 0;
	}
	
	public int inDegree(int vertex) {
		return 0;
	}
	
	public int outDegree(int vertex) {
		return 0;
	}
	
	private class EdgeIterator implements Iterator<Edge>{
		
		
		public EdgeIterator() {
			//TODO
		}
		
		
		public boolean hasNext() {
			//TODO
			return false;
		}
		
		public Edge next() {
			//TODO
			return null;
		}
		
		public void remove() {
			return;
		}

	}
	
	public Iterator<Edge> iterator() {
		return new EdgeIterator();
	}
	
}
