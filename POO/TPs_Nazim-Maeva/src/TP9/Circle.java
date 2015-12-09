package TP9;
import java.io.IOException;

public class Circle implements Shape{
	
	@Override
	public void accept(ShapeVisitor visitor) {
		try {
			visitor.visit(this);
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
}
