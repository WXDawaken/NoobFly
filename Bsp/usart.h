#ifndef _USART_H
#define _USART_H

#include "stm32f4xx.h"
#include <stdio.h>

void USART_Initialize(void);
void USART1_SendByte(uint8_t Data);
void USART1_SendNByte(uint8_t *pData, uint16_t Length);
void USART1_SendString(uint8_t *String);


#endif 
