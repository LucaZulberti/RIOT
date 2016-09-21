/**
 * @defgroup    drivers_tsl256x TSL256X Light Sensor
 * @ingroup     drivers_sensors
 * @brief       Driver for the TSL256X Light to digital converter.
 *              The driver will initialize the ...
 *              After initialization ...
 *              This driver only implements basic functionality.
 *
 * @{
 *
 * @file
 * @brief       Interface definition for the TSL256X light sensor driver.
 *
 * @author      Luca Zulberti <zulberti.luca@gmail.com>
 */

#ifndef TSL256X_H
#define TSL256X_H

#include <stdint.h>
#include <stdbool.h>
#include "periph/i2c.h"

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef TSL256X_I2C_ADDRESS
    #ifdef TSL256X_I2C_ADDRESS_GND
    #define TSL256X_I2C_ADDRESS     0x29
    #elif defined(TSL256X_I2C_ADDRESS_FLOAT)
    #define TSL256X_I2C_ADDRESS     0x39
    #elif defined(TSL256X_I2C_ADDRESS_VDD)
    #define TSL256X_I2C_ADDRESS     0x49
    #else
    #error "Please specify the I2C address using the appropriate #define"
    #endif
#endif /* TSL256X_I2C_ADDRESS */

#define TSL256X_INTEG_LOW          0x00 /**< 13.7ms integration time */
#define TSL256X_INTEG_MEDIUM       0x01 /**< 101ms integration time */
#define TSL256X_INTEG_HIGH         0x02 /**< 402ms integration time */

#define TSL256X_INTERRUPT_DISABLED        0x00
#define TSL256X_INTERRUPT_LEVEL           0x01
#define TSL256X_INTERRUPT_SMBALERT        0x02
#define TSL256X_INTERRUPT_TESTMODE        0x03

#define TSL256X_INTERRUPT_ANY             0x00 /**< Every ADC cycle generates interrupt */
#define TSL256X_INTERRUPT_OUT_THR         0x01 /**< Any value outside of threshold range */
#define TSL256X_INTERRUPT_2_OUT_THR       0x02 /**< 2 integration time periods out of range */
#define TSL256X_INTERRUPT_3_OUT_THR       0x03 /**< 3 integration time periods out of range */
#define TSL256X_INTERRUPT_4_OUT_THR       0x04 /**< 4 integration time periods out of range */
#define TSL256X_INTERRUPT_5_OUT_THR       0x05 /**< 5 integration time periods out of range */
#define TSL256X_INTERRUPT_6_OUT_THR       0x06 /**< 6 integration time periods out of range */
#define TSL256X_INTERRUPT_7_OUT_THR       0x07 /**< 7 integration time periods out of range */
#define TSL256X_INTERRUPT_8_OUT_THR       0x08 /**< 8 integration time periods out of range */
#define TSL256X_INTERRUPT_9_OUT_THR       0x09 /**< 9 integration time periods out of range */
#define TSL256X_INTERRUPT_10_OUT_THR      0x0A /**< 10 integration time periods out of range */
#define TSL256X_INTERRUPT_11_OUT_THR      0x0B /**< 11 integration time periods out of range */
#define TSL256X_INTERRUPT_12_OUT_THR      0x0C /**< 12 integration time periods out of range */
#define TSL256X_INTERRUPT_13_OUT_THR      0x0D /**< 13 integration time periods out of range */
#define TSL256X_INTERRUPT_14_OUT_THR      0x0E /**< 14 integration time periods out of range */
#define TSL256X_INTERRUPT_15_OUT_THR      0x0F /**< 15 integration time periods out of range */

/**
  * @brief Device descriptor for TSL256X light sensor.
  */
typedef struct {
    i2c_t i2c;              /**< I2C device used for the connection */
    uint8_t addr;           /**< sensor's slave address on the I2C bus */
    bool initialized;       /**< sensor status, true if sensor is initialized */
} tsl256x_t;

/**
  * @brief Data structure representing digital output from the device
  */
typedef struct {
    uint8_t low;            /**< low byte of the output */
    uint8_t high;           /**< high byte of the output */
} tsl256x_data8_t;

/**
  * @brief Data union representing digital output from the device
  */
typedef union {
    uint16_t data16;        /**< 16 byte output */
    tsl256x_data8_t data8;  /**< 8 byte output */
} tsl256x_data_t;

/**
  * @brief Data structure holding all the settings
  */
typedef struct {
    uint16_t thr_high;      /**< interrupt high threshold */
    uint16_t thr_low;       /**< interrupt low threshold */
    uint8_t gain;           /**< conversion gain (0 = 1x, 1 = 16x) */
    uint8_t integ;          /**< integration time */
    uint8_t int_mode;       /**< interrupt mode */
    uint8_t int_occur;      /**< interrupt occurrency */
    void (*cb_fnc)(tsl256x_t *dev, tsl256x_data_t* data, uint8_t ch); /**< interrupt callback function */
} tsl256x_set_t;

/**
  * @brief Data structure holding all the configuration parameters used for initialization
  */
typedef struct {
    i2c_t i2c;              /**< I2C bus used */
    uint8_t addr;           /**< sensor's I2C address */
    tsl256x_set_t sett;     /**< sensor's settings */
} tsl256x_params_t;

/**
 * @brief Initialize the TSL256X light sensor driver.
 *
 * @param[out] dev          device descriptor of accelerometer to initialize
 * @param[in] i2c           I2C bus used for the connection
 * @param[in] settings      data structure holding all settings of the device
 *
 * @return                  0 on success
 * @return                  -1 if settings are wrong
 * @return                  -2 if initialization of I2C bus failed
 * @return                  -3 if sensor test failed
 * @return                  -4 if sensor configuration failed
 */
int tsl256x_init(tsl256x_t *dev, i2c_t i2c, tsl256x_set_t *settings);

/**
 * @brief Read light sensor's data.
 *
 * @param[in]  dev          device descriptor of accelerometer
 * @param[out] data         output data structure
 * @param[in] ch            channel to read (0 = Visible and IR, 1 = IR)
 *
 * @return                  0 on success
 * @return                  -1 if dev is not initialized
 * @return                  -2 if arguments are wrong
 */
int tsl256x_read(tsl256x_t *dev, tsl256x_data_t *data, uint8_t ch);

#ifdef __cplusplus
}
#endif

#endif /* TSL256X_H */
/** @} */

