/************************************************************************//** *
 * \file main.c
 *
 * \addtogroup main
 * \{
 *
 * \brief
 *
 * \note
 *
 * \author 
 * \date 
 *
 ****************************************************************************/

/****************************************************************************
 *                              INCLUDE FILES                               *
 ****************************************************************************/
#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>
#include <util/delay.h>

#include "BuGPIO.h"
#include "BuPWM.h"
#include "AvrGPIO.h"
#include "AvrPWM.h"
#include "types.h"
#include "BuChipControl.h"
#include "AvrChipControl.h"

#include "MM_MotorDriver.h"

/****************************************************************************
 *                     PRIVATE FUNCTION DECLARATIONS                        *
 ****************************************************************************/

#define CPU_PRESCALE(n) (CLKPR = 0x80, CLKPR = (n))
int core(void);
/****************************************************************************
 *                              PRIVATE DATA                                *
 ****************************************************************************/
 PWMDevice * Pwm;
 GPIODevice * Gpio;
 ChipControlDevice * ChipControl;
/****************************************************************************
 *                     PRIVATE FUNCTION DEFINITIONS                         *
 ****************************************************************************/

#ifdef __AVR__
int main(void)
{
  core();
}
#endif

int core(void)
{
    ChipControl = Create_ChipControl(); //does prescalar as side-effect
    Gpio = GPIO_Create();
    Pwm = AvrPWM_Create(Gpio);
    MMMD_Init(Pwm, Gpio);
    MMMD_SetWheelSpeed(70, 70);
    MMMD_SetWheelDirection(FORWARD, FORWARD);
    MMMD_SetPower(1, 1);
    _delay_ms(1000);
    _delay_ms(1000);
    _delay_ms(1000);
    _delay_ms(1000);
    MMMD_SetWheelSpeed(10, 60);

    while (1)
    {
    }
    return 0;
}

