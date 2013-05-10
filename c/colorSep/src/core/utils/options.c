/*
 * =====================================================================================
 *
 *       Filename:  options.c
 *
 *    Description:  
 *
 *        Version:  
 *        Created:  五月 09, 2013
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Zenki (Zenki.J.Zha), zenki2001cn@163.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include <getopt.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "options.h"
#include "debug.h"

static int parse_params_action (const char *argv[], int ii, Tag t)
{
    int skip;
    skip = 0;

    switch (t.type) {
        case TAGTYPE_BOOL:
            *((int*)t.data) = 1;
            break;
        case TAGTYPE_INT:
            if (argv[ii+1]==NULL) return PARSE_ERROR;
            *((int*)t.data) = atoi(argv[ii+1]);
            skip = 1;
            break;
        case TAGTYPE_FLOAT:
            if (argv[ii+1]==NULL) return PARSE_ERROR;
            *((float*)t.data) = atof(argv[ii+1]);
            skip = 1;
            break;
        case TAGTYPE_STRING:
            if (argv[ii+1]==NULL) return PARSE_ERROR;
            strcpy((char*)t.data, argv[ii+1]);
            skip = 1;
            break;
    }

    return skip;
}

static int parse_params(int argc, const char *argv[], int numtags, Tag* t)
{
    int ii, ij, status, count;

    for (ii=0; ii<numtags; ii++) {
        if (strcmp(t[ii].name,"")==0)
            return TAGERR_NAME;
        if (t[ii].type > TAGTYPE_STRING)
            return TAGERR_TYPE;
        if (t[ii].data == NULL)
            return TAGERR_DATA;
    }

    count = 0;
    for (ii=1; ii<argc; ii++) {
        for (ij=0; ij<numtags; ij++) {
            if (strcmp(t[ij].name, argv[ii])==0) {
                status = parse_params_action(argv, ii, t[ij]);
                if (status<0) {
                    return status;
                }

                ii += status;
                ij = numtags;
                count++;
            }
        }
    }
    return count;
}

static void dumpOptions(Options *options)
{
    DEBUG("-----------dump options ------------\n");
    DEBUG("config_file_path: %s\n", options->config_file_path);
    DEBUG("server_ip_addr: %s\n", options->server_ip_addr);
    DEBUG("server_port: %d\n", options->server_port);
    DEBUG("------------------------------------\n");
}

static void initOptions(Options *options)
{
    /* 配置文件路径 */
    memset(options->config_file_path, 0, 255);
    strcpy(options->config_file_path, "");

    /* tcp服务器地址 */
    memset(options->server_ip_addr, 0, 24);
    strcpy(options->server_ip_addr, DEFAULT_SERVER_IP);

    /* tcp服务器端口 */
    options->server_port = DEFAULT_SERVER_PORT;
}

int getOptions(Options *options, int argc, const char *argv[])
{
    int rtn;
    static Tag tag[] = {
        {"--config", TAGTYPE_STRING, options->config_file_path},
        {"--addr", TAGTYPE_STRING, options->server_ip_addr},
        {"--port", TAGTYPE_INT, &options->server_port},
    }; 

    initOptions(options);

    int size = (sizeof(tag)/sizeof(Tag));
	rtn = parse_params(argc, argv, size, tag);

    dumpOptions(options);

	DEBUG("get custom options num: %d\n", rtn);

    return rtn;
}
