package TP9;

public class RectangleSerializer {
	
	static RectangleSerializer instance = new RectangleSerializer();
	
	public int code()
	{
		return 0;
	}
	
	@Override
	public String Serialize(Rectangle c) {
		return code() + " " + "...";
	}

	@Override
	public Rectangle unserialize(String s)
	{
		Scanner scanner = new Scanner(s);
		scanner.next();
		int cx = scanner.nextInt();
		scanner.close();
		return new Rectangle('ShapeFactory');
	}


}
