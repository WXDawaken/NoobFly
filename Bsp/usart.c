#include "usart.h"

void USART_GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

  GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;                          
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;                       
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);                             


  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;                         
  GPIO_Init(GPIOA, &GPIO_InitStructure);                          
}


void USART_Configuration(void)
{
  USART_InitTypeDef USART_InitStructure;
  USART_InitStructure.USART_BaudRate = 115200;                       
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;        
  USART_InitStructure.USART_StopBits = USART_StopBits_1;            
  USART_InitStructure.USART_Parity = USART_Parity_No ;              
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;    
  USART_Init(USART1, &USART_InitStructure);                        
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);                     
  USART_Cmd(USART1, ENABLE);                                       
}

void USART_Initialize(void)
{
  USART_GPIO_Configuration();
  USART_Configuration();
}


void USART1_SendByte(uint8_t Data)
{
  while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);      
  USART_SendData(USART1, Data);
  while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);       
}


void USART1_SendNByte(uint8_t *pData, uint16_t Length)
{
  while(Length--)
  {
    USART1_SendByte(*pData);
    pData++;
  }
}


void USART1_SendString(uint8_t *String)
{
  while((*String) != '\0')
  {
    USART1_SendByte(*String);
    String++;
  }
  USART1_SendByte(0x0A);                         
}


int fputc(int ch, FILE *f)
{
  USART1_SendByte((uint8_t)ch);

  return ch;
}

int fgetc(FILE *f)
{
  while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);

  return (int)USART_ReceiveData(USART1);
}

