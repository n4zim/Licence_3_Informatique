package TP4;

import java.util.ArrayList;
import java.util.List;

public class Filter<T>
{
	public final Predicate<T> predicate;
	
	public Filter(Predicate<T> predicate)
	{
		this.predicate = predicate;
	}
	
	public List<T> apply(List<T> list)
	{
		List<T> result = new ArrayList<>();
		
		for(T i : list)
			if(predicate.test(i))
				result.add(i);
		
		return result;
	}
}
