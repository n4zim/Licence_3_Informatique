package TDTP4_Ex4;

public class Forum extends ForumComposite{

    @Override
    protected String title() { return "Forum du turfu"; }
    
    public void print(){ print(0); }
    
    public static void main(String[] args) {
        Forum forum = new Forum();
        
        Topic topic1 = new Topic("La théorie des bananes");
        topic1.addElement(new Message("Chapitre 1 : La concentration bananique"));
        topic1.addElement(new Message("Chapitre 2 : Tout est banane"));
        forum.addElement(topic1);
        
        Topic topic2 = new Topic("YOLO");
        topic2.addElement(new Message("YOLO ?"));
        topic2.addElement(new Message("YOLO !"));
        forum.addElement(topic2);
        
        forum.print();
        
        for(String message : forum) System.out.println(message);
    }
}
