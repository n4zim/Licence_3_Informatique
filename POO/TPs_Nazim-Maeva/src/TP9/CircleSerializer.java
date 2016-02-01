package TP9;

import java.util.Scanner;

public class CircleSerializer implements ShapeSerializer<Circle>{
	
	static CircleSerializer instance = new CircleSerializer();
	
	public int code() {
		return 0;
	}
	
	@Override
	public String Serialize(Circle c) {
		return code() + " " + "...";
	}

	@Override
	public Circle unserialize(String s) {
		Scanner scanner = new Scanner(s);
		scanner.next();
		@SuppressWarnings("unused")
		int cx = scanner.nextInt();
		scanner.close();
		return new Circle();
	}

}
