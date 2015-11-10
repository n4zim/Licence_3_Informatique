package TP5_Ex2;

import java.awt.Point;
import java.util.ArrayList;
import java.util.List;

public abstract class AbstractShape implements Shape {

	private List<Point> points;

	public AbstractShape() {
		points = new ArrayList<>();
	}
	
	protected void addPoints(Point p) {
		points.add(p);
	}
	
	public int getNumberOfPoints() {
		return points.size();
	}
	
	public Point point(int index) {
		return points.get(index);
	}
	
	public abstract point(Graphics graphics) {
		
	}
	
}
