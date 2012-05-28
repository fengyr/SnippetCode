#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <sys/types.h>

#undef PDEBUG			/* undef it, just in case */
#ifdef USE_DEBUG
#  ifdef __KERNEL__
     /* This one if debugging is on, and kernel space */
#    define PDEBUG(fmt, args...) printk( KERN_DEBUG "scull: " fmt, ## args)
#  else
     /* This one for user space */
#    define PDEBUG(fmt, args...) fprintf(stderr, fmt, ## args)
#  endif
#else
#  define PDEBUG(fmt, args...)	/* not debugging: nothing */
#endif

void show1(int i, int j, int p);
void show2(int i, int j, int p);
void show3(int i, int j, int p);
void localtime_h(time_t time, struct tm *ret_time);
void localtimes(time_t time, long timezone, struct tm *tm_time);

#define ShowWindowWidth 1000
