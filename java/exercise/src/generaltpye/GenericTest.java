package generaltpye;

/*
 * =====================================================================================
 *
 *       Filename:  GenericTest.java
 *
 *    Description:  泛型测试
 *
 *        Version:  
 *        Created:  十一月 28, 2012
 *       Revision:  none
 *
 *         Author:  Zenki (Zenki.J.Zha), zenki2001cn@163.com
 *   Organization:  
 *
 * =====================================================================================
 */

import java.util.Arrays;
import java.io.File;

/**
 * @Synopsis 实现一个泛型的接口，需要指定一个具体的类型作为参数
 */
public class GenericTest<T> implements GenericInterface<String> {
    private T mObj; 

    public GenericTest(T arg) {
        mObj = arg;
    }

    public void showObjClass() {
        String name = mObj.getClass().getName();
        System.out.println("GenericTest mObj class name: " + name);

        if (mObj instanceof File) {
            System.out.println("mObj is instanceof File!");
            // 以下代码无法执行，因为Java在编译时期，无法确定T的具体类型
            /* if (mObj.exists()) {
             *     System.out.println("file is exists");
             * } else {
             *     System.out.println("file is not exists");
             * } */
        } 
    }

    /**
     * @Synopsis Java的泛型无法在运行时获取到参数类型的信息
     *
     * @Returns 
     */
    public String showTypeInfo() {
        // 尝试获取泛型变量mObj的类型，结果返回为[]
        Class clsObj = mObj.getClass();
        String typeInfoObj = Arrays.toString(clsObj.getTypeParameters());

        // 通过获取this类的参数类型，结果返回为[T]
        Class cls = this.getClass();
        String typeInfo = Arrays.toString(cls.getTypeParameters());

        System.out.println("T mObj typeInfo = " + typeInfoObj + " GenericTest<T> typeInfo = " + typeInfo);

        return typeInfo;
    }
} 
