/*****************************************************************
 Copyright (C), 2005-2006, Gentech System. CO., LTD.

 FileName: dm642server.c

 <Author>    <Version>    <Date>
  zenki         1.14      2006.06.16
  Mat		1.11	  2006.04.21
  
 Description: encode video date and send them to net;

 Module list:
 1.initialize socket
 2.transform data
 3.load log
 4.handle message
 5.query information

 *****************************************************************/
#include "dm642server.h"

char MULTICASTIP[MAXCARD][16];           /* multicast IP address */
int  MULTICASTPORT[MAXCARD];
char NETIP[16];
int  NETPORT;

int  GroupPorts[MAXCARD];                    /* card port */
unsigned int totalcard, totalport;
int fds[MAXCARD];                            /* card fd */
int client_fds[MAXCARD];                     /* client fd */
int station_fd;
int station_id[MAXCARD];


struct sockaddr_in group_addr;
struct sockaddr_in station_addr;
struct sockaddr_in station_send_addr;
struct sockaddr_in client_addr[64];
struct sockaddr_in client_send_addr[64];

int videoPara[MAXCARD][4];                          /* save param */
int IBPmode[MAXCARD][3];                			/* save IBP */
int Quant[MAXCARD][3];                				/* save quant */
int SIGNAL[MAXCARD];
int OSD[MAXCARD];
int new_oid[] = {0, 0};

/********************/
FRAMES_STATISTICS frames_statistics;
unsigned long  VideoFrames[MAXCARD];
unsigned long  AudioFrames[MAXCARD];
unsigned long  FramesLost[MAXCARD];
unsigned long  QueueOverflow[MAXCARD];
unsigned long  CurBps[MAXCARD];
/********************/

TRAP_LEV level;
int codeSize[MAXCARD];                       /* video class: 0 -> cif, 1 -> d1 */
int codeCtrl[MAXCARD];                       /* dmdu ID */
int channelHandle[MAXCARD];					 /* channel handle */
int videoSignal[MAXCARD];
int videoSignalRespond[MAXCARD];

unsigned short timeFormat[] = {_OSD_YEAR4, _OSD_YEAR2, _OSD_MONTH3, _OSD_MONTH2, _OSD_DAY,
                               _OSD_WEEK3, _OSD_CWEEK1, _OSD_HOUR24, _OSD_HOUR12, _OSD_MINUTE,
                               _OSD_SECOND};






/**************************** initialize socket ******************************
 */
int initStation(){
     int share = 1;
     int loop = 15;
     struct ip_mreq mreq;

     station_fd = socket(AF_INET, SOCK_DGRAM, 0);
     if(station_fd == -1){
          fprintf(stderr, "Station socket err");
          return -1;
     }
     if(setsockopt(station_fd, SOL_SOCKET, SO_REUSEADDR, &share, sizeof(share)) < 0){
          fprintf(stderr, "Station use err");
     }
     memset(&station_addr, 0, sizeof(station_addr));
     station_addr.sin_family = AF_INET;
     station_addr.sin_addr.s_addr = inet_addr(NETIP);
     station_addr.sin_port = htons(NETPORT);
     if(bind(station_fd, (struct sockaddr*)&station_addr, sizeof(station_addr)) < 0){
          fprintf(stderr, "Station bind err");
          return -1;
     }

     memset(&station_send_addr, 0, sizeof(station_send_addr));
     station_send_addr.sin_family = AF_INET;
     station_send_addr.sin_addr.s_addr = inet_addr(NETIP);
     station_send_addr.sin_port = htons(NETPORT);

     if(setsockopt(station_fd, IPPROTO_IP, IP_MULTICAST_TTL, &loop, sizeof(loop)) < 0){
          fprintf(stderr, "Station loop err");
          return -1;
     }
     mreq.imr_multiaddr.s_addr = inet_addr(NETIP);
     mreq.imr_interface.s_addr = htonl(INADDR_ANY);
     if(mreq.imr_multiaddr.s_addr == -1){
          fprintf(stderr, "Station:invalid address err");
          return -1;
     }
     if(setsockopt(station_fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) < 0){
          fprintf(stderr, "Station add member err");
          return -1;
     }

     return 0;
}

/* setup client UDP */
int initClient(int y){
     int fd;
     int loop, share = 1;
     struct ip_mreq mreq;

     if((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
          fprintf(stderr, "%d socket err", y);
          return -1;
     }

     if(setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &share, sizeof(share)) < 0){
          fprintf(stderr, "%d Multiple use err", y);
     }

     memset(&client_addr[y], 0, sizeof(client_addr[y]));
     client_addr[y].sin_family = AF_INET;
     client_addr[y].sin_addr.s_addr = inet_addr(MULTICASTIP[y]);
     client_addr[y].sin_port = htons(MULTICASTPORT[y]);
     if(bind(fd, (struct sockaddr*)&client_addr[y], sizeof(client_addr[y])) < 0){
          fprintf(stderr, "%d Bind client err", y);
          return -1;
     }

     memset(&client_send_addr[y], 0, sizeof(client_send_addr[y]));
     client_send_addr[y].sin_family = AF_INET;
     client_send_addr[y].sin_addr.s_addr = inet_addr(MULTICASTIP[y]);
     client_send_addr[y].sin_port = htons(MULTICASTPORT[y]);

     loop = 15;
     if(setsockopt(fd, IPPROTO_IP, IP_MULTICAST_TTL, &loop, sizeof(loop)) < 0){
          fprintf(stderr, "%d setsockopt err", y);
     }

     mreq.imr_multiaddr.s_addr = inet_addr(MULTICASTIP[y]);
     mreq.imr_interface.s_addr = htonl(INADDR_ANY);
     if(mreq.imr_multiaddr.s_addr == -1){
          fprintf(stderr, "%d invalid address err", y);
     }
     if(setsockopt(fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) < 0){
          fprintf(stderr, "%d Add member err", y);
          return -1;
     }
     return fd;
}

