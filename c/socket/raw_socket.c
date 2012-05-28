/**       © Copyright 2010 Zenki All Rights Reserved.
 *
 *       @file   sample.h
 *       @version 1.1
 *       @author zenki2001cn
 *       @date  日期
 *
 *       @brief askljdlajlsjdlfjal.
 *       ajslkdjlkasjf.
 *       ajsldjlaf.
 * =====================================================================================
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
/*#include <linux/in.h>*/
#include <linux/if_ether.h>

#undef PDEBUG
#ifdef USE_DEBUG
    #ifdef __KERNEL__
        #define PDEBUG(fmt, args...) printk( KERN_DEBUG "kernel: " fmt, ## args)
    #else
        #define PDEBUG(fmt, args...) fprintf(stderr, fmt, ## args)
    #endif
#else
    #define PDEBUG(fmt, args...)
#endif


/**
* @brief  初始化原始套接字
* @param  interface 接口名称
* @param  num 数量
* @return socket文件描述符
*/
int raw_socket_init(char *interface, int num)
{
    int fd;

    /*fd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_IP));*/
    fd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    
    if (-1 == fd) {
        PDEBUG("%s\n", "socket init error");
        return -1;
    }
    
    return fd;
}

int main(int argc, const char *argv[])
{
    int fd;
    char buf[1024];
    ssize_t rsize;


    fd = raw_socket_init("eth0");

    PDEBUG("start\n");
    
    while (1) {
        memset(buf, 0, sizeof(buf));
        rsize = recvfrom(fd, buf, 0, 0, 0, 0);
        PDEBUG("%d, %s\n", rsize, buf);
        sleep(10);
    }

    return 0;
}

/**
 * @brief  
 * @param  param desc
 * @return 
 */
void function_name();

