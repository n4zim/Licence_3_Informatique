package TP9;

public interface ShapeSerializer<S extends Shape> {
	int code();
	String Serialize(S shape);
	S unserialize(String string);

}
