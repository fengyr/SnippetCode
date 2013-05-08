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

#include <stdio.h>
#include <stdarg.h>

#undef DEBUG                
#ifdef USE_DEBUG
#  define DEBUG(fmt, args...) fprintf(stderr, fmt, ## args)
#else
#  define DEBUG(fmt, args...)
#endif

#endif /* end of include guard: _debug_H_ */
