#ifndef COMMON_PLUGIN_H
#define COMMON_PLUGIN_H

#include <unistd.h>

struct Plugin_Operations{
    int (*InitDecoder) ();
    int (*FreeDecoder) ();
    int (*OpenFile)    (char * fn, int);
    int (*SeekTo)      (int pos);
    int (*TellPos)     ();
    int (*CloseFile)   ();
    int (*OpenStream)  (int fd, ssize_t (*reader)(int fd, void *buf, size_t count));
    int (*CloseStream) ();
    int (*BuffPercentage) ();
};

int OnVideo(unsigned char * yLayer, unsigned char * uLayer, unsigned char * vLayer, int width, int height, int fps);
int OnAudio(unsigned char * audio, int len, int depth, int hz);
int OnFileEnd();
int Plugin_Main(struct Plugin_Operations*, int, char ** );
void setPNG(int);

#endif
