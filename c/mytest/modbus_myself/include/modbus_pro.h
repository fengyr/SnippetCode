/*
 * =====================================================================================
 *
 *       Filename:  modbus_pro.h
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
#ifndef _modbus_pro_H_
#define _modbus_pro_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
 * <------------------------ MODBUS SERIAL LINE PDU (1) ------------------->
 *              <----------- MODBUS PDU (1') ---------------->
 *  +-----------+---------------+----------------------------+-------------+
 *  | Address   | Function Code | Data                       | CRC/LRC     |
 *  +-----------+---------------+----------------------------+-------------+
 *  |           |               |                                   |
 * (2)        (3/2')           (3')                                (4)
 *
 * (1)  ... MB_SER_PDU_SIZE_MAX = 256
 * (2)  ... MB_SER_PDU_ADDR_OFF = 0
 * (3)  ... MB_SER_PDU_PDU_OFF  = 1
 * (4)  ... MB_SER_PDU_SIZE_CRC = 2
 *
 * (1') ... MB_PDU_SIZE_MAX     = 253
 * (2') ... MB_PDU_FUNC_OFF     = 0
 * (3') ... MB_PDU_DATA_OFF     = 1
 */

/* ----------------------- Defines ------------------------------------------*/
#define MB_PDU_SIZE_MAX     253 /*!< Maximum size of a PDU. */
#define MB_PDU_SIZE_MIN     1   /*!< Function Code */
#define MB_PDU_FUNC_OFF     0   /*!< Offset of function code in PDU. */
#define MB_PDU_DATA_OFF     1   /*!< Offset for response data in PDU. */

/* modbus address */
#define MB_ADDRESS_BROADCAST                    ( 0 )       /*! Modbus broadcast address. */
#define MB_ADDRESS_MIN                          ( 1 )       /*! Smallest possible slave address. */
#define MB_ADDRESS_MAX                          ( 247 )     /*! Biggest possible slave address. */

/* modbus func code */
#define MB_FUNC_NONE                            (  0 )
#define MB_FUNC_READ_COILS                      (  1 )
#define MB_FUNC_READ_DISCRETE_INPUTS            (  2 )
#define MB_FUNC_WRITE_SINGLE_COIL               (  5 )
#define MB_FUNC_WRITE_MULTIPLE_COILS            ( 15 )
#define MB_FUNC_READ_HOLDING_REGISTER           (  3 )
#define MB_FUNC_READ_INPUT_REGISTER             (  4 )
#define MB_FUNC_WRITE_REGISTER                  (  6 )
#define MB_FUNC_WRITE_MULTIPLE_REGISTERS        ( 16 )
#define MB_FUNC_READWRITE_MULTIPLE_REGISTERS    ( 23 )

/* modbus exception define */
#define ILLEGAL_FUNC                            (0x01) 
#define ILLEGAL_ADDR                            (0x02)
#define ILLEGAL_VAL                             (0x03)

#ifdef __cplusplus
}
#endif

#endif /* end of include guard: _modbus_pro_H_ */
