/**
 * @file main.c
 * @author ömer ÇAYLAN
 * @brief Pwm freq calculation //!TEST CODE
 * @version 0.1
 * @date 2021-01-06
 * 
 * @copyright Copyright (c) 2021
 * NOTE: Pwm capture test kodu
 * Cozum: Timer overflow oldugu icin pwm degeri duzgun olcmuyordu, interrupt icinde overflow'u aktif ederek bu arada ki 
 * farki cikararak cozum bulundu.
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
volatile uint32_t flag = 1;
volatile uint32_t captureVal1;
volatile uint32_t captureVal2;
uint32_t g_timerOverflowInterruptCount = 0;
volatile uint32_t tempOverflow1 = 0;
volatile uint32_t tempOverflow2 = 0;

//channel interrupr handle ediliyor ve sirasiyla degerler degiskenlere ataniyor-
//"flag" sadece sirali okumak icin, overflow olduysa o anki overflowtime i aliyoruz ve bir birinden cikariyoruz.
void FTM_INPUT_CAPTURE_HANDLER(void)
{

    if ((FTM_GetStatusFlags(BOARD_FTM_BASEADDR) & kFTM_TimeOverflowFlag) == kFTM_TimeOverflowFlag)
    {
        /* Clear overflow interrupt flag.*/
        FTM_ClearStatusFlags(BOARD_FTM_BASEADDR, kFTM_TimeOverflowFlag);
        g_timerOverflowInterruptCount++;
    }
    else if ((FTM_GetStatusFlags(BOARD_FTM_BASEADDR) & FTM_CHANNEL_FLAG) == FTM_CHANNEL_FLAG)
    {
        /* Clear interrupt flag.*/
        FTM_ClearStatusFlags(BOARD_FTM_BASEADDR, FTM_CHANNEL_FLAG);

        if (ftmIsrFlag == false)
        {
            if (flag == 1)
            {
                flag = 0;
                captureVal1 = BOARD_FTM_BASEADDR->CONTROLS[BOARD_FTM_INPUT_CAPTURE_CHANNEL].CnV;
                tempOverflow1 = g_timerOverflowInterruptCount;
            }
            else if (flag == 0)
            {
                flag = 1;

                captureVal2 = BOARD_FTM_BASEADDR->CONTROLS[BOARD_FTM_INPUT_CAPTURE_CHANNEL].CnV;

                tempOverflow2 = g_timerOverflowInterruptCount;
                ftmIsrFlag = true;
            }
        }
    }

    __DSB();
}

int main(void)
{
    ftm_config_t ftmInfo;

    /* Board pin, clock, debug console init */
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    FTM_GetDefaultConfig(&ftmInfo);
    /* Initialize FTM module */
    FTM_Init(BOARD_FTM_BASEADDR, &ftmInfo);

    /* Setup dual-edge capture on a FTM channel pair */
    FTM_SetupInputCapture(BOARD_FTM_BASEADDR, BOARD_FTM_INPUT_CAPTURE_CHANNEL, kFTM_FallingEdge, 0);

    /* Set the timer to be in free-running mode */
    BOARD_FTM_BASEADDR->MOD = 0xFFFF;

    /* Enable channel interrupt when the second edge is detected */
    FTM_EnableInterrupts(BOARD_FTM_BASEADDR, FTM_CHANNEL_INTERRUPT_ENABLE);
    FTM_EnableInterrupts(BOARD_FTM_BASEADDR, kFTM_TimeOverflowInterruptEnable);

    /* Enable at the NVIC */
    EnableIRQ(FTM_INTERRUPT_NUMBER);

    FTM_StartTimer(BOARD_FTM_BASEADDR, kFTM_SystemClock);

    uint32_t captureDiff = 0;
    float channelBusSpeed = 60;
    float clkMs = 0;
    float pulseWidth = 0;
    uint32_t tempOver = 0;
    while (true)
    {
        if (ftmIsrFlag == true)
        {

            if (captureVal2 > captureVal1)
            {
                captureDiff = captureVal2 - captureVal1;
            }
            else
            {
                captureDiff = ((0xFFFF - captureVal1) + captureVal2);
            }

            clkMs = (float)(1 / channelBusSpeed);
            tempOver = tempOverflow2 - tempOverflow1;
            pulseWidth = (float)(((tempOver * 65536 - captureVal1 + captureVal2) + 1) * clkMs);
            PRINTF("\r\n freq = %d\r\n", (uint32_t)(1000000 * (1 / pulseWidth)));

            ftmIsrFlag = false;
        }
    }
}
