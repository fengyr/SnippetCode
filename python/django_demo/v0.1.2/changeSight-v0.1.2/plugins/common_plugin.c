#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <math.h>
#include <sys/poll.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/socket.h>
#include "netdb.h"
#include "netinet/in.h"
#include <arpa/inet.h>


#include "common_plugin.h"
#include "config.h"
#include "receiver.h"
#include "misc.h"

/* #define RESPONSE(module, x)  do{                                        \ */
/*     if ((x)>=0) printf("OK:%s:%d\n", (module),(x));                     \ */
/*     else printf("ERR:%s:%d\n", (module), (x));                          \ */
/*     fflush(stdout);                                                     \ */
/*   }while(0) */


#define RESPONSE(module, x) do{	        \
	if((x) >= 0) ;                  \
	else ;                          \
    }while(0)


pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t session = PTHREAD_MUTEX_INITIALIZER;

int SHMId=-1;
int pid=-1;
void * pScreen;

int ScreenWidth=704,ScreenHeight=576;

char audiobuf[MAX_AUDIO_BUF];
int audiofd=-1;
int audiolen=0;
int audio_depth=0;
int audio_hz=0;
int AudioOnline=0;
int exiting=0;
int OpenMethod=-1;		/* 0->file, 1->stream */
int SpeedAdj=0;
int Background=0;
int ShowBackground=0;
int mypid=0;
int pos = -1;

struct Plugin_Operations* plugin;
struct timeval * myclock=NULL;



int initSHM(char * id, char method, int size)
{
  key_t key;
  int shmid;

  CHECK(key = ftok(id, method), -1);

  if ((shmid = shmget(key, size, IPC_CREAT | IPC_EXCL | 0666)) == -1){
    CHECK(shmid = shmget(key, 0, 0), -1);
  }

  return shmid;
}

void * attachSHM(int key)
{
  void * p;
  if (key>=0){
    CHECK(p=shmat(key, 0, 0), NULL);
  }
  return p;
}

void detachSHM(void *p)
{
  if (p) {
    shmdt(p);
  }  
}



ssize_t myread(int fd, void *buf, size_t count)
{
  int ret, n;
  struct pollfd fds[1];

  fds[0].fd = fd;
  fds[0].events = POLLIN;

  n=poll(fds, 1, 400);
  if (n==1){
    ret=read(fd, buf, count);
  } else {
    ret=0;
    setPNG(1);
  }
  return ret;
}



void setPNG(int a)
{
  Background=a;
  ShowBackground=1;
}

int OnFileEnd()
{
  int ret=-1;


  pthread_mutex_lock(&session);
  printf("FileEnd\n"); fflush(stdout);
  pthread_mutex_unlock(&session);

  if (plugin->CloseFile) 
    ret=plugin->CloseFile();
  return ret;
}

#define BufForImage 6
//指向图像的指针
typedef struct{
    long int now;
    char data[1];
}VideoFrame;

int OnVideo(unsigned char * yLayer, unsigned char * uLayer, unsigned char * vLayer, int width, int height, int fps)
{
    static int FrameNum = 0;
    int size=width*height;
    int i, nfps;
    VideoFrame *pic = (VideoFrame*)pScreen;
   
    i = FrameNum % BufForImage;
    memcpy(pic->data +i*size*3/2, yLayer, size);
    memcpy(pic->data+i*size*3/2+size, uLayer, size/4);
    memcpy(pic->data+i*size*3/2+size+size/4, vLayer, size/4);
   
    pic->now = FrameNum;
    //printf("OnVideo :: the %dth Frame is write to Buf %d\n",  pic->now, i);
    FrameNum ++;
    
    
    if (myclock) {
	nfps=(int)(fps*exp(SpeedAdj*log(1.5)));
	if (OpenMethod==1)		/* Stream based, playback a bit faster to avoid buffer block */
	    if ((plugin->BuffPercentage)&&(plugin->BuffPercentage()>80)){
	nfps+=5;
	    }
	    else return 0;
	if (nfps>0){
	    tick(myclock, nfps);
	}else{
	    keepSleeping();
	}
    }
    
    
    return 0;
}

int OnAudio(unsigned char * audio, int len, int depth, int hz)
{

  int alen;
  
  if (exiting) return 0;
  
  audio_depth=depth;
  audio_hz=hz;
  if (AudioOnline){
    ISNULL(pScreen, -1);
    alen=P16CIF(pScreen)->audio.len;
    P16CIF(pScreen)->audio.depth=depth;
    P16CIF(pScreen)->audio.hz=hz;
    if (alen+len < MAX_AUDIO_BUF){
      memcpy(P16CIF(pScreen)->audio.abuf+alen, audio, len);
      if (P16CIF(pScreen)->audio.len!=alen)
	memcpy(P16CIF(pScreen)->audio.abuf+P16CIF(pScreen)->audio.len, audio, len);
      P16CIF(pScreen)->audio.len+=len;
    }
  }
}



//FileName为****:Pose，Pose的默认值为0
char* ParseFile(char* FullName, int *FilePos)  
{
    char *FileName;

    FileName= strtok(FullName, ":");
    
    char *p=strtok(NULL, "\0"); 
    
    //设定Pos的默认值
    if(p == NULL)
	*(FilePos) = 0;
    else
	*(FilePos) = atoi(p);
    
    return FileName;
}



