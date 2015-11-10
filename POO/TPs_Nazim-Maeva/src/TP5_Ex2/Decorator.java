package TP5_Ex2;

import java.awt.Graphics;
import java.awt.Point;

public abstract class Decorator implements Shape {
	
	protected Shape decoratedShape;
	
	public Decorator(Shape decoratedShape) {
		this.decoratedShape = decoratedShape;
	}
	
	abstract protected void PaintDecoration(Graphics graphics);

	@Override
	public void paint(Graphics graphics) {
		decoratedShape.paint(graphics);
		PaintDecoration(graphics);
	}

	@Override
	public int getNumberOfPoints() {
		return decoratedShape.getNumberOfPoints();
	}

	@Override
	public Point point(int index) {
		return decoratedShape.point(index);
	}

}