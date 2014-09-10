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
#include "ini.h"
#include "commandline.h"
#include "debug.h"

static Ini *s_ini;

static int ini_handler(void* user, const char* section, 
                       const char* name, const char* value)
{
    Ini* ini = (Ini*) user;

    if (MATCH(name, "CameraFile")) {
        strcpy(ini->CameraFile, value);
    } else if (MATCH(name, "UseBWenh")) {
        ini->UseBWenh = atoi(value);
    } else if (MATCH(name, "ParaNum1")) {
        ini->ParaNum1 = atof(value);
    } else if (MATCH(name, "ParaNum2")) {
        ini->ParaNum2 = atof(value);
    } else {
        return 0;
    }

    return 1;
}

static int parse_hook(const char *name, void *data)
{
    if ((strcmp(name, "--config") == 0) &&
        (strcmp((char*)data, "") != 0)) {
        DEBUG("parse_hook: name = %s, data = %s\n", name, (char*)data);
        if (ini_parse((char*)data, ini_handler, s_ini) < 0) {
            fprintf(stderr, "getOptions: load ini error\n");
            return -1;
        }
        return 0;
    } 
    return -1;
}

static void initOptions(Options *options)
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
    strcpy(ini->CameraFile, "../para/file_conf/CameraCalib.xml");
    ini->UseBWenh = 100;
    ini->ParaNum1 = 10.1;
    ini->ParaNum2 = 20.1;
}

static void dumpOptions(Options *options)
{
    DEBUG("-----------dump command line ------------\n");
    DEBUG("config_file_path: %s\n", options->cmd.config_file_path);
    DEBUG("server_ip_addr: %s\n", options->cmd.server_ip_addr);
    DEBUG("server_port: %d\n", options->cmd.server_port);
    DEBUG("-----------dump ini config --------------\n");
    DEBUG("CameraFile: %s\n", options->ini.CameraFile);
    DEBUG("UseBWenh: %d\n", options->ini.UseBWenh);
    DEBUG("ParaNum1: %f\n", options->ini.ParaNum1);
    DEBUG("ParaNum2: %f\n", options->ini.ParaNum2);
}

static void print_usage()
{
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

int getOptions(Options *options, int argc, const char *argv[])
{
    int rtn;
    static Tag tag[] = {
        {"--config", TAGTYPE_STRING, options->cmd.config_file_path},
        {"--localaddr", TAGTYPE_STRING, options->cmd.server_ip_addr},
        {"--localport", TAGTYPE_INT, &options->cmd.server_port},
        {"-h", TAGTYPE_BOOL, &options->cmd.help_mode},
        {"--help", TAGTYPE_BOOL, &options->cmd.help_mode},
        {"--camerafile", TAGTYPE_STRING, options->ini.CameraFile},
    }; 

    s_ini = &(options->ini);
    initOptions(options);
    // 从命令行获取的参数动态改变配置文件的参数
    set_parse_hook(parse_hook);

    int size = (sizeof(tag)/sizeof(Tag));
    rtn = parse_params(argc, argv, size, tag);

    if (options->cmd.help_mode) {
        print_usage();
    }

    if (strcmp(options->cmd.config_file_path, "") == 0) {
        printf("getOptions: use default sysconf\n");
    }

    dumpOptions(options);

    DEBUG("get command line arg num: %d\n", rtn);

    return rtn;
}
