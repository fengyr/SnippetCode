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

    List<? super Fruit> mList;

    public GenericPatten() {
        mList = new ArrayList<Fruit>();
        mList.add(new Apple());
        mList.add(new Jonathan());
        mList.add(new Fruit());
    }
    
    public void showFruit() {
        int len = mList.size();
        int i;
        for (i = 0; i < len; i++) {
            System.out.println("item class: " + mList.get(i).getClass().getName());
        }

        /* for (Object item : mList) {
         *     System.out.println("item class: " + item.getClass().getName());
         * }  */
    }
} 
