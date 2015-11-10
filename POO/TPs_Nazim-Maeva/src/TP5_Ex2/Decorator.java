package TP5_Ex2;

import java.awt.Graphics;
import java.awt.Point;

public abstract Decorator implements Shape {
	
	protected Shape decoratedShape;
	
	public Decorator(Shape decoratedShape) {
		this.decoratedShape = decoratedShape;
	}
	
	protected void PaintDecoration(Graphics graphics) {
		
	}

	@Override
	public void paint(Graphics graphics) {
		decoratedShape.paint(graphics);
		paintDecoration(graphics);
	}

	@Override
	public int getNumberOfPoints() {
		return decoratedShape.getNumberOfPoints();
	}

	@Override
	public Point point(int index) {
		return decoratedShape.point(index);
	}
	
	public void paint(Graphics graphics) {
		decoratedShape.point(graphics);
		paintDecorated(graphics);
	}

}