/* setup card UDP */
int initUDP(int x){
    int fd;
    int loop, share = 1;
    struct sockaddr_in addr;
    struct ip_mreq     mreq;

    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &share, sizeof(share)) < 0){
         fprintf(stderr, "%d Multiple use err", x);
    }

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(MULTICASTIP[x]);
    addr.sin_port = htons(GroupPorts[x]);
    if(bind(fd, (struct sockaddr*)&addr, sizeof(addr)) < 0){
        fprintf(stderr, "%d Bind err", x);
        return -1;
    }
    loop = 15;
    if(setsockopt(fd, IPPROTO_IP, IP_MULTICAST_TTL, &loop, sizeof(loop)) < 0){
        fprintf(stderr, "%d setsockopt err", x);
    }
    printf("initUDP:%d connect to %s:%d\n", fd, MULTICASTIP[x], GroupPorts[x]);

    memset(&group_addr, 0, sizeof(group_addr));
    group_addr.sin_family = AF_INET;
    group_addr.sin_addr.s_addr = inet_addr(MULTICASTIP[x]);
    group_addr.sin_port = htons(GroupPorts[x]);

//    inet_aton(MULTICASTIP[x], (struct in_addr*)&mreq.imr_multiaddr.s_addr);
//    mreq.imr_interface.s_addr = htonl(GroupPorts[x]);

//    if (mreq.imr_multiaddr.s_addr == -1){
//	    perror("Multiaddr err");
//	    return -1;
//    }
//    if (setsockopt(fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) < 0){
//	    perror("Add member err");
//	    return -1;
//   }
    if(connect(fd, (struct sockaddr*)&group_addr, sizeof(group_addr)) < 0){
        fprintf(stderr, "%d Connect err", x);
        return -1;
    }

    return fd;
}

STREAM_READ_CALLBACK StreamRead_CallBack(int ChannelNum, void *pBuf, int frameType, int length,
                                         int frameNum){
    switch(frameType)
        {
        case PktSysHeader:break;
        case PktIFrames:
        case PktPFrames:
        case PktBBPFrames:
        case PktAudioFrames:
        {
            writeUDP(fds[ChannelNum], (char*)pBuf, length);
            break;
        }
        case PktMotionDetection:break;
        default:
            break;
        }
}

/* setup DSP */
int COpen(int i, int size){     /* size: 0 -> CIF, 1 -> D1 */
    int  j;
    char *smap[] = {"CIF", "D1"};

    if((channelHandle[i] = ChannelOpen(i, (STREAM_READ_CALLBACK)StreamRead_CallBack)) < 0){ /* Open channel */
        fprintf(stderr, "Open port %d failed\n", i);
        for(j = 0; j < i; j++){
            ChannelClose(j);
        }
        exit(1);
    }

    /* initialize UDP */
    fds[i] = initUDP(i);
    
    /* initialize UDP */
    client_fds[i] = initClient(i);
    if(fds[i] < 0){
        return;
    }


	if (size){
		SetDefaultQuant(channelHandle[i], 12, 12, 17);
		SetIBPMode(channelHandle[i], 15, 1, 0, 15);
		SetupBitrateControl(channelHandle[i], 2000000);
		if(SetEncodePictureFormat(channelHandle[i], ENC_4CIF_FORMAT) < 0){
            printf("Error: Set D1 of Channel %d failed.\n", i);
            exit(1);
        }
	}
	else{
		SetDefaultQuant(channelHandle[i], 15, 15, 20);
		SetIBPMode(channelHandle[i], 25, 1, 0, 25);
		SetupBitrateControl(channelHandle[i], 1000000);
	}

	//brCBR -> 0, brVBR -> 1
    SetBitrateControlMode(channelHandle[i], brVBR);
    SetOsd(channelHandle[i], 1);

    /* start video */
    if(StartVideoCapture(channelHandle[i]) < 0){
        printf("Capture Channel %d failed.\n", i);
    }
    else{
	printf("start capture %d ok\n", i);
    }
}
/*
 *************************** initialize socket *******************************/

/**************************** transform data ******************************
 */
int writeUDP(int fd, char *pBuf, int len){
    int t, l;
    t = len;

    while(len > 0){
        t = len;
        if(t > 4096)
            t = 4096;
        l = write(fd, pBuf, t);
        if(l < 0){
             fprintf(stderr, "writeUDP err fd:%d  buf:%s len:%d", fd, pBuf, len);
	     perror("writeUDP");
             return -1;
        }
        pBuf += l;
        len -= l;
    }

    return 0;
}
/*
 *************************** transform data *******************************/

