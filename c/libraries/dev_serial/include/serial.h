/*
 * =====================================================================================
 *
 *       Filename:  serial.h
 *
 *    Description:  
 *
 *        Version:  
 *        Created:  九月 04, 2014
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Zenki (Zenki.J.Zha), zenki2001cn@163.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef _SERIAL_COM_H_
#define _SERIAL_COM_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef struct packet {
	unsigned char 	*data;
	int 			len;
} BPacket, *PBPacket;

int serial_open(const char *dev_name);

int serial_init(int fd, int baudrate, int databits, int stopbits, int parity);

int serial_read_data(int fd, unsigned char *buf, int len);

int serial_write_data(int fd, unsigned char *buf, int len);

int serial_close(int fd);

#ifdef __cplusplus
}
#endif

#endif
