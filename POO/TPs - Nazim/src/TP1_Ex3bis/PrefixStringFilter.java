package TP1_Ex3bis;

public class PrefixStringFilter implements StringFilter{
    final private int prefixLength;

    public PrefixStringFilter(int prefixLength) {
        this.prefixLength = prefixLength;
    }

    @Override
    public String filter(String string) {
        return string.substring(0, prefixLength);
    }
    
    
}
