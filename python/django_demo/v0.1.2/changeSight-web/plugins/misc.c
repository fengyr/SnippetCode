#include "misc.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

static int sleeping=0;

struct timeval * getClock()
{
  struct timeval *tv=(struct timeval *)malloc(sizeof(struct timeval));
  gettimeofday(tv, NULL);
  return tv;
}
 
void tick(struct timeval * p, int fps)
{
  int interval, gap;
  struct timeval now;
  
  
  gap=1000000/fps;
  
  if (p==NULL) usleep(gap);
  else{
    gettimeofday(&now, NULL);
    
    interval=(now.tv_sec-p->tv_sec)*1000000;
    interval+=(long)(now.tv_usec) - (long)(p->tv_usec);
    
    if (interval<gap)
      usleep(gap-interval);
    else usleep(5000);
    
    gettimeofday(&now, NULL);
    p->tv_sec=now.tv_sec;
    p->tv_usec=now.tv_usec;
  }

}
 
void freeClock(struct timeval *p)
{
  if (p!=NULL) free(p);
}

void keepSleeping()
{
  sleeping=1;
  while (sleeping){
    usleep(100000);
  }
}

void wakeUp()
{
  if (sleeping)
    sleeping=0;
}
