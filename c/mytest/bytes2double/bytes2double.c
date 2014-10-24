/*
 * =====================================================================================
 *
 *       Filename:  bytes2float.c
 *
 *    Description:  modbus协议的数据格式处理。float和2个16位类型相互转换。
 *
 *        Version:  
 *        Created:  十月 23, 2014
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Zenki (Zenki.J.Zha), zenki2001cn@163.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdio.h>
#include <stdint.h>
#include <string.h>

static inline uint16_t bswap_16(uint16_t x)
{
    return (x >> 8) | (x << 8);
}

static inline uint32_t bswap_32(uint32_t x)
{
    return (bswap_16(x & 0xffff) << 16) | (bswap_16(x >> 16));
}

static inline uint64_t bswap_64(uint64_t x)
{
    return (bswap_32(x & 0xffffffff) << 32) | (bswap_32(x >> 32));
}

// 小端模式提取float类型
float get_float(const uint16_t *src)
{
    float f;
    uint32_t i;

    i = (((uint32_t)src[1]) << 16) + src[0];
    memcpy(&f, &i, sizeof(float));

    return f;
}

// 大端模式提取float类型
float get_float_dcba(const uint16_t *src)
{
    float f;
    uint32_t i;

    i = bswap_32((((uint32_t)src[1]) << 16) + src[0]);
    memcpy(&f, &i, sizeof(float));

    return f;
}

/* // 小端模式提取float类型
 * float get_float_64(const uint32_t *src)
 * {
 *     float f;
 *     uint64_t i;
 * 
 *     i = (((uint64_t)src[1]) << 32) + src[0];
 *     memcpy(&f, &i, sizeof(float));
 * 
 *     return f;
 * }
 * 
 * // 大端模式提取float类型
 * float get_float_dcba_64(const uint32_t *src)
 * {
 *     float f;
 *     uint64_t i;
 * 
 *     i = bswap_64((((uint64_t)src[1]) << 32) + src[0]);
 *     memcpy(&f, &i, sizeof(float));
 * 
 *     return f;
 * } */

// 小端模式设置float类型
void set_float(float f, uint16_t *dest)
{
    uint32_t i;

    memcpy(&i, &f, sizeof(uint32_t));
    dest[0] = (uint16_t)i;
    dest[1] = (uint16_t)(i >> 16);
}

// 大端模式设置float类型
void set_float_dcba(float f, uint16_t *dest)
{
    uint32_t i;

    memcpy(&i, &f, sizeof(uint32_t));
    i = bswap_32(i);
    dest[0] = (uint16_t)i;
    dest[1] = (uint16_t)(i >> 16);
}

/* void set_float_64(float f, uint32_t *dest)
 * {
 *     uint64_t i;
 * 
 *     memcpy(&i, &f, sizeof(uint64_t));
 *     printf("debug i: %lld\n", i);
 *     dest[0] = (uint32_t)i;
 *     dest[1] = (uint32_t)(i >> 32);
 * }
 * 
 * // 大端模式设置float类型
 * void set_float_dcba_64(float f, uint32_t *dest)
 * {
 *     uint32_t i;
 * 
 *     memcpy(&i, &f, sizeof(uint64_t));
 *     i = bswap_64(i);
 *     dest[0] = (uint32_t)i;
 *     dest[1] = (uint32_t)(i >> 32);
 * } */

int main(int argc, const char *argv[])
{
    float f_src = 101.123456789; 
    float f_dest;
    uint16_t i_dest[2];

    set_float(f_src, i_dest);
    f_dest = get_float(i_dest);
    printf("小端模式: f_src=%.9f, i_dest=%d, f_dest=%.9f\n", f_src, *((uint32_t*)i_dest), f_dest);

    set_float_dcba(f_src, i_dest);
    f_dest = get_float_dcba(i_dest);
    printf("大端模式: f_src=%.9f, i_dest=%d, f_dest=%.9f\n", f_src, *((uint32_t*)i_dest), f_dest);  

/*     float f_src64 = 99910101.1101; 
 *     float f_dest64;
 *     uint32_t i_dest64[2];
 * 
 *     set_float_64(f_src64, i_dest64);
 *     f_dest64 = get_float_64(i_dest64);
 *     printf("小端模式: f_src=%.3f, i_dest=%lld, f_dest=%.3f\n", f_src64, *((uint64_t*)i_dest64), f_dest64);
 * 
 *     set_float_dcba_64(f_src64, i_dest64);
 *     f_dest = get_float_dcba_64(i_dest64);
 *     printf("大端模式: f_src=%.3f, i_dest=%d, f_dest=%.3f\n", f_src64, *((uint32_t*)i_dest64), f_dest64);   */

    return 0;
}