int Plugin_Main(struct Plugin_Operations* pg, int argc, char **argv)
{
  char cmdline[1024];
  int ret,n;
  int netfd=0;
  struct VideoBlock vscreen; 
  struct pollfd fd_stdin[1];
  struct sockaddr_in addr;

  plugin=pg;
  
  ISNULL(plugin, -1);
  if (argc!=3){
    printf("Usage:\n  ./dec_xxx SHMKey LocalIP\n");
    return -1;
  }

  setLocalIP(argv[2]);
  
  nice(3);

  /* Init */
  setPNG(2);
  

  key_t key=ftok(argv[1], 'S');

  //printf("the file for shmget is %s\n", argv[1]);
  int size = sizeof(long int) + BufForImage*ScreenHeight*ScreenWidth*3/2;
  while((SHMId = shmget(key, 0, 0)) < 0)
  {
      perror(" Plugin_Main: shmget");
      _exit(1);
      
  }
    
  if((pScreen = (char*)shmat(SHMId, 0, 0)) ==(char*) -1){
      perror("Plugin_Main : shmat");
      _exit(1);
  }
  
  /* Init audio fd */
  audiofd=socket(AF_INET, SOCK_DGRAM, 0);
  addr.sin_family = AF_INET;
  addr.sin_port   = htons(3456); /* Audio port */
  addr.sin_addr.s_addr = inet_addr("127.0.0.1");
  connect(audiofd, (struct sockaddr *)&addr, sizeof(addr));
  mypid=getpid();

  if (plugin->InitDecoder) 
    plugin->InitDecoder();
  myclock=getClock();
  
  while(1){
      ret=-1;
      
      fd_stdin[0].fd = 0;
      fd_stdin[0].events = POLLIN;
      n=poll(fd_stdin, 1, 400);
      if (n==1){
	  if (fgets(cmdline, 1024, stdin)==NULL)
	      break;
	  
      }else{
	  
	  fflush(stdout);
	  fsync(0);
	  continue;
      }
      
      //    fprintf(stderr, "recv:%s\n", cmdline);
      
      if (cmdline[strlen(cmdline)-1]=='\n') cmdline[strlen(cmdline)-1]=0;
      
      pthread_mutex_lock(&session);
    
      if (!strncmp(cmdline, "AudioOn", 7)) {
	  if (audio_depth!=0) {
	      AudioOnline=1;
	  } else RESPONSE("AudioOn", -1);
      } else if (!strncmp(cmdline, "AudioOff", 8)) {
	  AudioOnline=0;
      } else if (!strncmp(cmdline, "AdjustSpeed", 11)){
	  SpeedAdj=atoi(cmdline+12);
	  wakeUp();
      } else if (!strncmp(cmdline, "Exit", 4)) {
	  exiting=1;
	  if (OpenMethod==0){
	      //printf("exit\n");
	      if (plugin->CloseFile) plugin->CloseFile();
	      } else if (OpenMethod==1) {
	      if (plugin->CloseStream) plugin->CloseStream();
	      closeNet(netfd);
	  }
	  
	  break;
      } else if (!strncmp(cmdline, "OpenFile", 8)){
	  if (plugin->OpenFile){
	      char FileName[100] = "";
	      int FilePos = 0;
		  
	      strcpy(FileName, ParseFile(cmdline+9,  &FilePos));
	      
	      //printf("the cmd name is %s, which is open file %s at pos %d\n", cmdline, FileName, FilePos);
	      ret=plugin->OpenFile(FileName, FilePos);
	      if (ret>=0) OpenMethod=0;
	  }
	  RESPONSE("OpenFile", ret);
      } else if (!strncmp(cmdline, "CloseFile", 9)) {
	      if (plugin->CloseFile) 
		  ret=plugin->CloseFile();
	      RESPONSE("CloseFile", ret);
      } else if (!strncmp(cmdline, "OpenStream", 10)) {
	  fprintf(stderr,"in open stream:%s\n", cmdline);
	  netfd=ParseNet(cmdline+11);
	  
	  if ((netfd>=0)&&(plugin->OpenStream)){
	      ret=plugin->OpenStream(netfd, myread);
	      if (ret>=0) OpenMethod=1;
	  }
	  RESPONSE("OpenStream", ret);
      } else if (!strncmp(cmdline, "CloseStream", 11)) {
	  if (plugin->CloseStream)
	      ret=plugin->CloseStream();
	  RESPONSE("CloseStream", ret);
      } else if (!strncmp(cmdline, "Seek", 4)) {
	      int percent=atoi(cmdline+5);
	      
	      if (plugin->SeekTo){
		  ret=plugin->SeekTo(percent);
	      }
	      RESPONSE("Seek", ret);
      } else if (!strncmp(cmdline, "Tell", 4)) {
	      if (plugin->TellPos)
		  ret=plugin->TellPos();
	      RESPONSE("Tell", ret);
      }
      
      pthread_mutex_unlock(&session);
  }

  if (plugin->FreeDecoder){
      plugin->FreeDecoder();
      //printf("free\n");
  }
  freeClock(myclock); myclock=NULL;
  
  /* Free */
  detachSHM(pScreen);
  
  RESPONSE("Exited", 0);
  
  return 0;
}
