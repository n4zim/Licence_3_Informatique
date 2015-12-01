package TP7;

import java.awt.Graphics;
import java.awt.Point;

public class MoveState0 implements ShapeState {
	private Point point;

	public MoveState0(Point point) { this.point = point; }
	
	@Override
	public void paint(ShapePanelContext shapePanelContext, Graphics graphics) {
		Main.drawCross(point, graphics);
	}

	@Override
	public void mousePressed(ShapePanelContext shapePanelContext, Point point) {
		Shape shape = shapePanelContext.shapePanel().getShape(point);
		if(shape != null) shapePanelContext.setState(new MoveState1(point, shape));
		else {
			System.out.println("Aucune forme séléctionnée");
			shapePanelContext.setState(new NullState());
		}
	}

	@Override
	public void mouseReleased(ShapePanelContext shapePanelContext, Point point) {}

	@Override
	public void mouseMoved(ShapePanelContext shapePanelContext, Point point) {
		this.point = point;
		shapePanelContext.shapePanel().repaint();
	}
}
