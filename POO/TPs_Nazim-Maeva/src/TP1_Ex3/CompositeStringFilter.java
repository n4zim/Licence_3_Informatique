package TP1_Ex3;

class CompositeStringFilter implements StringFilter {
	private final StringFilter[] filters;

	public CompositeStringFilter(StringFilter[] filters) {
		this.filters = filters;
	}

	public String filter(String string) {
		for(StringFilter filter: filters) {
			string = filter.filter(string);
		}

		return string;
	}
	
	public static void main(String[] args) {
		StringFilter[] filters = {};
		CompositeStringFilter csf = new CompositeStringFilter(filters);
		csf.filter("Un");
		csf.filter("Deux");
		System.out.println(csf.filter("Trois"));
	}

}
