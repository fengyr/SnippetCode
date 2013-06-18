/*
 * =====================================================================================
 *
 *       Filename:  camera_server.h
 *
 *    Description:  
 *
 *        Version:  
 *        Created:  五月 10, 2013
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Zenki (Zenki.J.Zha), zenki2001cn@163.com
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef _camera_server_H_
#define _camera_server_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "connection.h"

struct camera_server_t {
    Socket *sock;

    int (*init)(struct camera_server_t*, const char*, int);
    void (*run)(struct camera_server_t*, int);
    void (*quit)(struct camera_server_t*);
};
typedef struct camera_server_t CameraServer, *PCameraServer;

CameraServer* create_camera_server_instance();

#ifdef __cplusplus
}
#endif

#endif /* end of include guard: _tcp_server_H_ */
