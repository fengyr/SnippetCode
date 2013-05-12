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

/* 解析的类型标签*/
#define TAGTYPE_BOOL 0
#define TAGTYPE_INT 1
#define TAGTYPE_FLOAT 2
#define TAGTYPE_STRING 3

/* 返回错误值 */
#define PARSE_ERROR -1
#define TAGERR_NAME -2
#define TAGERR_TYPE -3
#define TAGERR_DATA -4

typedef struct {
	const char *name;       /* text to look for */
	int type;               /* what to do about it */
	void *data;             /* pointer for possible return of data */
} Tag;

//////////////////////////////////////////////////////
//          public interface                        //
//////////////////////////////////////////////////////
#define DEFAULT_SERVER_IP       ""
#define DEFAULT_SERVER_PORT     11014

typedef struct options_t {
    char config_file_path[255];     /* --config 配置文件路径 */
    char server_ip_addr[24];        /* --addr   tcp服务器的IP地址 */
    int server_port;                /* --port   tcp服务器的端口*/
} Options, *POptions;

int getOptions(Options *options, int argc, const char *argv[]);

#endif /* end of include guard: _command_line_H_ */
