/*
 * =====================================================================================
 *
 *       Filename:  options.h
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
#ifndef _command_line_H_
#define _command_line_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "ini.h"
#include "commandline.h"
//////////////////////////////////////////////////////
//          配置文件参数结构                        //
//////////////////////////////////////////////////////
#define MATCH(name, n) (strcmp((name), (n)) == 0)

typedef struct ini_t {
} Ini, *PIni;

//////////////////////////////////////////////////////
//          命令行参数结构                          //
//////////////////////////////////////////////////////
typedef struct {
    char config_file_path[256];     /* --config 配置文件路径 */
    char server_ip_addr[24];        /* --addr   tcp服务器的IP地址 */
    int server_port;                /* --port   tcp服务器的端口 */
    int help_mode;                  /* -h 打印帮助信息 */
} CmdLine;

//////////////////////////////////////////////////////
//          public interface                        //
//////////////////////////////////////////////////////
#define DEFAULT_SERVER_IP       ""
#define DEFAULT_SERVER_PORT     11014

typedef struct options_t {
    CmdLine cmd;                    /* 从命令行读取的信息 */
    Ini ini;                        /* 从配置文件中读取的信息 */
} Options, *POptions;

int setOptionsToStr(Options *options, char *buf, int buf_size);
int getOptions(Options *options, int argc, const char *argv[]);

#ifdef __cplusplus
}
#endif

#endif /* end of include guard: _command_line_H_ */
