/**
 * @ingroup     drivers_tsl256xt
 * @{
 *
 * @file
 * @brief        Register definitions for the TSL256X driver.
 *
 * @author      Luca Zulberti <zulberti.luca@gmail.com>
 */

#ifndef TSL256X_REG_H
#define TSL256X_REG_H

#ifdef __cplusplus
extern "C"
{
#endif

#define TSL256X_CONTROL           0x00 /**< Control of basic functions */
#define TSL256X_TIMING            0x01 /**< Integration time/gain control */
#define TSL256X_THRESHLOWLOW      0x02 /**< Low byte of low interrupt threshold */
#define TSL256X_THRESHLOWHIGH     0x03 /**< High byte of low interrupt threshold */
#define TSL256X_THRESHHIGHLOW     0x04 /**< Low byte of high interrupt threshold */
#define TSL256X_THRESHHIGHHIGH    0x05 /**< High byte of high interrupt threshold */
#define TSL256X_INTERRUPT         0x06 /**< Interrupt control */
#define TSL256X_CRC               0x08 /**< Factory test — not a user register */
#define TSL256X_ID_REG            0x0A /**< Part number / Rev ID */
#define TSL256X_DATA0LOW          0x0C /**< Low byte of ADC channel 0 */
#define TSL256X_DATA0HIGH         0x0D /**< High byte of ADC channel 0 */
#define TSL256X_DATA1LOW          0x0E /**< Low byte of ADC channel 1 */
#define TSL256X_DATA1HIGH         0x0F /**< High byte of ADC channel 1 */

#define TSL256X_COMMAND_CMD             (1 << 7)
#define TSL256X_COMMAND_CLEAR           (1 << 6)
#define TSL256X_COMMAND_WORD            (1 << 5)
#define TSL256X_COMMAND_BLOCK           (1 << 4)
#define TSL256X_COMMAND_ADDR_MASK       0x0F

#define TSL256X_CONTROL_POWER_MASK      0x03

#define TSL256X_TIMING_GAIN             (1 << 4)
#define TSL256X_TIMING_MANUAL           (1 << 3)
#define TSL256X_TIMING_INTEG_MASK       0x03

#define TSL256X_INTERRUPT_INTR_MASK     0x30
#define TSL256X_INTERRUPT_PERSIST_MASK  0x0F

#define TSL256X_ID_REG_MASK             0xF0

/**
 * @brief Device ID
 */
#define TSL2560CS_ID                    0x00
#define TSL2561CS_ID                    0x01
#define TSL2560FN_ID                    0x04
#define TSL2561FN_ID                    0x05

#ifdef __cplusplus
}
#endif

#endif /* TSL256X_REG_H */
/** @} */
