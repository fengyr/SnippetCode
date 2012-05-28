#include "serial.h"
/*-----------------------------------------------------------------------------
 *  baudrate define
 *-----------------------------------------------------------------------------*/
int speed_arr[9] = {B4800, B9600, B19200, B38400, B57600, B115200, B230400, B460800, B921600};

int name_arr[9] = {4800, 9600, 19200, 38400, 57600, 115200, 230400, 460800, 921600};

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  OpenDev
 *  Description:  Open serial tty
 * =====================================================================================
 */
int OpenDev(char *dev_name){
	int fd;
	fd = open(dev_name, O_RDWR | O_NOCTTY);
	if (-1 == fd){
		//
		perror("open dev err");
	}

	return fd;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  SetSpeed
 *  Description:  Set serial speed
 * =====================================================================================
 */
int SetSpeed(int fd, int speed){
	if (-1 != fd){
		int status = -1;
		struct termios opt;

		tcgetattr(fd, &opt);
		int i;
		for (i=0; i<sizeof(speed_arr)/sizeof(int); i++){
			if (speed == name_arr[i]){
				tcflush(fd, TCIOFLUSH);
				cfsetispeed(&opt, speed_arr[i]);
				cfsetospeed(&opt, speed_arr[i]);
				status = tcsetattr(fd, TCSANOW, &opt);
				if (status != 0){
					//
					perror("set speed err");
					return status;
				}
				tcflush(fd, TCIOFLUSH);
			}
		}
		return status;
	}
	else{
		return -1;
	}
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  SetParity
 *  Description:  Set serial parity
 * =====================================================================================
 */
int SetParity(int fd, int databits, int stopbits, char parity){
	struct termios opt;
	if (tcgetattr(fd, &opt) != 0){
		//
		perror("tcgetattr err");
		return -1;
	}
	cfmakeraw(&opt);
	opt.c_cflag &= ~CSIZE;
	switch (databits){
		case 7:
			opt.c_cflag |= CS7;
			break;
		case 8:
			opt.c_cflag |= CS8;
			break;
		default:
			perror("databits err");
			return -1;
	}
	switch (stopbits){
		case 1:
			opt.c_cflag &= ~CSTOPB;
			break;
		case 2:
			opt.c_cflag |= CSTOPB;
			break;
		default:
			perror("stopbits err");
			return -1;
	}
	switch (parity){
		case 'n':
		case 'N':
			opt.c_cflag &= ~PARENB;
			opt.c_iflag &= ~INPCK;
			break;
		case 'o':
		case 'O':
			opt.c_cflag |= (PARODD | PARENB);
			opt.c_iflag |= INPCK;
			break;
		case 'e':
		case 'E':
			opt.c_cflag |= PARENB;
			opt.c_cflag &= ~PARODD;
			opt.c_iflag |= INPCK;
			break;
		case 's':
		case 'S':
			opt.c_cflag &= ~PARENB;
			opt.c_cflag &= ~CSTOPB;
			break;
		default:
			perror("parity err");
			return -1;
	}

	if ((parity != 'n') || (parity != 'N'))
		opt.c_iflag |= INPCK;
	tcflush(fd, TCIFLUSH);
	opt.c_cc[VTIME] = 50;
	opt.c_cc[VMIN] = 0;
	if (tcsetattr(fd, TCSANOW, &opt) != 0){
		perror("tcsetattr err");
		return -1;
	}
	return 0;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  ReadData
 *  Description:  
 * =====================================================================================
 */
int ReadData(int fd, unsigned char *buf, int len){
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

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  WriteData
 *  Description:  
 * =====================================================================================
 */
int WriteData(int fd, unsigned char *buf, int len){
	int writesize;
	writesize = write(fd, (void*)buf, len);
	if (writesize < 0){
		perror("write err");
	}
	return writesize;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  Close
 *  Description:  Close serial
 * =====================================================================================
 */
int CloseDev(int fd){
	if (fd != -1){
		close(fd);
		return 0;
	}
	return -1;
}
