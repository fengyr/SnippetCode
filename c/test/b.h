/*
 * =====================================================================================
 *
 *       @file      b.h
 *       @brief      
 *
 *       @internal
 *       @Version:  1.0
 *       @date:  2010年05月18日 14时33分36秒
 *       Revision: none
 *       Compiler:  gcc
 *
 *       @author:  Zenki (zhajun), zenki2001cn@163.com
 *       Company:  
 *
 * =====================================================================================
 */

#ifndef _MAIN_H_
#define _MAIN_H_

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

/**
 * this struct is xxy
 */
typedef struct _bit {
	int a;
	char b;
} BIT;
/**
 * @brief  do something
 * @param  param1
 * @return 0
 */
void function_name();

#endif
