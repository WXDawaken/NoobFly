#include "bsp.h"
#include "os.h"
#include "mpu.h"
#include "motion.h"
#include "stm32f4xx.h"

Motion motion;
euler init_e={0.0f,0.0f,0.0f};
void led()
{
	GPIO_ToggleBits(GPIOC,GPIO_Pin_4);
	//GPIOC->ODR|=GPIO_Pin_4;
}


void Motion_Detection()
{
  
  Motion_Detect(&motion);
   
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
	iic_Init();
	Motion_Init(&motion,init_e,300L);
//	Motion_Set_Detect_Freq(300L);
	f_add(led,100,0);
	f_add(Motion_Detection,(ulong)(motion.Dt*1000),0);
  while(1)
  {
	  os_run();
		//led();
  }
}



