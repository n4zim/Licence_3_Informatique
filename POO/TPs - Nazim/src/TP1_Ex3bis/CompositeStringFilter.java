package TP1_Ex3bis;

public class CompositeStringFilter implements StringFilter{
    final StringFilter[] filters;

    public CompositeStringFilter(StringFilter... filters) {
        this.filters = filters;
    }

    @Override
    public String filter(String string) {
        for(StringFilter filter : filters){
            string = filter.filter(string);
        }
        
        return string;
    }
    
    
}
