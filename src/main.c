#include <stm32f4xx.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../lib/delay.h"
#include "../lib/tm_stm32f4_spi.h"
#include "../lib/tm_stm32f4_dac.h"

#define LED_PIN GPIO_Pin_14

void Init_USART2()
{

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);


    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_USART2);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource6, GPIO_AF_USART2);
    // Initialize pins as alternating function
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOD, &GPIO_InitStruct);


    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_Cmd(USART2, ENABLE);

    USART_Init(USART2, &USART_InitStructure);
    /* Enable RXNE interrupt */
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	NVIC_EnableIRQ(USART2_IRQn);
}

void Init_USART6()
{

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

    GPIO_InitTypeDef GPIO_InitStruct;
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

    NVIC_InitTypeDef NVIC_InitStruct;
    NVIC_InitStruct.NVIC_IRQChannel = USART6_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
    NVIC_Init(&NVIC_InitStruct);
}


void setDac1(int16_t dac)
{

}

void initDAC()
{
	// GPIO setup PA4
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AN;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &GPIO_InitStruct);
//
//	// DAC 1 Setup
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
//	DAC_InitTypeDef DAC1_InitStructure;
//	DAC_Init(DAC_Channel_1, &DAC1_InitStructure);
//	DAC_Cmd(DAC_Channel_1 ,ENABLE);
//
//	DAC_InitTypeDef DAC2_InitStructure;
//	DAC_Init(DAC_Channel_2, &DAC2_InitStructure);
//	DAC_Cmd(DAC_Channel_2 ,ENABLE);
//
//    DAC_TypeDef DAC_InitStruct;
//    DAC_InitStruct.CR |= DAC_CR_EN1 | DAC_CR_EN2;
//    DAC_InitStruct.DHR12R1 = 0;
//    DAC_InitStruct.DHR12R2 = 0;
//    DAC_Init(DAC_Channel_1|DAC_Channel_2, &DAC_InitStruct);
    TM_DAC_Init(TM_DAC1);
    TM_DAC_Init(TM_DAC2);
}

char recieveBuffer[2000];
volatile int i = 0;


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
void USART2_IRQHandler()
{
//	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
//	{
//		recieveBuffer[i] = (char)USART_ReceiveData(USART2);
//		if(recieveBuffer[i] == '\r')
//		{
//			recieveBuffer[i]=0;
//			printf("%s\n",recieveBuffer);
//			i = 0;
//
//			UU_PutString(USART2, "OVO JE KURAC");
//
//		}
//		else
//		{
//			i++;
//			printf("Charrcved");
//		}
//
//	}
}

void messageHandler(char buffer[20])
{
	if(buffer[0] == 'F')
	{
		if(buffer[1]=='L')
		{
			char left[10];
			strncpy(left, buffer + 2, strlen(buffer)-1);
			uint16_t left_speed=atoi(left);
			printf("FL: %d\n",left_speed);
			TM_DAC_SetValue(TM_DAC2, left_speed);
			//DAC_SetChannel2Data(DAC_Align_12b_L, 4095 - left_speed);
			//set direction
			//set dac
		}
		if(buffer[1]=='R')
		{
			char right[10];
			strncpy(right, buffer + 2, strlen(buffer)-1);
			uint16_t right_speed=atoi(right);
			printf("FR: %d\n",right_speed);
			TM_DAC_SetValue(TM_DAC1, 4095 - right_speed);
			//DAC_SetChannel1Data(DAC_Align_12b_R, 4095 - right_speed);
		}
	}
	else if(buffer[0] == 'R')
	{
		if(buffer[1]=='L')
		{
			char left[10];
			strncpy(left, buffer + 2, strlen(buffer)-1);
			uint16_t left_speed=atoi(left);
			printf("RL: %d\n",left_speed);
		}
		if(buffer[1]=='R')
		{
			char right[10];
			strncpy(right, buffer + 2, strlen(buffer)-1);
			uint16_t right_speed=atoi(right);
			printf("RR: %d\n",right_speed);
		}
	}

}


char recieveBuffer1[1000];
int n = 0;
int rows_count = 0;
void USART6_IRQHandler()
{
	//USART_SendData(USART2, 'T');
	// Do Something    /* RXNE handler */
//	if(USART_GetITStatus(USART6, USART_IT_RXNE) != RESET)
//	{
//			recieveBuffer1[n] = (char)USART_ReceiveData(USART6);
//			if(recieveBuffer1[n] == '\n')
//			{
//
//				if(rows_count>10)
//				{
//					recieveBuffer1[n+1] = '\r';
//					recieveBuffer1[n+2] = 0;
//					/*for(int i = 0; i<n;i++)
//					{
//						USART_SendData(USART2,(uint16_t)recieveBuffer1[i]);
//
//					}*/
//					printf("********************************/////   %d\n\n\n",n);
//					printf("%s\n\r",recieveBuffer1);
//					n = 0;
//					rows_count=0;
//					UU_PutString(USART2, recieveBuffer1);
//				}
//				else
//				{
//					rows_count++;
//					n++;
//				}
//			}
//			else
//			{
//				n++;
//			}
//	}
//
	if(USART_GetITStatus(USART6, USART_IT_RXNE) != RESET)
	{
		recieveBuffer[i] = (char)USART_ReceiveData(USART6);
		if(recieveBuffer[i] == '\n')
		{
			recieveBuffer[i]=0;
			i = 0;
			messageHandler(recieveBuffer);
//			printf("%s\n",recieveBuffer);
//			//UU_PutString(USART6, "WORLD\n");
//			if(strstr(recieveBuffer, "Stre:") != NULL) {
//			    char *sub_str = strstr(recieveBuffer, "Stre:");
//			    char num[100];
//			    strncpy(num, sub_str + 6, strlen(sub_str)-1);
//			    //UU_PutString(USART6, sub_str);
//			    // (num[0]-48)*100+
//			    uint8_t pwmDuty=atoi(num);
//			    printf("BROJ :%d\n", pwmDuty);
//			    DAC_SetChannel1Data(DAC_Align_8b_R, 255-(pwmDuty*2));
			//}
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

	//Init_USART2();

	Init_USART6();

	/* GPIOG Peripheral clock enable */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	GPIO_InitTypeDef  GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = LED_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitTypeDef  GPIO_InitStructureIn;
	GPIO_InitStructureIn.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_2;
	GPIO_InitStructureIn.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructureIn.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOC, &GPIO_InitStructureIn);

	initDAC();

	TM_DAC_SetValue(TM_DAC1, 4095);
	TM_DAC_SetValue(TM_DAC2, 0);

	while (1)
	{
		if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13)==1)
		{
			GPIO_ResetBits(GPIOB, LED_PIN);

		    TM_DelayMillis(1000);

			UU_PutString(USART6, "Mali Mario\n\r");
//			//uint8_t data[3] = {0x00,0x00,0x01};
//			uint16_t rcv;
//			send_BLDC_data(0x00,0x00,0x01);
//			//SPI1_Transmit(data, &rcv, 3);
//
//			TM_DelayMillis(1);
//
//			//uint8_t data1[1] = {0x80};
//			uint16_t data = get_BLDC_data(0x80);
//			printf("MSG 0x00 %x", rcv);

		}
		else
		{
			GPIO_SetBits(GPIOB, LED_PIN);
			TM_DelayMicros(25);
		}

	}


 }
