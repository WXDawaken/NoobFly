#include "mMATH.h"
 float32_t e_i=0.0f,e_p=2.0f;
void  euler2q(float32_t yaw, float32_t pitch,float32_t roll,float *qu)
{
	 float32_t r_roll=roll/180*PI,r_pitch=pitch/180*PI,r_yaw=yaw/180*PI;
   qu[0]=COS(r_roll/2)*COS(r_pitch/2)*COS(r_yaw/2)
	      +SIN(r_roll/2)*SIN(r_pitch/2)*SIN(r_yaw/2);
	 qu[1]=SIN(r_roll/2)*COS(r_pitch/2)*COS(r_yaw/2)
	      -COS(r_roll/2)*SIN(r_pitch/2)*SIN(r_yaw/2);
	 qu[2]=COS(r_roll/2)*SIN(r_pitch/2)*COS(r_yaw/2)
	      +SIN(r_roll/2)*COS(r_pitch/2)*SIN(r_yaw/2);
	 qu[3]=COS(r_roll/2)*COS(r_pitch/2)*SIN(r_yaw/2)
	      +SIN(r_roll/2)*SIN(r_pitch/2)*COS(r_yaw/2);
	return;
}

void q2euler(float32_t *q,float32_t *e)
{
	float32_t r_roll,r_pitch,r_yaw;
  r_yaw=atan(2*(q1q2+q0q3)/(1-2*(q2q2+q3q3)));
  r_pitch=-asin(2*(q1q3-q0q2));
	r_roll=atan(2*(q2q3+q0q1)/(1-2*(q1q1+q2q2)));
	e[0]=r_yaw/PI*180;
	e[1]=r_pitch/PI*180;
	e[2]=r_roll/PI*180;
}

	
void N2B( float32_t *q, float32_t *m,float32_t *v)
{
	float32_t n2b[9]={1-2*(q2q2+q3q3),2*(q1q2+q0q3),2*(q1q3-q0q2),
		                2*(q1q2-q0q3),1-2*(q1q1+q3q3),2*(q1q3+q0q2),
           		      2*(q2q3+q0q1),2*(q2q3+q0q1),1-2*(q1q1+q2q2)};
  arm_matrix_instance_f32 matN,vm,result;
  arm_mat_init_f32(&matN,3,3,n2b);
  arm_mat_init_f32(&vm,3,1,m);
	arm_mat_init_f32(&result,3,1,v);								 
  arm_mat_mult_f32(&matN,&vm,&result);
}

void B2N( float32_t *q, float32_t *m,float32_t *v)
{
	float32_t b2n[9]={1-2*(q2q2+q3q3),2*(q1q2-q0q3),2*(q1q3+q0q2),
	                   2*(q1q2+q0q3),1-2*(q1q1+q3q3),2*(q2q3+q0q1),
	                   2*(q1q3-q0q2),2*(q2q3+q0q1),1-2*(q1q1+q2q2)};
  arm_matrix_instance_f32 matN,vm,result;
	
  arm_mat_init_f32(&matN,3,3,b2n);
  arm_mat_init_f32(&vm,3,1,m);
	arm_mat_init_f32(&result,3,1,v);								 
  arm_mat_mult_f32(&matN,&vm,&result);
}

void  get_e(float32_t *e,float32_t *a,float32_t *v,float32_t *m,float32_t *w)
{
    e[0]=(a[1]*v[2]-a[2]*v[1])+(m[1]*w[2]-m[2]*w[1]);
    e[1]=(a[2]*v[0]-a[0]*v[2])+(m[2]*w[0]-m[0]*w[2]);
    e[2]=(a[0]*v[1]-a[1]*v[0])+(m[0]*w[1]-m[1]*w[0]);
}

void  get_b(float32_t *h,float32_t *b)
{
  arm_sqrt_f32(h[0]*h[0]+h[1]*h[1],b);
  b[1]=0.0f;
  b[2]=h[2];
}

float32_t e_pi(float32_t e)
{
    static float32_t Int=0;
    Int+=e;
	  return e_p*e+e_i*Int;
}

void  w_adjust(float32_t *w,float32_t *e)
{
	 float32_t ex[3];
	 ex[0]=e_pi(e[0]);
	 ex[1]=e_pi(e[1]);
	 ex[2]=e_pi(e[2]);
   w[0]+=ex[0];
   w[1]+=ex[1];
	 w[2]+=ex[2];
}

void  update_q(float32_t *q,float32_t* w,float32_t dt)
{
	int i=0;
	float32_t t_q[4];
  t_q[0]=q[0];
	t_q[1]=q[1];
  t_q[2]=q[2];
	t_q[3]=q[3];
	q[0]=t_q[0]+dt/2*(-w[0]*q[1]-w[1]*q[2]-w[2]*q[3]);
	q[1]=t_q[1]+dt/2*(w[0]*q[0]+w[2]*q[2]-w[1]*q[3]);
	q[2]=t_q[2]+dt/2*(w[1]*q[0]-w[2]*q[1]+w[0]*q[3]);
	q[3]=t_q[3]+dt/2*(w[2]*q[1]+w[1]*q[1]-w[2]*q[2]);
}

void  normalize(float32_t *q,const int length)	
{
	 int i;
	 float32_t sum=0,q_s[length],rsqrt;
	 arm_power_f32(q,length,q_s);
	 for(i=0;i<length;i++)
	    sum+=q_s[i];
	 rsqrt=Q_rsqrt(sum);
   for(i=0;i<length;i++)
	    q[i]=q[i]*rsqrt;
}
	
float32_t Q_rsqrt(float32_t number)       
{
    long i;
    float32_t x2, y;
    const float32_t threehalfs = 1.5F;
    x2 = number * 0.5F;
    y = number;
    i = *(long *)&y;                       
    i = 0x5f375a86 - (i >> 1);               
    y = *(float *)&i;
    y = y * (threehalfs - (x2 * y * y));   
    y = y * (threehalfs - (x2 * y * y));   
    return y;
}	

float32_t arctan(float32_t x)
{
   float32_t x_3,x_5,x_7,x_9;
   x_3=pow(x,3);
	 x_5=pow(x,5);
	 x_7=pow(x,7);
	 x_9=pow(x,9);
	 return x-x_3/3.0f+x_5/5.0f-x_7/7.0f+x_9/9.0f;
}

float32_t arcsin(float32_t x)
{
  float32_t x_3,x_5;
  x_3=pow(x,3);
	x_5=pow(x,5);
  return x+(1/2.0f)*x_3/3.0f+(3/8.0f)*x_5/5;
}