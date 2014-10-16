/*
 * =====================================================================================
 *
 *       Filename:  modbus_func_m.c
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

int mb_read_coils(unsigned char *pframe, unsigned short *len)
{
}

int mb_read_input_register(unsigned char *pframe, unsigned short *len)
{
}

int mb_read_holding_register(unsigned char *pframe, unsigned short *len)
{
}

int mb_read_discrete_inputs(unsigned char *pframe, unsigned short *len)
{
}

int mb_write_coils(unsigned char *pframe, unsigned short *len)
{
}

int mb_write_multi_coils(unsigned char *pframe, unsigned short *len)
{
}

int mb_write_holding_register(unsigned char *pframe, unsigned short *len)
{
}

int mb_write_multi_holding_register(unsigned char *pframe, unsigned short *len)
{
}

int mb_read_write_multi_holding_register(unsigned char *pframe, unsigned short *len)
{
}


#ifdef __cplusplus
}
#endif

#endif /* end of include guard: _modbus_func_m_H_ */
