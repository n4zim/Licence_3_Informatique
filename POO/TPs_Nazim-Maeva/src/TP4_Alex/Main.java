package TP4;

import javax.swing.JFrame;

public class Main 
{

	public static void main(String[] args)
	{
		JFrame frame = new JFrame();
		frame.add(new MonCamion());
		frame.pack();
		frame.setVisible(true);
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
	}

}
