/*
 * =====================================================================================
 *
 *       Filename:  zlogwrap.h
 *
 *    Description:  
 *
 *        Version:  
 *        Created:  七月 22, 2013
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Zenki (Zenki.J.Zha), zenki2001cn@163.com
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef _zlogwrap_H_
#define _zlogwrap_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "zlog.h"

#define LOG_STDOUT  0
#define LOG_STDERR  1
#define LOG_FILE    2

#define LOG_CONFIG_PATH     "/var/log/colorserver/.server_log.conf"
#define LOG_FILE_DIR        "/var/log/colorserver"

struct logger_t;
typedef struct logger_t Logger, *PLogger;

typedef void (*logfunc)(Logger *logger, const char *fmt);

struct logger_t {
    int log_on;
    int log_mode;
    zlog_category_t *zc; 
    logfunc  log_i;
    logfunc  log_e;
    logfunc  log_d;
};

int logger_init(Logger *logger, 
                 int mode, 
                 const char *log_conf_path, 
                 const char *log_file_dir);
void logger_destroy(Logger *logger);

#ifdef __cplusplus
}
#endif

#endif /* end of include guard: _zlogwrap_H_ */