/**************************** load log ******************************
 */
int parseArg(int argc, char** argv){
     int skip = 1;

     if(argc < 3){
          return -1;
     }
     if(argv[1][0] == '-'){
          skip++;
     }

     get_deploy(argv[skip], argv[skip + 1]);

     //Load_log();

     return 0;
}

int Load_log(){
     FILE               *file;
     HEADINF            *inf, *info;
     ALLINF             *all;
     int                i, dmduid;

     if(deploy_head(&inf) == 1){
          if(inf){
               strcpy(NETIP, inf -> addr);
               NETPORT = inf -> port;

               info = inf;
          }else{
               printf("Read nmp err\n");
          }
          free_head(&info);

          if(deploy_all(&all) == 1){
               for(i = 0; i < all -> quantity; i++){
		    dmduid = all -> dmduid[i];
                    codeCtrl[dmduid] = dmduid;
                    strcpy(MULTICASTIP[dmduid], all -> videoaddr[i]);
                    MULTICASTPORT[dmduid] = all -> controlport[i];
                    GroupPorts[dmduid] = all -> videoport[i];
                    station_id[dmduid] = all -> id[i];

                    if(strcmp(all -> type[i], "dm642_cif") == 0){
                         codeSize[dmduid] = 0;
                    }else if(strcmp(all -> type[i], "dm642_d1") == 0){
                         codeSize[dmduid] = 1;
                    }
               }
          }else{
               printf("Read information err\n");
               exit(0);
          }
          free_all(&all);
     }else{
          exit(0);
     }
}
/*
 *************************** load log *******************************/

/**************************** main ****************************
 */
