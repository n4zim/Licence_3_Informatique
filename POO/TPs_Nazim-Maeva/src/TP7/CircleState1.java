package TP7;

import java.awt.Color;
import java.awt.Graphics;
import java.awt.Point;

public class CircleState1 implements ShapeState {
	private final Point center;
	private Point point;
	
	
	public CircleState1(Point point) { this.center = this.point = point; }

	@Override
	public void paint(ShapePanelContext shapePanelContext, Graphics graphics) {
		int radius = (int)center.distance(point);
		int minX = center.x-radius;
		int minY = center.y-radius;
		int width = 2*radius;
		int height = 2*radius;
		
		graphics.setColor(Color.RED);
		graphics.drawOval(minX, minY, width, height);	
	}

	@Override
	public void mousePressed(ShapePanelContext shapePanelContext, Point point) {}

	@Override
	public void mouseReleased(ShapePanelContext shapePanelContext, Point point) {
		shapePanelContext.shapePanel().add(new Circle(center, point));
		System.out.println("Cercle créé !");
		shapePanelContext.setState(new NullState());
	}

	@Override
	public void mouseMoved(ShapePanelContext shapePanelContext, Point point) {
		this.point = point;
		shapePanelContext.shapePanel().repaint();
	}
}
