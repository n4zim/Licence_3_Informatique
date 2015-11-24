package TP7;

import java.awt.Graphics;
import java.awt.Point;

public class RectangleState0 implements ShapeState {
	private Point point;
	
	public RectangleState0(Point point) { this.point = point; }

	@Override
	public void paint(ShapePanelContext shapePanelContext, Graphics graphics) {
		Main.drawCross(point, graphics);
	}

	@Override
	public void mousePressed(ShapePanelContext shapePanelContext, Point point) {
		shapePanelContext.setState(new RectangleState1(point));
	}

	@Override
	public void mouseMoved(ShapePanelContext shapePanelContext, Point point) {
		this.point = point;
		shapePanelContext.shapePanel().repaint();
	}
	
	@Override
	public void mouseReleased(ShapePanelContext shapePanelContext, Point point) {}
}
