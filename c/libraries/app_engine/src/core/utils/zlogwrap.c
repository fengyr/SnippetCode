/*
 * =====================================================================================
 *
 *       Filename:  zlogwrap.c
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
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>
#include <stdio.h>

#include "zlogwrap.h"
#include "debug.h"

static char const *LOG_CONFIG_TEMPLE[] = {
    "[global]",
    "default format = \"%d [%-5V] - %m%n\"",
    "",
    "[formats]",
    "simple = \"%d %-6V (%c:%F:%L) - %m%n\"",
    "",
    "[rules]",
    "stdout.*   >stdout;simple",
    "stderr.*   >stderr;simple",
    NULL,
};

static int s_init_zlog_success = 0;

static int make_config(const char *log_conf_path, const char *log_file_dir)
{
    mkdir(log_file_dir, S_IRWXU|S_IRWXG|S_IROTH|S_IXOTH);

    mode_t mode = S_IRWXU|S_IRWXG|S_IROTH|S_IXOTH;
    int fd = open(log_conf_path, O_CREAT | O_RDWR | O_TRUNC, mode);
    if (fd < 0) {
        /* fprintf(stderr, "make_config: create file error, %s\n", log_conf_path); */
        return -1;
    }

    char const **line;
    ssize_t wsize;
    for (line = LOG_CONFIG_TEMPLE; *line != NULL; line++) {
        wsize = write(fd, *line, strlen(*line));
        if (wsize < 0) {
            fprintf(stderr, "make_config: write error, %s\n", *line);
            return -1;
        }

        wsize = write(fd, "\n", strlen("\n"));
        if (wsize < 0) {
            fprintf(stderr, "make_config: write error\n");
            return -1;
        }
    }

    char new_line[1024];
    memset(new_line, 0, sizeof(new_line));
    sprintf(new_line, "file.*   \"%s/app.log\", 10MB ~ \"%s/app-%%d(%%Y%%m%%d).#3s.log\"", 
            log_file_dir, log_file_dir);
    wsize = write(fd, new_line, strlen(new_line));
    if (wsize < 0) {
        fprintf(stderr, "make_config: write error, %s\n", new_line);
        return -1;
    }
    wsize = write(fd, "\n", strlen("\n"));
    if (wsize < 0) {
        fprintf(stderr, "make_config: write error\n");
        return -1;
    }

    close(fd);

    return 0;
}

void my_zlog_info(Logger *logger, const char *fmt)
{
    if (!logger->log_on || !s_init_zlog_success) {
        return;
    }

    zlog_info(logger->zc, fmt);
}

void my_zlog_debug(Logger *logger, const char *fmt)
{
    if (!logger->log_on || !s_init_zlog_success) {
        return;
    }

    zlog_debug(logger->zc, fmt);
}

void my_zlog_error(Logger *logger, const char *fmt)
{
    if (!logger->log_on || !s_init_zlog_success) {
        return;
    }

    zlog_error(logger->zc, fmt);
}

int logger_init(Logger *logger, 
                 int mode, 
                 const char *log_conf_path, 
                 const char *log_file_dir)
{
    int rc;
    zlog_category_t *zc;

    logger->log_i = my_zlog_info;
    logger->log_d = my_zlog_debug;
    logger->log_e = my_zlog_error;

    int ret = make_config(log_conf_path, log_file_dir);
    if (ret < 0) {
        goto ERROR;
    }

    rc = zlog_init(log_conf_path);
	if (rc) {
        fprintf(stderr, "logger_init: zlog_init error\n");
        goto ERROR;
	}

    if (mode == LOG_STDOUT) {
        zc = zlog_get_category("stdout");
    } else if (mode == LOG_FILE) {
        zc = zlog_get_category("file");
    }

    if (!zc) {
        fprintf(stderr, "logger_init: zlog_get_category error\n");
        goto ERROR;
    }

    logger->zc = zc;
    logger->log_mode = mode;
    logger->log_on = 1;

    s_init_zlog_success = 1;

    return 0;

ERROR:
    logger_destroy(logger);

    return -1;
}

void logger_destroy(Logger *logger)
{
    if (logger != NULL) {
        zlog_fini();
    }
}
