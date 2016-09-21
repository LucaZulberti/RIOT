/*
 * Copyright (C) 2016 @@@@@@@@@@PHYTEC Messtechnik GmbH@@@@@@@@@@@@@@
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 *
 */

/**
 * @ingroup     drivers_tsl256xt
 * @{
 *
 * @file
 * @brief       Driver for the TSL256X Light to digital converter.
 *
 * @author      Luca Zulberti <zulberti.luca@gmail.com>
 *
 * @}
 */

#include <stdint.h>
#include <stdbool.h>
#include "periph/i2c.h"
#include "tsl256x.h"
#include "tsl256x_reg.h"

#define ENABLE_DEBUG    (1)
#include "debug.h"

#define I2C_SPEED                  I2C_SPEED_FAST

int tsl256x_test(tsl256x_t *dev)
{
    char reg;

    /* Acquire exclusive access to the bus. */
    i2c_acquire(dev->i2c);
	
    reg = 0x03;
    if (i2c_write_reg(dev->i2c, dev->addr, TSL256X_COMMAND_CMD | TSL256X_CONTROL, reg) != 1) {
        /* Release the bus for other threads. */
        DEBUG("tsl256x.c: setting power on failed\n");
        i2c_release(dev->i2c);                
        return -1;                       
    }
    
    /* Verify power status */
    i2c_read_reg(dev->i2c, dev->addr, TSL256X_COMMAND_CMD | TSL256X_CONTROL, &reg);
    if (reg != 0x03) {
        /* Release the bus for other threads. */
        DEBUG("tsl256x.c: power on failed (0x%0x)\n", reg);
        i2c_release(dev->i2c);                
        return -1;                       
    }

    /* read ID */
    if (i2c_read_reg(dev->i2c, dev->addr, TSL256X_COMMAND_CMD | TSL256X_ID_REG, &reg) != 1) {
        /* Release the bus for other threads. */
        DEBUG("tsl256x.c: read ID failed\n");
        i2c_release(dev->i2c);
        return -1;
    }
    i2c_release(dev->i2c);

    reg >>= 4;
    if (reg != TSL2560CS_ID || reg != TSL2560FN_ID || reg != TSL2561CS_ID || reg != TSL2561FN_ID) {
        DEBUG("tsl256x.c: unknown ID (0x%x)\n", reg);
       // return -1;
    }

    return 0;
}

int tsl256x_init(tsl256x_t *dev, i2c_t i2c, tsl256x_set_t *settings)
{
    char reg;

    /* writing device descriptor */
    dev->i2c = i2c;
    dev->addr = TSL256X_I2C_ADDRESS;
    dev->initialized = false;

    DEBUG("\ntsl12561.c: i2C address= 0x%x\n", dev->addr);

    /* controlling settings structure */
    if (settings->integ > 0x02 || settings->int_mode > 0x03 || \
            settings->int_occur > 0x0f || settings->gain > 1)
        return -1;
    
    /* a callback function is needed if interrupt is enabled */
    if (settings->int_mode != 0 && settings->cb_fnc == NULL)
        return -1;

    i2c_acquire(dev->i2c);
    /* initialize the I2C bus */
    if (i2c_init_master(i2c, I2C_SPEED) < 0) {
        i2c_release(dev->i2c);
        return -2;
    }
    i2c_release(dev->i2c);

    /* sensor test and power on */
    if (tsl256x_test(dev))
        return -3;

    /* start configuration procedure */
    i2c_acquire(dev->i2c);
    
    /* timing settings */
    reg = (settings->gain == 1 ? TSL256X_TIMING_GAIN : 0) | \
          (settings->integ && TSL256X_TIMING_INTEG_MASK);
    if (i2c_write_reg(dev->i2c, dev->addr, TSL256X_COMMAND_CMD | TSL256X_TIMING, reg) != 1) {
        i2c_release(dev->i2c);
        return -5;
    }

    if (settings->int_mode == 0)
        goto end_conf; /* no other configurations are needed */

    /* interrupt configuration TODO */

end_conf:
    i2c_release(dev->i2c);

    dev->initialized = true;

    return 0;
}

int tsl256x_read(tsl256x_t *dev, tsl256x_data_t *data, uint8_t ch)
{
    char  buf[2];

    if (dev->initialized == false)
        return -1;

    if (ch > 1 || data == NULL)
        return -2;

    i2c_acquire(dev->i2c);
    if (i2c_read_regs(dev->i2c, dev->addr, TSL256X_COMMAND_CMD | \
                TSL256X_COMMAND_WORD | \
                (ch ? TSL256X_DATA1LOW : TSL256X_DATA0LOW), \
                buf, 2) != 2) {
        i2c_release(dev->i2c);
        return -1;
    }
    i2c_release(dev->i2c);

    data->data8.low = buf[0];
    data->data8.high = buf[1];

    return 0;
}

