#include "bsp.h"
#include "os.h"
#include "mpu.h"
#include "motion.h"

u8 ac[6],gy[6],mg[6];
float r_ac[3],r_gy[3],r_mg[3],a_ac[3],a_gy[3],a_mg[3];
float mag_adjust[3];
float ac_adjust[3],gy_adjust[3];
float q[4]={1,0,0,0},euler[3];
void led()
{
	GPIO_ToggleBits(GPIOC,GPIO_Pin_4);
	//GPIOC->ODR|=GPIO_Pin_4;
}
void get_Adjusted_Sensor_Data();
void Adjust_Sensor_Data();
void Motion_Detection();




void Motion_Detection()
{
  
  Motion_Detect(a_ac,a_mg,a_gy,q,euler);
  printf("\nyaw: %f pitch: %f roll: %f\n",euler[0],euler[1],euler[2]);
}

void get_Adjusted_Sensor_Data()
{
	  int i;
    mpu9150_RawData(r_ac,r_gy,r_mg);
    for(i=0;i<3;i++)
	  {
		  a_ac[i]=r_ac[i]+ac_adjust[i];
		  a_gy[i]=r_gy[i]+gy_adjust[i];
			a_mg[i]=r_mg[i]*mag_adjust[i];	
		}
//		printf("------------Adjusted Value-------------\n");
//	  printf("ac:");
//	  for(i=0;i<3;i++)
//      printf("%f\t",a_ac[i]);
//	  printf("\ngy:");
//	  for(i=0;i<3;i++)
//		  printf("%f\t",a_gy[i]);
//    printf("\nmag:");
//	  for(i=0;i<3;i++)
//		  printf("%f\t",a_mg[i]);
//    printf("\n");
}

void Adjust_Sensor_Data()
{
	 int i;

   mpu9150_Adjust_Val(ac_adjust,gy_adjust,mag_adjust);
	 printf("------------Inital Adjust Value-------------\n");
	 printf("ac:");
	 for(i=0;i<3;i++)
     printf("%f\t",ac_adjust[i]);
	 printf("\ngy:");
	 for(i=0;i<3;i++)
		 printf("%f\t",gy_adjust[i]);
   printf("\nmag:");
	 for(i=0;i<3;i++)
		 printf("%f\t",mag_adjust[i]);
   printf("\n");
}

int main(void)
{
	float init_e[3]={0.0f,0.0f,0.0f};
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
	mpu9150_Init();
	Adjust_Sensor_Data();
	Motion_Init(init_e,q);
	f_add(led,100,0);
  f_add(get_Adjusted_Sensor_Data,3,0);
	f_add(Motion_Detection,3,0);
  while(1)
  {
	  os_run();
		//led();
//		SCL_H;
//		GPIO_WriteBit(GPIOC,GPIO_Pin_4,1);
//		Delay();
//		SCL_L;
//    GPIO_WriteBit(GPIOC,GPIO_Pin_4,0);
//		Delay();
  }
}



