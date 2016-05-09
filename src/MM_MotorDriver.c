#include "MM_MotorDriver.h"
#include "AvrPWM.h"

#define LEFT 4 //B7
#define RIGHT 3 //B6
#define FREQUENCY 2000 //2kHz

#define LEFT_IN1_PORT GPIO_PORTD
#define LEFT_IN1_PIN GPIO_PIN0
#define LEFT_IN2_PORT GPIO_PORTD
#define LEFT_IN2_PIN GPIO_PIN1


#define RIGHT_IN1_PORT GPIO_PORTB
#define RIGHT_IN1_PIN GPIO_PIN6
#define RIGHT_IN2_PORT GPIO_PORTB
#define RIGHT_IN2_PIN GPIO_PIN5


PWMDevice * MyPwm;
GPIODevice * MyGpio;

PWM_ID Left;
PWM_ID Right;

GPIO_HANDLE RightIN1;
GPIO_HANDLE RightIN2;

GPIO_HANDLE LeftIN1;
GPIO_HANDLE LeftIN2;

GPIOConfig Config;

void MMMD_Init(PWMDevice * pwm, GPIODevice * gp)
{
    MyPwm = pwm;
    MyGpio = gp;

    Left = AvrPWM_PinIdFromHardwareInfo(LEFT, 0);
    Right = AvrPWM_PinIdFromHardwareInfo(RIGHT, 0);
    MyPwm->PWMSetFreq(Left, 2000);
    MyPwm->PWMSetFreq(Right, 2000);

    Config.direction = OUTPUT;
    Config.val = GPIO_HIGH;

    RightIN1.port = RIGHT_IN1_PORT;
    RightIN1.pin = RIGHT_IN1_PIN;
    Config.port = RIGHT_IN1_PORT;
    Config.pin = RIGHT_IN1_PIN;
    MyGpio->GPIOConfigPin(Config, &RightIN1);

    RightIN2.port = RIGHT_IN2_PORT;
    RightIN2.pin = RIGHT_IN2_PIN;
    Config.port = RIGHT_IN2_PORT;
    Config.pin = RIGHT_IN2_PIN;
    MyGpio->GPIOConfigPin(Config, &RightIN2);

    LeftIN1.port = LEFT_IN1_PORT;
    LeftIN1.pin = LEFT_IN1_PIN;
    Config.port = LEFT_IN1_PORT;
    Config.pin = LEFT_IN1_PIN;
    MyGpio->GPIOConfigPin(Config, &LeftIN1);

    LeftIN2.port = LEFT_IN2_PORT;
    LeftIN2.pin = LEFT_IN2_PIN;
    Config.port = LEFT_IN2_PORT;
    Config.pin = LEFT_IN2_PIN;
    MyGpio->GPIOConfigPin(Config, &LeftIN2);

}

MMMD_STATUS MMMD_SetWheelSpeed (uint8_t left, uint8_t right)
{
    //cannot set higher than 100. Set to 100 if so? for now, just fail.
    if (left > 100 || right > 100)
    {
        //error
        return MMMD_STATUS_ERROR;
    }

    MyPwm->PWMSetDutyCycle(Left, left);
    MyPwm->PWMSetDutyCycle(Right, right);
    return MMMD_STATUS_SUCCESS;
}

MMMD_STATUS MMMD_SetWheelDirection(uint8_t leftdir, uint8_t rightdir)
{
    switch (leftdir)
    {
        case FORWARD:
            MyGpio->GPIOSetHigh(&LeftIN1);
            MyGpio->GPIOSetLow(&LeftIN2);
            break;
        case BACKWARD:
            MyGpio->GPIOSetLow(&LeftIN1);
            MyGpio->GPIOSetHigh(&LeftIN2);
            break;
        case BRAKE:
            MyGpio->GPIOSetHigh(&LeftIN1);
            MyGpio->GPIOSetHigh(&LeftIN2);
            break;
    }
    switch (rightdir)
    {
        case BACKWARD:
            MyGpio->GPIOSetHigh(&RightIN1);
            MyGpio->GPIOSetLow(&RightIN2);
            break;
        case FORWARD:
            MyGpio->GPIOSetLow(&RightIN1);
            MyGpio->GPIOSetHigh(&RightIN2);
            break;
        case BRAKE:
            MyGpio->GPIOSetHigh(&RightIN1);
            MyGpio->GPIOSetHigh(&RightIN2);
            break;
    }

    return MMMD_STATUS_SUCCESS;
}

MMMD_STATUS MMMD_SetPower(uint8_t left, uint8_t right)
{
    if (left)
    {
        MyPwm->PWMEnable(Left);
    }
    else
    {
        MyPwm->PWMDisable(Left);
    }
    if (right)
    {
        MyPwm->PWMEnable(Right);
    }
    else
    {
        MyPwm->PWMDisable(Right);
    }
    return MMMD_STATUS_SUCCESS;
}
