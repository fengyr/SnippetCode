/*
 * =====================================================================================
 *
 *       Filename:  devices.h
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

#ifndef _devices_H_
#define _devices_H_

#ifdef __cplusplus
extern "C" {
#endif

struct hw_module_t;
struct hw_module_methods_t;
struct hw_device_t;

/**
 * @Synopsis 抽象模块对象，包含模块的通用属性。
 * 其中包含一个模块方法，用于得到设备对象。
 */
typedef struct hw_module_t {
    unsigned int    tag;                // 模块标识
    unsigned char   version_major;      // 主版本号
    unsigned char   version_minor;      // 次版本号
    const char *id;                     // 模块ID，根据该ID搜索模块列表
    const char *name;                   // 模块名称
    const char *author;                 // 模块作者
    struct hw_module_methods_t *methods;
    void *dso;                          // dlopen handle 
    unsigned int reserved[24];          // 预留
} HwModule, *PHwModule;

/**
 * @Synopsis 模块方法，其中默认的open方法可以得到
 * 一个设备对象。
 */
typedef struct hw_module_methods_t {
    int (*__init)(const struct hw_module_t *module, const char *id, 
                  struct hw_device_t **device);
} HwModuleMethod, *PHwModuleMethod;

typedef struct hw_device_t {
    unsigned int tag;                   // 设备标识
    unsigned int version;               // 设备版本
    const struct hw_module_t *module;
    unsigned int reserved[24];          // 预留
    int (*__exit)(struct hw_device_t *device);
} HwDevice, *PHwDevice;

//////////////////////////////////////////////////////
//          public interface                        //
//////////////////////////////////////////////////////
#define MODULE_COMMON_TAG           10
#define DEVICE_COMMON_TAG           11

#define HW_MODULE_INFO_SYM          HMI
#define HW_MODULE_INFO_SYM_AS_STR   "HMI"

#undef offsetof
#ifdef __compiler_offsetof
#define offsetof(TYPE,MEMBER) __compiler_offsetof(TYPE,MEMBER)
#else
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#endif

#define container_of(ptr, type, member) ({  \
    const typeof(((type *)0)->member ) *__mptr = (ptr); \
    (type *)((char *)__mptr - offsetof(type, member));})

/**
 * @Synopsis 根据模块ID获取模块对象。
 *
 * @Param id
 * @Param module
 *
 * @Returns 成功获取返回0，没有找到可用模块返回-2。
 */
int hw_get_module(const char *id, const char *so_path, struct hw_module_t **module);

int dump_module_info(struct hw_module_t *module);
int dump_device_info(struct hw_device_t *device);

#ifdef __cplusplus
}
#endif

#endif /* end of include guard: _devices_H_ */
