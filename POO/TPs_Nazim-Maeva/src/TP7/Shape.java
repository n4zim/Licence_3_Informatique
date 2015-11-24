package TP7;
import java.awt.Graphics;
import java.awt.Point;

public interface Shape {
	public void paint(Graphics graphics);
	public boolean contains(Point point);
	public void translate(int dx, int dy);
}