int main(int argc, char** argv){
    int      i, j, k, max_fd, rc, readNum, channel;
    char     buf[64], *p[2];
    fd_set   read_fds;

    snmp_thread();

    for(i = 0; i < MAXCARD; i++){
        memset(MULTICASTIP[i], 0, 16);
		client_fds[i] = -1;
		fds[i] = -1;
		MULTICASTPORT[i] = -1;
		codeCtrl[i] = -1;
		NETIP[i] = 0;
		station_id[i] = 0;
		channelHandle[i] = -1;
        videoSignal[i] = 1;
        videoSignalRespond[i] = 0;
		
		videoPara[i][0] = -1;
		videoPara[i][1] = -1;
		videoPara[i][2] = -1;
		videoPara[i][3] = -1;
		
		IBPmode[i][0] = 25;
		IBPmode[i][1] = 1;
		IBPmode[i][2] = 25;
		
		Quant[i][0] = 18;
		Quant[i][1] = 18;
		Quant[i][2] = 23;
		
		VideoFrames[i] = -1;
		AudioFrames[i] = -1;
		FramesLost[i] = -1;
		QueueOverflow[i] = -1;
		CurBps[i] = -1;
		
		SIGNAL[i] = 1;
		OSD[i] = 0;
    }
    station_fd = -1;

    if(parseArg(argc, argv) < 0){
         printf("Usage:./dm642server [-d] [NetManagerIP] [StationID]\n");
         printf("-d: daemon mode\n");
         printf("NetManagerIP: Get message from Net manager\n");
         printf("StationID: Current station ID\n");
         exit(1);
    }

    Load_log();
    
/*    for (i=0; i<MAXCARD; i++) {
         printf("%d-%d-%d-%d-%d\n", MULTICASTPORT[i], codeCtrl[i],
                station_id[i], MULTICASTPORT[i],codeSize[i]);
    }
*/
    if(!strcmp(argv[1], "-d")){
         if(fork()){             /* father proc exit */
              exit(0);
         }
    }

    if(InitDSPs() < 0){
         exit(1);
    }

    totalcard = GetTotalDSPs();
    totalport = GetTotalChannels();
    printf("TotalDSP:%d TotalChannel:%d\n", totalcard, totalport);

    for(i = 0; i < totalport; i++){
         if(codeCtrl[i] != -1){
              COpen(codeCtrl[i], codeSize[i]);
         }
    }
    if(initStation() < 0){
         exit(1);
    }

    /* agent */
	// 初始化注册oid的号码
	oid para_oid[1088][3];
	for (i=0, j=0, k=1; i<1088; i++){
		para_oid[i][0] = 10;
		para_oid[i][1] = j;
		para_oid[i][2] = k;

		// 超过17个属性，更新通道号
		if (k == 17){
			k = 1;
			j++;
			continue;
		}
		// 更新属性号
		k++;
	}

	for (i=0; i<MAXCARD; i++){
			register_int_instance(&para_oid[i*17+0][0], 3, &videoPara[i][0], op_videoPara);
			register_int_instance(&para_oid[i*17+1][0], 3, &videoPara[i][1], op_videoPara);
			register_int_instance(&para_oid[i*17+2][0], 3, &videoPara[i][2], op_videoPara);
			register_int_instance(&para_oid[i*17+3][0], 3, &videoPara[i][3], op_videoPara);

			register_int_instance(&para_oid[i*17+4][0], 3, &IBPmode[i][0], op_IBPmode);
			register_int_instance(&para_oid[i*17+5][0], 3, &IBPmode[i][1], op_IBPmode);
			register_int_instance(&para_oid[i*17+6][0], 3, &IBPmode[i][2], op_IBPmode);

			register_int_instance(&para_oid[i*17+7][0], 3, &Quant[i][0], op_Quant);
			register_int_instance(&para_oid[i*17+8][0], 3, &Quant[i][1], op_Quant);
			register_int_instance(&para_oid[i*17+9][0], 3, &Quant[i][2], op_Quant);
			register_read_only_int_instance(&para_oid[i*17+10][0], 3, &SIGNAL[i], op_Signal);
			register_int_instance(&para_oid[i*17+11][0], 3, &OSD[i], op_Osd);

			register_read_only_ulong_instance(&para_oid[i*17+12][0], 3, &VideoFrames[i], op_framesStatistics);
			register_read_only_ulong_instance(&para_oid[i*17+13][0], 3, &AudioFrames[i], op_framesStatistics);
			register_read_only_ulong_instance(&para_oid[i*17+14][0], 3, &FramesLost[i], op_framesStatistics);
			register_read_only_ulong_instance(&para_oid[i*17+15][0], 3, &QueueOverflow[i], op_framesStatistics);
			register_read_only_ulong_instance(&para_oid[i*17+16][0], 3, &CurBps[i], op_framesStatistics);
	}

    //monitor camera
    pthread_t trd_monitor;
    if (pthread_create(&trd_monitor, NULL, thread_monitor, (void*)NULL) < 0){
         level = error;
         snmptrap_oid(level, new_oid, 2,"Create monitor thread falied\n", NULL, 0);
    }
    /* I/O multi select loop */
    int len;
    do{
         FD_ZERO(&read_fds);
         max_fd = 0;
         for(i = 0; i < MAXCARD; i++){
              if(client_fds[i] != -1){
                   FD_SET(client_fds[i], &read_fds);
                   if(client_fds[i] > max_fd){
                        max_fd = client_fds[i];
                   }
              }
         }
         if(station_fd != -1){
              FD_SET(station_fd, &read_fds);
              if(station_fd > max_fd){
                   max_fd = station_fd;
              }
         }
         max_fd++;

         rc = select(max_fd, &read_fds, NULL, NULL, NULL);
         if(rc == 0) continue;

         if(station_fd != -1){
              if(FD_ISSET(station_fd, &read_fds)){
                   len = sizeof(station_addr);
                   memset(buf, 0, sizeof(buf));
                   if((readNum = recvfrom(station_fd, buf, sizeof(buf), 0,
                                          (struct sockaddr*)&station_addr, &len)) == -1){
                        fprintf(stderr, "Station receive err");
                        continue;
                   }
                   buf[readNum] = '\0';
                   if(buf[readNum - 1] == '\n'){
                        buf[readNum - 1] = '\0';
                   }

                   //p[0] = strtok(buf, ":");
                   if(strncmp(buf, "encoder", 7) != 0){
                        continue;
                   }

                   p[0] = strtok(&buf[8], " ");
                   channel = atoi(p[0]);
                   for(i = 0; i < MAXCARD; i ++){
                        if(station_id[i] == channel){
                             channel = i;
                             break;
                        }
                   }

                   Respond_Handle(1, &buf[14], channel);
              }
         }

         for(i = 0; i < MAXCARD; i++){
              if(client_fds[i] < 0){
                   continue;
              }else if(FD_ISSET(client_fds[i], &read_fds)){
                   memset(buf, 0, sizeof(buf));
                   len = sizeof(client_addr[i]);
                   if((readNum = recvfrom(client_fds[i], buf, sizeof(buf), 0,
                                          (struct sockaddr*)&client_addr[i], &len)) < 0){
                       // fprintf(stderr, "Receive from No.%d client err\n", i);
		       sleep(1000);
                        continue;
                   }
                   buf[readNum] = '\0';
                   if(buf[readNum - 1] == '\n'){
                        buf[readNum - 1] = '\0';
                   }

                   //p[0] = strtok(buf, ":");
                   if(strncmp(buf, "encoder", 7) != 0){
                        continue;
                   }

                   Respond_Handle(0, &buf[8], i); /* handle message */
              }
         }
    }while(1);

    exit(0);
}
/*
 *************************** main *****************************/

/**************************** Monitor camera ******************************
 */
