/*
 * =====================================================================================
 *
 *       Filename:  quick_sort.c
 *
 *    Description:  快速排序
 *
 *        Version:  1.0
 *        Created:  2009年07月30日 10时39分33秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Zenki
 *        Company:  
 *
 * =====================================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void quick_sort(int *list, int low, int high)
{
    // 递归调用的终止条件
	if (low >= high)
		return;

    /*首先将轴元素保存一个拷贝*/
	int key = list[low];
	int i = low;
	int j = high;

	while (i < j) {
        /*寻找小于key的数，将它放到i的位置*/
		while (i < j && list[j] >= key )
			--j;
		list[i] = list[j];
        /* output("sorting sub", list, 10);    // 打印子排序结果 */
		
        /*必须再寻找大于key的数，将它放到j的位置，为key留下存放的位置*/
		while ( i < j && list[i] <= key )
			++i;
		list[j] = list[i];
        /* output("sorting sub", list, 10);    // 打印子排序结果 */
	}
    /*将key放置到正确位置*/
	list[i] = key;

    output("sorting", list, 10);            // 打印一次排序的结果

    /*对除了A[i]以外的数组区间A[low]~A[i-1]、A[i+1]~A[high]做递归排序*/
	quick_sort(list, low, i-1);
	quick_sort(list, i+1, high);
}
