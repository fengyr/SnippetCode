/*
 * =====================================================================================
 *
 *       Filename:  serial.c
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

#include <string.h>
#include <unistd.h>
#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#include "serial.h"

static int speed_arr[9] = {B4800, B9600, B19200, B38400, B57600, B115200, B230400, B460800, B921600};
static int name_arr[9] = {4800, 9600, 19200, 38400, 57600, 115200, 230400, 460800, 921600};

int serial_open(const char *dev_name)
{
    int fd;

    fd = open(dev_name, O_RDWR | O_NOCTTY);

    if (-1 == fd){
        perror("open dev err");
    }

    return fd;
}

int serial_init(int fd, int baudrate, int databits, int stopbits, int parity)
{
    int i;
    int ret_val;
    char buffer[32];
    struct termios options;

    tcgetattr( fd, &options );
    cfmakeraw( &options );
    switch( baudrate )
    {
        case 50:    cfsetspeed( &options, B50 ); break;
        case 75:    cfsetspeed( &options, B75 ); break;
        case 150:   cfsetspeed( &options, B150 ); break;
        case 300:   cfsetspeed( &options, B300 ); break;
        case 600:   cfsetspeed( &options, B600 ); break;
        case 1200:  cfsetspeed( &options, B1200 ); break;
        case 2400:  cfsetspeed( &options, B2400 ); break;
        case 4800:  cfsetspeed( &options, B4800 ); break;
        case 9600:  cfsetspeed( &options, B9600 ); break;
        case 19200: cfsetspeed( &options, B19200 ); break;
        case 38400: cfsetspeed( &options, B38400 ); break;
        case 115200:cfsetspeed( &options, B115200 ); break;
        default: {
                     perror("baudrate error");
                     return -1;
                 }
    }

    options.c_cflag &= ~CSIZE;
    switch( databits ) {
        case 5:     options.c_cflag |= CS5; break;
        case 6:     options.c_cflag |= CS6; break;
        case 7:     options.c_cflag |= CS7; break;
        case 8:     options.c_cflag |= CS8; break;
        default: {
                     perror("databits error");
                     return -1;
                 }
    }

    options.c_cflag |= PARENB;
    switch( parity )
    {
        case 'o':
        case 'O':   options.c_cflag |= PARODD; break;
        case 'e':
        case 'E':   options.c_cflag &= ~PARODD; break;
        case 'n':
        case 'N':   options.c_cflag &= ~PARENB; break;
        default: {
                     perror("parity error");
                     return -1;
                 }
    }

    switch (stopbits){
        case 1:
            options.c_cflag &= ~CSTOPB;
            break;
        case 2:
            options.c_cflag |= CSTOPB;
            break;
        default: {
                     perror("stopbits err");
                     return -1;
                 }
    }

    tcflush(fd, TCIOFLUSH);
    options.c_cc[VTIME] = 15;
    options.c_cc[VMIN] = 0;

    if( tcsetattr( fd, TCSANOW, &options ) != 0 ) {
        fprintf( stderr, "set attribute failed."); 
        close( fd );
        return -1;
    }

    return fd;
}

int serial_read_data(int fd, unsigned char *buf, int len)
{
    int readsize;
    readsize = read(fd, (void*)buf, len);
    if (readsize < 0){
        perror("read err");
    }
    /* 	int i;
     * 	for (i=0; i<readsize;i++) {
     * 		printf("%x|", buf[i]);
     * 	}
     */

    return readsize;
}

int serial_write_data(int fd, unsigned char *buf, int len)
{
    int writesize;
    writesize = write(fd, (void*)buf, len);
    if (writesize < 0){
        perror("write err");
    }

    return writesize;
}

int serial_close(int fd)
{
    if (fd != -1){
        close(fd);
        return 0;
    }

    return -1;
}
