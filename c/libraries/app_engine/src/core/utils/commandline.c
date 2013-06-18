/*
 * =====================================================================================
 *
 *       Filename:  commandline.c
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
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "commandline.h"

static ParseHook s_parse_hook;

int parse_params_action(const char *argv[], int ii, Tag t)
{
    int skip;
    skip = 0;

    switch (t.type) {
        case TAGTYPE_BOOL:
            *((int*)t.data) = 1;
            break;
        case TAGTYPE_INT:
            if (argv[ii+1]==NULL) return PARSE_ERROR;
            *((int*)t.data) = atoi(argv[ii+1]);
            skip = 1;
            break;
        case TAGTYPE_FLOAT:
            if (argv[ii+1]==NULL) return PARSE_ERROR;
            *((float*)t.data) = atof(argv[ii+1]);
            skip = 1;
            break;
        case TAGTYPE_STRING:
            if (argv[ii+1]==NULL) return PARSE_ERROR;
            strcpy((char*)t.data, argv[ii+1]);
            skip = 1;
            break;
    }

    return skip;
}

int parse_params(int argc, const char *argv[], int numtags, Tag* t)
{
    int ii, ij, status, count;

    for (ii=0; ii<numtags; ii++) {
        if (strcmp(t[ii].name,"")==0)
            return TAGERR_NAME;
        if (t[ii].type > TAGTYPE_STRING)
            return TAGERR_TYPE;
        if (t[ii].data == NULL)
            return TAGERR_DATA;
    }

    count = 0;
    for (ii=1; ii<argc; ii++) {
        for (ij=0; ij<numtags; ij++) {
            if (strcmp(t[ij].name, argv[ii]) == 0) {
                status = parse_params_action(argv, ii, t[ij]);
                if (status<0) {
                    return status;
                }

                if (s_parse_hook != NULL) {
                    s_parse_hook(t[ij].name, t[ij].data);    
                }

                ii += status;
                ij = numtags;
                count++;
            }
        }
    }
    return count;
}

void set_parse_hook(ParseHook hook)
{
    s_parse_hook = hook;
}
