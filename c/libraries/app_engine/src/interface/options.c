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
#include "appe_app.h"

static Options s_options;

int iniParseHook(void* user, const char* section, 
               const char* key, const char* value)
{
    Ini* ini = (Ini*) user;

    if (MATCH(key, "param3")) {
        strcpy(ini->param3, value);
    } else if (MATCH(key, "param1")) {
        ini->param1 = atoi(value);
    } else if (MATCH(key, "param2")) {
        ini->param2 = atof(value);
    } else {
        return -1;
    }

    return 0;
}

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
    ini->param1 = 100;
    ini->param2 = 10.1;
    strcpy(ini->param3, "hello wrold.");

    return 0;
}

void printOptions(Options *options)
{
    DEBUG("-----------dump command line ------------\n");
    DEBUG("config_file_path: %s\n", options->cmd.config_file_path);
    DEBUG("server_ip_addr: %s\n", options->cmd.server_ip_addr);
    DEBUG("server_port: %d\n", options->cmd.server_port);
    DEBUG("-----------dump ini config --------------\n");
    DEBUG("param1: %d\n", options->ini.param1);
    DEBUG("param2: %f\n", options->ini.param2);
    DEBUG("param3: %s\n", options->ini.param3);
}

void printUsage(Options *options)
{
    if (options->cmd.help_mode) {
        printf("Usage: ColorSepServer [OPTIONS] [VALUE]             \n");
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
        printf("    --camerafile        设置CameraFile              \n");

        exit(2);
    }
}

int formatOptions(Options *options, char *ops_fmt_buf, int buf_size)
{
    sprintf(ops_fmt_buf, "[%s]\nparam1=%d\n[%s]\nparam2=%f\n[%s]\nparam3=%s\n", 
            "UseBWenh", 111000, "ParamNum", 22.000, "CameraFile", "./camera.conf");     
    return OPTIONS_WRITE;
}

int pushTags(Options *options, Tag **tag)
{
    static Tag tags[] = {
        {"--config", TAGTYPE_STRING, options->cmd.config_file_path},
        {"--localaddr", TAGTYPE_STRING, options->cmd.server_ip_addr},
        {"--localport", TAGTYPE_INT, &options->cmd.server_port},
        {"-h", TAGTYPE_BOOL, &options->cmd.help_mode},
        {"--help", TAGTYPE_BOOL, &options->cmd.help_mode},
    };

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
