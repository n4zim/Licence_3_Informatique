package TP7;
import java.awt.Color;
import java.awt.Graphics;
import java.awt.Point;
import javax.swing.JFrame;

public class Main {
	public static void drawCross(Point point, Graphics graphics) {
		graphics.setColor(Color.BLACK);
		int x = point.x;
		int y = point.y;
		graphics.drawLine(-3+x, 0+y, 3+x, 0+y);
		graphics.drawLine(0+x, -3+y, 0+x, 3+y);
	}
	
	public static void main(String[] args) {
		JFrame frame = new JFrame("POO - TD/TP 7");
		ShapePanel panel = new ShapePanel();

		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.getContentPane().add(panel);
		frame.pack();
		frame.setVisible(true);
	}
}
