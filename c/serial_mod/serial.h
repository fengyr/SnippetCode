#ifndef _SERIAL_COM_H_
#define _SERIAL_COM_H_

#include <string.h>
#include "unistd.h"
#include "termios.h"
#include "stdio.h"
#include "stdlib.h"
#include "sys/types.h"
#include "sys/stat.h"
#include "fcntl.h"
#include "errno.h"

typedef struct packet {
	unsigned char 	*data;
	int 			data_size;
}BPACKET, *PBPACKET;

//open device
int OpenDev(char *dev_name);
//
int SetSpeed(int fd, int speed);
//
int SetParity(int fd, int databits, int stopbits, char parity);
//
int ReadData(int fd, unsigned char *buf, int len);
//
int WriteData(int fd, unsigned char *buf, int len);
//
int CloseDev(int fd);

#endif



