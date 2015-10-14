package TP1_Ex3bis;

public class PostfixStringFilter implements StringFilter{
    final private int postfixLength;

    public PostfixStringFilter(int postfixLength) {
        this.postfixLength = postfixLength;
    }
    
    @Override
    public String filter(String string) {
        int beginIndex = string.length() - postfixLength;
        
        if(beginIndex < 0)
            beginIndex = 0;
        
        return string.substring(beginIndex);
    }
    
}
