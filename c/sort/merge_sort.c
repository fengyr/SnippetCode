/*
 * =====================================================================================
 *
 *       Filename:  merge_sort.c
 *
 *    Description:  测试merge排序
 *
 *        Version:  
 *        Created:  十一月 02, 2012
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

/**
 * @Synopsis 该算法的思想是将待排序的数组全部对半分，最终只有一个元素为止。
 *           然后通过递归，将所有划分的数组相互比较排序，比较的数组是按照
 *           划分的反向顺序，最终将所有划分的数组都合并起来。
 *
 * @Param array 数组指针
 * @Param len   数组长度
 */
void merge_sort(int *array, int len)
{
    /* 递归结束的标识 */
    if (len == 1) {
        return;
    }

    /* 比较前半段元素 */
    merge_sort(array, len/2);
    /* 比较后半段元素 */
    merge_sort(array + len/2, (len+1) /2);

    /* 以下代码是在两次递归返回时调用的，所以这时的数组已经开始将划分的数组
     * 组合起来了。所以第一次比较时，数组应该有两个元素了。
     * 比较前需要对当前一半的数组元素做一份拷贝 */
    int copy_array[len/2];
    memcpy(copy_array, array, len/2*sizeof(int));
    output("\ncopy_array", copy_array, len/2);      // 打印子排序结果

    int i = 0;          // i用来遍历前一半拷贝的数组
    int j = len/2;      // j用来遍历另一半数组
    int k = 0;          // k用来遍历本次排序的数组，保存结果

    /* 遍历两半数组，直到它们超出数组长度。
     * 这时，从比较的两半数组中，将小的数和前面的数做交换。 */
    while ((i < len/2) && (j < len)) {
        printf("array[j]:%d, copy_array[i]:%d\n", array[j], copy_array[i]);
        array[k++] = (array[j] < copy_array[i]) ? array[j++] : copy_array[i++];
        /* output("sorting 1", array, len);         // 打印子排序结果 */
    }

    /* 跳出该循环可能会产生数组元素拷贝不全的情况，因此还需要补充两个拷贝过程： */

    /* 上一个循环结束后，如果后一半的数组比较前一半数组小，那么后一半的数组
     * 会先遍历结束，那么就需要将前一半未遍历的数组拷贝过来，其元素自然要比遍历过的大。 */
    while (i < len/2) {
        printf("array[k]:%d, copy_array[i]:%d\n", array[k], copy_array[i]);
        array[k++] = copy_array[i++];
        output("copy_left >", array, len);         // 打印子排序结果
    }

    /* 上一个循环结束后，如果前一半的数组比较后一半数组小，那么前一半的数组
     * 会先遍历结束，那么就需要将后一半未遍历的数组拷贝过来，其元素自然要比遍历过的大。 */
    while (j < len) {
        printf("array[k]:%d, array[j]:%d\n", array[k], array[j]);
        array[k++] = array[j++];
        output("copy_right <", array, len);         // 打印子排序结果
    }
} 
