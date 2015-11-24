package TP7;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Point;
import java.awt.RenderingHints;
import java.util.ArrayList;
import java.util.List;
import javax.swing.JPanel;

public class ShapePanel extends JPanel {
	private static final long serialVersionUID = 1L;
	private List<Shape> shapes;
	//private ShapePanelContext context;
	
	public ShapePanel() {
		shapes = new ArrayList<>();
		setPreferredSize(new Dimension(300, 300));
		setFocusable(true);
	}
	
	public void add(Shape shape) {
		shapes.add(shape);
		repaint();
	}
	
	public Shape getShape(Point point) {
		for(int i = shapes.size(); i >= 0; i--) {
			Shape shape = shapes.get(i);
			if(shape.contains(point)) return shape;
		}
		return null;
	}
	
	@Override
	protected void paintComponent(Graphics graphics) {
		super.paintComponent(graphics);
		
		Graphics2D g2d = (Graphics2D)graphics;
		g2d.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
		
		shapes.forEach(s->s.paint(graphics)); // Consumer <Shape>
	}
}
