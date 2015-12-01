package TP7;
import java.awt.Graphics;
import java.awt.Point;

public class MoveState1 implements ShapeState {
	private Point point;
	private Shape shape;

	public MoveState1(Point point, Shape shape) {
		this.point = point;
		this.shape = shape;
	}
	
	@Override
	public void paint(ShapePanelContext shapePanelContext, Graphics graphics) {}

	@Override
	public void mousePressed(ShapePanelContext shapePanelContext, Point point) {}

	@Override
	public void mouseReleased(ShapePanelContext shapePanelContext, Point point) {
		System.out.println("Objet déplacé !");
		shapePanelContext.setState(new NullState());
	}

	@Override
	public void mouseMoved(ShapePanelContext shapePanelContext, Point point) {
		shape.translate(point.x-this.point.x, point.y-this.point.y);
		this.point = point;
		shapePanelContext.shapePanel().repaint();
	}

}
