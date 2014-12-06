/*
 * =====================================================================================
 *
 *       Filename:  options_in.h
 *
 *    Description:  
 *
 *        Version:  
 *        Created:  十一月 13, 2014
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Zenki (Zenki.J.Zha), zenki2001cn@163.com
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef _options_in_H_
#define _options_in_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "ini.h"
#include "commandline.h"

#define MATCH(name, n) (strcmp((name), (n)) == 0)

#define OPTIONS_WRITE   1
#define OPTIONS_ABORT   0

typedef struct options_t Options, *POptions;
typedef struct ini_t Ini, *PIni;
typedef struct cmd_line_t CmdLine, *PCmdLine;

//////////////////////////////////////////////////////
//          inner interface                        //
//////////////////////////////////////////////////////
int getOptions(Options *options, int argc, const char *argv[]);

//////////////////////////////////////////////////////
//          public interface                        //
//////////////////////////////////////////////////////
/**
 * @Synopsis 格式化ini数据，用于保存。
 *
 * @Param options
 * @Param buf       系统分配的内存，用于存储ini数据。
 * @Param buf_size  分配内存的大小。
 *
 * @Returns 返回OPTIONS_ABORT则不保存当前值，返回OPTIONS_WRITE则保存。
 */
int formatOptions(Options *options, char *buf, int buf_size);

/**
 * @Synopsis 初始化Options，设置默认值。
 *
 * @Param options
 * @Param argc
 * @Param argv[]
 *
 * @Returns 
 */
int initOptions(Options *options, int argc, const char *argv[]);

/**
 * @Synopsis        向系统推送Tags实例，用于解析命令行参数。
 *
 * @Param options
 * @Param tag       用于返回调用端的Tag地址。
 *
 * @Returns         返回用户端定义的Tag大小。
 */
int pushTags(Options *options, Tag **tag);

/**
 * @Synopsis 向系统推送Ini实例，让系统获取Ini接口。
 *
 * @Param options
 * @Param ini   用于返回调用端的Ini地址。
 *
 * @Returns 
 */
int pushIni(Options *options, Ini **ini);

/**
 * @Synopsis 向系统推送Options实例，让系统获取Options接口
 *
 * @Param options   用于返回调用端的Options地址。
 *
 * @Returns 
 */
int pushOptions(Options **options);

/**
 * @Synopsis        Ini解析回调函数。
 *
 * @Param user      系统传递给用户的Ini指针。
 * @Param section   返回当前Ini的section段名称。
 * @Param name      返回当前Key字符串
 * @Param value     返回当前value字符串。
 *
 * @Returns 
 */
int iniParseHook(void* user, const char* section, const char* key, const char* value);

/**
 * @Synopsis 打印输出Options。
 *
 * @Param options
 */
void printOptions(Options *options);

/**
 * @Synopsis 打印帮助信息
 *
 * @Param options
 */
void printUsage(Options *options);

#ifdef __cplusplus
}
#endif

#endif /* end of include guard: _options_in_H_ */
