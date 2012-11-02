/*
 * =====================================================================================
 *
 *       Filename:  insert_sort.c
 *
 *    Description:  插入排序
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

/* 另一个版本，序号从0开始 */
/* void insert_sort(int *array, int len)
 * {
 *     int i = 0;
 *     // 从第一个元素开始，按照以下的规则遍历数组，进行元素比较
 *     for(i = 0; i < len - 1; ++i) {
 *         int j = i + 1;                          
 *         int key = array[j];                     // 把第“i+1”个元素作为比较对象
 *         while(j > 0 && key < array[j-1]) {      // 发现前面的元素大于比较，就将前面的元素往后移动
 *             array[j] = array[j-1];
 *             --j;                                // 递减序号，并把比较范围缩短，继续比较
 *             [> output("sorting sub", array, len);  // 打印子排序结果 <]
 *         }
 *         array[j] = key;                         // 最终空出适当的位置，放置比较对象
 *         output("sorting ", array, len);         // 打印子排序结果
 *     }
 * } */

void insert_sort(int *array, int len)
{
    int i = 0;
    // 从第二个元素开始，按照以下的规则遍历数组，进行元素比较
    for(i = 1; i < len; ++i) {
        int j = i - 1;                          // 记录前一个元素的标号
        int key = array[i];                     // 把第“i”个元素作为比较对象
        while(j >= 0 && array[j] > key) {       // 发现前面的元素大于比较对象，就将前面的元素往后移动
            array[j+1] = array[j];
            --j;                                // 递减序号，并把比较范围缩短，继续比较
            /* output("sorting sub", array, len);  // 打印子排序结果 */
        }
        array[j+1] = key;                       // 最终空出适当的位置，放置比较对象
        output("sorting ", array, len);         // 打印子排序结果
    }
}
