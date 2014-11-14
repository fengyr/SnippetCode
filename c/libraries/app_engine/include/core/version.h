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
 *   TODO:
 *      0.新建项目时，各文件的耦合关系，特别是interface，Option.
 *      1.消息机制支持延迟功能。
 *      2.TCP支持多种模式，包括poll、epoll等。
 *      3.网络协议独立到protocol模块中。
 *      4.支持外部模块的自定义配置。
 *      5.线程的任务模型。
 *      6.XML读写模块。
 *
 *  0.4.9.B:
 *      1.调整Options结构。
 *
 *  0.4.9:
 *      1.加入sqlite支持。
 *      2.调整目录结构，拆分多个模块，可分开编译。
 *
 *  0.4.8.B:
 *      1.加入simpleini库，优化Options模块，实现INI可读写。
 *      2.调整头文件包含关系。
 *
 *  0.4.8:
 *      1.修复TcpServerGroups和TcpSlaveGroups中调用hashmap造成的空指针问题。
 *      2.测试用例增加创建多个TcpServer和TcpSlave。
 *
 *  0.4.7:
 *      1.加入tcp_slave线程接收和recv方法。
 *
 *  0.4.6:
 *      1.加入modbus协议，支持RTU和TCP模式。
 *      2.modbus暂时支持主机(master)方式。
 *
 *  0.4.5:
 *      1.采用动态方式加载模块。
 *
 *  0.4.4:
 *      1.添加一些注释，优化部分代码结构。
 *      2.添加设备模块的定义，目前支持静态添加。
 *
 *  0.4.3:
 *      1.更新zlog至1.2.12.
 *      2.修复MysqlClient不正确释放，导致的段错误。
 *      3.修复Socket不正确释放，导致的段错误。
 *      4.添加mysql查询数据、执行SQL语句功能。
 *
 *  0.4.2:
 *      1.服务端加入mobile数据请求类型。
 *
 *  0.4.1:
 *      1.添加部分注释。
 *      2.优化部分目录结构。
 *
 *  0.4.0:
 *      1.优化网络服务的代码，加入groups机制。
 *      2.将网络处理优化成可注册的形式。
 *      3.加入抽象数据库，支持Mysql。
 *      4.提供解析Options接口，减少程序依赖性。
 *
 *  0.3.5:
 *      1.添加SIGSEGV信号处理。
 *
 *  0.3.4:
 *      1.添加tcp_slave_groups模块.
 *      2.优化tcp_slave模块.
 *      3.添加客户端连接的IP和端口信息.
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

#define APP_VERSION "0.4.9.B"

#ifdef __cplusplus
}
#endif

#endif /* end of include guard: _version_H_ */
