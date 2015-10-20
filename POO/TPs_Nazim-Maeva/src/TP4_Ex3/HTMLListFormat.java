package TP4_Ex3;

import TDTP4.ListFormat;

public class HTMLListFormat implements ListFormat {

    @Override
	public String getTop() { return "<ul>"; };
	
    @Override
	public String getBottom() { return "</ul>"; };
	
    @Override
	public String getPre() { return "<li>"; };
	
    @Override
	public String getPost() { return "</li>"; };
    
}