/*
 * =====================================================================================
 *
 *       Filename:  tcp_slave.c
 *
 *    Description:  
 *
 *        Version:  
 *        Created:  八月 11, 2013
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Zenki (Zenki.J.Zha), zenki2001cn@163.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <assert.h>

#include "app.h"
#include "zlogwrap.h"
#include "tcp_slave.h"
#include "debug.h"

static App *s_app = get_app_instance();

static int set_block_mode(int fd)
{
    long fflag;
    char err[256];
    Logger *logger = s_app->logger;

    if( (fflag = fcntl(fd, F_GETFL, NULL)) < 0) { 
        memset(err, 0, sizeof(err));
        sprintf(err, "Net: Slave Connect Server, F_GETFL, %s.", strerror(errno));
        logger->log_e(logger, err);

        return -1;
    } 

    fflag &= (~O_NONBLOCK); 
    if( fcntl(fd, F_SETFL, fflag) < 0) { 
        memset(err, 0, sizeof(err));
        sprintf(err, "Net: Slave Connect Server, F_SETFL, %s.", strerror(errno));
        logger->log_e(logger, err);

        return -1;
    }

    return 0;
}

static int set_noblock_mode(int fd)
{
    long fflag;
    char err[256];
    Logger *logger = s_app->logger;

    if ((fflag = fcntl(fd, F_GETFL, NULL)) < 0) { 
        memset(err, 0, sizeof(err));
        sprintf(err, "Net: Slave Connect Server, F_GETFL, %s.", strerror(errno));
        logger->log_e(logger, err);

        return -1;
    } 

    fflag |= O_NONBLOCK; 
    if( fcntl(fd, F_SETFL, fflag) < 0) { 
        memset(err, 0, sizeof(err));
        sprintf(err, "Net: Slave Connect Server, F_SETFL, %s.", strerror(errno));
        logger->log_e(logger, err);

        return -1;
    }

    return 0;
}

int slave_tcp_init(TcpSlave *slave, const char *name, const char *server_ip, int server_port)
{
    int res;
    char err[256];
    Logger *logger = s_app->logger;
    fd_set myset; 
    struct timeval tv;
    socklen_t lon;
    int valopt;

    memset(slave->slave_name, 0, sizeof(slave->slave_name));
    strcpy(slave->slave_name, name);
    memset(slave->server_ip, 0, sizeof(slave->server_ip));
    strcpy(slave->server_ip, server_ip);
    slave->server_port = server_port;

    DEBUG("slave_tcp_init: slave_name=%s, server_ip=%s, server_port=%d\n", 
           slave->slave_name, slave->server_ip, slave->server_port);

    slave->slave_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (slave->slave_fd == -1){
        memset(err, 0, sizeof(err));
        sprintf(err, "Net: Slave Init, %s.", strerror(errno));
        logger->log_e(logger, err);
        goto ERROR;
    }

    memset(&slave->serveraddr, 0, sizeof(slave->serveraddr));
    slave->serveraddr.sin_family = AF_INET;
    slave->serveraddr.sin_port = htons(server_port);
    if ((NULL == server_ip) || (strcmp(server_ip, "") == 0)) {
        slave->serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    } else {
        inet_aton(server_ip, &(slave->serveraddr.sin_addr));
    }
    memset(&(slave->serveraddr.sin_zero), 0, sizeof(slave->serveraddr.sin_zero));

    // logger status
    memset(err, 0, sizeof(err));
    sprintf(err, "Net: Slave Connecting Server, %s:%d.", 
            slave->server_ip, slave->server_port);
    logger->log_i(logger, err);

    // Set non-blocking 
    set_noblock_mode(slave->slave_fd);

    // Trying to connect with timeout 
    res = connect(slave->slave_fd, (struct sockaddr*)&slave->serveraddr, sizeof(slave->serveraddr)); 
    if (res < 0) { 
        if (errno == EINPROGRESS) { 
            DEBUG("Net: EINPROGRESS in connect() - selecting\n"); 

            slave->status = ENUM_TCP_CONNECTING;
            do { 
                tv.tv_sec = 3; 
                tv.tv_usec = 0; 
                FD_ZERO(&myset); 
                FD_SET(slave->slave_fd, &myset); 
                res = select(slave->slave_fd + 1, NULL, &myset, NULL, &tv); 
                if (res < 0 && errno != EINTR) { 
                    memset(err, 0, sizeof(err));
                    sprintf(err, "Net: Slave Connect Server, select, %s.", strerror(errno));
                    logger->log_e(logger, err);
                } else if (res > 0) { 
                    // Socket selected for write 
                    lon = sizeof(int); 
                    if (getsockopt(slave->slave_fd, SOL_SOCKET, SO_ERROR, (void*)(&valopt), &lon) < 0) { 
                        memset(err, 0, sizeof(err));
                        sprintf(err, "Net: Slave Connect Server, getsockopt, %s.", strerror(errno));
                        logger->log_e(logger, err);
                    } 
                    // Check the value returned... 
                    if (valopt) { 
                        res = -1;
                        memset(err, 0, sizeof(err));
                        sprintf(err, "Net: Slave Connect Server, delayed connection, %s.", strerror(errno));
                        logger->log_e(logger, err);
                    } else {
                        slave->status = ENUM_TCP_CONNECTED;
                    }
                    break; 
                } else { 
                    memset(err, 0, sizeof(err));
                    sprintf(err, "Net: Slave Connect Server, select, %s.", strerror(errno));
                    logger->log_e(logger, err);
                } 
            } while (1); 
        } else { 
            slave->status = ENUM_TCP_ERROR;

            memset(err, 0, sizeof(err));
            sprintf(err, "Net: Slave Connect Server, %s.", strerror(errno));
            logger->log_e(logger, err);
        } 
    } else {
        slave->status = ENUM_TCP_CONNECTED;

        memset(err, 0, sizeof(err));
        sprintf(err, "Net: Slave Connect Server Success.");
        logger->log_i(logger, err);
    } 

    // Set to blocking mode again... 
    set_block_mode(slave->slave_fd);

/*     [> ioctl(slave->slave_fd, FIONBIO, &ul); //设置为非阻塞模式 <]
       unsigned long ul = 1;
 *     fcntl(slave->slave_fd, F_SETFL, O_NONBLOCK);
 *     res = connect(slave->slave_fd, (struct sockaddr*)&slave->serveraddr, sizeof(slave->serveraddr));
 * 
 *     memset(err, 0, sizeof(err));
 *     if (res == -1) {
 *         sprintf(err, "Net: Slave Connect Server, slave_name=%s, server_ip=%s, server_port=%d, %s.", 
 *                 name, server_ip, server_port, strerror(errno));
 *         logger->log_e(logger, err);
 *     } else {
 *         sprintf(err, "Net: Slave Connect Server Success.");
 *         logger->log_i(logger, err);
 *     } */
    
    return res;

