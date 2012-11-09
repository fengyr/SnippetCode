package thread;

public class ThreadDemo extends Thread {

    private boolean quit = false;
    private SumCal sumCal = null;
    private SumCal sumCal2 = null;
    private String threadName = null;
    private byte[] lock = new byte[0];

    public ThreadDemo(String name, SumCal sum, SumCal sum2) {
        System.out.println("ThreadDemo init");

        sumCal = sum;
        sumCal2 = sum2;
        threadName = name;
    }

    private void print() {
        sumCal.add();
        sumCal2.add();
        synchronized (sumCal) {
            System.out.println(threadName + " ");

           try {
                Thread.sleep(1000);
            }
            catch(Exception e) {
                // todo
           }
            sumCal.print();
            sumCal2.print();
        }
    }

    public void run() {
        while (!quit) {
            print();

            try {
                Thread.sleep(1000);
            }
            catch(Exception e) {
                // todo
            }
        }
    }
}
