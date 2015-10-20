package TDTP4;

import java.awt.Dimension;
import java.awt.Graphics;
import javax.swing.JFrame;
import javax.swing.JPanel;

public class MonCamion extends JPanel {

	private static final long serialVersionUID = 1L;

	public MonCamion() {
		setPreferredSize(new Dimension(130, 110));
	}
	
	private void peindreCamion(PeintreMarseillais peintre) {
		peintre.dessinerRectangle(10, 10, 70, 70);
		peintre.dessinerRectangle(80, 45, 40, 35);
		peintre.dessinerCercle(40, 80, 14);
		peintre.dessinerCercle(100, 80, 14);
	}
	
	protected void paintComponent(Graphics g) {
		super.paintComponent(g);
		PeintreMarseillais peintre = new PeintreMarseillais(g);
		peindreCamion(peintre);
	}
	
	public static void main(String[] args) {
		JFrame frame = new JFrame();
		frame.add(new MonCamion());
		frame.pack();
		frame.setVisible(true);
		frame.setTitle("Camion du turfu");
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
	}
	
}
