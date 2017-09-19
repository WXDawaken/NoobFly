#include "bsp.h"
#include "os.h"
#include "mpu.h"
#include "motion.h"
#include "PWM.h"
#include "stm32f4xx.h"
#include <stdlib.h>
#include "string.h"
uint8_t exit_flag;
uint8_t init_flag;
Motion motion;
PWM_Type pwm;
euler expect_eul;
union float2char
{
  float f;
  char c[4];
};

void led()
{
	GPIO_ToggleBits(GPIOC,GPIO_Pin_4);
	//GPIOC->ODR|=GPIO_Pin_4;
}

void Decode_Cmd()
{
	unsigned char ch;
	union float2char fr;
	int i;
//	if(isStart)
//	{
//	  		 exit_flag=0;
//		     isStart=0;
//				 pwm.channel_duty[0]=6;
//				 pwm.channel_duty[1]=6;
//				 pwm.channel_duty[2]=6;
//				 pwm.channel_duty[3]=6;
//				 PWM_Set_Duty(&pwm);
//				 printf("OK TURN ON\n");
//	}
//	if(isStop)
//	{
//	   		exit_flag=1;
//		    isStop=0;
//          pwm.channel_duty[0]=5;
//					pwm.channel_duty[1]=5;
//					pwm.channel_duty[2]=5;
//					pwm.channel_duty[3]=5;		  
//			    PWM_Set_Duty(&pwm);
//				 printf("OK SHUTDOWN\n");
//	
//	}
	if(isframe)
	{
		isframe=0;
		 while(!fifo_uart3.isempty)
		 {
				ch=FIFO_Read(&fifo_uart3);
				if(ch>0x00 && ch<0xD)
				{
			  switch(ch%3)
				{
					case 1:
								for(i=0;i<4;i++)
					        fr.c[i]=FIFO_Read(&fifo_uart3);
					      motion.Eul_PI[ch/3].Kp=fr.f;
					      printf("r:%f\n",fr.f);
								break;
					case 2:
						    for(i=0;i<4;i++)
					        fr.c[i]=FIFO_Read(&fifo_uart3);
					      motion.Eul_PI[ch/3].Ki=fr.f;
					      printf("r:%f\n",fr.f);
						    break;
					case 0:
						   	for(i=0;i<4;i++)
					        fr.c[i]=FIFO_Read(&fifo_uart3);
					      motion.Eul_PI[ch/3-1].Kd=fr.f;
					      printf("r:%f\n",fr.f);
					      break;
				 }
			 }
			 if(ch==0x0D)
			 {
			   			for(i=0;i<4;i++)
					        fr.c[i]=FIFO_Read(&fifo_uart3);
				      motion.Expect_Eul.Pitch=fr.f;
				     printf("r:%f\n",fr.f);
			 }
			 if(ch==0x00)
			 {
				  exit_flag=0;
				 pwm.channel_duty[0]=6;
				 pwm.channel_duty[1]=6;
				 pwm.channel_duty[2]=6;
				 pwm.channel_duty[3]=6;
				 PWM_Set_Duty(&pwm);
				 printf("OK TURN ON\n");
			 }
			 if(ch==0xff)
			 {
				  exit_flag=1;
          pwm.channel_duty[0]=5;
					pwm.channel_duty[1]=5;
					pwm.channel_duty[2]=5;
					pwm.channel_duty[3]=5;		  
			    PWM_Set_Duty(&pwm);
				 printf("OK SHUTDOWN\n");
			 } 
	 }
  }
//    while(!fifo_uart3.isempty)
//		{
//		    ch=FIFO_Read(&fifo_uart3);
//			  USART3_SendByte(ch);
//		}
}
void Motion_Detection()
{
 //  Motion_Init(&motion,300L)
	 static double start;
	 Motion_Detect(&motion);
	if(!motion.isOK)
		 {
         if(!init_flag)
				 {
					   start=Seconds();
					   init_flag=1;
				 }
				 else if(Seconds()-start>10.0f)
				 {
				 motion.Expect_Eul.Yaw=motion.Eul.Yaw;
				 motion.Expect_Eul.Pitch=motion.Eul.Pitch;
				 motion.Expect_Eul.Roll=motion.Eul.Roll;
				 motion.isOK=1;
				 pwm.channel_duty[0]=6;
				 pwm.channel_duty[1]=6;
				 pwm.channel_duty[2]=6;
				 pwm.channel_duty[3]=6;
				 PWM_Set_Duty(&pwm);
				 printf("OK\n");
			   }
		 }
}
/****************************************
    4(Ë³Ê±Õë)  1
    3          2

****************************************/
void Motion_Ctrl_t()
{
	int i;
		if(!exit_flag)
		{
		if(motion.isOK)
		{
			if(fabs(motion.Eul.Pitch)>motion.Eul_Limit.Pitch | fabs(motion.Eul.Roll)>motion.Eul_Limit.Roll)
			{
					pwm.channel_duty[0]=5;
					pwm.channel_duty[1]=5;
					pwm.channel_duty[2]=5;
					pwm.channel_duty[3]=5;
					exit_flag=1;
			}
			else
			{
			Motion_Ctrl(&motion);
			pwm.channel_duty[0]=pwm.channel_duty[0]+motion.Roll_Correct+motion.Pitch_Correct-motion.Yaw_Correct;
			pwm.channel_duty[1]=pwm.channel_duty[1]+motion.Roll_Correct-motion.Pitch_Correct+motion.Yaw_Correct;
			pwm.channel_duty[2]=pwm.channel_duty[2]-motion.Roll_Correct-motion.Pitch_Correct-motion.Yaw_Correct;
			pwm.channel_duty[3]=pwm.channel_duty[3]-motion.Roll_Correct+motion.Pitch_Correct+motion.Yaw_Correct;
			for(i=0;i<4;i++)
				pwm.channel_duty[i]=(pwm.channel_duty[i]>10)?10:((pwm.channel_duty[i]<5)?5:pwm.channel_duty[i]);
			}
	//		printf("%f",pwm.channel_duty[3]);
			PWM_Set_Duty(&pwm);
		}
	}
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
	pwm.freq=50L;
	pwm.channel_num=4;
	pwm.channel_duty=(float*)malloc(4*sizeof(float));
	pwm.channel_duty[0]=5;
	pwm.channel_duty[1]=5;
	pwm.channel_duty[2]=5;
	pwm.channel_duty[3]=5;
	PWM_Init(&pwm);
	Motion_Init(&motion,300L);
	motion.Eul_Limit.Pitch=65;
	motion.Eul_Limit.Roll=65;
	PID_Init(&motion.Error_PI[0],60,0.01,0,65536);
	PID_Init(&motion.Error_PI[1],60,0.01,0,65536);
	PID_Init(&motion.Error_PI[2],60,0.01,0,65536);
	PID_Init(&motion.Eul_PI[0],0.0001,0,0,65536);
	PID_Init(&motion.Eul_PI[1],0.0001,0,0,65536);
	PID_Init(&motion.Eul_PI[2],0.0001,0,0,65536);
	
	f_add(Motion_Detection,(ulong)(motion.Dt*1000),0);
	f_add(Motion_Ctrl_t,(ulong)(motion.Dt*1000),0);
	f_add(Decode_Cmd,(ulong)(motion.Dt*1000),0);
  while(1)
  {
	  os_run();
		//Motion_Init(&motion,300L);
//		Delay1ms();
//		led();
//		Delay1ms();
//		led();
		//led();
  }
}



