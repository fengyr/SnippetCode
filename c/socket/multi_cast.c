/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  发送和接收组播数据的示例程序
 *
 *        Version:  1.0
 *        Created:  
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Zenki
 *        Company:  
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>

/* 设置本地IP地址，必须符合当前的IP设定。可设定为127.0.0.1 */
#define     RECV_LOCAL_IP   "192.168.1.101"
#define     RECV_MULTI_IP   "224.3.2.1"
#define     RECV_PORT   9001

/* 设置本地IP地址，必须符合当前的IP设定。可设定为127.0.0.1 */
#define     SEND_LOCAL_IP   "192.168.1.101"
#define     SEND_MULTI_IP   "224.3.2.1"
#define     SEND_PORT   9001

struct sockaddr_in local_addr_in, remote_addr_in;


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  init_multi_server
 *  Description:  初始化组播的服务器端
 * =====================================================================================
 */
int init_multi_server(char *interface_ip, char *multi_ip, int multi_port)
{
    int fd;
    struct ip_mreq mreq;
    int loop = 1;
    int ttl = 15;

    memset(&local_addr_in, 0, sizeof(local_addr_in));
    memset(&mreq, 0, sizeof(mreq));

    /*设置本地的sockaddr_in结构，用于绑定接收地址和端口*/
    local_addr_in.sin_family = AF_INET;
    local_addr_in.sin_port = htons(multi_port);
    inet_aton(multi_ip, &local_addr_in.sin_addr);

    /*设置ip_mreq结构，目的是指定接收数据的组播ip地址和接收该组播的网络设备接口*/
    inet_aton(multi_ip, &mreq.imr_multiaddr.s_addr);
    inet_aton(interface_ip, &mreq.imr_interface.s_addr);
    /*mreq.imr_interface.s_addr = htonl(INADDR_ANY);*/
    if (mreq.imr_multiaddr.s_addr == -1) {
        printf("multiaddr error\n");
        return -1;
    }

    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if ( fd < 0 ) {
        perror("socket err");
        return -1;
    }
    if ( bind(fd, (struct sockaddr*)&local_addr_in, sizeof(local_addr_in)) < 0 ) {
        perror("bind err");
        return -1;
    }
    /*设置TTL生存期*/
    if ( setsockopt(fd, IPPROTO_IP, IP_MULTICAST_TTL, &ttl, sizeof(ttl)) < 0 ) {
        perror("set ttl err");
        return -1;
    }
    /*设置是否接收回环数据*/
    if ( setsockopt(fd, IPPROTO_IP, IP_MULTICAST_LOOP, &loop, sizeof(loop)) < 0 ) {
        perror("set loop err");
        return -1;
    }
    /*加入组播组*/
    if ( setsockopt(fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) < 0 ) {
        perror("add membership err");
        return -1;
    }
    /*退出组播组*/
    /*setsockopt(fd, IPPROTO_IP, IP_DROP_MEMBERSHIP, &mreq, sizeof(mreq));*/

    return fd;
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  init_multi_client
 *  Description:  初始化组播的客户端
 * =====================================================================================
 */
int init_multi_client(char *interface_ip, char *multi_ip, int multi_port)
{
    int fd;
    struct in_addr if_req;
    int loop = 1;
    int ttl = 15;

    memset(&remote_addr_in, 0, sizeof(remote_addr_in));

    remote_addr_in.sin_family = AF_INET;
    remote_addr_in.sin_port = htons(multi_port);
    inet_aton(multi_ip, &remote_addr_in.sin_addr);

    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if ( fd < 0 ) {
        perror("socket err");
        return -1;
    }
    /*指定发送数据的接口地址*/
    inet_aton(interface_ip, &if_req);
    if ( setsockopt(fd, IPPROTO_IP, IP_MULTICAST_IF, &if_req, sizeof(if_req)) < 0 ) {
        perror("set if err");
        return -1;
    }
    /*设置TTL生存期*/
    if ( setsockopt(fd, IPPROTO_IP, IP_MULTICAST_TTL, &ttl, sizeof(ttl)) < 0 ) {
        perror("set ttl err");
        return -1;
    }
    /*设置是否接收回环数据*/
    if ( setsockopt(fd, IPPROTO_IP, IP_MULTICAST_LOOP, &loop, sizeof(loop)) < 0 ) {
        perror("set loop err");
        return -1;
    }

    return fd;
}

int main(int argc, char *argv[])
{
    int net_fd;
    char buf[1024];
    ssize_t size;
    int len;
    int server;
    
    if (argc < 2) {
        printf("multi_cast [1 | 0]\n");
        printf("1 --> server\n");
        printf("0 --> client\n");
        exit(1);
    }
    else {
        server = atoi(argv[1]);
    }

    if ( server ) {
        net_fd = init_multi_server(RECV_LOCAL_IP, RECV_MULTI_IP, RECV_PORT);
    }
    else {
        net_fd = init_multi_client(SEND_LOCAL_IP, SEND_MULTI_IP, SEND_PORT);

    }

    for (;;) {
        if ( server ) {
            len = sizeof(local_addr_in);
            /*size = recvfrom(net_fd, buf, sizeof(buf), 0, (struct sockaddr*)&local_addr_in, &len);*/
            size = recv(net_fd, buf, sizeof(buf), 0);
            printf("recv buf: %s, size: %d\n", buf, size);
        }
        else {
            size = sendto(net_fd, "hello", sizeof("hello"), 0, (struct sockaddr*)&remote_addr_in, sizeof(remote_addr_in));
            printf("send buf: %s, size: %d\n", "hello", size);
        }

        sleep(1);
    }

    return 0;
}


