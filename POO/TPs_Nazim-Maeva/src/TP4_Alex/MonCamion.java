package TP4;

import java.awt.Dimension;
import java.awt.Graphics;

import javax.swing.JPanel;

public class MonCamion extends JPanel 
{
	private static final long serialVersionUID = 1L;

	public MonCamion()
	{
		setPreferredSize(new Dimension(130, 110));
	}
	
	private void peindreCamion(PeintreMarseillais peintre)
	{
		peintre.dessinerRectangle(10, 10, 70, 70);
		peintre.dessinerRectangle(80, 45, 40, 35);
		peintre.dessinerCercle(40, 80, 10);
		peintre.dessinerCercle(100, 80, 10);
	}
	
	protected void paintComponent(Graphics g)
	{
		super.paintComponent(g);
		PeintreMarseillais peintre = new PeintreMarseillais(g);
		peindreCamion(peintre);
	}
}
