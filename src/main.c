#include <stm32f4xx.h>
#include <stdio.h>
#include "../lib/delay.h"

#define LED_PIN GPIO_Pin_14
#define SPI1_SS GPIO_Pin_4

#define BLDC_ENABLE GPIO_Pin_1


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

void Init_USART6()
{

    /* Bit configuration structure for GPIOA PIN9 and PIN10 */
    GPIO_InitTypeDef GPIO_InitStruct;
    /* Enalbe clock for USART1, AFIO and GPIOA */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);



    GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_USART6);
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_USART6);
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOC, &GPIO_InitStruct);


    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE);

    USART_Cmd(USART6, ENABLE);

	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    /* Configure USART1 */
    USART_Init(USART6, &USART_InitStructure);
    /* Enable RXNE interrupt */
    USART_ITConfig(USART6, USART_IT_RXNE, ENABLE);


	// Enable interrupt fromUSART2(NVIC level)
	NVIC_EnableIRQ(USART6_IRQn);
}

void Init_SPI1()
{
	/*
	 * PB3 -> SCK
	 * PB4 -> MISO
	 * PB5 -> MOSI
	 * PA4 -> SS
	 */

	GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    // Initialize pins as alternating function
    GPIO_InitStruct.GPIO_Pin = SPI1_SS;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_SPI1);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_SPI1);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_SPI1);
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOB, &GPIO_InitStruct);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
    SPI_Cmd(SPI1, ENABLE);
    SPI_InitTypeDef SPI_InitStructure;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_Init(SPI1, &SPI_InitStructure);

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

void UU_PutChar(USART_TypeDef* USARTx, uint8_t ch)
{
  while(!(USARTx->SR & USART_SR_TXE));
  USARTx->DR = ch;
}

void UU_PutString(USART_TypeDef* USARTx, char * str)
{
  while(*str != 0)
  {
    UU_PutChar(USARTx, *str);
    str++;
  }
}

char recieveBuffer1[4000];
int n = 0;
int rows_count = 0;
void USART6_IRQHandler()
{
	//USART_SendData(USART2, 'T');
	// Do Something    /* RXNE handler */
	if(USART_GetITStatus(USART6, USART_IT_RXNE) != RESET)
	{
			recieveBuffer1[n] = (char)USART_ReceiveData(USART6);
			if(recieveBuffer1[n] == '\n')
			{

				if(rows_count>10)
				{
					recieveBuffer1[n+1] = "\r";
					recieveBuffer1[n+2] = 0;
					/*for(int i = 0; i<n;i++)
					{
						USART_SendData(USART2,(uint16_t)recieveBuffer1[i]);

					}*/
					printf("********************************/////   %d\n\n\n",n);
					printf("%s\n\r",recieveBuffer1);
					n = 0;
					rows_count=0;
					UU_PutString(USART2, recieveBuffer1);
				}
				else
				{
					rows_count++;
					n++;
				}
			}
			else
			{
				n++;
			}

//

	}
}


void init_BLDC_Controller()
{
	GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);


    GPIO_InitStruct.GPIO_Pin = BLDC_ENABLE;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOB, &GPIO_InitStruct);
    GPIO_SetBits(GPIOB, BLDC_ENABLE);

}

void send_BLDC_data(uint8_t address, uint8_t dataH, uint8_t dataL)
{
	GPIO_SetBits(GPIOA, SPI1_SS);
	SPI_I2S_SendData(SPI1, address);
	SPI_I2S_SendData(SPI1, dataH);
	SPI_I2S_SendData(SPI1, dataL);
	GPIO_ResetBits(GPIOA, SPI1_SS);
}

uint16_t get_BLDC_data(uint8_t address)
{
	GPIO_SetBits(GPIOA, SPI1_SS);
	SPI_I2S_SendData(SPI1, address);
	uint16_t data = SPI_I2S_ReceiveData(SPI1);
	GPIO_ResetBits(GPIOA, SPI1_SS);
	return data;
}

int main() {

	SystemInit();

	TM_Delay_Init();

	Init_USART2();

	Init_USART6();

	Init_SPI1();

	init_BLDC_Controller();

	/* GPIOG Peripheral clock enable */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

	GPIO_InitTypeDef  GPIO_InitStructure;

	/* Configure PG6 and PG8 in output pushpull mode */
	GPIO_InitStructure.GPIO_Pin = LED_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitTypeDef  GPIO_InitStructureIn;
	GPIO_InitStructureIn.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructureIn.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructureIn.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOC, &GPIO_InitStructureIn);

	printf("Ovo je zaista kul stvar");




	while (1)
	{
		if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13)==1)
		{
			GPIOB->BSRRL = LED_PIN;
			UU_PutString(USART2, "Mali Mario\n\r");

		}
		else
		{

			GPIOB->BSRRH = LED_PIN;
		}

	}


 }
