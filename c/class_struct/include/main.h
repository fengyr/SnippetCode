/**
 *       File       main.h
 *
 *       Revision:  none
 *       Compiler:  gcc
 *
 *       Version    1.0
 *       Author:    Zenki (zhajun), zenki2001cn@163.com
 *       Date:      2010年06月04日 16时45分18秒
 *
 * =====================================================================================
 */
#ifndef MAIN_IEJ8MVJB
#define MAIN_IEJ8MVJB

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"
#include "class.h"
#include "debug.h"

typedef struct _MAIN MAIN;

typedef struct _MAIN_ELEMENT {
    DEMO demo1;
    DEMO demo2;
} MAIN_ELEMENT;

typedef struct _MAIN_INTERFACE {
    int (*main)(MAIN *this, int argc, const char *argv[]);
} MAIN_INTERFACE;

struct _MAIN {
    MAIN_ELEMENT Attr;
    MAIN_INTERFACE Iface;

    void (*initialize)(MAIN *this);
    void (*destroy)(MAIN *this);
};

void main_init(MAIN *this);
void main_free(MAIN *this);

#endif /* end of include guard: MAIN_IEJ8MVJB */
