#ifndef RECEIVER_H
#define RECEIVER_H

int ParseNet(char * desp);	/* Desp:"[TCP/UDP/MULTICAST]:x.x.x.x:port", return fd*/
int closeNet(int fd);
int setLocalIP(char * ip);

#endif
