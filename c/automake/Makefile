#
# Makefile Script for coinds-400x demo fo linux  
#
# writed by wrh at 2002.3.4 
#

CC = gcc
LDFLAGS=-lpthread -L../lib -lds40xxhc -lSDL
CFLAGS=-g

#target: looptime 

#looptime: looptime.c tmsdkpu.h 
#	$(CC) `gtk-config --cflags` looptime.c -o looptime `gtk-config --libs` $(CFLAG) 

TARGET=dm642server

dm642server: agent_api.o readdeploy.o dm642server.o
	$(CC) $(CCFLAG) $(LDFLAGS) readdeploy.o agent_api.o dm642server.o -o dm642server


#myserver-cif: myserver-cif.c ds40xxhcApi.h 

#myserver-d1: myserver-d1.c ds40xxhcApi.h

clean:
	rm -rf dmdu $(TARGET) *.o

deploy: $(TARGET)	
	rm -rf ./dmdu/*
	mkdir -p dmdu/server
	mkdir -p lib
	cp myserver ./dmdu/server
	cp *.so ./lib

	tar -jcvf m4enc.tar.bz2 dmdu lib
	rm -f /var/www/update/install/m4enc.tar.bz2
	mv m4enc.tar.bz2 /var/www/update/install/

	@echo "------------------------------"
	@echo Ok, now edit /var/www/update/m4enc.sh to make it active.

