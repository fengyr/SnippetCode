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

#include <string.h>

/* 命令行解析的类型标签*/
#define TAGTYPE_BOOL 0
#define TAGTYPE_INT 1
#define TAGTYPE_FLOAT 2
#define TAGTYPE_STRING 3

/* 命令行返回错误值 */
#define PARSE_ERROR -1
#define TAGERR_NAME -2
#define TAGERR_TYPE -3
#define TAGERR_DATA -4

//////////////////////////////////////////////////////
//          配置文件参数结构                        //
//////////////////////////////////////////////////////
#define MATCH(name, n) strcmp(name, n) == 0

/**
 * @Synopsis 配置文件包含的配置项
 */
typedef struct ini_t {
    char CameraFile[255];
    char CalibBWPath[255];
    char RunFile[255];

    int UseBWenh;
    int UseClusterSep;
    int UseClusterData;

    char ClusterDataPath[255];
    float ParaNum1;
    float ParaNum2;

    int DetectCollapse;
    float C_thresh;

    int QUIET_LEVEL;
    int PROCESS_TIME;
    int Resize_Mode;
    int UseNetConnect;
    int UseGLCM;
    int WithCalib;
    int InitSetPIC;
    int UseColorSep;
    int UseSubThresh;
    int Subthresh_Seppart;

    int Classifier;
    char TrainFile[255];
    int MaxClass;
    int TrainType;
    char TrainTag[128];
    int TrainSample;
} Ini, *PIni;

//////////////////////////////////////////////////////
//          命令行参数结构                          //
//////////////////////////////////////////////////////
typedef struct {
	const char *name;
	int type;
	void *data;
} Tag;

typedef struct {
    char config_file_path[255];     /* --config 配置文件路径 */
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

int getOptions(Options *options, int argc, const char *argv[]);

#ifdef __cplusplus
}
#endif

#endif /* end of include guard: _command_line_H_ */
