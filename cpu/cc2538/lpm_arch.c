/*
 * Copyright (C) 2014 Loci Controls Inc.
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     cpu_cc2538
 * @{
 *
 * @file
 * @brief       Implementation of the kernels power management interface
 *
 * @author      Ian Martin <ian@locicontrols.com>
 * @author      Luca Zulberti <zulberti.luca@gmail.com>
 *
 * @}
 */

#include "arch/lpm_arch.h"
#include "cc2538_sys_ctrl.h"

/** LPM modes correlation
  *
  * LPM_ON          -> CC2538 Active
  * LPM_IDLE        -> CC2538 Sleep 
  * LPM_SLEEP       -> CC2538 Deep Sleep (PM0)
  * LPM_POWERDOWN   -> CC2538 PM2
  * LPM_OFF         -> CC2538 PM3
  */

enum lpm_mode current_mode = LPM_UNKNOWN;

void lpm_arch_init(void)
{
    /* Setting the R/S/DCGCXX registers */
    cc2538_sys_ctrl_t *sys_ctrl = SYS_CTRL;

    /* GPT clock gate */
    sys_ctrl->SCGCGPT = sys_ctrl->RCGCGPT;
    sys_ctrl->DCGCGPT = 0;

    /* SSI clock gate*/
    sys_ctrl->SCGCSSI = sys_ctrl->RCGCSSI;
    sys_ctrl->DCGCSSI = 0;

    /* UART clock gate */
    sys_ctrl->cc2538_sys_ctrl_unnamed2.SCGCUART = 
        sys_ctrl->cc2538_sys_ctrl_unnamed1.RCGCUART;
    sys_ctrl->cc2538_sys_ctrl_unnamed3.DCGCUART = 0;

    /* I2C clock gate */
    sys_ctrl->SCGCI2C = sys_ctrl->RCGCI2C;
    sys_ctrl->DCGCI2C = 0;

    /* Security Core clock gate */
    sys_ctrl->SCGCSEC = sys_ctrl->RCGCSEC;
    sys_ctrl->DCGCSEC = 0;

    /* RF Core clock gate */
    sys_ctrl->SCGCRFC = sys_ctrl->RCGCRFC;
    sys_ctrl->DCGCRFC = sys_ctrl->RCGCRFC;
}

enum lpm_mode lpm_arch_set(enum lpm_mode target)
{
    cc2538_sys_ctrl_t *sys_ctrl = SYS_CTRL;

    if (target == current_mode)
        return current_mode;

    switch (target) {
    case LPM_ON:
        /* If 16MHz selected */
        if (sys_ctrl->cc2538_sys_ctrl_clk_ctrl.CLOCK_CTRLbits.OSC == 1) {
            /* start 32MHz clock */
            sys_ctrl->cc2538_sys_ctrl_clk_ctrl.CLOCK_CTRLbits.OSC = 0;
        }
        /* and wait for it */
        while (sys_ctrl->cc2538_sys_ctrl_clk_sta.CLOCK_STAbits.OSC != 0) {}
        return LPM_ON;
    case LPM_IDLE:
        SCB->SCR &= ~SCB_SCR_SLEEPDEEP_Msk;
        break;
    case LPM_SLEEP:
        SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
        sys_ctrl->PMCTL = 0x00;
        break;
    case LPM_POWERDOWN:
        SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
        sys_ctrl->PMCTL = 0x02;
        /* TODO Clock down */
        break;
    case LPM_OFF:
        SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
        sys_ctrl->PMCTL = 0x03;
        /* TODO Clock down */
        break;
    default:
        return LPM_UNKNOWN;
    }

    __WFI();

    return current_mode;
}

enum lpm_mode lpm_arch_get(void)
{
    return current_mode;
}

void lpm_arch_awake(void)
{
}

void lpm_arch_begin_awake(void)
{
}

void lpm_arch_end_awake(void)
{
}
