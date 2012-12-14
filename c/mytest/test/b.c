/* ======================================================================================
 * File         : b
 * Author       : Zenki
 * Last Change  : 06/02/2010 | 11:39:41 上午 | 星期三,六月
 * Description  : 
 * ======================================================================================
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#undef PDEBUG
#ifdef USE_DEBUG
#ifdef __KERNEL__
#define PDEBUG(fmt, args...) printk( KERN_DEBUG "kernel: " fmt, ## args)
#else
#define PDEBUG(fmt, args...) fprintf(stderr, fmt, ## args)
#endif
#else
#define PDEBUG(fmt, args...)
#endif

typedef char X[20];

/**
* @brief  this is a func
* @param  void
* @return void
*/
static void function_name()
{
    char str[] = "abcdefg";
    char *p;
    X x;
    int i;
	char info[20] =
	    { 'h', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', '\0' };
    /*char *p1 = "abc";*/
    /*char *p2 = "def";*/
    for (i = 0; i < 20; i++) {
        x[i] = info[i];
    }
    p = str;
    str[2] = 'z';
    PDEBUG("%s\n", str);
    p[2] = 'z';
    PDEBUG("%s\n", p);
}

/**
 * @brief  
 * @param  param desc
 * @return 
 * @see    
 * @note   
 */
void function_name2()
{
    char *p[4] = {"123", "456", "789", "xyz"};
    int i;

    for (i = 0; i < 4; i++) {
        printf("%d-->%p\n", i, p[i]);
    }

    for (i = 0; i < 4; i++) {
        printf("%d-->%p\n", i, *(p+i));
    }
}


int main(int argc, const char *argv[])
{
	int a;

	a = 0;
	assert(a != 1);

	/*function_name();*/
    function_name2();

	return 0;
}
