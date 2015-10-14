package TP1_Ex3bis;

public class AsciiStringFilter implements StringFilter{

    @Override
    public String filter(String string) {
        StringBuilder sb = new StringBuilder(string.length());
        
        for(int i = 0; i < string.length(); ++i){
            char currentChar = string.charAt(i);
            
            if(currentChar < 128)
                sb.append(currentChar);
        }
        
        return sb.toString();
    }
    
}
