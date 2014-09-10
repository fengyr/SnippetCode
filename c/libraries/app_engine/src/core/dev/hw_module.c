/*
 * =====================================================================================
 *
 *       Filename:  hw_module.c
 *
 *    Description:  
 *
 *        Version:  
 *        Created:  九月 05, 2014
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
#include <dlfcn.h>

#include "debug.h"
#include "hw_module.h"

static int s_modules_init = 0;

static int load(const char *id, const char *so_path, struct hw_module_t **pHmi)
{
    int status;
    void *handle;
    struct hw_module_t *hmi;
    const char *sym = HW_MODULE_INFO_SYM_AS_STR;

    handle = dlopen(so_path, RTLD_NOW);
    
    if (NULL == handle) {
        char const *err = dlerror();
        DEBUG("load: dlopen err: %s, so path: %s\n", err, so_path);
        status = -1;
        goto DONE;
    }

    hmi = (struct hw_module_t *)dlsym(handle, sym);
    if (hmi == NULL) {
        char const *err = dlerror();
        DEBUG("load: find symbol err: %s, so path: %s\n", err, so_path);
        status = -2;
        goto DONE;
    }

    if (strcmp(id, hmi->id) != 0) {
        DEBUG("load: id=%s != hmi->id=%s\n", id, hmi->id);
        status = -3;
        goto DONE;
    }

    hmi->dso = handle;
    status = 0; 

DONE:
    if (status != 0) {
        hmi = NULL;
        if (handle != NULL) {
            dlclose(handle);
            handle = NULL;
        }
    } else {
        DEBUG("loaded module id=%s path=%s hmi=%p handle=%p\n",
                id, so_path, *pHmi, handle);
    }

    *pHmi = hmi;

    return status;
}

int hw_get_module(const char *id, const char *so_path, struct hw_module_t **module)
{
    return load(id, so_path, module);
}
