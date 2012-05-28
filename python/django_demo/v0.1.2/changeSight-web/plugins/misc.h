#ifndef MISC_H
#define MISC_H

#include <sys/time.h>
#include <time.h>

#define SHOWERR do{printf("Err on line %d of file:%s\n", __LINE__, __FILE__);fflush(stdout);}while(0)
#define CHECK(x,ret) do{if ((x)<0) {SHOWERR; return ret;}}while(0)
#define ISNULL(x,ret) do{ if (((x)==0)&&(ret<0)) {SHOWERR; return ret;}}while(0)


struct timeval * getClock();
void tick(struct timeval * p, int fps);
void freeClock(struct timeval *p);
void keepSleeping();
void wakeUp();     

#endif
