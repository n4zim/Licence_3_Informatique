package TP1_Ex3;

public class StringFilterTools
{
	static public String[] filter(String[] strings, StringFilter filter)
	{
		String[] result = new String[strings.length];

		for(int i = 0; i < strings.length; i++)
			result[i] = filter.filter(strings[i]);

		return result;
	}
}