/*
 * =====================================================================================
 *
 *       Filename:  binary_search.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2010年02月23日 10时48分40秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Zenki
 *        Company:  
 *
 * =====================================================================================
 */
#include <stdio.h>

int binarySearch(int *array, int start, int len, int key) 
{
    int high = start + len, low = start - 1, guess;

    while (high - low > 1) {
        guess = (high + low) / 2;

        if (array[guess] < key)
            low = guess;
        else
            high = guess;
    }

    if (high == start + len)
        return ~(start + len);
    else if (array[high] == key)
        return high;
    else
        return ~high;
}

int bin_search(int x, int *array, int s, int e)
{
    int i;
    while ( s <= e ) {
        printf("s: %d, e: %d\n", s, e);
        i = (s+e)/2;
        if ( array[i] == x) {
            return i;
        }

        if ( array[i] < x ) {
            s = i + 1;
        }
        else {
            e = i - 1;
        }
    }

    return -1;
}

int main(int argc, char *argv[])
{
    int res = -1;
    int array[] = {1, 4, 7, 9, 55, 100, 775, 999};

    res = bin_search(7756, array, 0, sizeof(array)/sizeof(int)-1);
    printf("bin_search res: %d num: %d\n", res, sizeof(array)/sizeof(int));

    res = binarySearch(array, 1, sizeof(array)/sizeof(int), 7788);
    printf("binarySearch res: %d num: %d\n", res, sizeof(array)/sizeof(int));

    return 0;
}
