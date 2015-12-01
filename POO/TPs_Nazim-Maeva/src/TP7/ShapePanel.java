package TP7;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Point;
import java.awt.RenderingHints;
import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.util.ArrayList;
import java.util.List;
import javax.swing.JPanel;

public class ShapePanel extends JPanel {
	private static final long serialVersionUID = 1L;
	private List<Shape> shapes;
	private ShapePanelContext context;
	
	public ShapePanel() {
		shapes = new ArrayList<>();
		setPreferredSize(new Dimension(300, 300));
		setFocusable(true);
		addKeyListener(new KeyListener());
		MouseListener mouselistener = new MouseListener();
		addMouseListener(mouselistener);
		addMouseMotionListener(mouselistener);
		
		context = new ShapePanelContext(this);
	}
	
	public void add(Shape shape) {
		shapes.add(shape);
		repaint();
	}
	
	public Shape getShape(Point point) {
		for(int i = shapes.size()-1; i >= 0; i--) {
			Shape shape = shapes.get(i);
			if(shape.contains(point)) { return shape; }
		}
		return null;
	}
	
	@Override
	protected void paintComponent(Graphics graphics) {
		super.paintComponent(graphics);
		
		Graphics2D g2d = (Graphics2D)graphics;
		g2d.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
		
		shapes.forEach(s->s.paint(graphics)); // Consumer <Shape>
		
		context.paint(graphics);
	}
	
	private class MouseListener extends MouseAdapter {
		@Override public void mousePressed(MouseEvent e) { context.mousePressed(e.getPoint()); }
		@Override public void mouseReleased(MouseEvent e) { context.mouseReleased(e.getPoint()); }
		@Override public void mouseDragged(MouseEvent e) { context.mouseMoved(e.getPoint()); }
		@Override public void mouseMoved(MouseEvent e) { context.mouseMoved(e.getPoint()); }
	}
	private class KeyListener extends KeyAdapter {
		@Override
		public void keyTyped(KeyEvent e) {
			Point point = getMousePosition();
			if(point != null) context.keyTyped(e.getKeyChar(), point);
		}
	}
}
