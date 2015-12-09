package TP9;

import java.io.IOException;
import java.io.PrintWriter;
import java.io.Writer;
import java.util.List;

public class ShapeWriter implements ShapeVisitor {
	
	private PrintWriter writer;
	
	public ShapeWriter(Writer writer)
	{
		this.writer = new PrintWriter(writer);
	}
	
	public void write(Shape shape) throws IOException
	{
		shape.accept(this);
	}
	
	public void writeAll(List<Shape> shapes) throws IOException
	{
		for(Shape shape: shapes) 
			write(shape);
	}
	
	public void close()
	{
		writer.close();
	}
	
	public void visit(Circle circle) throws IOException
	{
		writer.println(CircleSerializer.instance.serialize(circle));
	}
	
	public void visit(Rectangle rectangle) throws IOException
	{
		writer.println(RectangleSerializer.instance.serialize(rectangle));
	}
}
