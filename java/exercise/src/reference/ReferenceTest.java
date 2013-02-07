package reference;

import java.lang.ref.SoftReference;
import java.lang.ref.ReferenceQueue;

public class ReferenceTest {
    private DemoClass demoClass;
    
    public ReferenceTest() {
        demoClass = new DemoClass();
    }

    /**
     * @Synopsis 将强引用类型对象置空，并调用System.gc()后，该对象指向空.
     *
     * @Returns 
     */
    public void testStrongReference() {
        printObject(demoClass);
        demoClass = null;

        System.out.println("call System.gc()");
        System.gc();
        System.gc();
        printObject(demoClass);
    }

    /**
     * @Synopsis 测试软引用对象，在释放强引用对象后，仍然可以得到该对象的实例
     *
     * @Returns 
     */
    public void testSoftReference() {
        SoftReference softRef = new SoftReference(demoClass);

        testStrongReference();

        DemoClass softDemoClass = (DemoClass) softRef.get();
        System.out.println("softDemoClass = " + softDemoClass);
    }

    /**
     * @Synopsis 测试引用队列
     *
     * @Returns 
     */
    public void testReferenceQueue() {
        ReferenceQueue<DemoClass> queue = new ReferenceQueue<DemoClass>();
        SoftReference<DemoClass> softRef = new SoftReference(demoClass, queue);

        testStrongReference();

        DemoClass softDemoClass = (DemoClass) softRef.get();
        System.out.println("softRef.get softDemoClass = " + softDemoClass);

        softRef = (SoftReference<DemoClass>) queue.poll();
        // 该处有可能会为空
        if (softRef != null) {
            softDemoClass = (DemoClass) softRef.get();
            System.out.println("softDemoClass = " + softDemoClass);
        }
    }

    private void printObject(Object object) {
        System.out.println("demoClass = " + object);
    }

    /**
     * @Synopsis 用于测试的内部类
     */
    class DemoClass {
        DemoClass() {
        }
    }
}
