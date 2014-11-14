/*
 * =====================================================================================
 *
 *       Filename:  options_in.c
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
#include <getopt.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "options_in.h"
#include "app.h"

static Ini *s_ini;
static Tag *s_tag;

static int parse_hook(const char *name, void *data)
{
    if ((strcmp(name, "--config") == 0) &&
        (strcmp((char*)data, "") != 0)) {
        DEBUG("parse_hook: name = %s, data = %s\n", name, (char*)data);
        if (ini_parse2((char*)data, iniParseHook, s_ini) < 0) {
            fprintf(stderr, "getOptions: load ini error\n");
            return -1;
        }
        return 0;
    } 
    return -1;
}

int getOptions(Options *options, int argc, const char *argv[])
{
    int rtn = -1;

    pushIni(options, &s_ini);

    initOptions(options, argc, argv);
    // 从命令行获取的参数动态改变配置文件的参数
    set_parse_hook(parse_hook);

    int size = pushTags(options, &s_tag);
    if (s_tag != NULL) {
        rtn = parse_params(argc, argv, size, s_tag);
        DEBUG("get cmdline num: %d\n", rtn);
    }

    printUsage(options);

    printOptions(options);

    return rtn;
}

