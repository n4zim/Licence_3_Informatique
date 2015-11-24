package TP7;

import java.awt.Graphics;
import java.awt.Point;

public interface ShapeState {
	void paint(ShapePanelContext shapePanelContext, Graphics graphics);
	void mousePressed(ShapePanelContext shapePanelContext, Point point);
	void mouseReleased(ShapePanelContext shapePanelContext, Point point);
	void mouseMoved(ShapePanelContext shapePanelContext, Point point);
}
