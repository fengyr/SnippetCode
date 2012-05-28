/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Zenki
 *        Company:  
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void pop_sort(int *array, int size)
{
    int tmp;
    int i, j;

    for (i = 0; i < size; i++) {
        for (j = i+1; j < size; j++) {
            if ( array[j] < array[i] ) {
                tmp = array[j];
                array[j] = array[i];
                array[i] = tmp;
            }
        }
    }
}

int main(int argc, char *argv[])
{
    int i;
    int array[] = {10, 8, 4, 9, 2, 4, 5, 3, 19, 11};

    pop_sort(array, 10);

    for (i = 0; i < 10; i++) {
        printf("%d,", array[i]);
    }
    printf("\n");
	
    return 0;
}


