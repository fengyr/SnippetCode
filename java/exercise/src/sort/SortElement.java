package sort;

import java.util.Comparator;

/*
 * 由对象类实现比较方法，并可以扩展更多的比较方法
 *
 */
public class SortElement {

    String mName;
    int mId;

    static public SortNameCompator sortName = new SortNameCompator();
    static public SortIdCompator sortId = new SortIdCompator();

    public SortElement(String name, int id) {
        mName = name;
        mId = id;
    }

    public String getName() {
        return mName;
    }

    public int getId() {
        return mId;
    }

    static private class SortNameCompator implements Comparator {

        public int compare(Object obj1, Object obj2) {
            String strName1 = ((SortElement) obj1).getName();
            String strName2 = ((SortElement) obj2).getName();
            return strName1.compareTo(strName2);
        }
    }

    static private class SortIdCompator implements Comparator {

        public int compare(Object obj1, Object obj2) {
            int id1 = ((SortElement) obj1).getId();
            int id2 = ((SortElement) obj2).getId();
            if ( id1 > id2 ) {
                return 1;
            }
            else if (id1 < id2) {
                return -1;
            }
            else {
                return 0;
            }
        }
    }
}
