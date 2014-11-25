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
#include "app.h"

static Options s_options;

static Tag tags[] = {
    {"--config", TAGTYPE_STRING, s_options.cmd.config_file_path},
    {"--localaddr", TAGTYPE_STRING, s_options.cmd.server_ip_addr},
    {"--localport", TAGTYPE_INT, &s_options.cmd.server_port},
    {"-h", TAGTYPE_BOOL, &s_options.cmd.help_mode},
    {"--help", TAGTYPE_BOOL, &s_options.cmd.help_mode},
};

int initOptions(Options *options, int argc, const char *argv[])
{
    memset(options, 0, sizeof(Options));
    CmdLine *cmd = &(options->cmd);
    Ini *ini = &(options->ini);

    //////////////////  命令行参数默认配置 //////////////////
    /* 配置文件路径 */
    strcpy(cmd->config_file_path, "");

    /* tcp服务器地址 */
    strcpy(cmd->server_ip_addr, DEFAULT_SERVER_IP);

    /* tcp服务器端口 */
    cmd->server_port = DEFAULT_SERVER_PORT;

    /* 打印帮助信息并退出 */
    cmd->help_mode = 0;

    //////////////////  配置文件默认配置 //////////////////

    return 0;
}

int iniParseHook(void* user, const char* section, 
               const char* key, const char* value)
{
    Ini* ini = (Ini*) user;

    /* if (MATCH(key, "param3")) {
     *     printf("section=%s, key=%s, value=%s\n", section, key, value);
     * } else if (MATCH(key, "param1")) {
     *     printf("section=%s, key=%s, value=%d\n", section, key, atoi(value));
     * } else if (MATCH(key, "param2")) {
     *     printf("section=%s, key=%s, value=%f\n", section, key, atof(value));
     * } else {
     *     return -1;
     * }  */

    return 0;
}

int formatOptions(Options *options, char *buf, int buf_size)
{
    char tmp[1024];
    memset(tmp, 0, sizeof(tmp));
    sprintf(tmp, "[Param1]\nParam1 = %s\n", "hello");
    strcat(buf, tmp);

    memset(tmp, 0, sizeof(tmp));
    sprintf(tmp, "[Param2]\nParam2 = %d\n", 100);
    strcat(buf, tmp); 

    return OPTIONS_WRITE;
}

void printUsage(Options *options)
{
    if (options->cmd.help_mode) {
        printf("Usage: App [OPTIONS] [VALUE]                        \n");
        printf("                                                    \n");
        printf("参数设置的优先级为：                                \n");
        printf("    命令行 > 配置文件 > 默认配置                    \n");
        printf("                                                    \n");
        printf("命令行参数:                                         \n");
        printf("    --config            配置文件路径                \n");
        printf("    --localaddr         本机服务的IP地址            \n");
        printf("    --localport         本机服务的端口              \n");
        printf("    -h,--help           打印帮助信息                \n");
        printf("                                                    \n");
        printf("配置文件参数:                                       \n");

        exit(2);
    }
}

void printOptions(Options *options)
{
    DEBUG("-----------dump command line ------------\n");
    DEBUG("config_file_path: %s\n", options->cmd.config_file_path);
    DEBUG("server_ip_addr: %s\n", options->cmd.server_ip_addr);
    DEBUG("server_port: %d\n", options->cmd.server_port);
    DEBUG("help_mode: %d\n", options->cmd.help_mode);
    DEBUG("-----------dump ini config --------------\n");
}

int pushTags(Options *options, Tag **tag)
{
    Tag *tags_p = tags;
    *tag = tags_p;

    return (sizeof(tags)/sizeof(Tag));
}

int pushIni(Options *options, Ini **ini)
{
    *ini = &options->ini;

    return 0;
}

int pushOptions(Options **options)
{
    Options *ops = &s_options;

    *options = ops;

    return 0;
}
