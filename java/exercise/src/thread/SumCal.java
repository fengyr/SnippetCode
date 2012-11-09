package thread;

public class SumCal {

    private int i = 0;

    public SumCal() {
    }

    synchronized public void add() {
        i ++;    
    }

    synchronized public void print() {
        //System.out.println("i = " + i + this.hashCode());
        System.out.println("i = " + i);
    }
}
