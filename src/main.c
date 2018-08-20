#include <stm32f4xx.h>
#include <stdio.h>
#include "../lib/delay.h"

#define LED_PIN GPIO_Pin_14

GPIO_InitTypeDef  GPIO_InitStructure;

uint32_t multiplier;

int main() {

	SystemInit();

	TM_Delay_Init();

	/* GPIOG Peripheral clock enable */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	/* Configure PG6 and PG8 in output pushpull mode */
	GPIO_InitStructure.GPIO_Pin = LED_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	printf("Ovo je zaista kul stvar");
	while (1)
	{
	// Set PG6 and PG8
	GPIOB->BSRRL = LED_PIN;

	TM_DelayMillis(5000);

	GPIOB->BSRRH = LED_PIN;

	TM_DelayMillis(5000);

	}


 }
