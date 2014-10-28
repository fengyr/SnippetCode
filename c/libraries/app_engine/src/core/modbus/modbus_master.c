/*
 * =====================================================================================
 *
 *       Filename:  modbus_master.c
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
#include <string.h>
#include <unistd.h>

#include "modbus_master.h"
#include "app.h"

static void set_float(float src, uint16_t *dest)
{
    modbus_set_float(src, dest);
}

static float get_float(uint16_t *src)
{
    return modbus_get_float(src);
}

static void set_float_dcba(float src, uint16_t *dest)
{
    modbus_set_float_dcba(src, dest);
}

static float get_float_dcba(uint16_t *src)
{
    return modbus_get_float_dcba(src);
}

static int read_bits(struct modbus_master_t *modbus_m, 
                        int addr, int nb, uint8_t *dest)
{
    if (!modbus_m) {
        return -1;
    }

    uint8_t *tab_value;
    tab_value = (uint8_t*) malloc(nb*sizeof(uint8_t));

    int rc = modbus_read_bits(modbus_m->ctx, addr, nb, tab_value);

    int i = 0;
    int nb_points = nb;

    while (nb_points > 0) {
        int nb_bits = (nb_points > 8) ? 8 : nb_points;

        dest[i] = modbus_get_byte_from_bits(tab_value, i*8, nb_bits);
        nb_points -= nb_bits;
        i++;
    }

    if (tab_value != NULL) {
        free(tab_value);
        tab_value = NULL;
    }

    return rc;
}

static int write_bit(struct modbus_master_t *modbus_m, 
                        int coil_addr, int status)
{
    if (!modbus_m) {
        return -1;
    }

    int rc = modbus_write_bit(modbus_m->ctx, coil_addr, status);

    return rc;
}

static int write_bits(struct modbus_master_t *modbus_m, 
                         int addr, int nb, const uint8_t *data)
{
    if (!modbus_m) {
        return -1;
    }

    uint8_t *tab_value;
    tab_value = (uint8_t*) malloc(nb*sizeof(uint8_t));
    modbus_set_bits_from_bytes(tab_value, 0, nb, data);

    int rc = modbus_write_bits(modbus_m->ctx, addr, nb, tab_value);

    if (tab_value != NULL) {
        free(tab_value);
        tab_value = NULL;
    }

    return rc;
}

static int read_input_bits(struct modbus_master_t *modbus_m, 
                           int addr, int nb, uint8_t *dest)
{
    if (!modbus_m) {
        return -1;
    }

    uint8_t *tab_value;
    tab_value = (uint8_t*) malloc(nb*sizeof(uint8_t));

    int rc = modbus_read_input_bits(modbus_m->ctx, addr, nb, tab_value);

    int i = 0;
    int nb_points = nb;

    while (nb_points > 0) {
        int nb_bits = (nb_points > 8) ? 8 : nb_points;

        dest[i] = modbus_get_byte_from_bits(tab_value, i*8, nb_bits);
        nb_points -= nb_bits;
        i++;
    }

    if (tab_value != NULL) {
        free(tab_value);
        tab_value = NULL;
    }

    return rc;
}

static int write_register(struct modbus_master_t *modbus_m, 
                             int reg_addr, int value)
{
    if (!modbus_m) {
        return -1;
    }

    int rc = modbus_write_register(modbus_m->ctx, reg_addr, value);

    return rc;
}

static int read_registers(struct modbus_master_t *modbus_m, 
                             int addr, int nb, uint16_t *dest)
{
    if (!modbus_m) {
        return -1;
    }

    int rc = modbus_read_registers(modbus_m->ctx, addr, nb, dest);

    return rc;
}

static int write_registers(struct modbus_master_t *modbus_m, 
                              int addr, int nb, const uint16_t *data)
{
    if (!modbus_m) {
        return -1;
    }

    int rc = modbus_write_registers(modbus_m->ctx, addr, nb, data);

    return rc;
}

static int write_and_read_registers(struct modbus_master_t *modbus_m, 
                                       int write_addr, int write_nb,
                                       const uint16_t *src, int read_addr, 
                                       int read_nb, uint16_t *dest)
{
    if (!modbus_m) {
        return -1;
    }

    int rc = modbus_write_and_read_registers(modbus_m->ctx, 
                                             write_addr, write_nb,
                                             src, 
                                             read_addr, read_nb, 
                                             dest);

    return rc;
}

static int read_input_registers(struct modbus_master_t *modbus_m, 
                                   int addr, int nb, uint16_t *dest)
{
    if (!modbus_m) {
        return -1;
    }

    int rc = modbus_read_input_registers(modbus_m->ctx, addr, nb, dest);

    return rc;
}

static ModbusMaster* __init_modbus_master(modbus_t *ctx, ModbusConfig *config)
{
    const App *app = get_app_instance();
    Logger *logger = app->logger;
    ModbusMaster *master = NULL;

    master = (ModbusMaster*) malloc(sizeof(ModbusMaster));

    // 初始化方法
    master->ctx = ctx;
    master->read_bits = read_bits;
    master->read_input_bits = read_input_bits;
    master->read_input_registers = read_input_registers;
    master->read_registers = read_registers;
    master->write_bit = write_bit;
    master->write_bits = write_bits;
    master->write_register = write_register;
    master->write_registers = write_registers;
    master->write_and_read_registers = write_and_read_registers;
    master->set_float = set_float;
    master->get_float = get_float;
    master->set_float_dcba = set_float_dcba;
    master->get_float_dcba = get_float_dcba;

    if (config != NULL) {
        master->config.debug_mode = config->debug_mode;
        master->config.recovery_mode = config->recovery_mode;
        master->config.res_timeout_sec = config->res_timeout_sec;
        master->config.res_timeout_usec = config->res_timeout_usec;
        master->config.byte_timeout_sec = config->byte_timeout_sec;
        master->config.byte_timeout_usec = config->byte_timeout_usec;

        char buf[256];
        memset(buf, 0, sizeof(buf));
        sprintf(buf, "Modbus: debug_mode = %d.", config->debug_mode);
        logger->log_i(logger, buf);

        memset(buf, 0, sizeof(buf));
        sprintf(buf, "Modbus: recovery_mode = %d.", config->recovery_mode);
        logger->log_i(logger, buf);

        memset(buf, 0, sizeof(buf));
        sprintf(buf, "Modbus: response timeout = %d - %d.", config->res_timeout_sec, config->res_timeout_usec);
        logger->log_i(logger, buf);

        memset(buf, 0, sizeof(buf));
        sprintf(buf, "Modbus: byte timeout = %d - %d.", config->byte_timeout_sec, config->byte_timeout_usec);
        logger->log_i(logger, buf);
    } else { /* 使用默认设置 */
        uint32_t old_res_timeout_sec, old_res_timeout_usec;
        uint32_t old_byte_timeout_sec, old_byte_timeout_usec;

        modbus_get_response_timeout(ctx, &old_res_timeout_sec, &old_res_timeout_usec);
        modbus_get_byte_timeout(ctx, &old_byte_timeout_sec, &old_byte_timeout_usec);

        master->config.debug_mode = TRUE;
        master->config.recovery_mode = MODBUS_ERROR_RECOVERY_NONE;
        master->config.res_timeout_sec = old_res_timeout_sec;
        master->config.res_timeout_usec = old_res_timeout_usec;
        master->config.byte_timeout_sec = old_byte_timeout_sec;
        master->config.byte_timeout_usec = old_byte_timeout_usec;
    }

    /* 设置调试模式 */
    /* printf("========ctx=%p, debug_mode=%d, recovery_mode=%d, res_t1=%d, res_t2=%d, byte_t1=%d, byte_t2=%d\n", 
     *         ctx, master->config.debug_mode, master->config.recovery_mode, master->config.res_timeout_sec, master->config.res_timeout_usec,
     *         master->config.byte_timeout_sec, master->config.byte_timeout_usec); */
    modbus_set_debug(ctx, master->config.debug_mode);
    /* 设置恢复模式 */
    modbus_set_error_recovery(ctx, master->config.recovery_mode);
    /* 设置超时时间 */
    modbus_set_response_timeout(ctx, master->config.res_timeout_sec, master->config.res_timeout_usec);
    modbus_set_byte_timeout(ctx, master->config.byte_timeout_sec, master->config.byte_timeout_usec);

    if (modbus_connect(ctx) == -1) {
        logger->log_e(logger, "Modbus: Connect To Slave Failed.");
        free_modbus_master(master);
        return NULL;
    }
 
    logger->log_i(logger, "Modbus: Connect To Slave Success.");

    return master;

}


