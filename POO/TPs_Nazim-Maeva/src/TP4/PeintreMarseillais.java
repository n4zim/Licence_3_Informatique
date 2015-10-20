package TP4;

import java.awt.Graphics;

public class PeintreMarseillais
{
	
	private final Graphics g;
	
	public PeintreMarseillais(Graphics g)
	{
		this.g = g;
	}

	public void dessinerRectangle(int x, int y, int width, int height)
	{
		g.drawRect(x, y, width, height);
	}

	public void dessinerCercle(int x, int y, int rayon)
	{
		g.drawOval(x-rayon, y-rayon, rayon*2, rayon*2);
	}

}
