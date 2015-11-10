package TP5_Ex2;

import java.awt.Color;
import java.awt.Graphics;
import java.awt.Point;
import java.util.List;

public class Polygon extends AbstractShape {

	private Color color;
	
	public Polygon(Color color, List<Point> points) {
		super();
		this.color = color;
		points.forEach(p->addPoints(p));
	}

	@Override
	public void paint(Graphics graphics) {
		
	}

}
