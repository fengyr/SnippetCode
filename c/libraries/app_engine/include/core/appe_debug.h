/*
 * =====================================================================================
 *
 *       Filename:  debug.h
 *
 *    Description:  
 *
 *        Version:  
 *        Created:  五月 08, 2013
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Zenki (Zenki.J.Zha), zenki2001cn@163.com
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef _debug_H_
#define _debug_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdarg.h>

#undef DEBUG                
#undef BUG_REPORT
#undef ASSERT_TRUE

#ifdef USE_DEBUG
#define DEBUG(fmt, args...) fprintf(stderr, fmt, ## args)

#define BUG_REPORT(_cond, _format, _args ...) \
    printf("\nLine %d: assertion error for '%s': " _format "\n", __LINE__, # _cond, ## _args)

#define ASSERT_TRUE(_cond, _format, __args...) {  \
    if (_cond) {                                  \
        printf("OK\n");                           \
    } else {                                      \
        BUG_REPORT(_cond, _format, ## __args);    \
    }                                             \
};
#else
#define DEBUG(fmt, args...)

#define BUG_REPORT(_cond, _format, _args ...)

#define ASSERT_TRUE(_cond, _format, __args...)
#endif

#ifdef __cplusplus
}
#endif

#endif /* end of include guard: _debug_H_ */
