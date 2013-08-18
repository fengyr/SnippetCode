/*
 * =====================================================================================
 *
 *       Filename:  version.h
 *
 *    Description:  
 *
 *        Version:  
 *        Created:  六月 19, 2013
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Zenki (Zenki.J.Zha), zenki2001cn@163.com
 *   Organization:  
 *
 *  0.3.3:
 *      1.添加ping协议。
 *      2.优化socket发送和接收的缓冲区大小。
 *      3.去除SIGV的信号处理。
 *      4.完成信号处理后，恢复默认的信号处理方式。
 *
 *  0.3.2:
 *      1.修正tcp服务启动过早，导致无法更新options.
 *
 *  0.3.1:
 *      1.更新libstruct.a。
 *      2.添加zlog日志记录。
 *      3.优化程序结构。
 *      4.修复一些小bugs。
 *
 * =====================================================================================
 */
#ifndef _version_H_
#define _version_H_

#ifdef __cplusplus
extern "C" {
#endif

#define APP_VERSION "0.3.3"

#ifdef __cplusplus
}
#endif

#endif /* end of include guard: _version_H_ */
