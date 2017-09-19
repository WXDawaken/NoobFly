#ifndef _USART_H
#define _USART_H

#include "stm32f4xx.h"
#include <stdio.h>
#include "FIFO.h"
extern unsigned char isframe;
extern unsigned char isStart;
extern unsigned char isStop;

extern FIFO fifo_uart3;
void USART_Initialize(void);
void USART1_SendByte(uint8_t Data);
void USART1_SendNByte(uint8_t *pData, uint16_t Length);
void USART1_SendString(uint8_t *String);
void USART3_SendByte(uint8_t Data);
void USART3_SendNByte(uint8_t *pData, uint16_t Length);
void USART3_SendString(uint8_t *String);

#endif 
