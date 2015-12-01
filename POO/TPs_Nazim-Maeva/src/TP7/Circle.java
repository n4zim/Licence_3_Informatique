package TP7;
import java.awt.Color;
import java.awt.Graphics;
import java.awt.Point;

public class Circle implements Shape {
	private Point center, point;
	
	public Circle(Point center, Point point) {
		this.center = center;
		this.point = point;
	}

	private int radius() {
		//return (int)Math.sqrt(Math.pow((center.x-point.x),2)+Math.pow((center.y-point.y),2));
		return (int)center.distance(point);
	}
	private int minX() { return center.x-radius(); }
	private int minY() { return center.y-radius(); }
	private int width() { return 2*radius(); }
	private int height() { return 2*radius(); }

	
	@Override
	public void paint(Graphics graphics) {
		graphics.setColor(new Color(50, 0, 0, 80));
		graphics.fillOval(minX(), minY(), width(), height());
		graphics.setColor(Color.BLACK);
		graphics.drawOval(minX(), minY(), width(), height());
	}

	@Override
	public boolean contains(Point point) { return radius() >= center.distance(point); }

	@Override
	public void translate(int dx, int dy) {
		point.x += dx;
		point.y += dy;
		center.x += dx;
		center.y += dy;
	}
}
