package TP6_Remaster;

public class Constant implements Formula{
    final private double value;

    public Constant(double value) {
        this.value = value;
    }
    

    @Override
    public double asValue() {
        return value;
    }

    @Override
    public String asString() {
        return value + "";
    }
    
}
