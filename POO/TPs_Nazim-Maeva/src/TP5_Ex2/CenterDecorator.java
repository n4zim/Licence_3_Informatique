package TP5_Ex2;

import java.awt.Color;
import java.awt.Graphics;
import java.awt.Point;

public class CenterDecorator extends Decorator {

	private int radius;
	private Color color;
	
	public CenterDecorator(Shape shape, int radius, Color color) {
		super(shape);
		this.radius = radius;
		this.color = color;
	}
	
	private Point getCenter() {
        Point center = decoratedShape.getPoint(0);
        for(int i = 1; i < decoratedShape.getNumberOfPoints(); i++)
	        center.translate(
        		(decoratedShape.getPoint(i).x / (i + 1)),
        		(decoratedShape.getPoint(i).y / (i + 1))
	        );
        return center;
	}

	@Override
	protected void PaintDecoration(Graphics graphics) {
		graphics.setColor(color);
		graphics.fillOval(
			(int)getCenter().getX(),
			(int)getCenter().getX(),
			(radius * 2),
			(radius * 2)
		);
	}	
	
}
