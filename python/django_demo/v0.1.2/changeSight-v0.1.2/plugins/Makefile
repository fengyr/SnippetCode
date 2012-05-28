INCLUDES= -I.  -I/usr/include/mpeg2dec 
#CFLAGS= -fno-PIC -O4 -march=pentium4 -mtune=pentium4 -pipe -ffast-math -mmmx -msse -msse2 -fomit-frame-pointer -D_REENTRANT -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64 $(INCLUDES)
CFLAGS= -g $(INCLUDES)
LDFLAGS= -lm -lpthread
CC=gcc
#PLUGINS=dec_mpeg2 dec_hik_d1  dec_hik_m4 dec_hik_cif
PLUGINS=dec_mpeg2 dec_hik_d1 dec_hik_m4 dec_hik_cif
PLUGDEPS=common_plugin.c misc.c receiver.c 

all: $(PLUGINS) 
	cp $(PLUGINS) ..

dec_hik_m4: dec_hik_m4.c $(PLUGDEPS)
	$(CC) $(CFLAGS) -lSDL -lm4play $(LDFLAGS) $@.c $(PLUGDEPS) -o $@

dec_hik_cif: dec_hik_cif.c $(PLUGDEPS)
	$(CC) $(CFLAGS) -lSDL -lm4play $(LDFLAGS) $@.c $(PLUGDEPS) -o $@

dec_hik_d1: dec_hik_d1.c $(PLUGDEPS)
	$(CC) $(CFLAGS) -lSDL -lm4play $(LDFLAGS) $@.c $(PLUGDEPS) -o $@

dec_mpeg2: dec_mpeg2.c $(PLUGDEPS) 
	$(CC) $(CFLAGS) -lmpeg2 $(LDFLAGS) $@.c $(PLUGDEPS) -o $@

clean:
	rm -rf *.o $(PLUGINS) *~
