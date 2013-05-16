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
#ifndef __INI_H__
#define __INI_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

int ini_parse(const char* filename, 
              int (*handler)(void* user, const char* section, 
                             const char* name, const char* value),
              void* user);

int ini_parse_file(FILE* file,
                   int (*handler)(void* user, const char* section, 
                                  const char* name, const char* value),
                   void* user);

#ifndef INI_ALLOW_MULTILINE
#define INI_ALLOW_MULTILINE 1
#endif

#ifdef __cplusplus
}
#endif

#endif /* __INI_H__ */
