#ifndef __PID_H_
#define __PID_H_
#include "PIDType.h"

void PID_Init(PID_Type *pid_inst,float Kp,float Ki,float Kd,float int_limit);
float PID_Ctrl(PID_Type *pid_inst,float error);


#endif