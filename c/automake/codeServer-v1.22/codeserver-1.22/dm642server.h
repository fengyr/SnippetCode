
/***********************************************************************
 Copyright (C), 2005-2006, Gentech System. CO., LTD.

 FileName: dm642server.h

 <Author>    <Version>    <Date>
  Zenki        1.13       2006.05.09

 Description: header file for dm642server.c;
              include the header files;

 Function list:
 ......

 ***********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "errno.h"
#include "fcntl.h"
#include "netdb.h"
#include "ds40xxhcApi.h"
#include "DataType.h"
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>
#include "readdeploy.h"
#include "iconv.h"
#include "agent_api.h"
#include <time.h>

#define MAXCARD 64
#define UnicodeToGb(High, Low)             (High * 256 + Low + 256)
#define Turn(X)                            (((int)(X / 8)) * 8)

//handle information
void Query(char, int, int);
int Respond_Handle(int, char*, int);
int writeUDP(int, char*, int);
STREAM_READ_CALLBACK StreamRead_CallBack(int, void*, int, int, int);

//initialize socket
int initStation();
int initClient(int);
int initUDP(int);
int COpen(int, int);

//load log and parse command line
int parseArg(int, char**);
int Load_log();

//monitor thread
void* thread_monitor(void*);
//agent
void* op_videoPara(CALLBACK);

void* op_IBPmode(CALLBACK);

void* op_Quant(CALLBACK);

void* op_Signal(CALLBACK);

void* op_Osd(CALLBACK);

void* op_framesStatistics(CALLBACK);
