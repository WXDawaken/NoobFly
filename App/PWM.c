#include "PWM.h"
#include "stdlib.h"
void PWM_Init(PWM_Type *pwm)
{
	TIM_TimeBaseInitTypeDef timx_base_init;
	TIM_OCInitTypeDef timx_oc_init;
	long time_period;
	GPIO_InitTypeDef pwm_pin;
	RCC_APB1PeriphClockCmd(RCC_TIMx,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource0,GPIO_AF_TIM2);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource1,GPIO_AF_TIM2);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_TIM2);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_TIM2);
  pwm_pin.GPIO_Mode=GPIO_Mode_AF;
	pwm_pin.GPIO_OType=GPIO_OType_PP;
	pwm_pin.GPIO_Speed=GPIO_High_Speed;
	pwm_pin.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
	GPIO_Init(GPIOA,&pwm_pin);
 	timx_base_init.TIM_Prescaler=0;
	timx_base_init.TIM_CounterMode=TIM_CounterMode_Up;
	timx_base_init.TIM_ClockDivision=TIM_CKD_DIV1;
	timx_base_init.TIM_Period=SystemCoreClock/pwm->freq;
	TIM_TimeBaseInit(TIMx,&timx_base_init);
  timx_oc_init.TIM_OCMode=TIM_OCMode_PWM1;
  timx_oc_init.TIM_OCPolarity=TIM_OCPolarity_High;
  timx_oc_init.TIM_Pulse=SystemCoreClock/pwm->freq*(pwm->channel_duty[0]/100);
	timx_oc_init.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OC1Init(TIMx,&timx_oc_init);
	TIM_OC2Init(TIMx,&timx_oc_init);
	TIM_OC3Init(TIMx,&timx_oc_init);
	TIM_OC4Init(TIMx,&timx_oc_init);
	TIM_Cmd(TIMx,ENABLE);
  TIM_CtrlPWMOutputs(TIMx,ENABLE);
}

void PWM_Set_Duty(PWM_Type *pwm)
{
   long time_period=SystemCoreClock/pwm->freq;
	 TIMx->CCR1=time_period*(pwm->channel_duty[0]/100);
	 TIMx->CCR2=time_period*(pwm->channel_duty[1]/100);
	 TIMx->CCR3=time_period*(pwm->channel_duty[2]/100);
	 TIMx->CCR4=time_period*(pwm->channel_duty[3]/100);
}