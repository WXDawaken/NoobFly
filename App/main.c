#include "bsp.h"
#include "os.h"
#include "mpu.h"
void led()
{
	GPIO_ToggleBits(GPIOC,GPIO_Pin_4);
	//GPIOC->ODR|=GPIO_Pin_4;
}

u8 ac[6],gy[6];
float r_ac[3],r_gy[3];
void getRaw()
{
   int i;
	 short temp;
	 mpu6050_RawData(ac,gy);
	 printf("ac:");
	 for(i=0;i<3;i++)
   {
	    temp=ac[i*2]<<8|ac[i*2+1];
		  r_ac[i]=temp/16384.0f;
		  printf("%f\t",r_ac[i]);
	 }
	 printf("\ngy:");
	 for(i=0;i<3;i++)
   {
	    temp=gy[i*2]<<8|gy[i*2+1];
		  r_gy[i]=temp/16.4f;
		  printf("%f\t",r_gy[i]);
	 }
	 printf("\n");
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
	mpu6050_Init();
	f_add(led,100,0);
  f_add(getRaw,100,0);
  while(1)
  {
	  os_run();
		//led();
  }
}



