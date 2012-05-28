
CC=gcc

CFLAGS=-Wall -O4 -ffast-math -D__X264__ -DHAVE_MALLOC_H -DHAVE_MMXEXT -DHAVE_SSE2 -DARCH_X86 -DSYS_LINUX -DHAVE_PTHREAD -s -fomit-frame-pointer -lSDL

CFLAGS_x264=-I/root/x264/ -L/root/x264 -lx264 -lm -lpthread

CFLAGS_ffmpeg=-Wdeclaration-after-statement -Wno-switch  -I/root/ffmpeg -I/root/ffmpeg/libavutil -I/root/ffmpeg/libavcodec -I/root/ffmpeg/libavformat -L/root/ffmpeg/libavformat -L/root/ffmpeg/libavcodec -L/root/ffmpeg/libavutil -I/usr/local/include/ -L/usr/local/lib -lavformat -lavcodec -lavutil  -lz -ldl

OBJS=  /root/x264/matroska.o /root/x264/muxers.o

h264_ts: net.c $(OBJS)
	$(CC) $(CFLAGS)   -o h264_ts -g  net.c $(OBJS) $(CFLAGS_x264) $(CFLAGS_ffmpeg)
	cp h264_ts ..

clean:
	rm -rf h264_ts
	
