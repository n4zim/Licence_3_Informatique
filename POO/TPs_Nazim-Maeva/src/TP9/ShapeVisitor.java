package TP9;

import java.io.IOException;

public interface ShapeVisitor {
	void visit(Circle circle) throws IOException;
	void visit(Rectangle rectangle) throws IOException;
}
