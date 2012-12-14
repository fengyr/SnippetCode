/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  
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
#include <stdlib.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <signal.h>
#include <time.h>

int main(int argc, char *argv[])
{
    int error;
    struct addrinfo hints, *res, *result;
    struct sockaddr_in *temp_addr;
    char ip_addr[256];
    char *hostname = "Zenki-laptop";
    /*char *hostname = NULL;*/
    char *service = NULL;
    /*char *service = "ftp";*/

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_flags = AI_CANONNAME | AI_ALL |AI_ADDRCONFIG;
    /*hints.ai_socktype = SOCK_DGRAM;*/

    error = getaddrinfo(hostname, service, &hints, &res);
    if ( error != 0) {
        fprintf(stderr, "getaddrinfo err: %s\n", gai_strerror(error));
        exit(1);
    }

    result = res;
    do {
        temp_addr = (struct sockaddr_in*)res->ai_addr;
        /*ip_addr = inet_ntoa(temp_addr->sin_addr);*/
        inet_ntop(AF_INET, &(temp_addr->sin_addr.s_addr), ip_addr, sizeof(struct sockaddr_in));

        printf("ai_canonname: %s ip_addr: %s\n", res->ai_canonname, ip_addr);
    } while( (res = res->ai_next) != NULL );

    freeaddrinfo(result);

    return 0;
}


