package typetest;

public class ListTestTest extends ListTest {
	
	public ListTestTest() {
		System.out.println("ListTestTest init");
	}

    public static void main (String [] args) {
        ListTest lt = new ListTest();
        ListTestTest ltt = new ListTestTest();

        if ( lt instanceof ListTest ) {
            System.err.println("lt is ListTest");
        }
        if ( ltt instanceof ListTestTest ) {
            System.err.println("ltt is ListTestTest");
        }

        if ( ltt instanceof ListTest ) {
            System.err.println("ltt is ListTest");
        }

        if ( ListTest.class.isInstance(ltt) ) {
            System.err.println("ltt is ListTest");
        }

        if ( ((Object)ltt).getClass() == ListTest.class ) {
            System.err.println("ltt is not ListTest");
        }
    }
	
}
