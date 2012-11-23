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

import java.lang.Exception;

public class TestJni {
    static {
        // System.out.println(System.getProperty("java.library.path"));
        System.load("/home/zenki/study/java/jni/jni_demo/bin/libJniTest.so");
    }

    public native static void hello();
    public native static int hello2(String[] str);
    public native static String hello3(int[] array, int i);
    public native static void callInner(InnerTestClass inner);

    static public class InnerTestClass {
        public InnerTestClass() {
        } 

        public void callInnerClass(String msg) throws Exception {
            System.out.println(msg);

            throw new Exception("Exception innerClass");
        }
    } 

    public static void main (String [] args) {
        TestJni test = new TestJni();

        // 直接JNI调用
        test.hello();

        // 传递String数组
        String[] testString = {"hello2 zenki!", "hello2 world!"};
        test.hello2(testString);

        // 传递int数组
        int array[] = {1, 2, 3, 4, 5};
        test.hello3(array, 5);

        // 传递内部类对象
        InnerTestClass inner = new InnerTestClass();
        test.callInner(inner);
    }
}
