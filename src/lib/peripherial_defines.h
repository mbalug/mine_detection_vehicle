#include <stm32f4xx.h>
#include "tm_stm32f4_spi.h"
#include "tm_stm32f4_dac.h"


#define LED_PIN GPIO_Pin_14
#define LEFT_MOTOR_DIRECTION GPIO_Pin_2
#define RIGHT_MOTOR_DIRECTION GPIO_Pin_3

void initUSART2();
void initUSART6();
void initDAC();
void initMotorDirectionPins();
void initNucleoButtonLed();
