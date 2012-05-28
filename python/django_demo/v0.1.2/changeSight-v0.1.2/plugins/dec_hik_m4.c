#include "common_plugin.h"
#include "misc.h"
#include "playsdkpu.h"
#include <stdio.h>
#include <pthread.h>
#include <errno.h>

pthread_t NetPoller;

unsigned char mp4head[40]={0x34, 0x48, 0x4B ,0x48 ,0xFE, 0xB3, 0xD0, 0xD6, 0x02, 0x03, 0x02, 0x20 , 0x00, 0x00, 0x00, 0x00, 0x03, 0x10, 0x01, 0x10, 0x01, 0x10, 0x10, 0x00, 0x80, 0x3E, 0x00, 0x00, 0x01, 0x10, 0x00, 0x00, 0x11, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

/* Old headers of DS4004H card. */
//  unsigned char mp4head[40]={52, 77, 75, 72, 254, 179, 208,214, 4, 2, 2, 32, 0, 0, 0, 0,3, 16, 1, 16, 1, 16, 16, 0, 64,31, 0, 0, 1, 16, 0, 0, 0, 0, 0,0, 0, 0, 0, 0};

int netfd=-1;
ssize_t (*netread)(int fd, void *buf, size_t count)=NULL;

void * netPoller(void * data)
{
  int size, t;
  char buf[4096];
  
  while(1){
    
    if (netread){
      size = netread(netfd, buf, 4096);
      if (size==0) continue;
      if (size<0){
	perror("netread");

	/* FIXME: network closed? */
	break;
      }
    }else {
      usleep(10000);
      continue;
    }

    if (size<0) break;
    t=Hik_PlayM4_InputData(0, buf, size);
    while (t<0){
      if (Hik_PlayM4_GetLastErrorCode()==M4PErr_BufOverflow) usleep(10000);
      else{
	size=0;
	break;
      }
    }
  }
}

void DecCallBack(int port, char *pBuf, int size, FRAME_INFO * FrameInfo,long reserved1, long reserved2)
{
  static int sz;
  
  if (FrameInfo->frame_rate){
    //Video
    sz=FrameInfo->width*FrameInfo->height;
    
    OnVideo(pBuf, pBuf+sz+sz/4, pBuf+sz, FrameInfo->width, FrameInfo->height, FrameInfo->frame_rate);
  }else if (FrameInfo->width==0){
    //Audio
    OnAudio(pBuf, size, 16, 16000);
  }
}

void FileEndCallBack(int port)
{
  OnFileEnd();
}


int InitDecoder()
{
  
  INITINFO initinfo;

  initinfo.uWidth=352;
  initinfo.uHeight=288;

  return Hik_PlayM4_Init(initinfo, 0);
}

int FreeDecoder ()
{
  return Hik_PlayM4_DeInit();
}

int OpenFile(char * fn, int pos)
{
  PLAYRECT rect;

  rect.x=0;rect.y=0;
  rect.uWidth=352;rect.uHeight=288;  

  CHECK(Hik_PlayM4_OpenFile(0, fn), -2);
  CHECK(Hik_PlayM4_SetDecCallBack(0, DecCallBack), -3); 
  CHECK(Hik_PlayM4_SetFileEndCallBack(0, FileEndCallBack), -4);
  CHECK(Hik_PlayM4_Play(0, rect), -5);

  SeekTo(pos);
  return 0;
}

int SeekTo(int pos)
{
  return Hik_PlayM4_SetPlayPos(0, pos);
}

int TellPos()
{
  return Hik_PlayM4_GetPlayPos(0);
}

int CloseFile()
{
  return Hik_PlayM4_CloseFile(0);
}

int OpenStream(int fd, ssize_t (*reader)(int fd, void *buf, size_t count))
{
  PLAYRECT rect;

  rect.x=0;rect.y=0;
  rect.uWidth=352;rect.uHeight=288;  

  CHECK(Hik_PlayM4_OpenStream(0, mp4head, 40, SOURCE_BUF_MAX), -1);
  CHECK(Hik_PlayM4_SetDecCallBack(0, DecCallBack), -1);
  CHECK(Hik_PlayM4_Play(0, rect), -1);

  netfd=fd;
  netread=reader;

  CHECK(pthread_create(&NetPoller, NULL, netPoller, NULL), -1);
  CHECK(pthread_detach(NetPoller), -2);

  return 0;
}

int CloseStream()
{
  pthread_cancel(NetPoller);

  netfd=-1;
  netread=NULL;
  
  return Hik_PlayM4_CloseStream(0);
}

struct Plugin_Operations hik_plugin={
  .InitDecoder=InitDecoder,
  .FreeDecoder=FreeDecoder,
  .OpenFile=OpenFile,
  .SeekTo=SeekTo,
  .TellPos=TellPos,
  .CloseFile=CloseFile,
  .OpenStream=OpenStream,
  .CloseStream=CloseStream
};

int main(int argc, char ** argv)
{
  Plugin_Main(&hik_plugin, argc, argv);
}
