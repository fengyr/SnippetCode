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
#include <signal.h>

#include "appe_app.h"
#include "zlogwrap.h"
#include "appe_tcp_slave.h"
#include "appe_debug.h"

static int SERVER_QUIT = 0;

static int set_block_mode(int fd)
{
    long fflag;
    char err[256];
    App *s_app = appe_get_app_instance();
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
    App *s_app = appe_get_app_instance();
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

static int _recv(AppeTcpSlave *slave)
{
    AppeRecvHandler *handler = slave->pHandlers;
    ssize_t total_size = 0;
    char *buffer = NULL;
    int rc = 0;

    if (handler != NULL) {
        AppeRecvHandlerCall call = handler->onRecvAndReplay;
        int req_size = handler->req_size;
        int recv_size = 0;

        // 请求的最大数据<=0，则立即返回。
        if (req_size <= 0) {
            buffer = (char*) malloc(SLAVE_DYN_BUF_SIZE*sizeof(char));
            if (!buffer) {
                if (call != NULL) {
                    (*call)(slave, NULL, -1);
                }
                return -1;
            }

            memset(buffer, 0, SLAVE_DYN_BUF_SIZE);
            total_size = recv(slave->slave_fd, buffer, SLAVE_DYN_BUF_SIZE, 0);
        } else {
            buffer = (char*) malloc(req_size*sizeof(char));
            if (!buffer) {
                if (call != NULL) {
                    (*call)(slave, NULL, -1);
                }
                return -1;
            }

            memset(buffer, 0, req_size);
            while (total_size != req_size) {
                recv_size = recv(slave->slave_fd, 
                                 &buffer[total_size], 
                                 req_size - total_size, 0);
                if (recv_size <=0) {
                    total_size = -1;
                    break;
                }
                total_size += recv_size;
            }
        }

        if (total_size <= 0) {
            slave->status = ENUM_TCP_DISCONNECTED;
            SERVER_QUIT = 1;
            // 如果服务端断开，则返回空的缓冲区
            if (call != NULL) {
                (*call)(slave, NULL, 0);
            }
            goto BUFFER_FREE;
        }

        if (call != NULL) {
            rc = (*call)(slave, (void*)buffer, total_size);
        }

        if (rc < 0) {
            DEBUG("thread_slave_recv: handler failed.\n");
        } else {
            DEBUG("thread_slave_recv: handler...\n");
        }

BUFFER_FREE:
        if (buffer != NULL) {
            free(buffer);
            buffer = NULL;
        }
    } else {
        DEBUG("%s: not found AppeRecvHandler\n", slave->slave_name);
    }

    return rc;
}

static void* thread_slave_recv(void *param)
{
    pthread_detach(pthread_self());
    AppeTcpSlave *slave = (AppeTcpSlave*) param;

    int rc;
    int max_fd;
    fd_set read_fds;

    SERVER_QUIT = 0;

    // set timeout
    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 500000;

    while (!SERVER_QUIT) {
        /* begin set fd_set */
        FD_ZERO(&read_fds);
        max_fd = -1;

        FD_SET(slave->slave_fd, &read_fds);
        max_fd = slave->slave_fd;
        /* end set fd_set */

        DEBUG("%s: enter select, max_fd = %d\n", slave->slave_name, max_fd);

        rc = select(max_fd + 1, &read_fds, 0, 0, &timeout);  
        if (rc < 0) {
            perror("thread_slave_recv: select error");
            sleep(1);
            continue;
        } else if (rc == 0) {
            DEBUG("%s: select continue\n", slave->slave_name);
            usleep(5000);
            continue;
        }

        if (FD_ISSET(slave->slave_fd, &read_fds)) {
            DEBUG("%s: enter recv handler... status = %d\n", slave->slave_name, slave->status);
            _recv(slave);
        }

        // 休眠10ms
        usleep(5000);
    }

    DEBUG("%s: enter recv thread exit... status = %d\n", slave->slave_name, slave->status);
    slave->pthread = -1;
    pthread_exit(NULL);
}

int appe_slave_register_handler(AppeTcpSlave *slave, AppeRecvHandler *handler)
{
    if (!handler) {
        return -1;
    }

    // Init AppeRecvHandler
    if (!slave->pHandlers) {
        slave->pHandlers = (AppeRecvHandler*) malloc(sizeof(AppeRecvHandler));
    }
    memcpy(slave->pHandlers, handler, sizeof(AppeRecvHandler));
    DEBUG("slave_register_handler: slave name = %s, ip = %s, port = %d, handler = %p\n", 
            slave->slave_name, slave->server_ip, slave->server_port, slave->pHandlers);

    return 0;
}

int appe_slave_tcp_init(AppeTcpSlave *slave, 
                    const char *name, 
                    const char *server_ip, 
                    int server_port, 
                    int reconnect,
                    int auto_connect)
{
    int res, select_res;
    char err[256];
    App *s_app = appe_get_app_instance();
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
    slave->auto_connect = auto_connect;

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

    if (!reconnect) {
        // init thread
        slave->pthread = -1;
    }

    if (slave->pthread != -1) {
        SERVER_QUIT = 1;
        sleep(1.5);

        void *join;
        pthread_join(slave->pthread, &join);
        slave->pthread = -1;
    }
    
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
                select_res = select(slave->slave_fd + 1, NULL, &myset, NULL, &tv); 
                if (select_res < 0 && errno != EINTR) { 
                    memset(err, 0, sizeof(err));
                    sprintf(err, "Net: Slave Connect Server, select, %s.", strerror(errno));
                    logger->log_e(logger, err);
                } else if (select_res > 0) { 
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
                        res = 0;
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

        // 设置超时，默认1秒
        struct timeval timeout_r; 
        if (slave->pHandlers != NULL) {
            if (slave->pHandlers->recv_timeout > 0) {
                timeout_r.tv_sec = slave->pHandlers->recv_timeout;
            } else {
                timeout_r.tv_sec = 1;
            }
        } else {
            timeout_r.tv_sec = 1;
        }
        timeout_r.tv_usec = 0;
        setsockopt(slave->slave_fd, SOL_SOCKET, SO_RCVTIMEO, (const void*)&timeout_r, sizeof(timeout_r));

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

    if (slave->status == ENUM_TCP_CONNECTED) {
        int thread_res;
        thread_res = pthread_create(&slave->pthread, NULL, thread_slave_recv, (void*)slave);
        if (thread_res < 0) {
            perror("slave_register_handler: pthread_create error");
            exit(1);
        }
    }
    
    return res;

ERROR:
    appe_slave_tcp_close(slave, SLAVE_STATUS_CONNECT_FIRST);
    return -1;
}

int appe_slave_tcp_connect(AppeTcpSlave *slave, int auto_connect)
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

    return appe_slave_tcp_init(slave, slave_name, server_ip, server_port, 
                          SLAVE_STATUS_CONNECT_FIRST, auto_connect);
}

int appe_slave_tcp_recv(AppeTcpSlave *slave, void *data, int size)
{
    int ssize = -1;
    char err[256];
    App *s_app = appe_get_app_instance();
    Logger *logger = s_app->logger;

    if (!slave) {
        memset(err, 0, sizeof(err));
        sprintf(err, "Net: Slave Recv Data, Slave Is NULL.");
        logger->log_e(logger, err);
        return -1;
    }

    if (slave->slave_fd > 0) {
        ssize = recv(slave->slave_fd, data, size, 0);

        if (ssize <= 0) {
            slave->status = ENUM_TCP_DISCONNECTED;
            memset(err, 0, sizeof(err));
            sprintf(err, "Net: Slave Recv Data, %s.", strerror(errno));
            logger->log_e(logger, err);
        }
    } else {
        memset(err, 0, sizeof(err));
        sprintf(err, "Net: Slave Recv Data, Server Disconnected.");
        logger->log_e(logger, err);
    }

    return ssize;
}

int appe_slave_tcp_send(AppeTcpSlave *slave, void *data, int size)
{
    int ssize = -1;
    char err[256];
    App *s_app = appe_get_app_instance();
    Logger *logger = s_app->logger;

    if (!slave) {
        memset(err, 0, sizeof(err));
        sprintf(err, "Net: Slave Send Data, Slave Is NULL.");
        logger->log_e(logger, err);
        return -1;
    }


    if (slave->slave_fd > 0) {
        ssize = send(slave->slave_fd, data, size, 0);

        if ((ssize <= 0) && (slave->auto_connect == SLAVE_AUTO_RECONNECT)) {
            memset(err, 0, sizeof(err));
            sprintf(err, "Net: Slave Send Data, %s.", strerror(errno));
            logger->log_e(logger, err);

            appe_slave_tcp_close(slave, SLAVE_STATUS_RECONNECTED);

            char slave_name[256];
            char server_ip[64];
            memset(slave_name, 0, 256);
            memset(server_ip, 0, 64);
            strcpy(slave_name, slave->slave_name);
            strcpy(server_ip, slave->server_ip);
            int server_port = slave->server_port;
            int auto_connect = slave->auto_connect;
            int res = appe_slave_tcp_init(slave, slave_name, server_ip, server_port, SLAVE_STATUS_RECONNECTED, auto_connect);

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

int appe_slave_tcp_close(AppeTcpSlave *slave, int reconnect)
{
    void *res = NULL;
    char err[256];
    App *s_app = appe_get_app_instance();
    Logger *logger = s_app->logger;

    SERVER_QUIT = 1;
    sleep(1.5);

    if (!slave) {
        memset(err, 0, sizeof(err));
        sprintf(err, "Net: Slave Close, Slave Is NULL.");
        logger->log_e(logger, err);
        goto PTHREAD_FREE;
    }

    if (slave->slave_fd > 0) {
        close(slave->slave_fd);
        slave->slave_fd = -1;
        slave->status = ENUM_TCP_CLOSED;

        memset(err, 0, sizeof(err));
        sprintf(err, "Net: Slave %s Closed.", slave->slave_name);
        logger->log_i(logger, err);
    }

PTHREAD_FREE:
    fprintf(stderr, "%s recv server exit...\n", slave->slave_name);

    if (slave->pthread != -1) {
        pthread_join(slave->pthread, &res);
        slave->pthread = -1;
    }

    if (!reconnect) {
        if (slave->pHandlers != NULL) {
            free(slave->pHandlers);
            slave->pHandlers = NULL;
        } 
    }

    return 0;
}

int appe_slave_tcp_disconnect(AppeTcpSlave *slave)
{
    void *res = NULL;
    char err[256];
    App *s_app = appe_get_app_instance();
    Logger *logger = s_app->logger;

    SERVER_QUIT = 1;
    sleep(1.5);

    if (!slave) {
        memset(err, 0, sizeof(err));
        sprintf(err, "Net: Slave Disconnect, Slave Is NULL.");
        logger->log_e(logger, err);
        goto PTHREAD_FREE;
    }

    if (slave->slave_fd > 0) {
        close(slave->slave_fd);
        slave->slave_fd = -1;
        slave->status = ENUM_TCP_DISCONNECTED;

        memset(err, 0, sizeof(err));
        sprintf(err, "Net: Slave %s Disconnecting...", slave->slave_name);
        logger->log_i(logger, err);
    }

PTHREAD_FREE:
    fprintf(stderr, "%s recv server exit...\n", slave->slave_name);

    if (slave->pthread != -1) {
        pthread_join(slave->pthread, &res);
        slave->pthread = -1;
    }
    
    return 0;
}
