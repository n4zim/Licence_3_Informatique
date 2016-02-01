import java.util.Iterator;
import java.util.List;


public class GraphStubs implements Iterable<Edge>{

	
	public GraphStubs(int upperBound) {
		//TODO
	}
	
	public void addVertex(int indexVertex) {
		//TODO
	}
	
	
	public void addArc(Arc arc) {
		//TODO
	}
	
	public void addEdge(Edge e) {
		//TODO
	}
	
	public List<Edge> neighbours(int vertex) {
		//TODO
		return null;
	}
	
	public List<Arc> inNeighbours(int vertex) {
		//TODO
		return null;
	}
	
	public List<Arc> outNeighbours(int vertex) {
		//TODO
		return null;
	}

	public int degree(int vertex) {
		//TODO
		return 0;
	}
	
	public int inDegree(int vertex) {
		//TODO
		return 0;
	}
	
	public int outDegree(int vertex) {
		//TODO
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