void* thread_monitor(void *arg){
    int totalport;
    totalport = GetTotalChannels();
    char trapinfo[128];
    int i;
    char timeinfo[64];
    time_t date;
    struct tm *ptm;

    while (1){
         for(i = 0; i < totalport; i++){
              if(codeCtrl[i] != -1){
                   if(GetVideoSignal(channelHandle[codeCtrl[i]]) == 1){
                        // format information
                        memset(trapinfo, 0, sizeof(trapinfo));
                        memset(timeinfo, 0, sizeof(timeinfo));
                        date = time(NULL);
                        ptm = localtime(&date);
                        sprintf(timeinfo, "%d-%02d-%02d-%02d-%02d-%02d",
                                ptm->tm_year + 1900, ptm->tm_mon + 1, ptm->tm_mday,
                                ptm->tm_hour, ptm->tm_min, ptm->tm_sec);
                        sprintf(trapinfo, "encoder:video_disappear %d %s\n",
                                station_id[i], timeinfo);

                        if (videoSignal[codeCtrl[i]]){
                             level = error;
                             snmptrap_oid(level, new_oid, 2, trapinfo, NULL, 0);
                             videoSignal[codeCtrl[i]] = 0;
                        }
                        else {
                             videoSignal[codeCtrl[i]] = 1;
                        }
                        if (!videoSignalRespond[i]){
                             if (sendto(station_fd, trapinfo, strlen(trapinfo), 0,
                                        (struct sockaddr*)&station_send_addr,
                                        sizeof(station_send_addr)) < 0){
                                  perror("send signal info error");
                             }
                        }
                   }
                   else {
                        if (videoSignalRespond[i]) {
                             memset(trapinfo, 0, sizeof(trapinfo));
                             memset(timeinfo, 0, sizeof(timeinfo));
                             date = time(NULL);
                             ptm = localtime(&date);
                             sprintf(timeinfo, "%d-%02d-%02d-%02d-%02d-%02d",
                                     ptm->tm_year + 1900, ptm->tm_mon + 1, ptm->tm_mday,
                                     ptm->tm_hour, ptm->tm_min, ptm->tm_sec);
                             sprintf(trapinfo, "encoder:cancel:video_disappear %d %s\n",
                                     station_id[i], timeinfo);

                             if (sendto(station_fd, trapinfo, strlen(trapinfo), 0,
                                        (struct sockaddr*)&station_send_addr,
                                        sizeof(station_send_addr)) < 0){
                                  perror("send signal info error");
                             }
                        }
                        videoSignalRespond[i] = 0;
                   }
                   SetOsd(channelHandle[codeCtrl[i]], 0);
                   SetOsd(channelHandle[codeCtrl[i]], 1);
              }
         }
	 sleep(60);
    }
}
/*
 *************************** Monitor camera *******************************/

/**************************** Handle message ******************************
 */
