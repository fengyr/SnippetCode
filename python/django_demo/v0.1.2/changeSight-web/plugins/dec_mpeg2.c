#include "common_plugin.h"
#include "misc.h"
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <pthread.h>
#include <mpeg2.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>


#define BUFFER_SIZE 4096

mpeg2dec_t * decoder;
const mpeg2_info_t * info;
FILE *f=NULL;
pthread_t FileReader;
long int FileLen=0;
int netfd=-1;
ssize_t (*netread)(int fd, void *buf, size_t count)=NULL;

void * fileReader(void * data)
{
  mpeg2_state_t state;
  const mpeg2_sequence_t * sequence;
  size_t size, total=0, skip=0;
  uint8_t buffer[BUFFER_SIZE];

  size=(size_t)-1;
  do {    
    state = mpeg2_parse (decoder);
    sequence = info->sequence;
    switch (state) {
    case STATE_BUFFER:
      if(f){			/* File based mode */
	size = fread (buffer, 1, BUFFER_SIZE, f);
	skip = 0;
      } else if (netfd>0){	/* Net based mode */
	size = netread(netfd, buffer, BUFFER_SIZE);
	if (size<0){
	  perror("netread");
	  /* FIXME: network closed? */
	  setPNG(1);
	  return NULL;
	}
	if (size==0) continue;
	
	/* FIXME: need to do package sequence checking */
	skip = 0;
      } else break;
      mpeg2_buffer (decoder, buffer + skip, buffer + size);
      break;
    case STATE_SLICE:
    case STATE_END:
    case STATE_INVALID_END:
      
      if (info->display_fbuf){
	OnVideo((unsigned char*)(info->display_fbuf->buf[0]), 
		(unsigned char*)(info->display_fbuf->buf[1]),
		(unsigned char*)(info->display_fbuf->buf[2]),
		sequence->width, sequence->height, 25);
      }
      
      break;
    default:
      break;
    }
  } while (size);

  if (f){			/* File mode */
    OnFileEnd();
  }  
}

int InitDecoder()
{
  ISNULL(decoder=mpeg2_init(), -1);
  ISNULL(info = mpeg2_info (decoder), -1);

  return 0;
}

int FreeDecoder ()
{
  ISNULL(decoder, -1);
  mpeg2_close(decoder);
  return 0;
}

int OpenFile(char * fn, int pos)
{
  ISNULL(f=fopen(fn, "rb"),-1);
  SeekTo(pos);
  CHECK(pthread_create(&FileReader, NULL, fileReader, NULL), -1);
  CHECK(pthread_detach(FileReader), -2);

  return 0;
}

int UpdateFileLen()
{
  struct stat fs;
  
  ISNULL(f, -1);
  CHECK(fstat(fileno(f), &fs), -1);
  FileLen=fs.st_size;
  return 0;
}

int SeekTo(int pos)
{
  CHECK(UpdateFileLen(), -1);
  
  long Newpos = pos*(FileLen/100);
  
  fseek(f, Newpos, 0);

/* FIXME: End current frame and find new frame */
}

int TellPos()
{
  CHECK(UpdateFileLen(), -1);
  return ftell(f)*100/FileLen;
}

int CloseFile()
{
  ISNULL(f, 0);
  fclose(f);
  f=NULL;
  pthread_cancel(FileReader);
  
  return 0;  
}

int OpenStream(int fd, ssize_t (*reader)(int fd, void *buf, size_t count))
{
  netfd=fd;
  netread=reader;
  
  CHECK(pthread_create(&FileReader, NULL, fileReader, NULL), -1);
  CHECK(pthread_detach(FileReader), -2);
  return 0;
}

int CloseStream()
{
  netfd=-1;

  pthread_cancel(FileReader);

  return 0;  
}


int main(int argc, char ** argv)
{

    struct Plugin_Operations mpeg2_plugin;

    mpeg2_plugin.InitDecoder = InitDecoder;
    mpeg2_plugin.FreeDecoder = FreeDecoder;
    mpeg2_plugin.OpenStream = OpenStream;
    mpeg2_plugin.OpenFile = OpenFile;
    mpeg2_plugin.TellPos = TellPos;
    mpeg2_plugin.SeekTo = SeekTo;
    mpeg2_plugin.CloseFile = CloseFile;
    mpeg2_plugin.CloseStream = CloseStream;
    
    Plugin_Main(&mpeg2_plugin, argc, argv);
}

