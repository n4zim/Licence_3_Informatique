package TP7;
import java.awt.Graphics;
import java.awt.Point;

public class ShapePanelContext {
	private ShapeState currentState;
	private ShapePanel panel;
	
	public ShapePanelContext(ShapePanel panel) {
		this.panel = panel;
		currentState = new NullState();
	}
	
	public void setState(ShapeState state) {
		this.currentState = state;
		panel.repaint();
	}
	
	public ShapePanel shapePanel() { return panel; }
	
	public void paint(Graphics graphics) { currentState.paint(this, graphics); }
	
	public void mousePressed(Point point) { currentState.mousePressed(this, point); }
	public void mouseReleased(Point point) { currentState.mouseReleased(this, point); }
	public void mouseMoved(Point point) { currentState.mouseMoved(this, point); }

	public void keyTyped(char keyChar, Point point) {
		switch(keyChar) {
			case 'c':
				setState(new CircleState0(point));
				System.out.println("Etat : Cercle");
				break;
			case 'r':
				setState(new RectangleState0(point));
				System.out.println("Etat : Rectangle");
				break;
			case 'm':
				setState(new MoveState0(point));
				System.out.println("Etat : Move");
		}
	}
}
