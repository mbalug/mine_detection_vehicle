/*
 * delay.h
 *
 *  Created on: Aug 20, 2018
 *      Author: stm
 */

#ifndef LIB_DELAY_H_
#define LIB_DELAY_H_

#include <stdio.h>
#include <stm32f4xx.h>

void TM_Delay_Init(void);
void TM_DelayMicros(uint32_t micros);
void TM_DelayMillis(uint32_t millis);

#endif /* LIB_DELAY_H_ */
