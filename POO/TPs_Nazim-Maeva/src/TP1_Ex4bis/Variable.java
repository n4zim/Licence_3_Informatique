package TP1_Ex4bis;

public class Variable implements Formula{
    final private String name;
    private double value;

    public Variable(String name, double value) {
        this.name = name;
        this.value = value;
    }
    
    public void set(double value){
        this.value = value;
    }

    @Override
    public double asValue() {
        return value;
    }

    @Override
    public String asString() {
        return name;
    }
     
}
