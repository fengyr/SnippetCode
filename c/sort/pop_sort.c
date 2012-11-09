/*
 * =====================================================================================
 *
 *       Filename:  pop_sort.c
 *
 *    Description:  冒泡排序
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


#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

/**
 * @Synopsis 
 *
 * 时间复杂度：O(n^2)
 *
 * @Param array
 * @Param size
 */
void pop_sort(int *array, int size)
{
    int tmp;
    int i, j;

    // 对数组的每一个元素进行比较，0<i<size；
    // 一次比较的结果是将最大值放置到数组的末尾，
    // 需要比较size次将所有元素排序。
    for (i = 0; i < size; i++) {
        // 选择除了A[i]以外的元素进行比较，将小于A[i]的元素与之交换
        for (j = i+1; j < size; j++) {
            if ( array[j] < array[i] ) {
                tmp = array[j];
                array[j] = array[i];
                array[i] = tmp;
            }
        }
    }
}
