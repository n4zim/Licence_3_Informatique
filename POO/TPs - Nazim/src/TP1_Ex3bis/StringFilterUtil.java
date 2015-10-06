package TP1_Ex3bis;

public class StringFilterUtil {
    String[] filter(String[] strings, StringFilter filter){
        String[] filtered = new String[strings.length];
        
        for(int i = 0; i < strings.length; ++i){
            filtered[i] = filter.filter(strings[i]);
        }
        
        return filtered;
    }
}
