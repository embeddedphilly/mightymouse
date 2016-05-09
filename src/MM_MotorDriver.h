#ifndef MM_MotorDriver_H
#define MM_MotorDriver_H
#include "types.h"
#include "CommonDefines.h"
#include "BuPWM.h"
#include "BuGPIO.h"

#define FORWARD 1
#define BACKWARD 2
#define BRAKE 3

typedef enum
{
    MMMD_STATUS_SUCCESS,
    MMMD_STATUS_ERROR
} MMMD_STATUS;

void MMMD_Init(PWMDevice * pwm, GPIODevice * gp);
MMMD_STATUS MMMD_SetWheelSpeed(uint8_t left, uint8_t right);
MMMD_STATUS MMMD_SetWheelDirection(uint8_t leftdir, uint8_t rightdir);
MMMD_STATUS MMMD_SetPower(uint8_t left, uint8_t right);

#endif // MM_MotorDriver_H
