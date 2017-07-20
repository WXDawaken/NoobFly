#ifndef __MOTION_H_
#define __MOTION_H_

#include "mMATH.h"
#include "MotionType.h"

extern float g[3];
void  get_b(float *h,float *b);
void  get_Erro(float *e,float *a,float *v,float *m,float *w);
float Erro_pi(float e,float p,float i);
void  w_adjust(pMotion p_motion);
void  update_q(pMotion p_motion);
void  update_mat(pMotion p_motion);
void Motion_Init(pMotion p_motion,euler init_e,unsigned long freq);
void Motion_Detect(pMotion p_motion);
void Motion_Update_Sensor(pMotion p_motion);

#endif