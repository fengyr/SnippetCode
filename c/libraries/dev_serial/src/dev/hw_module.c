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

static HwModuleSet s_modules_set;
static int s_modules_init = 0;

/* int hw_register_module(struct hw_module_t *module)
 * {
 *     if (0 == s_modules_init) {
 *         memset(&s_modules_set, 0, sizeof(HwModuleSet));
 *         s_modules_init = 1;
 *     }
 * 
 *     int ret = 0;
 *     static int module_set_id = 0;
 * 
 *     if (module_set_id < MAX_MODULES) {
 *         int i;
 *         for (i = 0; i < MAX_MODULES; i++) {
 *             if (!s_modules_set.modules[i]) {
 *                 s_modules_set.modules[i] = module;
 *                 module_set_id++;
 *                 goto DONE;
 *             } else {
 *                 continue;
 *             }
 *         }
 *         ret = -2;
 *     } else {
 *         ret = -1;
 *     }
 * 
 * DONE:
 *     return ret;
 * } */

/* int hw_free_module()
 * {
 *     int i;
 *     for (i = 0; i < MAX_MODULES; i++) {
 *         if (s_modules_set.modules[i]) {
 *             DEBUG("Free Module: <%s> SUCCESS.\n", s_modules_set.modules[i]->id);
 *         } else {
 *             continue;
 *         }
 *     } 
 * 
 *     return 0;
 * } */

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
