/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "board.h"
#include "fsl_ftm.h"

#include "pin_mux.h"
#include "clock_config.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define BOARD_FTM_BASEADDR FTM0

/* FTM channel used for input capture */
#define BOARD_FTM_INPUT_CAPTURE_CHANNEL kFTM_Chnl_0

/* Interrupt number and interrupt handler for the FTM base address used */
#define FTM_INTERRUPT_NUMBER FTM0_IRQn
#define FTM_INPUT_CAPTURE_HANDLER FTM0_IRQHandler

/* Interrupt to enable and flag to read */
#define FTM_CHANNEL_INTERRUPT_ENABLE kFTM_Chnl0InterruptEnable
#define FTM_CHANNEL_FLAG kFTM_Chnl0Flag

/* Get source clock for FTM driver */
#define FTM_SOURCE_CLOCK CLOCK_GetFreq(kCLOCK_BusClk)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile bool ftmIsrFlag = false;

/*******************************************************************************
 * Code
 ******************************************************************************/
volatile uint8_t counter = 1;
volatile uint32_t value1 = 0;
volatile uint32_t value2 = 0;
void FTM_INPUT_CAPTURE_HANDLER(void)
{
    if ((FTM_GetStatusFlags(BOARD_FTM_BASEADDR) & FTM_CHANNEL_FLAG) == FTM_CHANNEL_FLAG)
    {
        /* Clear interrupt flag.*/
        FTM_ClearStatusFlags(BOARD_FTM_BASEADDR, FTM_CHANNEL_FLAG);
        //     if (ftmIsrFlag == false)
        {

            if (counter == 1)
            {

                value1 = BOARD_FTM_BASEADDR->CONTROLS[BOARD_FTM_INPUT_CAPTURE_CHANNEL].CnV;
                counter = 2;
            }
            else if (counter == 2)
            {
                /* code */
                value2 = BOARD_FTM_BASEADDR->CONTROLS[BOARD_FTM_INPUT_CAPTURE_CHANNEL].CnV;
                ftmIsrFlag = true;
                counter = 1;
            }
        }
    }

    __DSB();
}

void delay(void)
{
    volatile uint32_t i = 0U;
    for (i = 0U; i < 80000U; ++i)
    {
        __asm("NOP"); /* delay */
    }
}

/*!
 * @brief Main function
 */
int main(void)
{
    ftm_config_t ftmInfo;
    uint32_t captureVal;

    /* Board pin, clock, debug console init */
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    BOARD_InitBootPeripherals();
    /* Print a note to terminal */
    PRINTF("\r\nFTM input capture example\r\n");
    uint32_t capture_diff = 0;
    uint32_t captureDif = 0;

    while (1)
    {
        if (ftmIsrFlag == true)
        {

            //  PRINTF("\r\nvalue1 C()V=%d\r\n", value1);
            //  PRINTF("\r\nvalue2 C()V=%d\r\n", value2);
            if (value2 > value1)
            {
                captureDif = value2 - value1;
                //  PRINTF("\r\n value2 - value1=%d\r\n", captureDif);

                //  PRINTF("\r\nfreq=%lf\r\n", (double)(1 / ((value2 - value1) * 0.0166)));
                //  PRINTF("\r\ndifference /60 =%d\r\n", (value2 - value1) / 60);
                //   capture_diff = (6001 - value1) + value2;
                // PRINTF("\r\ncapture_diff=%d\r\n", capture_diff);
            }
            else
            {
              //  captureDif = (6001 - value1) + value2;
            }

           // ftmIsrFlag = false;
        }

        PRINTF("\r\ncaptureDif=%d\r\n", captureDif);
    }
}
