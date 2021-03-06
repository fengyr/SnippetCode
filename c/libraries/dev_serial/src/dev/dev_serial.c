/*
 * =====================================================================================
 *
 *       Filename:  dev_serial.c
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

#include "dev_serial.h"
#include "serial.h"

static int dev_serial_close(struct hw_device_t *device)
{
    printf("dev_serial_close >>>>>>>\n");

    int ret = 0;
    SerialDevice *dev;

    if (!device) {
        printf("dev_serial_close: device=NULL\n");
        ret = -1;
        goto DONE;
    }

    dev = container_of(device, SerialDevice, common);

    if (dev) {
        printf("dev_serial_close: device free\n");
        dev->close(dev->fd);
        free(dev);
    }

DONE:
    return ret;
}

static int dev_serial_open(const struct hw_module_t *module, 
                const char *id, struct hw_device_t **device)
{
    SerialDevice *dev;

    if (!strcmp(id, MODULE_SERIAL_ID)) {
        dev = (SerialDevice*) malloc(sizeof(SerialDevice));
        if (!dev) {
            return -1;
        }
        memset(dev, 0, sizeof(SerialDevice));

        dev->common.tag = DEVICE_COMMON_TAG;
        dev->common.version = DEVICE_SERIAL_VERSION;
        dev->common.module = module;
        dev->common.__exit = dev_serial_close;

        dev->open = serial_open;
        dev->init = serial_init;
        dev->read = serial_read_data;
        dev->write = serial_write_data;
        dev->close = serial_close;

        *device = &(dev->common);
    }

    printf("dev_serial_open >>>>>>>>\n");

    return 0;
}

static struct hw_module_methods_t s_serial_module_methods = {
    __init: dev_serial_open,
};

struct serial_module_t HW_MODULE_INFO_SYM = {
    common: {
        tag: MODULE_COMMON_TAG,
        version_major: MODULE_SERIAL_MAJOR,
        version_minor: MODULE_SERIAL_MINOR,
        id: MODULE_SERIAL_ID,
        name: MODULE_SERIAL_NAME,
        author: MODULE_SERIAL_AUTHOR,
        methods: &s_serial_module_methods,
        dso: NULL,
    },
};