int Respond_Handle(int mark, char *buf, int channel){
    char           *p, *q, *format[5], head[4];
    int            i, j, brt, line;
    char           sendBuf[128];
    int            len = sizeof(client_send_addr[channel]);
    unsigned short X, Y;

    static unsigned short Fm2[256];

    memset(sendBuf, 0, sizeof(sendBuf));
    memset(head, 0, 4);

    memcpy(head, buf, 2);
    if(strcmp(head, "qp") == 0){
         Query('p', mark, channel);
    }
    if(strcmp(head, "qi") == 0){
         Query('i', mark, channel);
    }
    if(strcmp(head, "qq") == 0){
         Query('q', mark, channel);
    }
    if(strcmp(head, "ac") == 0){
         videoSignalRespond[channel] = 1;
    }
    if(strcmp(head, "qx") == 0){
         if((brt = GetVideoSignal(channelHandle[codeCtrl[channel]])) < 0){
              return -1;
         }
         if(!brt){
              if(sendto(client_fds[channel], "encoder:ret:normal\n",
                        strlen("encoder:ret:normal\n"), 0,
                        (struct sockaddr*)&client_send_addr[channel], len) < 0){
                   fprintf(stderr, "Send client %d err", channel);
              }
              if(mark){
                   if(sendto(station_fd, "encoder:ret:normal\n",
                             strlen("encoder:ret:normal\n"), 0,
                             (struct sockaddr*)&station_send_addr,
                             sizeof(station_send_addr)) < 0){
                        fprintf(stderr, "Send station err");
                   }
              }
         }else{
              if(sendto(client_fds[channel], "encoder:ret:error\n",
                        strlen("encoder:ret:error\n"), 0,
                        (struct sockaddr*)&client_send_addr[channel], len) < 0){
                   fprintf(stderr, "Send client %d err", channel);
              }
              if(mark){
                   if(sendto(station_fd, "encoder:ret:error\n", strlen("encoder:ret:error\n"), 0,
                             (struct sockaddr*)&station_send_addr,
                             sizeof(station_send_addr)) < 0){
                        fprintf(stderr, "Send station err");
                   }
              }
         }
    }
    if(strcmp(head, "cp") == 0){
         /* filter param */
         format[0] = strtok(buf, " ");
         for(i = 1; i < 5; i++){
              format[i] = strtok(NULL, " ");
         }

         for(i = 0; i < 4; i++){
              videoPara[channel][i] = atoi(format[i+1]);
              if (videoPara[channel][i] > 255){
                   videoPara[channel][i] = 255;
              }
              if (videoPara[channel][i] < 0){
                   videoPara[channel][i] = 0;
              }
         }
         if(SetVideoPara(channelHandle[channel], videoPara[channel][0], videoPara[channel][1], videoPara[channel][2], videoPara[channel][3]) < 0){
              memset(sendBuf, 0, sizeof(sendBuf));
              strcpy(sendBuf, "encoder:ret:set video para failed\n");
              if(sendto(client_fds[channel], sendBuf, strlen(sendBuf), 0,
                        (struct sockaddr*)&client_send_addr[channel], len) < 0){
                   fprintf(stderr, "Send client %d err", channel);
              }
              if(mark){
                   if(sendto(station_fd, sendBuf, strlen(sendBuf), 0,
                             (struct sockaddr*)&station_send_addr,
                             sizeof(station_send_addr)) < 0){
                        fprintf(stderr, "Send station err");
                   }
              }
              return -1;
         }
         Query('p', mark, channel);
    }
    if(strcmp(head, "ci") == 0){
         /* filter param */
         format[0] = strtok(buf, " ");
         for(i = 1; i < 4; i++){
              format[i] = strtok(NULL, " ");
         }
         for(i = 0; i < 3; i++){
              IBPmode[channel][i] = atoi(format[i+1]);
         }
         if (IBPmode[channel][0] < 12){
              IBPmode[channel][0] = 13;
         }
         else if (IBPmode[channel][1] < 0 || IBPmode[channel][1] > 2){
              //......
         }
         else if (IBPmode[channel][2] < 1){
              IBPmode[channel][2] = 1;
         }
         else if (IBPmode[channel][2] > 30){
              IBPmode[channel][2] = 30;
         }

         if(SetIBPMode(channelHandle[channel], IBPmode[channel][0], IBPmode[channel][1], 0, IBPmode[channel][2]) < 0){
              memset(sendBuf, 0, 128);
              strcpy(sendBuf, "encoder:ret:set ibpmode failed\n");
              if(sendto(client_fds[channel], sendBuf, strlen(sendBuf), 0,
                        (struct sockaddr*)&client_send_addr[channel], len) < 0){
                   fprintf(stderr, "Send client %d err", channel);
              }
              if(mark){
                   if(sendto(station_fd, sendBuf, strlen(sendBuf), 0,
                             (struct sockaddr*)&station_send_addr,
                             sizeof(station_send_addr)) < 0){
                        fprintf(stderr, "Send station err");
                   }
              }
              return -1;
         }

         Query('i', mark, channel);
    }
    if(strcmp(head, "cq") == 0){
         /* filter param */
         format[0] = strtok(buf, " ");
         for(i = 1; i < 4; i++){
              format[i] = strtok(NULL, " ");
         }
         for(i = 0; i < 3; i++){
              Quant[channel][i] = atoi(format[i+1]);
              if (Quant[channel][i] < 12){
                   Quant[channel][i] = 12;
              }
              if (Quant[channel][i] > 30){
                   Quant[channel][i] = 30;
              }
         }
         if(SetDefaultQuant(channelHandle[channel], Quant[channel][0], Quant[channel][1], Quant[channel][2]) < 0){
              memset(sendBuf, 0, 128);
              strcpy(sendBuf, "encoder:ret:set quant failed\n");
              if(sendto(client_fds[channel], sendBuf, strlen(sendBuf), 0,
                        (struct sockaddr*)&client_send_addr[channel], len) < 0){
                   fprintf(stderr, "Send client %d err", channel);
              }
              if(mark){
                   if(sendto(station_fd, sendBuf, strlen(sendBuf), 0,
                             (struct sockaddr*)&station_send_addr,
                             sizeof(station_send_addr)) < 0){
                        fprintf(stderr, "Send station err");
                   }
              }
              return -1;
         }
         Query('q', mark, channel);
    }
    if(strcmp(head, "se") == 0){
         format[0] = strtok(buf, " ");
         format[1] = strtok(NULL, " ");

         brt = atoi(format[1]);
         if(SetOsd(channelHandle[channel], brt) < 0){
              memset(sendBuf, 0, 128);
              strcpy(sendBuf, "encoder:ret:set osd failed\n");
              if(sendto(client_fds[channel], sendBuf, strlen(sendBuf), 0,
                        (struct sockaddr*)&client_send_addr[channel], len) < 0){
                   fprintf(stderr, "Send client %d err", channel);
              }
              if(mark){
                   if(sendto(station_fd, sendBuf, strlen(sendBuf), 0,
                             (struct sockaddr*)&station_send_addr,
                             sizeof(station_send_addr)) < 0){
                        fprintf(stderr, "Send station err");
                   }
              }
              return -1;
         }else{
              memset(sendBuf, 0, 128);
              sprintf(sendBuf, "encoder:ret:set osd %s\n", brt ? "on" : "off");
              if(sendto(client_fds[channel], sendBuf, strlen(sendBuf), 0,
                        (struct sockaddr*)&client_send_addr[channel], len) < 0){
                   fprintf(stderr, "Send client %d err", channel);
              }
              if(mark){
                   if(sendto(station_fd, sendBuf, strlen(sendBuf), 0,
                             (struct sockaddr*)&station_send_addr,
                             sizeof(station_send_addr)) < 0){
                        fprintf(stderr, "Send station err");
                   }
              }
         }
    }

    if(strcmp(head, "os") == 0){
         /* filter param */
         format[0] = strtok(buf, " ");
         for(i = 1; i < 5; i++){
              format[i] = strtok(NULL, " ");
         }
         format[5] = format[4] + 2;

         line = atoi(format[1]);
         X = (unsigned short)atoi(format[2]);
         Y = (unsigned short)atoi(format[3]);
         brt = atoi(format[4]);

         //ensure X position
         if(X > 0){
              if(X > 8){
                   X = Turn(X);
              }
              else{
                   X = 8;
              }
         }
         else if(X == 0){
              X = 8;
         }

         //ensure Y position
         if(Y > 287){
              Y = 287;
         }

         static unsigned short Fm1[] = {8, 3,
                                        _OSD_YEAR4, ':',
                                        _OSD_MONTH2, ':',
                                        _OSD_DAY, ' ',
                                        _OSD_WEEK3, ' ',
                                        _OSD_HOUR24, ':',
                                        _OSD_MINUTE, ':',
                                        _OSD_SECOND, '\0'};

         /* convert UNICODE to gbk */
         iconv_t    cd;
         char       **from, *to, out[256], tf[2];
         size_t     tolen, fromlen;
         int        val, z, tf_num;

         memset(Fm2, 0, sizeof(Fm2));
         memset(out, 0, sizeof(out));
         if(line){
              Fm2[0] = X;
              Fm2[1] = Y;
         }else{
              Fm1[0] = X;
              Fm1[1] = Y;
         }

         //////////////////////////////////////////////////
         cd = iconv_open("gb2312", "utf-8");
         if(cd == 0){
              return -1;
         }
         from = &format[5];
         fromlen = strlen(format[5]);
         to = out;
         tolen = 256;
         if(iconv(cd, from, &fromlen, &to, &tolen) == -1){
              return -1;
         }
         iconv_close(cd);
         /////////////////////////////////////////////////

         for(i = 0, j = 0; i < strlen(out); i++, j++){
              if(out[j] == '<'){
                   j++;
                   z = 0;
                   memset(tf, 0, sizeof(tf));
                   while(out[j] != '>'){
                        tf[z] = out[j];
                        j++;
                        z++;
                   }
                   tf_num = atoi(tf);
                   if(line){
                        Fm2[i + 2] = timeFormat[tf_num];
                   }else{
                        Fm1[i + 2] = timeFormat[tf_num];
                   }
              }else if((unsigned char)out[j] > 160){
                   if(line){
                        Fm2[i + 2] = UnicodeToGb(out[j], out[j + 1]);
                   }else{
                        Fm1[i + 2] = UnicodeToGb(out[j], out[j + 1]);
                   }
                   j++;
              }else{
                   if(line){
                        Fm2[i + 2] = out[j];
                   }else{
                        Fm1[i + 2] = out[j];
                   }
              }
         }
         if(line){
              Fm2[i + 2] = '\0';
         }else{
              Fm1[i + 1] = '\0';
         }

         /* setup Osd */
         memset(sendBuf, 0, 128);
         if(brt){
              if((SetOsdDisplayMode(channelHandle[channel], 0xff, 1, 1, Fm1, Fm2) < 0) ||
                 (SetOsd(channel, 1) < 0)){
                   strcpy(sendBuf, "encoder:ret:set osd mode failed\n");
              }else{
                   strcpy(sendBuf, "encoder:ret:set osd mode ok\n");
              }
         }else{
              if((SetOsdDisplayMode(channelHandle[channel], 0xff, 1, 0x0000, Fm1, Fm2) < 0) ||
                 (SetOsd(channelHandle[channel], 1) < 0)){
                   strcpy(sendBuf, "encoder:ret:set osd mode failed\n");
              }else{
                   strcpy(sendBuf, "encoder:ret:set osd mode ok\n");
              }
         }

         if(sendto(client_fds[channel], sendBuf, strlen(sendBuf), 0,
                   (struct sockaddr*)&client_send_addr[channel], len) < 0){
              fprintf(stderr, "Send client %d err", channel);
         }
         if(mark){
              if(sendto(station_fd, sendBuf, strlen(sendBuf), 0,
                        (struct sockaddr*)&station_send_addr,
                        sizeof(station_send_addr)) < 0){
                   fprintf(stderr, "Send station err");
              }
         }
    }

    return 0;
}
/*
 *************************** Handle message *******************************/

