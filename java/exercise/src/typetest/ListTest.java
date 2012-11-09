package typetest;

import java.util.ArrayList;

public class ListTest {
	
    public int mark = 0;
	public ArrayList list = new ArrayList();
	
	ListTest() {	
		System.out.println("ListTest init");
	}
	
	public void PrintList() {
		list.add("hello");
		list.add("world");
		
		System.out.println("List test, makr: " + mark);
		//System.out.println(list.subList(1, 2));
	}
	
}
