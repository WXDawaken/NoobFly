#ifndef __PWM_TYPE_H_
#define __PWM_TYPE_H_

typedef struct PWM_Type
{
   long freq;
	 int channel_num;
	 float* channel_duty;
}PWM_Type,pPWM_Type;


#endif