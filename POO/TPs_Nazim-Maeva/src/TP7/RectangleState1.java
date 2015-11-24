package TP7;

import java.awt.Color;
import java.awt.Graphics;
import java.awt.Point;

public class RectangleState1 implements ShapeState {
	private final Point point0;
	private Point point1;
	
	public RectangleState1(Point point) {
		this.point0 = this.point1 = point;
	}
	
	@Override
	public void paint(ShapePanelContext shapePanelContext, Graphics graphics) {
		int minX = Math.min(point0.x, point1.x);
		int minY = Math.min(point0.y, point1.y);
		int width = Math.abs(point0.x-point1.x);
		int height = Math.abs(point0.y-point1.y);
		
		graphics.setColor(Color.BLACK);
		graphics.drawRect(minX, minY, width, height);
	}

	@Override
	public void mousePressed(ShapePanelContext shapePanelContext, Point point) {}

	@Override
	public void mouseReleased(ShapePanelContext shapePanelContext, Point point) {
		shapePanelContext.shapePanel().add(new Rectangle(point0, point1));
		System.out.println("Rectangle créé !");
		shapePanelContext.setState(new NullState());
	}

	@Override
	public void mouseMoved(ShapePanelContext shapePanelContext, Point point) {
		this.point1 = point;
		shapePanelContext.shapePanel().repaint();
	}
}
