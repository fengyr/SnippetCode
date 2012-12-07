/*
 * =====================================================================================
 *
 *       Filename:  BarrierTest.java
 *
 *    Description:  
 *
 *        Version:  
 *        Created:  十一月 30, 2012
 *       Revision:  none
 *
 *         Author:  Zenki (Zenki.J.Zha), zenki2001cn@163.com
 *   Organization:  
 *
 * =====================================================================================
 */
package concurrentTest;

import java.util.concurrent.BrokenBarrierException;
import java.util.concurrent.CyclicBarrier;

public class BarrierTest extends Thread {
    // CyclicBarrier对象确保在所有线程执行完成后，运行构造方法中的线程对象，
    // 因此需要设置成static类型，使两个线程同时可访问。
    private static CyclicBarrier barrier = new CyclicBarrier(2, new Thread() {
        public void run() {
            try {
                Thread.sleep(1000);
            } catch (InterruptedException e) {
            }
            System.out.println("finish");
        };
    });

    private final int sleepMilSecs;

    public BarrierTest(int sleepMilSecs) {
        this.sleepMilSecs = sleepMilSecs;
    }

    public void reset() {
        barrier.reset();
    }

    @Override
    public void run() {
        try {
            Thread.sleep(sleepMilSecs);
            System.out.println(sleepMilSecs + " secs slept");
            barrier.await();
        } catch (InterruptedException e) {
        } catch (BrokenBarrierException e) {
        }
    }
}
