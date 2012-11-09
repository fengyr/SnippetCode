package sort;

import java.util.ArrayList;
import java.util.List;
import java.util.Collections;
import java.util.Comparator;

public class SortExample {

    private static final int SORT_BY_NAME = 0;
    private static final int SORT_BY_ID = 1;


    private class SortCompator implements Comparator {

        private int mSortType;

        public int compare(Object obj1, Object obj2) {
            switch ( mSortType ) {
            case SORT_BY_NAME :
                String strName1 = ((SortElement) obj1).getName();
                String strName2 = ((SortElement) obj2).getName();
                return strName1.compareTo(strName2);
            case SORT_BY_ID :
                int id1 = ((SortElement) obj1).getId();
                int id2 = ((SortElement) obj2).getId();
                if ( id1 > id2 ) {
                    return 1;
                }
                else {
                    return -1;
                }

            default :
            }

            return 0;
        }

        public SortCompator(int sortType) {
            mSortType = sortType;
        }
    }

    public void sortByName(List list) {
        Collections.sort(list, new SortCompator(SORT_BY_NAME));
    }

    public void sortById(List list) {
        Collections.sort(list, new SortCompator(SORT_BY_ID));
    }
}
