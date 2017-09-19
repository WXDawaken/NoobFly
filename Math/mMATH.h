#ifndef __MMATH_H_
#define __MMATH_H_
#include "arm_math.h"
#include <math.h>
#include "MathType.h"
#include "MotionType.h"

#ifndef PI
#define PI					3.14159265358979f
#endif

#ifdef ARM_MATH_CM4
#define COS(n) arm_cos_f32(n)
#define SIN(n) arm_sin_f32(n)
#define SQRT(s,r) arm_sqrt_f32(s,r)
#else
#define COS(n) cos(n)
#define SIN(n) sin(n)
#define SQRT(s,r) (*(r))=sqrt(s)
#endif
extern float e_i,e_p;

void  Euler2Q(euler e,quaternion *qu);
void  Q2Euler(quaternion qu,euler *eul);
void  R2Quater(float DCMgb[3][3],quaternion* qu);
void  Vector_Rotation(float mat[9], float *v_source,float *v_result);
void  Cross_Product3(float *m,float *n,float *c);
void  Normalize(float *q,const int length);
float Q_rsqrt(float number);
float low_pass(float sample[],float cutoff_freq);
#endif