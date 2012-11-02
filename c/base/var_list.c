/*
 * =====================================================================================
 *
 *       Filename:  var_list.c
 *
 *    Description:  测试可变参数的使用
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

#include <stdarg.h>
#include <stdio.h>

int var_list_test(const char *fmt, ...) 
{
    va_list ap;
    int d;
    char c, *s;
    int mark = 0;

    printf("Origin string fmt = %s\n", fmt);

    va_start(ap, fmt);
    while (*fmt) {
        switch (*fmt) {
            case '%':
                mark = 1;
                break;
            case 's':	      /* string */
                if (mark) {
                    s = (char*) va_arg(ap, char *);
                    printf("%s", s);
                    mark = 0;
                    break;
                }
            case 'd':	      /* int */
                if (mark) {
                    d = va_arg(ap, int);
                    printf("%d", d);
                    mark = 0;
                    break;
                }
            case 'c':	      /* char */
                if (mark) {
                    c = (char) va_arg(ap, int);
                    printf("%c", c);
                    mark = 0;
                    break;
                }
            default:
                printf("%c", *fmt);
                break;
        }

        fmt++;
    }

    printf("\n");
    va_end(ap);

    return 0;
}
