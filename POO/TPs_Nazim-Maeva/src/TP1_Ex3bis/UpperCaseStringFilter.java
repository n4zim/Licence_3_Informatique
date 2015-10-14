package TP1_Ex3bis;

public class UpperCaseStringFilter implements StringFilter{

    @Override
    public String filter(String string) {
        return string.toUpperCase();
    }
    
}
