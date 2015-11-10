package TP5_Ex2;

import java.awt.Dimension;
import javax.swing.JFrame;

public class Main {

	public static void main(String[] args) {
		JFrame frame = new JFrame("Shapes");
		ShapePanel panel = new ShapePanel();
		
		//panel.add(comp);

		panel.setPreferredSize(new Dimension(300, 300));
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.getContentPane().add(panel);
		frame.pack();
		frame.setVisible(true);
	}

}