ModbusMaster* create_modbus_master_rtu(const char *serial_name, 
                                       int baudrate, int databit, 
                                       char parity, int stopbit,
                                       int slave_addr,
                                       ModbusConfig *config)
{
    const App *app = get_app_instance();
    Logger *logger = app->logger;
    ModbusMaster *master = NULL;

    modbus_t *ctx = modbus_new_rtu(serial_name, baudrate, parity, databit, stopbit);

    if (ctx == NULL) {
        logger->log_e(logger, "Modbus: Create TCP Master Failed.");
        return NULL;
    }

    // 设置从站地址
    modbus_set_slave(ctx, slave_addr);

    // 通用modbus设置
    master = __init_modbus_master(ctx, config);

    return master;

}

ModbusMaster* create_modbus_master_tcp(const char *ip_addr, 
                                       int ip_port,
                                       ModbusConfig *config)
{
    const App *app = get_app_instance();
    Logger *logger = app->logger;
    ModbusMaster *master = NULL;

    modbus_t *ctx = modbus_new_tcp(ip_addr, ip_port);

    if (ctx == NULL) {
        logger->log_e(logger, "Modbus: Create TCP Master Failed.");
        return NULL;
    } else {
    }

    // 通用modbus设置
    master = __init_modbus_master(ctx, config);

    return master;
}

void free_modbus_master(ModbusMaster *modbus_m)
{
    const App *app = get_app_instance();
    Logger *logger = app->logger;

    if (modbus_m != NULL) {
        if (modbus_m->ctx != NULL) {
            modbus_close(modbus_m->ctx);
            modbus_free(modbus_m->ctx);
        }

        free(modbus_m);
        modbus_m = NULL;
        logger->log_i(logger, "Modbus: Free Modbus Master Success.");
    }
}