ERROR:
    slave_tcp_close(slave);
    return -1;
}

int slave_tcp_connect(TcpSlave *slave)
{
    char slave_name[256];
    char server_ip[64];
    memset(slave_name, 0, 256);
    memset(server_ip, 0, 64);
    strcpy(slave_name, slave->slave_name);
    strcpy(server_ip, slave->server_ip);
    int server_port = slave->server_port;

    DEBUG("slave_tcp_connect: slave_name=%s, server_ip=%s, server_port=%d\n", 
           slave_name, server_ip, server_port);

    return slave_tcp_init(slave, slave_name, server_ip, server_port);
}

int slave_tcp_send(TcpSlave *slave, void *data, int size)
{
    int ssize = -1;
    char err[256];
    Logger *logger = s_app->logger;

    if (!slave) {
        memset(err, 0, sizeof(err));
        sprintf(err, "Net: Slave Send Data, Slave Is NULL.");
        logger->log_e(logger, err);
        return -1;
    }

    if (slave->slave_fd > 0) {
        ssize = send(slave->slave_fd, data, size, 0);

        if (ssize <= 0) {
            memset(err, 0, sizeof(err));
            sprintf(err, "Net: Slave Send Data, %s.", strerror(errno));
            logger->log_e(logger, err);

            slave_tcp_close(slave);

            char slave_name[256];
            char server_ip[64];
            memset(slave_name, 0, 256);
            memset(server_ip, 0, 64);
            strcpy(slave_name, slave->slave_name);
            strcpy(server_ip, slave->server_ip);
            int server_port = slave->server_port;
            int res = slave_tcp_init(slave, slave_name, server_ip, server_port);

            memset(err, 0, sizeof(err));
            if (res < 0) {
                sprintf(err, "Net: Slave Reconnect, %s.", strerror(errno));
                logger->log_e(logger, err);
            } else {
                sprintf(err, "Net: Slave Resend Data.");
                logger->log_e(logger, err);

                /* sleep(1); */
                ssize = send(slave->slave_fd, data, size, 0);
                if (ssize < 0) {
                    memset(err, 0, sizeof(err));
                    sprintf(err, "Net: Slave Resend Data Error, %s.", strerror(errno));
                    logger->log_e(logger, err);
                }
            }
        }
    } else {
        memset(err, 0, sizeof(err));
        sprintf(err, "Net: Slave Send Data, Server Disconnected.");
        logger->log_e(logger, err);
    }

    return ssize;
}

int slave_tcp_close(TcpSlave *slave)
{
    char err[256];
    Logger *logger = s_app->logger;

    if (!slave) {
        memset(err, 0, sizeof(err));
        sprintf(err, "Net: Slave Close, Slave Is NULL.");
        logger->log_e(logger, err);
        return -1;
    }

    if (slave->slave_fd > 0) {
        close(slave->slave_fd);
        slave->slave_fd = -1;
        slave->status = ENUM_TCP_CLOSED;

        memset(err, 0, sizeof(err));
        sprintf(err, "Net: Slave %s Closed.", slave->slave_name);
        logger->log_i(logger, err);
    }

    return 0;
}

int slave_tcp_disconnect(TcpSlave *slave)
{
    char err[256];
    Logger *logger = s_app->logger;

    if (!slave) {
        memset(err, 0, sizeof(err));
        sprintf(err, "Net: Slave Disconnect, Slave Is NULL.");
        logger->log_e(logger, err);
        return -1;
    }

    if (slave->slave_fd > 0) {
        close(slave->slave_fd);
        slave->slave_fd = -1;
        slave->status = ENUM_TCP_DISCONNECTED;

        memset(err, 0, sizeof(err));
        sprintf(err, "Net: Slave %s Disconnecting...", slave->slave_name);
        logger->log_i(logger, err);
    }

    return 0;
}
