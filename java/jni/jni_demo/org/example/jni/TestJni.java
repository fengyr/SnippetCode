package org.example.jni;

public class TestJni {
    static {
        // System.out.println(System.getProperty("java.library.path"));
        System.load("/home/zenki/study/java/jni/jni_demo/bin/libJniTest.so");
    }

    public native static void hello();
    public native static int hello2(String[] str);
    public native static boolean hello3(String[] str, int[] array);
    public native static String hello4(String[] str, int[] array, int i);

    public static void main (String [] args) {
        TestJni test = new TestJni();

        test.hello();
        test.hello2(null);
        test.hello3(null, null);
        test.hello4(null, null, 0);
    }
}
