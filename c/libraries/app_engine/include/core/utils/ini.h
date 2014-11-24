/*
 * =====================================================================================
 *
 *       Filename:  ini.h
 *
 *    Description:  
 *
 *        Version:  
 *        Created:  五月 14, 2013
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Zenki (Zenki.J.Zha), zenki2001cn@163.com
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef _ini_H_
#define _ini_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

typedef int (*IniParseHook)(void*, const char*, const char*, const char*);
//////////////////////////////////////////////////////
//          public interface                        //
//////////////////////////////////////////////////////
// 只有读功能
int ini_parse(const char* filename, 
              int (*handler)(void* user, const char* section, 
                             const char* name, const char* value),
              void* user);

int ini_parse_file(FILE* file,
                   int (*handler)(void* user, const char* section, 
                                  const char* name, const char* value),
                   void* user);

#ifdef __cplusplus
}
#endif

// 使用Simpleini解析，可读写INI文件
int ini_parse2(const char* filename, 
              int (*handler)(void* user, const char* section, 
                             const char* name, const char* value),
              void* user);

int ini_parse_file2(void *ini,
                   int (*handler)(void* user, const char* section, 
                                  const char* name, const char* value),
                   void* user);

#endif /* end of include guard: _ini_H_ */
