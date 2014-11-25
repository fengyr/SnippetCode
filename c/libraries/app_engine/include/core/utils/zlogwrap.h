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

#define LOG_CONFIG_PATH     "/tmp/.app_engine_log.conf"
#define LOG_FILE_DIR        "/var/log/app_engine/"

struct logger_t;
typedef struct logger_t Logger, *PLogger;

typedef void (*logfunc)(Logger *logger, const char *fmt, ...);

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

// int check_logger(Logger *logger);

/* #define MY_ZLOG_INFO(logger, fmt, args...)  \
 *     do {                                                    \
 *         if (check_logger(logger) < 0) {                     \
 *             break;                                          \
 *         } else {                                            \
 *             zlog_info(logger->zc, fmt, ##args);        \
 *         }                                                   \
 *     } while (0);                                            \
 * 
 * #define MY_ZLOG_DEBUG(logger, fmt, args...)  \
 *     do {                                                    \
 *         if (check_logger(logger) < 0) {                     \
 *             break;                                          \
 *         } else {                                            \
 *             zlog_info(logger->zc, fmt, ##args);        \
 *         }                                                   \
 *     } while (0);                                            \
 * 
 * #define MY_ZLOG_ERROR(logger, fmt, args...)  \
 *     do {                                                    \
 *         if (check_logger(logger) < 0) {                     \
 *             break;                                          \
 *         } else {                                            \
 *             zlog_info(logger->zc, fmt, ##args);        \
 *         }                                                   \
 *     } while (0);                                            \ */


#ifdef __cplusplus
}
#endif

#endif /* end of include guard: _zlogwrap_H_ */
