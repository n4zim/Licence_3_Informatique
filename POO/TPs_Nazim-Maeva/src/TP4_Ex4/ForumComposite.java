package TDTP4_Ex4;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

abstract public class ForumComposite implements ForumElement{
    final protected List<ForumElement> elements = new ArrayList<>();

    @Override
    public void print(int nbSpaces) {
        StringBuilder sb = new StringBuilder();
        
        for(int i = 0; i < nbSpaces; i++) sb.append(' ');
        
        sb.append(title());
        System.out.println(sb);
        
        for(ForumElement elem : elements) elem.print(nbSpaces+2);
    }
    
    public void addElement(ForumElement element) { elements.add(element); }

    public List<ForumElement> getElements() { return elements; }
    
    abstract protected String title();
    
    private class MyIterator implements Iterator<String> {
        int curIndex = 0;
        Iterator<String> curIterator = null;

        public MyIterator() { setNext(); }
        
        private void setNext(){
            if(curIterator == null) curIterator = elements.get(curIndex).iterator();

            while(curIndex < elements.size() && !curIterator.hasNext()){
                if(++curIndex < elements.size()) curIterator = elements.get(curIndex).iterator();
            }
        }

        @Override
        public boolean hasNext() { return curIndex < elements.size(); }

        @Override
        public String next() {
            String value = curIterator.next();
            setNext();
            return value;
        }
    }

    @Override
    public Iterator<String> iterator() { return new MyIterator(); }
    
}
