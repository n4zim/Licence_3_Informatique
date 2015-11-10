package TP5_Ex2;

import java.awt.Graphics;
import java.awt.Point;

public interface Shape {
	void paint(Graphics graphics);
	int getNumberOfPoints();
	Point getPoint(int index);
}
