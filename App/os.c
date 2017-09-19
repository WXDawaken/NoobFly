#include "os.h"
 TASK os_Tasks[p_f_max]={0};
 
void tim_1ms(){  
   int i;
	 for(i=0;i<p_f_max;i++)
	{
	   if(os_Tasks[i].t_Run)
		 {
		   if(os_Tasks[i].t_Delay==0)
			 {
			   os_Tasks[i].t_Flag=1;
				 os_Tasks[i].t_Delay=os_Tasks[i].t_Interval;
			 }
			 else
				 os_Tasks[i].t_Delay--;
		 }
	}
}

void os_start(){
   int i;
	 for(i=0;i<p_f_max;i++)
	{
	   os_Tasks[i].t_Interval=0;
		 os_Tasks[i].t_Flag=0;
		 os_Tasks[i].t_Delay=0;
		 os_Tasks[i].t_Run=0x00000000;
	}
//	Timer_Init();
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	TIM_TimeBaseInitTypeDef timer;
	TIM_TimeBaseStructInit(&timer);
	timer.TIM_Prescaler=0;
	timer.TIM_CounterMode=TIM_CounterMode_Down;
	timer.TIM_ClockDivision=TIM_CKD_DIV1;
  timer.TIM_Period=167999;	
	TIM_TimeBaseInit(TIM3,&timer);
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
	TIM_Cmd(TIM3,ENABLE);
}

void f_add(void add(),ulong time,ulong Delay){
  int i;
	for(i=0;i<p_f_max &&(os_Tasks[i].t_Run);i++);
	if(i==p_f_max)
    return;
	else
	{
		os_Tasks[i].t_Run=add;
		os_Tasks[i].t_Delay=Delay;
		os_Tasks[i].t_Interval=time;
	} 
}

void f_kill(uchar num){
   if(num>=p_f_max)
		 return;
	 os_Tasks[num].t_Run=0x00000000;
	 os_Tasks[num].t_Delay=0;
	 os_Tasks[num].t_Interval=0;
	 os_Tasks[num].t_Flag=0;
}

void os_run(){
  int i;
	for(i=0;i<p_f_max;i++)
	{
		if(os_Tasks[i].t_Run)
		{
		 if(os_Tasks[i].t_Flag)
		 {
       		   
        os_Tasks[i].t_Flag=0;
			  os_Tasks[i].t_Run();
			  if(!os_Tasks[i].t_Interval)
				   f_kill(i);						
		 }
	  }
	}
}



