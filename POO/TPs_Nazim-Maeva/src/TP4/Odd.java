package TP4;

public class Odd implements Predicate<Integer>
{
	
	@Override
	public boolean test(Integer i)
	{
		return (i%2 == 1);
	}

}
