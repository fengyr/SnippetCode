/*
 * =====================================================================================
 *
 *       Filename:  commandline.h
 *
 *    Description:  
 *
 *        Version:  
 *        Created:  五月 31, 2013
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Zenki (Zenki.J.Zha), zenki2001cn@163.com
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef _commandline_H_
#define _commandline_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "ini.h"

/* 命令行解析的类型标签*/
#define TAGTYPE_BOOL    0
#define TAGTYPE_INT     1
#define TAGTYPE_FLOAT   2
#define TAGTYPE_STRING  3

/* 命令行返回错误值 */
#define PARSE_ERROR -1
#define TAGERR_NAME -2
#define TAGERR_TYPE -3
#define TAGERR_DATA -4

typedef struct {
	const char *name;
	int type;
	void *data;
} Tag;

//////////////////////////////////////////////////////
//          public interface                        //
//////////////////////////////////////////////////////
typedef int (*ParseHook)(const char *name, void *data);

int parse_params_action(const char *argv[], int ii, Tag t);
int parse_params(int argc, const char *argv[], int numtags, Tag* t);
void set_parse_hook(ParseHook hook);

#ifdef __cplusplus
}
#endif

#endif /* end of include guard: _commandline_H_ */
