/**
  ******************************************************************************
  * @file    Project/STM32F4xx_StdPeriph_Templates/stm32f4xx_it.c 
  * @author  MCD Application Team
  * @version V1.7.1
  * @date    20-May-2016
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2016 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_it.h"
#include "usart.h"
#include "Sys_Time.h"
#include "os.h"
unsigned char is1ms;
unsigned char is2ms;
unsigned char isframe;
unsigned char isStart;
unsigned char isStop;
FIFO fifo_uart3;
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
//    Sys_tick--;
//	  if(Sys_tick==1)
//				SysTick->CTRL&=~(uint32_t)(SysTick_CTRL_TICKINT_Msk|SysTick_CTRL_ENABLE_Msk);
		if(microsecs<999)
			 microsecs++;
		else
		{
			microsecs=0;
		  if(milsecs<999)
				milsecs++;
			else
			{
			  milsecs=0;
//				if(seconds<59)
					seconds++;
//				else
//				{
//				  seconds=0;
//					if(mintues<59)
//						mintues++;
//					else
//					{
//						mintues=0;
//						if(hours<23)
//							hours++;
//						else
//						{
//							hours=0;
//							days++;
//						}
//					}
//				}
			}
		}
}

/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f4xx.s).                                               */
/******************************************************************************/


void USART1_IRQHandler(void)
{
static unsigned char state=0;
	unsigned char ch;
	unsigned temp;
  if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
  {
		 ch=USART_ReceiveData(USART1); 
		 USART3_SendByte(ch);
		  switch(state)
			{
				case 0:
					    if(ch==0xAA)
								state=1;
					    break;
				case 1:
					    if(ch==0x55)
								 state=2;
							else
								 state=0;
							break;
				case 2:
					if(ch==29)
						state=3;
					else if(!fifo_uart3.isfull)
					{
	           FIFO_Write(&fifo_uart3,ch);
					}
					break;
				case 3:
					 if(ch==26)
					 {
						 state=0;
						 isframe=1;
					 }
					 else
					 {		
						  if(!fifo_uart3.isfull)
					     {
						       FIFO_Write(&fifo_uart3,'\r');
					    }
							if(!fifo_uart3.isfull)
					     {
							     FIFO_Write(&fifo_uart3,ch);
					    }
							state=2;
						}
					 break;
				default:
					state=0;
					break;
		 }
  }
}

void TIM3_IRQHandler(void)
{
	static int ms=0;
   if(TIM_GetITStatus(TIM3,TIM_IT_Update)!=RESET)
	 {
		 ms++;
	   tim_1ms();
	 }
	 TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
}

void USART3_IRQHandler(void)
{
	static unsigned char state=0;
	unsigned char ch;
	unsigned temp;
  if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
  {
		 ch=USART_ReceiveData(USART3); 
//		 if(ch==0XAA)
//			  isStart=1;
//		 if(ch==0X55)
//			  isStop=1;
		  switch(state)
			{
				case 0:
					    if(ch==0xAA)
								state=1;
					    break;
				case 1:
					    if(ch==0x55)
								 state=2;
							else
								 state=0;
							break;
				case 2:
					if(ch==29)
						state=3;
					else if(!fifo_uart3.isfull)
					{
	           FIFO_Write(&fifo_uart3,ch);
					}
					break;
				case 3:
					 if(ch==26)
					 {
						 state=0;
						 isframe=1;
					 }
					 else
					 {		
						  if(!fifo_uart3.isfull)
					     {
						       FIFO_Write(&fifo_uart3,'\r');
					    }
							if(!fifo_uart3.isfull)
					     {
							     FIFO_Write(&fifo_uart3,ch);
					    }
							state=2;
						}
					  break;
				default:
					state=0;
					break;
		 }
//   if(!fifo_uart3.isfull)
//    FIFO_Write(&fifo_uart3,ch);
	//USART3_SendByte(ch);
  }
}
