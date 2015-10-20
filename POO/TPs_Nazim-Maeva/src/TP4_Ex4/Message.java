package TDTP4_Ex4;

import java.util.Collections;
import java.util.Iterator;

public class Message implements ForumElement{
    final private String message;

    public Message(String message) { this.message = message; }

    @Override
    public void print(int nbSpaces) {
        StringBuilder sb = new StringBuilder(nbSpaces + message.length());
        for(; nbSpaces > 0; nbSpaces--) sb.append(' ');
        sb.append(message);
        System.out.println(sb);
    }

    @Override
    public Iterator<String> iterator() { return Collections.singleton(message).iterator(); }
    
}
