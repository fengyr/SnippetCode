/*
 * =====================================================================================
 *
 *       Filename:  message.h
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
#ifndef _message_H_
#define _message_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "message_handler.h"

struct message_t {
    int     id;
    int     arg1;
    int     arg2;
    void    *obj;

    struct message_handler_t *handler;  
};
typedef struct message_t Message, *PMessage;

void* create_message(int msg_id, int arg1, int arg2, void *obj);
void* create_empty_message(int msg_id);
void free_message(void *msg);

#ifdef __cplusplus
}
#endif

#endif /* end of include guard: _message_H_ */
