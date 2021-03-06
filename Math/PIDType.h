#ifndef __PIDTYPE_H_
#define __PIDTYPE_H_

#ifdef ARM_MATH_CM4
#include "arm_math.h"
#endif

typedef struct PID_Type
{
  float Kp;
  float Ki;
	float Kd;
	float Int;
	float Int_Limit;
	float LastError;
	float D_sample[2];
}PID_Type;
#endif