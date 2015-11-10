package TP5_Ex2;

import java.awt.Color;
import java.awt.Graphics;

public class BorderDecorator extends Decorator {

	private int radius;
	private Color color;
	
    public BorderDecorator(Shape shape, int radius, Color color) {
        super(shape);
        this.radius = radius;
        this.color = color;
    }
	
	@Override
	protected void PaintDecoration(Graphics graphics) {
		graphics.setColor(color);
        for(int i = 0; i < decoratedShape.getNumberOfPoints(); i++)
			graphics.fillOval(
				(int)decoratedShape.getPoint(i).getX(),
				(int)decoratedShape.getPoint(i).getY(),
				(radius * 2),
				(radius * 2)
			);
	}
	
}
