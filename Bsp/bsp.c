#include "bsp.h"
void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;                  
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;          
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;                 
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                    
  NVIC_Init(&NVIC_InitStructure);
}


void BSP_Init(void)
{                                                   
  NVIC_Configuration();     
  USART_Initialize();	
}

