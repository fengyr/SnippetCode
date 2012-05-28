#ifndef CONFIG_H
#define CONFIG_H

#include <linux/soundcard.h>

#define SHMLEN_CIF    20
#define SHMLEN_D1      6
#define CIF_WIDTH     352
#define CIF_HEIGHT    288
#define D1_WIDTH      (CIF_WIDTH*2)
#define D1_HEIGHT     (CIF_HEIGHT*2)

#define MAX_AUDIO_BUF  65520
#define PICDATALEN    1572864	/* 1.5MB */
/* m4dec parm: */
#define AUDIOSIZE         1280
#define AUDIOBANKS    20
#define BUFSIZE     SOURCE_BUF_MAX
#define SHMBUFLEN_MPEG4   20
#define SHMBUFLEN_MPEG2   6
#define NOT_DECODE_BFRAME 1   //0-2

struct VideoBlock{
  int width,        // -> width of the block
    height,         // -> height of the block
    type;            // -> [...|ISD1|HASALPHA|]
  unsigned char aLayer[D1_WIDTH*D1_HEIGHT];
  unsigned char * yLayer;
  unsigned char * uLayer;
  unsigned char * vLayer;
};

struct AVFrame{
  int width,        // -> width of each block
    height,        // -> height of each block
    type,           // -> [...KEYFRAME|NEWDATA|ISD1|HASALPHA|]
    now,last,
    audionow;
  unsigned char audio[AUDIOBANKS * AUDIOSIZE];
  struct VideoBlock block;
  unsigned char  video[1];
};

struct A16CIFAVFrame
{
  int width, height;		/* Default value is 16CIF, but may be smaller */
  unsigned char Layer[CIF_WIDTH*CIF_HEIGHT*16+CIF_WIDTH*CIF_HEIGHT*16/2];
  struct {
    int len, depth, hz;
    unsigned char abuf[MAX_AUDIO_BUF];
  }audio;
  char picdata[PICDATALEN];
};

/* Every pic is YV12 formated */
struct PIC			/* Parse the A16CIFAVFrame->picdata field */
{
  short total;			/* Total pics, 20 max */
  struct _apic
  {
    short width, height;	/* pic width, height*/
    int offset;			/* Offset of the pic */
  }index[20];			/* 20 pics max */
  char data[PICDATALEN-(sizeof(struct _apic)+2)]; /* pics are stored here */
};

static int SHMBUFSIZE = sizeof(struct A16CIFAVFrame);

#define P16CIF(p)  ((struct A16CIFAVFrame *)(p))
#define P16CIF_PIC(p) ((struct PIC*)(P16CIF(p)->picdata))


#endif
