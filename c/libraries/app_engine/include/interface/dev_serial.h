/*
 * =====================================================================================
 *
 *       Filename:  dev_serial.h
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
#ifndef _dev_serial_H_
#define _dev_serial_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "hw_module.h"

typedef struct serial_module_t {
    HwModule common;
} SerialModule, *PSerialModule;

typedef struct serial_device_t {
    HwDevice common;

    int fd;

    int (*open)(const char *dev_name);
    int (*init)(int fd, int baudrate, int databits, 
                       int stopbits, int parity);
    int (*read)(int fd, unsigned char *buf, int len);
    int (*write)(int fd, unsigned char *buf, int len);
    int (*close)(int fd);
} SerialDevice, *PSerialDevice;

#define MODULE_SERIAL_MAJOR     1
#define MODULE_SERIAL_MINOR     0
#define MODULE_SERIAL_ID        "serial"
#define MODULE_SERIAL_NAME      "RS232 and RS485 interface"
#define MODULE_SERIAL_AUTHOR    "zenki"

#define DEVICE_SERIAL_VERSION   1000

#ifdef __cplusplus
}
#endif

#endif /* end of include guard: _dev_serial_H_ */
