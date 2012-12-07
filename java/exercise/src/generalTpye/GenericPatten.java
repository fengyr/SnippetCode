/*
 * =====================================================================================
 *
 *       Filename:  GenericPatten.java
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

package generalType;

import java.util.List;
import java.util.ArrayList;

public class GenericPatten {
    class Fruit {
    }    

    class Apple extends Fruit {
    }

    class Jonathan extends Apple {
    }

    List<? super Fruit>     mListSuper;
    List<? extends Fruit>   mListExtends;
    List<?>                 mListUnbound;

    public GenericPatten() {
        mListSuper = new ArrayList<Fruit>();
        mListSuper.add(new Apple());
        mListSuper.add(new Jonathan());
        mListSuper.add(new Fruit());

        mListUnbound = mListSuper;
        // mListExtends = mListSuper;   // Error
    }
    
    public void showFruit() {
        int len = mListSuper.size();
        int i;
        for (i = 0; i < len; i++) {
            System.out.println("List<? super Fruit> item class: " + mListSuper.get(i).getClass().getName());
        }

        for (i = 0; i < len; i++) {
            System.out.println("List<?> item class: " + mListUnbound.get(i).getClass().getName());
        }

        /* Error
         * for (i = 0; i < len; i++) {
         *     System.out.println("List<? extends Fruit> item class: " + mListExtends.get(i).getClass().getName());
         * } */

        /* for (Object item : mListSuper) {
         *     System.out.println("item class: " + item.getClass().getName());
         * }  */
    }
} 
