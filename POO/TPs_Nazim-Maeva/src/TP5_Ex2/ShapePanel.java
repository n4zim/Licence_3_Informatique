package TP5_Ex2;

import java.awt.Graphics;
import java.util.ArrayList;
import javax.swing.JPanel;

public class ShapePanel extends JPanel {
	private static final long serialVersionUID = 1L;
	
	private ArrayList<Shape> shapes;
	
	public ShapePanel() {
		shapes = new ArrayList<>();
	}
	
	public void add(Shape shape) {
		shapes.add(shape);
		repaint();
	}
	
	@Override
	protected void paintComponent(Graphics g) {
		super.paintComponent(g);
		shapes.forEach(s->s.paint(g));
	}
	
}
