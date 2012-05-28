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

void output(char *text, int *list, int size)
{
	int i;
	printf("%s:", text);
	for ( i=0; i<size; i++ ) {
		printf("%d,", list[i]);
	}
	printf("\n");
}

void quick_sort(int *list, int low, int high)
{
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
		
        output("sorting sub", list, 10);
        /*必须再寻找大于key的数，将它放到j的位置，为key留下存放的位置*/
		while ( i < j && list[i] <= key )
			++i;
		list[j] = list[i];
        output("sorting sub", list, 10);
	}
    /*将key放置到正确位置*/
	list[i] = key;

	output("sorting", list, 10);

    /*为各个段递归排序*/
	quick_sort(list, low, i-1);
	quick_sort(list, i+1, high);
}

int main(int argc, char *argv[])
{
	int nums[] = {4, 5, 7, 1, 11, 10, 19, 22, 44, 19};

	output("before sort", nums, sizeof(nums)/sizeof(int));
	quick_sort(nums, 0, sizeof(nums)/sizeof(int)-1);
    output("after sort", nums, sizeof(nums)/sizeof(int));

	return 0;
}
