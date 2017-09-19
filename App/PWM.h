#ifndef __PWM_H_
#define __PWM_H_
#include "PWM_Type.h"
#include "bsp.h"

#define TIMx TIM2
#define RCC_TIMx RCC_APB1Periph_TIM2

void PWM_Init(PWM_Type *pwm);
void PWM_Set_Duty(PWM_Type *pwm);
#endif