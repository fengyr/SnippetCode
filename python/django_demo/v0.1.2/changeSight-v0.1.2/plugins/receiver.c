#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>

#include "receiver.h"
#include "misc.h"


struct FDDesp
{
  int fd;
  int type;
  char mip[32];
}fddesp[100];

int FDDespLen=0;
char LocalIP[48] = "";

char* getlocalip()
{
    FILE * f;
        
    if(LocalIP[0] = '\0'){
	
	f=popen("/sbin/ifconfig | head -2 | tail -1 | cut -d: -f 2 | cut -d\\  -f 1","r");
	fscanf(f,"%s",LocalIP);
	pclose(f);
    }
    
    return LocalIP;
}

int setLocalIP(char * ip)
{
    strcpy(LocalIP, ip);
}

void setSocketOpt(int fd, int tos, int tous)
{
  struct timeval val;
  int val2=110592;

  val.tv_sec=tos;
  val.tv_usec=tous;
  setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, (void*)&val, sizeof(val));
  setsockopt(fd, SOL_SOCKET, SO_SNDTIMEO, (void*)&val, sizeof(val));

  setsockopt(fd, SOL_SOCKET, SO_SNDBUF, (void*)&val2, sizeof(val2));
  setsockopt(fd, SOL_SOCKET, SO_RCVBUF, (void*)&val2, sizeof(val2));
}


int ParseNet(char * desp)	/* Desp:"[TCP/UDP/MULTICAST]:x.x.x.x:port", return fd*/
{
  int type=-1;
  long int port, i;
  char *ip, *p;
  int fd;
  struct sockaddr_in addr;
  struct ip_mreq mreq;
  int loop;
 
  if (!strncmp(desp, "TCP", 3)){
    type=0;
    fd=socket(AF_INET, SOCK_STREAM, 0);
    desp+=4;
  } else if (!strncmp(desp, "UDP", 3)) {
    type=1;
    fd=socket(AF_INET, SOCK_DGRAM, 0);
    desp+=4;
  } else if (!strncmp(desp, "MULTICAST", 9)) {
    type=2;
    fd=socket(AF_INET, SOCK_DGRAM, 0);
    desp+=10;
  }
  ip=strtok(desp, ":");
  p=strtok(NULL, "\0"); port=atoi(p);
  
  //printf("The ip is %s, the port is %d\n", ip, port);
   
  CHECK(fd, -1);
  if(type==0){			/* TCP, set timeout:0.1s */
    setSocketOpt(fd, 0, 100000);
  } else {			/* UDP, set timeout:1s */
    setSocketOpt(fd, 1, 0);
  }
  
  
  memset(&addr,0,sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_port   = htons(port);
  addr.sin_addr.s_addr = inet_addr(ip);

  if ((type==0)||(type==1)){	/* is TCP or UDP, need connect */
    CHECK(connect(fd, (struct sockaddr *)&addr, sizeof(addr)), -1);
  }
  if ((type==1)||(type==2)){	/* is UDP or MULTICAST, need bind */
    loop = 1;
    CHECK(setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &loop, sizeof(loop)), -1);
    CHECK(bind(fd,(struct sockaddr *)&addr,sizeof(addr)), -1);
  }

  if (type==2){
    /* is MULTICAST, active MULTICAST_LOOP */
    loop = 1;
    if (setsockopt(fd, IPPROTO_IP, IP_MULTICAST_LOOP, &loop, sizeof(loop))<0){
      perror("setsockopt_loop");
    }
    
    /* Join Multicast group */
    mreq.imr_multiaddr.s_addr = inet_addr(ip); 
    mreq.imr_interface.s_addr = inet_addr(LocalIP);
//    CHECK(setsockopt(fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (const void *)&mreq, sizeof(mreq)), -1);
  }
  
  fddesp[FDDespLen].fd=fd;
  fddesp[FDDespLen].type=type;
  strcpy(fddesp[FDDespLen].mip, ip);
  FDDespLen++;  
  
  return fd;
}


int closeNet(int fd)
{
  struct ip_mreq mreq;
  struct sockaddr_in addr;
  int len=sizeof(addr), i;
  int found=-1;

  for (i=0; i<FDDespLen; i++)
    if (fddesp[i].fd==fd){
      found=i;
      break;
    }
  if (found==-1){
    close(fd);
    return 0;
  }
  
  if (fddesp[found].type==2){
    mreq.imr_multiaddr.s_addr = inet_addr(fddesp[found].mip);
    mreq.imr_interface.s_addr = inet_addr(LocalIP);
    CHECK(setsockopt(fd, IPPROTO_IP, IP_DROP_MEMBERSHIP, (const void *)&mreq, sizeof(mreq)), -1);
  }
  close(fd);

  if (FDDespLen-1==found) FDDespLen--;
  else{
    fddesp[found].fd=fddesp[FDDespLen-1].fd;
    fddesp[found].type=fddesp[FDDespLen-1].type;
    strcpy(fddesp[found].mip, fddesp[FDDespLen-1].mip);
  }

  return 0;
}
