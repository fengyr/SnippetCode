/**
 *       File       a.c
 *
 *       Revision:  none
 *       Compiler:  gcc
 *
 *       Version    1.00.00
 *       Author:    Zenki (zhajun), zenki2001cn@163.com
 *       Date:      2010年06月18日 10时07分53秒
 *
 * =====================================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

static struct option long_options[] = {
    {"add", 1, 0, 'a'},
    {"append", 0, 0, 0},
    {"delete", 1, 0, 'd'},
    {"verbose", 0, 0, 0},
    {"create", 1, 0, 'c'},
    {"file", 1, 0, 0},
    {0, 0, 0, 0}
};


int main(int argc, const char *argv[])
{
    int c;
    int options_index = 0;

    while (1) {
        c = getopt_long(argc, (char * const*)argv, "a:b:c:d:", long_options, &options_index);

        if (-1 == c) {
            fprintf(stderr, "get opt err\n");
            break;
        }

        switch (c) {
        case 'a':
        case 0:
            fprintf(stderr, "%s\n", optarg);
            break;
        case 'b':
            fprintf(stderr, "%s\n", optarg);
            break;
        case 4:
        case 'c':
            fprintf(stderr, "%s\n", optarg);
            break;
        case 2:
        case 'd':
            fprintf(stderr, "%s\n", optarg);
            break;
        case 1:
            fprintf(stderr, "%s: %s\n", long_options[options_index].name, optarg);
            break;
        case 3:
            fprintf(stderr, "%s: %s\n", long_options[options_index].name, optarg);
            break;
        default:
            fprintf(stderr, "argv error: %d\n", c);
            break;
        }

        /*fprintf(stderr, "ret: %d, index: %d\n", c, options_index);*/
    }

    return 0;
}


