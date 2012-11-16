/*
 * =====================================================================================
 *
 *       Filename:  TestJni.java
 *
 *    Description:  JNI示例
 *
 *        Version:  0.1
 *        Created:  十一月 16, 2012
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Zenki (Zenki.J.Zha), zenki2001cn@163.com
 *   Organization:  
 *
 * =====================================================================================
 */

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

        String[] testString = {"hello zenki!", "hello world!"};
        test.hello2(testString);

        test.hello3(null, null);
        test.hello4(null, null, 0);
    }
}