/**************************** query information ****************************
 */
//query param
void Query(char option, int mark, int n){
     char    sendBuf[128];
     int     len = sizeof(client_send_addr[n]);

     memset(sendBuf, 0, sizeof(sendBuf));
     switch(option)
     {
     case 'p':                  /* query video */
     {
          VideoStandard_t videostandard = StandardNTSC;
          if(GetVideoPara(n, &videostandard, &videoPara[n][0], &videoPara[n][1], &videoPara[n][2],
                          &videoPara[n][3]) < 0){
               fprintf(stderr, "Get video para failed");
               strcpy(sendBuf, "encoder:ret:get video para failed\n");
               if(sendto(client_fds[n], sendBuf, strlen(sendBuf), 0,
                         (struct sockaddr*)&client_send_addr[n], len) < 0)
                    fprintf(stderr, "Send client %d err", n);
               if(mark){
                    if(sendto(station_fd, sendBuf, strlen(sendBuf), 0,
                              (struct sockaddr*)&station_send_addr,
                              sizeof(station_send_addr)) < 0){
                         fprintf(stderr, "Send station err");
                    }
               }
          }else{
               sprintf(sendBuf, "encoder:ret:channel %05d-brightness %d-contrast %d-saturation %d-hue %d\n", station_id[n], videoPara[n][0], videoPara[n][1], videoPara[n][2], videoPara[n][3]);
               if(sendto(client_fds[n], sendBuf, strlen(sendBuf), 0,
                         (struct sockaddr*)&client_send_addr[n], len) < 0){
                    fprintf(stderr, "Send client %d err", n);
               }
               if(mark){
                    if(sendto(station_fd, sendBuf, strlen(sendBuf), 0,
                              (struct sockaddr*)&station_send_addr,
                              sizeof(station_send_addr)) < 0){
                         fprintf(stderr, "Send station err");
                    }
               }
               break;
          }
     }
     case 'i':                  /* query IBP */
     {
          sprintf(sendBuf, "encoder:ret:channel %05d-kframe %d-bframe %d-framerate %d\n",
                  station_id[n], IBPmode[n][0], IBPmode[n][1], IBPmode[n][2]);
          if(sendto(client_fds[n], sendBuf, strlen(sendBuf), 0,
                    (struct sockaddr*)&client_send_addr[n], len) < 0){
               fprintf(stderr, "Send client %d err", n);
          }
          if(mark){
               if(sendto(station_fd, sendBuf, strlen(sendBuf), 0,
                         (struct sockaddr*)&station_send_addr, sizeof(station_send_addr)) < 0){
                    fprintf(stderr, "Send station err");
               }
          }
          break;
     }
     case 'q':                  /* query quant */
     {
          sprintf(sendBuf, "encoder:ret:channel %05d-iquant %d-pquant %d-bquant %d\n",
                  station_id[n], Quant[n][0], Quant[n][1], Quant[n][2]);
          if(sendto(client_fds[n], sendBuf, strlen(sendBuf), 0,
                    (struct sockaddr*)&client_send_addr[n], len) < 0){
               fprintf(stderr, "Send client %d err", n);
          }
          if(mark){
               if(sendto(station_fd, sendBuf, strlen(sendBuf), 0,
                         (struct sockaddr*)&station_send_addr, sizeof(station_send_addr)) < 0){
                    fprintf(stderr, "Send station err");
               }
          }
          break;
     }
     }
}
/*
 *************************** query information *****************************/

