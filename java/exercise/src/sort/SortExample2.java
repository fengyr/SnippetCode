package sort;

import java.util.ArrayList;
import java.util.List;
import java.util.Collections;
import java.util.Comparator;

/*
 * 1.让List类型抽象化，这样就可以将任意类型的List作为参数。
 * 2.将比较的行为抽象化，可以用来扩展比较方法。
 */
public class SortExample2 {
    public void sort(List list, Comparator comp) {
        Collections.sort(list, comp);
    }    
}
