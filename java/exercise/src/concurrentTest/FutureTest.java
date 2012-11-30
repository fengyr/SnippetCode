/*
 * =====================================================================================
 *
 *       Filename:  FutureTest.java
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

import java.util.concurrent.Callable;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;
 
public class FutureTest {
    private ExecutorService executor = Executors.newSingleThreadExecutor();
    private Callable<Object> task = new Callable<Object>() {
        public Object call() throws Exception {
            Thread.sleep(1000);
            Object result = "finished";
            return result;
        }
    };
 
    public void submitTask() {
        Future<Object> future = executor.submit(task);
        System.out.println("task submitted");

        try {
            System.out.println(future.get());
        } catch (InterruptedException e) {
            System.out.println("e = " + e.toString());
        } catch (ExecutionException e) {
            System.out.println("e = " + e.toString());
        } 

        // 此处如果不调用shutdown，则线程不会销毁。
        executor.shutdown();
    }
}