/* agent */
void* op_videoPara(CALLBACK type){
     VideoStandard_t videostandard = StandardNTSC;
	 int i;
     if (type == snmpset){
		for (i=0; i<64; i++){
			if(GetVideoPara(channelHandle[i], &videostandard,
						&videoPara[i][0], &videoPara[i][1],
                        &videoPara[i][2], &videoPara[i][3]) < 0){
				level = error;
				snmptrap_oid(level, new_oid, 2, "get video para error", NULL, 0);
			}
			if (SetVideoPara(channelHandle[i], videoPara[i][0],
                        videoPara[i][1], videoPara[i][2],
                        videoPara[i][3]) < 0){
				level = error;
				snmptrap_oid(level, new_oid, 2, "set video para error", NULL, 0);
			}
		  }
          return;
     }
     if (type == snmpget){
		for (i=0; i<64; i++){
			if (GetVideoPara(channelHandle[i], &videostandard,
						&videoPara[i][0], &videoPara[i][1],
                        &videoPara[i][2], &videoPara[i][3]) < 0){
				level = error;
				snmptrap_oid(level, new_oid, 2, "get video para error", NULL, 0);
			}
		}
     }
}

void* op_IBPmode(CALLBACK type){
		int i;
		if (type == snmpset){
			for (i=0; i<64; i++){
				if (SetIBPMode(channelHandle[i], IBPmode[i][0],
					IBPmode[i][1], 0, IBPmode[i][2]) < 0){
					level = error;
					snmptrap_oid(level, new_oid, 2, "set IBP error", NULL, 0);
			}
		  }
          return;
     }
}

void* op_Quant(CALLBACK type){
	int i;
    if (type == snmpset){
		for (i=0; i<64; i++){
			if (SetDefaultQuant(channelHandle[i], Quant[i][0],
                Quant[i][1], Quant[i][2]) < 0){
				level = error;
				snmptrap_oid(level, new_oid, 2, "set Quant error", NULL, 0);
			}
		}
        return;
    }
}

void* op_Signal(CALLBACK type){
	int i;
    if (type == snmpget){
		for (i=0; i<64; i++){
			if ((SIGNAL[i] = GetVideoSignal(channelHandle[i])) < 0){
				level = error;
				snmptrap_oid(level, new_oid, 2, "get signal error", NULL, 0);
			}
		}
        return;
    }
}

void* op_Osd(CALLBACK type){
	int i;
    if (type == snmpset){
		for (i=0; i<64; i++){
			if (SetOsd(channelHandle[i], OSD[i]) < 0){
				level = error;
				snmptrap_oid(level, new_oid, 2, "set osd error", NULL, 0);
			}
		}
        return;
    }
}
 
void* op_framesStatistics(CALLBACK type){
	int i;
     if (type == snmpget){
		for (i=0; i<64; i++){
			if (GetFramesStatistics(channelHandle[i], &frames_statistics) < 0){
				level = error;
				snmptrap_oid(level, new_oid, 2, "get frames statistics error", NULL, 0);
			}

			VideoFrames[i]   = frames_statistics.VideoFrames;
			AudioFrames[i]   = frames_statistics.AudioFrames;
			FramesLost[i]    = frames_statistics.FramesLost;
			QueueOverflow[i] = frames_statistics.QueueOverflow;
			CurBps[i]        = frames_statistics.CurBps;
		}
        return;
     }
}
