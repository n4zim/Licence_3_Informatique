package TDTP4;

import java.awt.Graphics;

public class PeintreMarseillais {

	private final Graphics g;
	
	public PeintreMarseillais(Graphics g) {
		this.g = g;
	}
	
	public void dessinerRectangle(int x, int y, int w, int h) {
		g.drawRect(x, y, w, h);
	}
	
	public void dessinerCercle(int x, int y, int r) {
		g.drawOval(x-r, y-r, r*2, r*2);
	}
	
}
