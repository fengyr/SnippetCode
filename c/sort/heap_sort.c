/*
 * =====================================================================================
 *
 *       Filename:  heap_sort.c
 *
 *    Description:  堆排序
 *
 *        Version:  
 *        Created:  十月 29, 2012
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Zenki (Zenki.J.Zha), zenki2001cn@163.com
 *   Organization:  
 *
 * =====================================================================================
 */

void swap(int *array, int a, int b)
{
    /* int temp = array[a];
     * array[a] = array[b];
     * array[b] = temp; */

    int temp = *(array + a);
    *(array + a) = *(array + b);
    *(array + b) = temp;
}


/**
 * @Synopsis 1. 完全二叉树的特点是第i个节点的父节点、左孩子、右孩子分别表示为i/2、2i、2i+1。
 *           该方法的思想是首先将最大的数全归并到父节点，根为最大。再将根与叶子交换，用
 *           同样的方法再排序，直到交换出所有的根。
 *
 *           2. 对于除根以外的每个节点i，A[i/2] >= A[i]，即所有节点值不得超过父节点的值。因此，
 *           最大的值存放在根，称为大根堆。该算法则利用该特性，反复筛选出最大值，再排序。
 *
 *           3. 反之，对于节点i(根除外)，其父节点A[i/2] <= A[i]时，为小根堆。
 *
 * @Param array 数组指针
 * @Param c     第i个节点的下标
 * @Param len   数组的长度
 */
void sink(int *array, int c, int len)
{
    // c为子树的父节点，2*c为左孩子，2*c+1为右孩子。
    int lc = 2 * c;
    if (lc >= len) {
        return;
    }

    // 求得左孩子和右孩子的最大值
    int rc = lc + 1; 
    int mc = (rc > len) ? lc : (array[lc] > array[rc]) ? lc : rc;

    if (array[c] >= array[mc]) {
        return;
    }

    // 将左右孩子的最大值与父节点交换
    /* output("swaping sub", array, 10);    // 打印交换前子排序结果 */
    /* printf("swap a:%d, b:%d, lc:%d, rc:%d, mc:%d\n", c, mc, array[lc], array[rc], mc);   */
    swap(array, c, mc);
    output("sorting sub", array, 10);    // 打印交换后子排序结果

    // 递归调用最终使得所有子树的父节点被排序
    sink(array, mc, len);
}

void heap_sort(int *array, int len)
{
    int i;
    int id = len - 1;

    printf("----- heap build -----\n");
    // 将堆中所有子树的父节点排序，根为最大值
    for (i = len/2; i >= 0; i--) {
        sink(array, i, id);
    }

    printf("\n----- heap sort -----\n");
    for (i = 1; i < len; i++) {
        // 将根与末端叶子交换，以此来固定最大值的位置。
        swap(array, 0, id-i+1);
        // 按照heap build方法继续对根排序，来求得最大值。
        sink(array, 0, id-i);
    }
}
