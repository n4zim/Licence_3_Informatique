package TDTP4;

public class LaTexListFormat implements ListFormat {

    @Override
	public String getTop() { return "\\begin{itemize}"; };
	
    @Override
	public String getBottom() { return "\\end{itemize}"; };
	
    @Override
	public String getPre() { return "\\item "; };
	
    @Override
	public String getPost() { return ""; };
	
}
