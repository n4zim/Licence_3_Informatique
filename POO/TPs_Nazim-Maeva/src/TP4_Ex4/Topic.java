package TDTP4_Ex4;

public class Topic extends ForumComposite{
    final private String name;

    public Topic(String name) { this.name = name; }

    @Override
    protected String title() { return "Topic : " + name; }
}
