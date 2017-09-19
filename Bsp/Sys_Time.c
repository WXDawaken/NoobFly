#include "Sys_Time.h"
#include "stm32f4xx.h"

volatile unsigned long long Sys_tick;
volatile unsigned long long milsecs;
volatile unsigned long long microsecs;
volatile unsigned long long seconds;
volatile unsigned long long mintues;
volatile unsigned long long hours;
volatile unsigned long long days;
volatile unsigned long long months;
volatile unsigned long long years;


void Sys_Time_Init(long long System_Freq)
{

	SysTick_Config(System_Freq/1000000);
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);
	//SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;
}