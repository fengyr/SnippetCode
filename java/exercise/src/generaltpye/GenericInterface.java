package generaltpye;

/*
 * =====================================================================================
 *
 *       Filename:  GenericInterface.java
 *
 *    Description:  
 *
 *        Version:  
 *        Created:  十一月 29, 2012
 *       Revision:  none
 *
 *         Author:  Zenki (Zenki.J.Zha), zenki2001cn@163.com
 *   Organization:  
 *
 * =====================================================================================
 */

/**
 * @Synopsis 定义一个泛型的接口，并有GenericTest类来实现
 */
public interface GenericInterface<T> {
    public T showTypeInfo();
}
