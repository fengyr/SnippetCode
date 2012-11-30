/*
 * =====================================================================================
 *
 *       Filename:  ExcutorEx.java
 *
 *    Description:  concurrent库示例
 *
 *        Version:  
 *        Created:  十一月 27, 2012
 *       Revision:  none
 *
 *         Author:  Zenki (Zenki.J.Zha), zenki2001cn@163.com
 *   Organization:  
 *
 * =====================================================================================
 */
package concurrentTest;

import java.util.concurrent.Executor;
import java.util.concurrent.Executors;
import java.util.concurrent.ThreadPoolExecutor;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.CountDownLatch;
import java.lang.Runnable;
import java.lang.Thread;

public class ExcutorEx {
    // ExecutorService是一个线程池管理者
    private ExecutorService mPool;
    private int mThreadNum;
    // CountDownLatch用来同步每个线程的执行
    private CountDownLatch mLatch;

    public ExcutorEx(int threadNum) {
        mThreadNum = threadNum;
        mPool = Executors.newFixedThreadPool(threadNum);
        mLatch = new CountDownLatch(1);
    }

    public void prepareThreads() {
        for (int i = 0; i < mThreadNum; i++) {
            ExcutorRunable r = new ExcutorRunable(i, mLatch);
            mPool.execute(r);
            System.out.println("Thread prepare id = " + i);
        }
    }

    public void start() {
        // 减少引用计数，使所有线程启动
        mLatch.countDown();
    }

    public void shutDown() {
        System.out.println("ExcutorEx shutdown!");

        mPool.shutdown();
        mPool.shutdownNow();
        try {
            // Wait a while for existing tasks to terminate
            if (!mPool.awaitTermination(60, TimeUnit.SECONDS)) {
                mPool.shutdownNow(); // Cancel currently executing tasks
                // Wait a while for tasks to respond to being cancelled
                if (!mPool.awaitTermination(60, TimeUnit.SECONDS))
                    System.err.println("Pool did not terminate");
            }
        } catch (InterruptedException ie) {
            // (Re-)Cancel if current thread also interrupted
            mPool.shutdownNow();
            // Preserve interrupt status
            Thread.currentThread().interrupt();
        }

    }

    protected class ExcutorRunable implements Runnable {
        private int mId;
        private CountDownLatch mLatch;

        protected ExcutorRunable(int id, CountDownLatch latch) {
            mId = id;
            mLatch = latch;
        }

        public void run() {
            int i = 0;

            try {
                // 当mLatch的引用计数为1时，await()使所有的线程阻塞
                mLatch.await();
            } catch(Exception e) {
            }
            while ((i++) < 5) {
                System.out.println("Thread running ID = " + mId);

                try {
                    Thread.sleep(1000); 
                } catch(Exception e) {
                }
            }
        }
    }
}
