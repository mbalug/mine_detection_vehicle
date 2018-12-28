#include <stm32f4xx.h>
#include "../lib/peripherial_defines.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../lib/delay.h"
#define HAL_RCC_MODULE_ENABLED

void uartPutChar(USART_TypeDef* USARTx, uint8_t ch)
{
	while (!(USARTx->SR & USART_SR_TXE))
		;
	USARTx->DR = ch;
}

void uartPutString(USART_TypeDef* USARTx, char * str)
{
	while (*str != 0)
	{
		uartPutChar(USARTx, *str);
		str++;
	}
}
void USART2_IRQHandler()
{
//	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
//	{
//		recieveBuffer[i] = (char)USART_ReceiveData(USART2);
//	}
}

void messageHandler(char buffer[20])
{
	if (buffer[0] == 'F')
	{
		if (buffer[1] == 'L')
		{
			printf("Received message: %s\n", buffer);
			char *fr_position = strstr(buffer, "FR");
			if (fr_position)
			{
				//printf("RIGHT; %s\n", fr_position);
				char right[10] =
				{ 0 };
				strncpy(right, fr_position + 2, strlen(buffer) - 1);
				uint16_t right_speed = atoi(right);
				//printf("FR: %d\n", right_speed);
				char left[10] =
				{ 0 };
				strncpy(left, buffer + 2, strlen(buffer) - strlen(fr_position));
				//printf("LEFT; %s\n", left);
				uint16_t left_speed = atoi(left);
				//printf("FL: %d\n", left_speed);
				if (left_speed > 500 && right_speed > 500)
				{
					GPIO_SetBits(GPIOG, LEFT_MOTOR_DIRECTION);
					TM_DAC_SetValue(TM_DAC2, left_speed);
					GPIO_ResetBits(GPIOG, RIGHT_MOTOR_DIRECTION);
					TM_DAC_SetValue(TM_DAC1, 4095 - right_speed);
					GPIO_SetBits(GPIOC, GPIO_Pin_8 | GPIO_Pin_9);
				}
				else
				{
					GPIO_ResetBits(GPIOC, GPIO_Pin_8 | GPIO_Pin_9);
				}
			}
		}
	}
	else if (buffer[0] == 'R')
	{
		if (buffer[1] == 'L')
		{
			//printf("Received message: %s\n", buffer);
			char *fr_position = strstr(buffer, "RR");
			if (fr_position)
			{
				//printf("RIGHT; %s\n", fr_position);
				char right[10] =
				{ 0 };
				strncpy(right, fr_position + 2, strlen(buffer) - 1);
				uint16_t right_speed = atoi(right);
				//printf("FR: %d\n", right_speed);
				char left[10] =
				{ 0 };
				strncpy(left, buffer + 2, strlen(buffer) - strlen(fr_position));
				//printf("LEFT; %s\n", left);
				uint16_t left_speed = atoi(left);
				//printf("FL: %d\n", left_speed);
				if (left_speed > 500 && right_speed > 500)
				{
					GPIO_ResetBits(GPIOG, LEFT_MOTOR_DIRECTION);
					TM_DAC_SetValue(TM_DAC2, left_speed);
					GPIO_SetBits(GPIOG, RIGHT_MOTOR_DIRECTION);
					TM_DAC_SetValue(TM_DAC1, 4095 - right_speed);
					GPIO_SetBits(GPIOC, GPIO_Pin_8 | GPIO_Pin_9);
				}
				else
				{
					GPIO_ResetBits(GPIOC, GPIO_Pin_8 | GPIO_Pin_9);
				}
			}
		}
	}

}

static char recieveBuffer[500];
static int i = 0;

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
	if (USART_GetITStatus(USART6, USART_IT_RXNE) != RESET)
	{
		recieveBuffer[i] = (char) USART_ReceiveData(USART6);
//		if (i == 0)
//		{
//			if (recieveBuffer[0] == 'F' || recieveBuffer[0] == 'R')
//			{
//				i++;
//
//			}
//			else
//			{
//				recieveBuffer[i] = 0;
//			}
//		}
//		else
//		{
		if (recieveBuffer[i] == '\n')
		{
			recieveBuffer[i] = 0;
			i = 0;
			messageHandler(recieveBuffer);
		}
		else
		{
			i++;
		}
//		}

	}
}


void RCC_IRQHandler(void)
{
	if (RCC_GetITStatus(RCC_IT_HSERDY) != RESET)
	{
		/* Clear HSERDY interrupt pending bit */
		RCC_ClearITPendingBit(RCC_IT_HSERDY);

		/* Check if the HSE clock is still available */
		if (RCC_GetFlagStatus(RCC_FLAG_HSERDY) != RESET)
		{
			/* Enable PLL: once the PLL is ready the PLLRDY interrupt is generated */
			RCC_PLLCmd(ENABLE);
			printf("PLL ENABLED");
		}
	}

	if (RCC_GetITStatus(RCC_IT_PLLRDY) != RESET)
	{
		/* Clear PLLRDY interrupt pending bit */
		RCC_ClearITPendingBit(RCC_IT_PLLRDY);

		/* Check if the PLL is still locked */
		if (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) != RESET)
		{
			/* Select PLL as system clock source */
			RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
		}
	}
}

int main()
{

	RCC_HSEConfig(RCC_HSE_Bypass | RCC_HSE_ON);
	RCC_PLLConfig(RCC_PLLSource_HSE, 8, 384, 4, 2, 2);
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
	RCC_HCLKConfig(RCC_SYSCLK_Div1);
	RCC_PCLK1Config(RCC_HCLK_Div2);
	RCC_PCLK2Config(RCC_SYSCLK_Div1);
//	SystemInit();
//	SystemCoreClockUpdate();


	/* Enable Clock Security System(CSS): this will generate an NMI exception
	 when HSE clock fails *****************************************************/
	//RCC_ClockSecuritySystemCmd(ENABLE);

	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = RCC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	RCC_ClocksTypeDef rcc_init_struct;
	RCC_GetClocksFreq(&rcc_init_struct);
	printf("CLOCK: %u\n",rcc_init_struct.SYSCLK_Frequency);
	printf("CLOCK HCLK %u\n",rcc_init_struct.HCLK_Frequency);

	TM_Delay_Init();
	initNucleoButtonLed();
//	initUSART6();
//	initDAC();
//	initMotorDirectionPins();

	while (1)
	{
		if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13) == 1)
		{
			GPIO_ResetBits(GPIOB, LED_PIN);

			TM_DelayMillis(1000);

			uartPutString(USART6, "Mali Mario\n\r");

		}
		else
		{
			GPIO_SetBits(GPIOB, LED_PIN);
			TM_DelayMillis(1000);
		}

	}

}
