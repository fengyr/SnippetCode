/*
 * =====================================================================================
 *
 *       Filename:  modbus_master.h
 *
 *    Description:  
 *
 *        Version:  
 *        Created:  十月 04, 2014
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Zenki (Zenki.J.Zha), zenki2001cn@163.com
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef _modbus_master_H_
#define _modbus_master_H_

#ifdef USE_MODBUS

#ifdef __cplusplus
extern "C" {
#endif

#include "modbus.h"

/* 
 * Modbus RTU Frame controller::
 * 
 *         [ Start Wait ] [Address ][ Function Code] [ Data ][ CRC ][  End Wait  ]
 *           3.5 chars     1b         1b               Nb      2b      3.5 chars
 * 
 *     Wait refers to the amount of time required to transmist at least x many
 *     characters.  In this case it is 3.5 characters.  Also, if we recieve a
 *     wait of 1.5 characters at any point, we must trigger an error message.
 *     Also, it appears as though this message is little endian. The logic is
 *     simplified as the following::
 * 
 *         block-on-read:
 *             read until 3.5 delay
 *             check for errors
 *             decode
 * 
 *     The following table is a listing of the baud wait times for the specified
 *     baud rates::
 * 
 *         ------------------------------------------------------------------
 *          Baud  1.5c (18 bits)   3.5c (38 bits)
 *         ------------------------------------------------------------------
 *          1200   13333.3 us       31666.7 us
 *          4800    3333.3 us        7916.7 us
 *          9600    1666.7 us        3958.3 us
 *         19200     833.3 us        1979.2 us
 *         38400     416.7 us         989.6 us
 *         ------------------------------------------------------------------
 *         1 Byte = start + 8 bits + parity + stop = 11 bits
 *         (1/Baud)(bits) = delay seconds
 *  */

typedef struct modbus_config_t {
    int                         debug_mode;         // 调试模式
    uint32_t                    res_timeout_sec;    // 响应超时时间，单位秒
    uint32_t                    res_timeout_usec;   // 响应超时时间，单位微妙
    uint32_t                    byte_timeout_sec;   // 字节超时时间，单位秒
    uint32_t                    byte_timeout_usec;  // 字节超时时间，单位微妙
    modbus_error_recovery_mode  recovery_mode;      // 恢复模式
} AppeModbusConfig, *PAppeModbusConfig;

typedef struct modbus_master_t {
    modbus_t        *ctx;
    AppeModbusConfig    config;

    /* 读多个线圈，功能码0x01，返回读入的字节数，否则返回-1 */
    int (*read_bits)(struct modbus_master_t *modbus_m, 
                     int addr, int nb, uint8_t *dest);

    /* 写单个线圈，功能码0x05，返回写入的字节数，否则返回-1 */
    int (*write_bit)(struct modbus_master_t *modbus_m, 
                     int coil_addr, int status);

    /* 写多个线圈，功能码0x0F，返回写入的字节数，否则返回-1 */
    int (*write_bits)(struct modbus_master_t *modbus_m, 
                      int addr, int nb, const uint8_t *data);

    /* 读输入离散量，功能码0x02，返回读取的数量，否则返回-1 */
    int (*read_input_bits)(struct modbus_master_t *modbus_m, 
                           int addr, int nb, uint8_t *dest);

    /* 写单个寄存器，功能码0x06，成功返回1，否则返回-1 */
    int (*write_register)(struct modbus_master_t *modbus_m, 
                          int reg_addr, int value);

    /* 读取多个寄存器，功能码0x03，返回读取的数量，否则返回-1 */
    int (*read_registers)(struct modbus_master_t *modbus_m, 
                          int addr, int nb, uint16_t *dest);

    /* 写多个寄存器，功能码0x10，返回写入的数量，否则返回-1 */
    int (*write_registers)(struct modbus_master_t *modbus_m, 
                           int addr, int nb, const uint16_t *data);

    /* 读写多个寄存器，功能码0x17，返回读取的数量，否则返回-1 */
    int (*write_and_read_registers)(struct modbus_master_t *modbus_m, 
                                    int write_addr, int write_nb,
                                    const uint16_t *src, int read_addr, 
                                    int read_nb, uint16_t *dest);

    /* 读输入寄存器，功能码0x04，返回读取的数量，否则返回-1 */
    int (*read_input_registers)(struct modbus_master_t *modbus_m, 
                                int addr, int nb, uint16_t *dest);

    /* 浮点类型转换 */
    void (*set_float)(float src, uint16_t *dest);
    float (*get_float)(uint16_t *src);
    void (*set_float_dcba)(float src, uint16_t *dest);
    float (*get_float_dcba)(uint16_t *src);
} AppeModbusMaster, *PAppeModbusMaster;

/* 创建AppeModbusMaster */
AppeModbusMaster* appe_create_modbus_master_rtu(const char *serial_name, 
                                       int baudrate, int databit, 
                                       char parity, int stopbit,
                                       int slave_addr,
                                       AppeModbusConfig *config);

AppeModbusMaster* appe_create_modbus_master_tcp(const char *ip_addr, 
                                       int ip_port,
                                       AppeModbusConfig *config);

/* 释放AppeModbusMaster资源 */
void appe_free_modbus_master(AppeModbusMaster *modbus_m);

#ifdef __cplusplus
}
#endif

#endif

#endif /* end of include guard: _modbus_master_H_ */
