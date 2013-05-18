/*
 * =====================================================================================
 *
 *       Filename:  runtime.h
 *
 *    Description:  
 *
 *        Version:  
 *        Created:  五月 18, 2013
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Zenki (Zenki.J.Zha), zenki2001cn@163.com
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef _runtime_H_
#define _runtime_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "app.h"

int on_app_create(struct app_runtime_t *app);
int on_app_destroy(struct app_runtime_t *app);

#ifdef __cplusplus
}
#endif

#endif /* end of include guard: _runtime_H_ */
