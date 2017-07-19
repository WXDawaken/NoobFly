#ifndef __MMATH_H_
#define __MMATH_H_
#include "arm_math.h"
#include <math.h>
#define COS(n) arm_cos_f32(n)
#define SIN(n) arm_sin_f32(n)
#define q0q0 q[0]*q[0]
#define q0q1 q[0]*q[1]
#define q0q2 q[0]*q[2]
#define q0q3 q[0]*q[3]
#define q1q1 q[1]*q[1]
#define q1q2 q[1]*q[2]
#define q1q3 q[1]*q[3]
#define q2q2 q[2]*q[2]
#define q2q3 q[2]*q[3]
#define q3q3 q[3]*q[3]

extern float32_t e_i,e_p;

void  euler2q(float32_t yaw,float32_t pitch,
	           float32_t roll,float32_t *q);
void q2euler(float32_t *q,float32_t *e);
void  N2B( float32_t *q, float32_t *m, float32_t *v);
void  B2N( float32_t *q, float32_t *m,float32_t *v);
void  get_b(float32_t *h,float32_t *b);
void  get_e(float32_t *e,float32_t *a,float32_t *v,float32_t *m,float32_t *w);
float32_t e_pi(float32_t e);
void  w_adjust(float32_t *w,float32_t *e);
void  update_q(float32_t *q,float32_t* w,float32_t dt);
void  normalize(float32_t *q,const int length);
float32_t Q_rsqrt(float32_t number);
float32_t arctan(float32_t x);
#endif