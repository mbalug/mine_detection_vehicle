#include <stm32f4xx.h>
#include <stdio.h>
#include "../lib/delay.h"

#define LED_PIN GPIO_Pin_14

GPIO_InitTypeDef  GPIO_InitStructure;

uint32_t multiplier;


void Init_USART2()
{

    /* Bit configuration structure for GPIOA PIN9 and PIN10 */
    GPIO_InitTypeDef GPIO_InitStruct;
    /* Enalbe clock for USART1, AFIO and GPIOA */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);



    GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_USART2);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource6, GPIO_AF_USART2);
    // Initialize pins as alternating function
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOD, &GPIO_InitStruct);


    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

    USART_Cmd(USART2, ENABLE);

	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    /* Configure USART1 */
    USART_Init(USART2, &USART_InitStructure);
    /* Enable RXNE interrupt */
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);


	// Enable interrupt fromUSART2(NVIC level)
	NVIC_EnableIRQ(USART2_IRQn);
}

volatile char recieveBuffer[2000];
volatile int i = 0;

void USART2_IRQHandler()
{
	//USART_SendData(USART2, 'T');
	// Do Something    /* RXNE handler */
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{
		recieveBuffer[i] = (char)USART_ReceiveData(USART2);
		if(recieveBuffer[i] == '\r')
		{
			recieveBuffer[i]=0;
			printf("%s\n",recieveBuffer);
			i = 0;
		}
		else
		{
			i++;
		}

	}
}

int main() {

	SystemInit();

	TM_Delay_Init();

	Init_USART2();
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
