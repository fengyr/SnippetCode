//±‡“Î ± π”√£∫gcc -s -shared -fPIC test.c -lSetupSerialPort -o test.so

#include "SetupSerialPort.h"


int main(int argc, char *argv[])
{
    int fd;
    char send_buf[50];
    char recv_buf[512];
    int recv_len = 0;

    if (argc < 2) {
        printf("arg error\n");
        return -1;
    }

    fd = OpenComPort (0, 1200, 8, "1", 'N');
    if (fd < 0) {
        printf("open port error\n");
        return -1;
    }

    memset(send_buf, 0, 50);
    sprintf(send_buf, "%s\r", argv[1]);
    WriteComPort (fd, send_buf, strlen(send_buf));

    memset(recv_buf, 0, 512);
    recv_len = ReadComPort(fd, recv_buf, 512);
    printf("recv: %s\n", recv_buf);

    CloseComPort (fd);

    return 0;
}

