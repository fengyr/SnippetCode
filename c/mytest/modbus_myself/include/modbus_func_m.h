/*
 * =====================================================================================
 *
 *       Filename:  modbus_func_m.h
 *
 *    Description:  
 *
 *        Version:  
 *        Created:  九月 26, 2014
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Zenki (Zenki.J.Zha), zenki2001cn@163.com
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef _modbus_func_m_H_
#define _modbus_func_m_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "modbus_pro.h"

/* 读线圈 */
int mb_read_coils(unsigned char *pframe, unsigned short *len);
/* 读输入寄存器 */
int mb_read_input_register(unsigned char *pframe, unsigned short *len);
/* 读保持寄存器 */
int mb_read_holding_register(unsigned char *pframe, unsigned short *len);
/* 读离散量输入寄存器 */
int mb_read_discrete_inputs(unsigned char *pframe, unsigned short *len);

/* 写线圈 */
int mb_write_coils(unsigned char *pframe, unsigned short *len);
/* 写多个线圈 */
int mb_write_multi_coils(unsigned char *pframe, unsigned short *len);
/* 写保持寄存器 */
int mb_write_holding_register(unsigned char *pframe, unsigned short *len);
/* 写多个保持寄存器 */
int mb_write_multi_holding_register(unsigned char *pframe, unsigned short *len);
/* 读写多个保持寄存器 */
int mb_read_write_multi_holding_register(unsigned char *pframe, unsigned short *len);

#ifdef __cplusplus
}
#endif

#endif /* end of include guard: _modbus_func_m_H_ */
