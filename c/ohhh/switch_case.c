/*
 * =====================================================================================
 *
 *       Filename:  switch_case.c
 *
 *    Description:  
 *
 *        Version:  
 *        Created:  一月 28, 2014
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Zenki (Zenki.J.Zha), zenki2001cn@163.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdio.h>

int yield_call(void) {
    static int i, state = 0;
    switch (state) {
    case 0: /* 函数起始位置 */
        for (i = 0; i < 10; i++) {
            state = 1;  /* 控制下一次case调用 */
            return i;
            case 1:;    /* 恢复case调用 */
        }
    }
}

int main(int argc, const char *argv[])
{
    printf("%d\n", yield_call());
    printf("%d\n", yield_call());
    printf("%d\n", yield_call());

    return 0;
}
