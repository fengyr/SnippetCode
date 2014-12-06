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

#include "appe_debug.h"
#include "hw_module.h"

static int load(const char *id, const char *so_path, struct hw_module_t **pHmi)
{
    int status = -1;
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
    // FIXME: 如果加载成功，还需要在后期做释放
    if (status != 0) {
        hmi = NULL;
        if (handle != NULL) {
            dlclose(handle);
            handle = NULL;
        }
    }

    *pHmi = hmi;
    DEBUG("loaded module id=%s path=%s hmi=%p handle=%p\n",
            id, so_path, *pHmi, handle);

    return status;
}

int hw_get_module(const char *id, const char *so_path, struct hw_module_t **module)
{
    return load(id, so_path, module);
}

int dump_module_info(struct hw_module_t *module)
{
    if (module != NULL) {
        fprintf(stderr, "=== Module Info ===\nTag: %d\nVersion_major: %d\nVersion_minor: %d\nID: %s\nName: %s\nAuthor: %s\nDSO: %p\n", 
                module->tag, module->version_major, module->version_minor, module->id, module->name, module->author, module->dso);

        int i;
        fprintf(stderr, "Reserved: ");
        for (i = 0; i < sizeof(module->reserved)/sizeof(unsigned int); i++) {
            fprintf(stderr, "%d", module->reserved[i]);
        }
        fprintf(stderr, "\n");

        return 0;
    }

    return -1;
}

int dump_device_info(struct hw_device_t *device)
{
    if (device != NULL) {
        fprintf(stderr, "=== Device Info ===\nTag: %d\nVersion: %d\nModule: %p\n", 
                device->tag, device->version, device->module);

        int i;
        fprintf(stderr, "Reserved: ");
        for (i = 0; i < sizeof(device->reserved)/sizeof(unsigned int); i++) {
            fprintf(stderr, "%d", device->reserved[i]);
        }
        fprintf(stderr, "\n");

        return 0;
    }

    return -1;

}
