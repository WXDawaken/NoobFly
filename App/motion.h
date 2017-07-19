#ifndef __MOTION_H_
#define __MOTION_H_
#include "stm32f4xx.h"
#include "bsp.h"
#include "mMATH.h"
#define DT 0.003f
extern float g[3];
void Motion_Init(float *q,float *e);
void Motion_Detect(float *a,float *m,float *w,float *q,float *e);

#endif