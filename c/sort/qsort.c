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

static int cmpstringp(const void *p1, const void *p2)
{
    /* The actual arguments to this function are "pointers to
       pointers to char", but strcmp(3) arguments are "pointers
       to char", hence the following cast plus dereference */

    return strcmp(* (char * const *) p1, * (char * const *) p2);
}

static int cmpintgp(const void *p1, const void *p2)
{
    /* The actual arguments to this function are "pointers to
       pointers to char", but strcmp(3) arguments are "pointers
       to char", hence the following cast plus dereference */

    /*printf("p1: %d, p2: %d\n", atoi(*(char* const*)p1) , atoi(*(char* const*)p2));*/

    return atoi(*(char* const*)p1) - atoi(*(char* const*)p2);
}

int sort_string(int argc, char *argv[])
{
    int j;

    if (argc < 2) {
        fprintf(stderr, "Usage: %s <string>...\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    qsort(&argv[1], argc - 1, sizeof(argv[1]), (int (*)(const void *, const void *))cmpstringp);

    for (j = 1; j < argc; j++)
        puts(argv[j]);

    return 0;
}

int sort_inter(int argc, char *argv[])
{
    int j;

    if (argc < 2) {
        fprintf(stderr, "Usage: %s <int>...\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    qsort(&argv[1], argc - 1, sizeof(argv[1]), (int (*)(const void *, const void *))cmpintgp);

    for (j = 1; j < argc; j++)
        puts(argv[j]);

    return 0;

}

int main(int argc, char *argv[])
{
    /* sort_inter(argc, argv); */

    sort_string(argc, argv);

    return 0;
}
