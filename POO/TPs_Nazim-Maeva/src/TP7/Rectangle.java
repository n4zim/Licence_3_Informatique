package TP7;
import java.awt.Color;
import java.awt.Graphics;
import java.awt.Point;

public class Rectangle implements Shape {
	private final Point p0, p1;
	
	public Rectangle(Point p0, Point p1) {
		this.p0 = p0;
		this.p1 = p1;
	}
	
	private int minX() { return Math.min(p0.x, p1.x); }
	private int minY() { return Math.min(p0.y, p1.y); }
	private int width() { return Math.abs(p0.x-p1.x); }
	private int height() { return Math.abs(p0.y-p1.y); }

	@Override
	public void paint(Graphics graphics) {
		graphics.setColor(new Color(50, 0, 0, 80));
		graphics.fillRect(minX(), minY(), width(), height());
		graphics.setColor(Color.BLACK);
		graphics.drawRect(minX(), minY(), width(), height());
	}

	@Override
	public boolean contains(Point point) {
		int maxX = minX()+width();
		int maxY = minY()+height();
		return point.x <= maxX && point.y <= maxY && minX() <= point.x && minY() <= point.y;
	}

	@Override
	public void translate(int dx, int dy) {
		p0.x += dx;
		p0.y += dy;
		p1.x += dx;
		p1.y += dy;
	}
}
