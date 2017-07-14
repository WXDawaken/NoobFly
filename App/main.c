#include "bsp.h"
#include "os.h"

void led()
{
	GPIO_ToggleBits(GPIOC,GPIO_Pin_4);
	//GPIOC->ODR|=GPIO_Pin_4;
}


int main(void)
{
  BSP_Init();
  os_start();
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
	GPIO_InitTypeDef led_pin;
	led_pin.GPIO_Mode=GPIO_Mode_OUT;
	led_pin.GPIO_OType=GPIO_OType_PP;
	led_pin.GPIO_Speed=GPIO_High_Speed;
	led_pin.GPIO_PuPd=GPIO_PuPd_NOPULL;
	led_pin.GPIO_Pin=GPIO_Pin_4;
	GPIO_Init(GPIOC,&led_pin);
	f_add(led,10000,0);
  while(1)
  {
	  os_run();
		//led();
  }
}



