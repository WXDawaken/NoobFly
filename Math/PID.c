#include "PID.h"
#include "mMath.h"
void PID_Init(PID_Type *pid_inst,float Kp,float Ki,float Kd,float int_limit)
{
  pid_inst->Int=0;
  pid_inst->LastError=0;
  pid_inst->Kp=Kp;
	pid_inst->Ki=Ki;
	pid_inst->Kd=Kd;
	pid_inst->Int_Limit=int_limit;
}


float PID_Ctrl(PID_Type *pid_inst,float error)
{
 float result,de;
 de=error-pid_inst->LastError;
 pid_inst->D_sample[1]=de;
 de=pid_inst->D_sample[0]=low_pass(pid_inst->D_sample,20);
 result=pid_inst->Kp*error+pid_inst->Int+pid_inst->Kd*de;
 if((fabs(pid_inst->Int)<pid_inst->Int_Limit) | (error>0&&pid_inst->Int<0) | (error<0 && pid_inst->Int>0))
	 pid_inst->Int+=error*pid_inst->Ki*0.003;
 pid_inst->LastError=error;
 return result;
}
