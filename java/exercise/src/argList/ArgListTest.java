/*
 * =====================================================================================
 *
 *       Filename:  ArgListTest.java
 *
 *    Description:  可变参数测试
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

package argList;

public class ArgListTest {
    public ArgListTest() {
    }

    /**
     * @Synopsis 可变参数以(T...)的形式实现，并且可以将该机制
     *           与泛型结合。使用泛型方法，需要在返回值前加上
     *           类型声明<T>
     *
     * @Param ts 泛型可变参数，以列表形式实现
     *
     * @Returns 
     */
    public <T> void showArgInfo(T... ts) {
        int i = 0;
        for (T e : ts) {
            System.out.println("T class name is " + e.getClass().getName() + " e = " + e.toString());
        }
    }
} 
