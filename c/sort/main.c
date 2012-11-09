/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  测试各种排序方法
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

void output(char *text, int *list, int size)
{
	int i;
	printf("%s: ", text);
	for ( i=0; i<size; i++ ) {
	    printf("%d,", list[i]);
	}
	printf("\n");
}

int main(int argc, char *argv[])
{
	int nums[] = {4, 55, 7, 1, 11, 10, 19, 44, 22, 19};

	output("before sort", nums, sizeof(nums)/sizeof(int));

/*     printf("quick sort start ==============>\n");
 *     quick_sort(nums, 0, sizeof(nums)/sizeof(int)); 
 * 
 *     printf("pop sort start ==============>\n");
 *     pop_sort(nums, sizeof(nums)/sizeof(int));   
 * 
 *     printf("insert sort start ==============>\n");
 *     insert_sort(nums, sizeof(nums)/sizeof(int));   
 * 
 *     printf("heap sort start ==============>\n");
 *     heap_sort(nums, sizeof(nums)/sizeof(int));       */

    printf("merge sort start ==============>\n");
    merge_sort(nums, sizeof(nums)/sizeof(int));    

    /* printf("shell sort start ==============>\n");
     * shell_sort(nums, sizeof(nums)/sizeof(int)); */

    output("\nfinish =>", nums, sizeof(nums)/sizeof(int));

	return 0;
}
