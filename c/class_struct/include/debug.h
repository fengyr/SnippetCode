/**
 *       File       debug.h
 *
 *       Revision:  none
 *       Compiler:  gcc
 *
 *       Version    1.0
 *       Author:    Zenki (zhajun), zenki2001cn@163.com
 *       Date:      2010年06月02日 13时19分52秒
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

#define MAKE(CLASS, VAR, ARGS...)   \
    do {                            \
        CLASS #VAR;                 \
        VAR_init(&VAR, ## ARGS);    \
    } while (0)                     

