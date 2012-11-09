/*
 * =====================================================================================
 *
 *       Filename:  shell_sort.c
 *
 *    Description:  Shell排序
 *
 *        Version:  
 *        Created:  十一月 05, 2012
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Zenki (Zenki.J.Zha), zenki2001cn@163.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>

/**
 * @Synopsis 该方法实质上采用一种策略，调用插入排序的算法进行排序。
 *
 * 时间复杂度：O(n^(3/2))
 *
 * @Param array
 * @Param len
 */
void shell_sort(int *array, int len)
{
    int h, i, j, temp;

    /* h的取值决定了排序时的分组和调用插入排序时的比较步长。
     * 直到当增量为1时，即对同一个数组进行排序。 */
    for(h = len; h > 1; ) {
        h = (h < 3) ? 1 : (h * 3 - 1) / 11;
        printf("\nh = %d\n", h);
/*         for (i = h; i < len; i++) {
 *             temp = array[i];
 *             for (j = i - h; j >= 0 && array[j] > temp; j -= h) {
 *                 array[j+h] = array[j];
 *             }
 * 
 *             array[j+h] = temp;
 *         } */

        /* 以上注释部分可替换成插入排序算法，步长设置为h */
        insert_sort_by_step(&array[h-1], len-h+1, h);
        output("final", array, len);            // 打印一次排序的结果
    }
}
