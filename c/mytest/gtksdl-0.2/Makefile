
EXECNAME	=	main

OBJS		=	gnome_app.o

#DEBUG		=	-g -ggdb

LIB		= `pkg-config --libs opencv` `sdl-config --libs` -L/usr/lib `pkg-config --libs gtk+-2.0 gthread-2.0` -L/usr/lib/i386-linux-gnu

CFLAGS 	=	-c -Wall $(DEBUG) $(OPTIMIZATION) `sdl-config --cflags` `pkg-config --cflags gtk+-2.0 gthread-2.0` `pkg-config --cflags opencv`
LDFLAGS = 	-Wall $(DEBUG)  $(LIB)

CC = g++
RM = rm -f

all: $(OBJS)

	$(CC) $(OBJS) ./libgtksdl.a $(LDFLAGS) -o $(EXECNAME)

gnome_app.o : gnome_app.c gtksdl.h

	$(CC) $< $(CFLAGS)

clean:
	$(RM) $(OBJS) core $(EXECNAME) $(SESSION)

depend:
	echo "No dependences"
