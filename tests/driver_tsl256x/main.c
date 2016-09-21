/**
 * @ingroup tests
 * @{
 *
 * @file
 * @brief       Test application for the TSL256X light sensor driver.
 *
 * @author      Luca Zulberti <zulberti.luca@gmail.com>
 *
 * @}
 */

#ifndef TEST_TSL256X_I2C
#error "TEST_TSL256X_I2C not defined"
#endif

#include <stdio.h>

#include "xtimer.h"
#include "tsl256x.h"

#define SLEEP       (1000 * 1000U)

int main(void)
{
    tsl256x_t dev;
    tsl256x_set_t settings;
    tsl256x_data_t data;
    int result;

    puts("TSL256X light sensor driver test application\n");
    printf("Initializing TSL256X light sensor at I2C_%i... ", TEST_TSL256X_I2C);

    settings.thr_high = 0xb000;
    settings.thr_low = 0x3000;
    settings.integ = TSL256X_INTEG_MEDIUM;
    settings.gain = 0; /* 1x gain */
    settings.int_mode = TSL256X_INTERRUPT_DISABLED;
    settings.int_occur = TSL256X_INTERRUPT_ANY;
    settings.cb_fnc = NULL;
	
    result = tsl256x_init(&dev, TEST_TSL256X_I2C, &settings);
    if (result == 0) {
        puts("[OK]\n");
    } else if (result == -3){
        puts("[Failed] (test failed)");
        return -1;
    }

    while (1) {
        tsl256x_read(&dev, &data, 0);
        printf("Visible light (raw): %d\n", data.data16);
        xtimer_usleep(SLEEP);
        tsl256x_read(&dev, &data, 1);
        printf("Infrared light (raw): %d\n", data.data16);
        xtimer_usleep(SLEEP);
    }

    return 0;
}
