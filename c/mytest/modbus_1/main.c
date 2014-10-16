/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  
 *
 *        Version:  
 *        Created:  十月 03, 2014
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Zenki (Zenki.J.Zha), zenki2001cn@163.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include "modbus.h"

#define BUG_REPORT(_cond, _format, _args ...) \
    printf("\nLine %d: assertion error for '%s': " _format "\n", __LINE__, # _cond, ## _args)

#define ASSERT_TRUE(_cond, _format, __args...) {  \
    if (_cond) {                                  \
        printf("OK\n");                           \
    } else {                                      \
        BUG_REPORT(_cond, _format, ## __args);    \
        goto close;                               \
    }                                             \
};

int main(int argc, const char *argv[])
{
    modbus_t *ctx = NULL;
    uint32_t old_response_to_sec;
    uint32_t old_response_to_usec;
    uint32_t new_response_to_sec;
    uint32_t new_response_to_usec;
    uint32_t old_byte_to_sec;
    uint32_t old_byte_to_usec;

    uint8_t *tab_rp_bits = NULL;
    uint16_t *tab_rp_registers = NULL;
    int nb_points;
    int i;
    int rc;


    /* 创建一个modbus_tcp主机 */
    ctx = modbus_new_tcp("127.0.0.1", 1502);
    if (ctx == NULL) {
        fprintf(stderr, "Unable to allocate libmodbus context\n");
        return -1;
    }

    /* 设置调试模式 */
    modbus_set_debug(ctx, TRUE);
    /* 设置恢复模式 */
    modbus_set_error_recovery(ctx,
                              MODBUS_ERROR_RECOVERY_LINK |
                              MODBUS_ERROR_RECOVERY_PROTOCOL);

    /* 设置从站地址 */
    modbus_set_slave(ctx, 17);

    /* 设置响应超时 */
    modbus_get_response_timeout(ctx, &old_response_to_sec, &old_response_to_usec);
    if (modbus_connect(ctx) == -1) {
        fprintf(stderr, "Connection failed: %s\n", modbus_strerror(errno));
        modbus_free(ctx);
        return -1;
    }
    modbus_get_response_timeout(ctx, &new_response_to_sec, &new_response_to_usec);

    /* 创建内存 */
    tab_rp_bits = (uint8_t *) malloc(0x25 * sizeof(uint8_t));
    memset(tab_rp_bits, 0, 0x25 * sizeof(uint8_t));

    tab_rp_registers = (uint16_t *) malloc(0x3 * sizeof(uint16_t));
    memset(tab_rp_registers, 0, 0x3 * sizeof(uint16_t));

    ///////////////////////////////////////////////////////
    ///                     单个线圈读写                ///
    ///////////////////////////////////////////////////////
    /* 写单个线圈，功能码0x05，返回写入的字节数，否则返回-1 */
    printf("\n--------- 写单个线圈 -----------\n");
    rc = modbus_write_bit(ctx, 0x130, 1);
    printf("1/2 modbus_write_bit: ");
    ASSERT_TRUE(rc == 1, "");

    /* 读多个线圈，功能码0x01，返回读入的字节数，否则返回-1 */
    printf("\n--------- 读单个线圈 -----------\n");
    rc = modbus_read_bits(ctx, 0x130, 1, tab_rp_bits);
    printf("2/2 modbus_read_bits: ");
    ASSERT_TRUE(rc == 1, "FAILED (nb points %d)\n", rc);
    ASSERT_TRUE(tab_rp_bits[0] == 1, "FAILED (%0X != %0X)\n",
                tab_rp_bits[0], 1);

    ///////////////////////////////////////////////////////
    ///                     多个线圈读写                ///
    ///////////////////////////////////////////////////////
    printf("\n--------- 写多个线圈 -----------\n");
    uint8_t tab_value[0x25];
    const uint8_t UT_BITS_TAB[] = { 0xCD, 0x6B, 0xB2, 0x0E, 0x1B };
    uint8_t value;

    {
        /* 自定义需要写入的多字节数组 */
        modbus_set_bits_from_bytes(tab_value, 0, 0x25, UT_BITS_TAB);

        /* 写多个线圈，功能码0x0F，返回写入的字节数，否则返回-1 */
        rc = modbus_write_bits(ctx, 0x130, 0x25, tab_value);
        printf("1/2 modbus_write_bits: ");
        ASSERT_TRUE(rc == 0x25, "rc=%d\n", rc);
    }

    printf("\n--------- 读多个线圈 -----------\n");
    /* 读多个线圈，功能码0x01，返回读入的字节数，否则返回-1 */
    rc = modbus_read_bits(ctx, 0x130, 0x25, tab_rp_bits);
    printf("2/2 modbus_read_bits: ");
    ASSERT_TRUE(rc == 0x25, "FAILED (nb points %d)\n", rc);

    i = 0;
    nb_points = 0x25;
    while (nb_points > 0) {
        int nb_bits = (nb_points > 8) ? 8 : nb_points;

        value = modbus_get_byte_from_bits(tab_rp_bits, i*8, nb_bits);
        ASSERT_TRUE(value == UT_BITS_TAB[i], "FAILED (%0X != %0X)\n",
                    value, UT_BITS_TAB[i]);

        nb_points -= nb_bits;
        i++;
    }

    ///////////////////////////////////////////////////////
    ///                     离散量输入                  ///
    ///////////////////////////////////////////////////////
    printf("\n--------- 离散量输入 -----------\n");
    const uint8_t UT_INPUT_BITS_TAB[] = { 0xAC, 0xDB, 0x35 };

    /* 读输入离散量，功能码0x02，返回读取的数量，否则返回-1 */
    rc = modbus_read_input_bits(ctx, 0x1C4, 0x16, tab_rp_bits);
    printf("1/1 modbus_read_input_bits: ");
    ASSERT_TRUE(rc == 0x16, "FAILED (nb points %d)\n", rc);

    i = 0;
    nb_points = 0x16;
    while (nb_points > 0) {
        int nb_bits = (nb_points > 8) ? 8 : nb_points;
        value = modbus_get_byte_from_bits(tab_rp_bits, i*8, nb_bits);
        ASSERT_TRUE(value == UT_INPUT_BITS_TAB[i], "FAILED (%0X != %0X)\n",
                    value, UT_INPUT_BITS_TAB[i]);

        nb_points -= nb_bits;
        i++;
    }

    ///////////////////////////////////////////////////////
    ///                     保持寄存器                  ///
    ///////////////////////////////////////////////////////
    printf("\n--------- 写单个寄存器 -----------\n");
    /* 写单个寄存器，功能码0x06，成功返回1，否则返回-1 */
    rc = modbus_write_register(ctx, 0x16B, 0x1234);
    printf("1/2 modbus_write_register: ");
    ASSERT_TRUE(rc == 1, "");

    /* 读取多个寄存器，功能码0x03，返回读取的数量，否则返回-1 */
    printf("\n--------- 读多个寄存器 -----------\n");
    rc = modbus_read_registers(ctx, 0x16B, 1, tab_rp_registers);
    printf("2/2 modbus_read_registers: ");
    ASSERT_TRUE(rc == 1, "FAILED (nb points %d)\n", rc);
    ASSERT_TRUE(tab_rp_registers[0] == 0x1234, "FAILED (%0X != %0X)\n",
                tab_rp_registers[0], 0x1234);

    printf("\n--------- 写多个寄存器 -----------\n");
    const uint16_t UT_REGISTERS_TAB[] = { 0x022B, 0x0001, 0x0064 };

    /* 写多个寄存器，功能码0x10，返回写入的数量，否则返回-1 */
    rc = modbus_write_registers(ctx, 0x16B, 0x3, UT_REGISTERS_TAB);
    printf("1/5 modbus_write_registers: ");
    ASSERT_TRUE(rc == 0x3, "");

    /* 读取多个寄存器，功能码0x03，返回读取的数量，否则返回-1 */
    rc = modbus_read_registers(ctx, 0x16B, 0x3, tab_rp_registers);
    printf("2/5 modbus_read_registers: ");
    ASSERT_TRUE(rc == 0x3, "FAILED (nb points %d)\n", rc);

    for (i=0; i < 0x3; i++) {
        ASSERT_TRUE(tab_rp_registers[i] == UT_REGISTERS_TAB[i],
                    "FAILED (%0X != %0X)\n",
                    tab_rp_registers[i], UT_REGISTERS_TAB[i]);
    }

    rc = modbus_read_registers(ctx, 0x16B, 0, tab_rp_registers);
    printf("3/5 modbus_read_registers (0): ");
    ASSERT_TRUE(rc == -1, "FAILED (nb_points %d)\n", rc);

    printf("\n--------- 读写多个寄存器 -----------\n");
    /* 读写多个寄存器，功能码0x17，返回读取的数量，否则返回-1 */
    nb_points = (0x3 > 0x1) ?  0x3 : 0x1;
    memset(tab_rp_registers, 0, nb_points * sizeof(uint16_t));
    rc = modbus_write_and_read_registers(ctx,
                                         0x16B + 1,
                                         0x3 - 1,
                                         tab_rp_registers,
                                         0x16B,
                                         0x3,
                                         tab_rp_registers);
    printf("4/5 modbus_write_and_read_registers: ");
    ASSERT_TRUE(rc == 0x3, "FAILED (nb points %d != %d)\n", rc, 0x3);

    ASSERT_TRUE(tab_rp_registers[0] == UT_REGISTERS_TAB[0],
                "FAILED (%0X != %0X)\n",
                tab_rp_registers[0], UT_REGISTERS_TAB[0]);

    for (i=1; i < 0x3; i++) {
        ASSERT_TRUE(tab_rp_registers[i] == 0, "FAILED (%0X != %0X)\n",
                    tab_rp_registers[i], 0);
    }

    ///////////////////////////////////////////////////////
    ///                     输入寄存器                  ///
    ///////////////////////////////////////////////////////
    printf("\n--------- 读输入寄存器 -----------\n");
    const uint16_t UT_INPUT_REGISTERS_TAB[] = { 0x000A };
    const float UT_REAL = 916.540649;
    const uint32_t UT_IREAL = 0x4465229a;
    const uint32_t UT_IREAL_DCBA = 0x9a226544;
    uint32_t ireal;
    float real;

    /* 读输入寄存器，功能码0x04，返回读取的数量，否则返回-1 */
    rc = modbus_read_input_registers(ctx, 0x108, 0x01, tab_rp_registers);
    printf("1/1 modbus_read_input_registers: ");
    ASSERT_TRUE(rc == 0x01, "FAILED (nb points %d)\n", rc);

    for (i=0; i < 0x01; i++) {
        ASSERT_TRUE(tab_rp_registers[i] == UT_INPUT_REGISTERS_TAB[i],
                    "FAILED (%0X != %0X)\n",
                    tab_rp_registers[i], UT_INPUT_REGISTERS_TAB[i]);
    }

    printf("\n--------- float类型测试 -----------\n");
    printf("1/4 Set float: ");
    modbus_set_float(UT_REAL, tab_rp_registers);
    if (tab_rp_registers[1] == (UT_IREAL >> 16) &&
        tab_rp_registers[0] == (UT_IREAL & 0xFFFF)) {
        printf("OK\n");
    } else {
        /* Avoid *((uint32_t *)tab_rp_registers)
         * https://github.com/stephane/libmodbus/pull/104 */
        ireal = (uint32_t) tab_rp_registers[0] & 0xFFFF;
        ireal |= (uint32_t) tab_rp_registers[1] << 16;
        printf("FAILED (%x != %x)\n", ireal, UT_IREAL);
        goto close;
    }

    printf("2/4 Get float: ");
    real = modbus_get_float(tab_rp_registers);
    ASSERT_TRUE(real == UT_REAL, "FAILED (%f != %f)\n", real, UT_REAL);

    printf("3/4 Set float in DBCA order: ");
    modbus_set_float_dcba(UT_REAL, tab_rp_registers);
    ireal = (uint32_t) tab_rp_registers[0] & 0xFFFF;
    ireal |= (uint32_t) tab_rp_registers[1] << 16;
    ASSERT_TRUE(tab_rp_registers[1] == (UT_IREAL_DCBA >> 16) &&
                tab_rp_registers[0] == (UT_IREAL_DCBA & 0xFFFF),
                "FAILED (%x != %x)\n", ireal, UT_IREAL_DCBA);

    printf("4/4 Get float in DCBA order: ");
    real = modbus_get_float_dcba(tab_rp_registers);
    ASSERT_TRUE(real == UT_REAL, "FAILED (%f != %f)\n", real, UT_REAL);

    ///////////////////////////////////////////////////////
    ///                     超时设置                    ///
    ///////////////////////////////////////////////////////
    printf("\n--------- 超时测试 -----------\n");
    modbus_get_response_timeout(ctx, &old_response_to_sec, &old_response_to_usec);
    modbus_get_byte_timeout(ctx, &old_byte_to_sec, &old_byte_to_usec);

    modbus_set_response_timeout(ctx, 0, 600000);
    rc = modbus_read_registers(ctx, 0x6E, 1, tab_rp_registers);
    printf("1/6 Adequate response timeout (0.6s > 0.5s): ");
    ASSERT_TRUE(rc == 1, "");

    modbus_set_byte_timeout(ctx, 0, 0);
    rc = modbus_read_registers(ctx, 0x6E, 1, tab_rp_registers);
    printf("2/6 Disable byte timeout: ");
    ASSERT_TRUE(rc == 1, "");


close:
    /* 释放内存 */
    free(tab_rp_bits);
    free(tab_rp_registers);

    /* 关闭连接 */
    modbus_close(ctx);
    modbus_free(ctx);
    
    return 0;
}
