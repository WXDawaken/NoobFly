#include "motion.h"
#include "stm32f4xx.h"
#include "bsp.h"
#include "mpu.h"
float g[3]={0.0f,0.0f,1.0};

void Motion_Init(pMotion p_motion,euler init_e,unsigned long freq)
{
	 unsigned char i;
	 p_motion->Eul=init_e;
   Euler2Q(p_motion->Eul,&p_motion->Quater);
	 update_mat(p_motion);
	 p_motion->Dt=1.0f/freq;
	 mpu9150_Init();
	 mpu9150_Adjust_Val(p_motion->Accel_Adjust,p_motion->Gyro_Adjust,p_motion->Mag_Adjust);
	#ifdef __DEBUG
	 printf("------------Inital Adjust Value-------------\n");
	 printf("ac:");
	 for(i=0;i<3;i++)
     printf("%f\t",p_motion->Accel_Adjust[i]);
	 printf("\ngy:");
	 for(i=0;i<3;i++)
		 printf("%f\t",p_motion->Gyro_Adjust[i]);
   printf("\nmag:");
	 for(i=0;i<3;i++)
		 printf("%f\t",p_motion->Mag_Adjust[i]);
   printf("\n");
	#endif
}
void Motion_Update_Sensor(pMotion p_motion)
{
	 int i;
   mpu9150_RawData(p_motion->Accel,p_motion->Gyro,p_motion->Mag);
   for(i=0;i<3;i++)
	  {
		  p_motion->Accel[i]=p_motion->Accel[i]+p_motion->Accel_Adjust[i];
		  p_motion->Gyro[i]=p_motion->Gyro[i]+p_motion->Gyro_Adjust[i];
			p_motion->Mag[i]=p_motion->Mag[i]*p_motion->Mag_Adjust[i];	
		} 
	 #ifdef __DEBUG
	  printf("\n------------Adjusted Value-------------\n");
	  printf("ac:");
	  for(i=0;i<3;i++)
      printf("%f\t", p_motion->Accel[i]);
	  printf("\ngy:");
	  for(i=0;i<3;i++)
		  printf("%f\t", p_motion->Gyro[i]);
    printf("\nmag:");
	  for(i=0;i<3;i++)
		  printf("%f\t",p_motion->Mag[i]);
    printf("\n");
	 #endif 
}
void Motion_Detect(pMotion p_motion)
{
	 float vg[3],h[3],b[3],w[3];
   Normalize(p_motion->Accel,3);
	 Normalize(p_motion->Mag,3);
	 Vector_Rotation(p_motion->N2B_Mat,g,vg);
	 Vector_Rotation(p_motion->B2N_Mat,p_motion->Mag,h);
	 get_b(h,b);
	 Vector_Rotation(p_motion->N2B_Mat,b,w);
	 get_Erro(p_motion->Erro,p_motion->Accel,vg,p_motion->Mag,w);
	 w_adjust(p_motion);
	 update_q(p_motion);
	 Normalize(p_motion->Quater.q,4);
	 Q2Euler(p_motion->Quater,&p_motion->Eul);
	 update_mat(p_motion);
}
void get_Erro(float *e,float *a,float *v,float *m,float *w)
{
	float aXv[3],mXw[3];
  Cross_Product3(a,v,aXv);
	Cross_Product3(m,w,mXw);
	e[0]=aXv[0]+mXw[0];
	e[1]=aXv[1]+mXw[1];
	e[2]=aXv[2]+mXw[2];
}
void  get_b(float *h,float *b)
{
  SQRT(h[0]*h[0]+h[1]*h[1],b);
  b[1]=0.0f;
  b[2]=h[2];
}

float Erro_pi(float e,float p,float i)
{
    static float Int=0;
    Int+=e;
	  return e_p*e+e_i*Int;
}

void  w_adjust(pMotion p_motion)
{
	 float ex[3];
	 ex[0]=Erro_pi(p_motion->Erro[0],p_motion->Kp,p_motion->Ki);
	 ex[1]=Erro_pi(p_motion->Erro[1],p_motion->Kp,p_motion->Ki);
	 ex[2]=Erro_pi(p_motion->Erro[2],p_motion->Kp,p_motion->Ki);
   p_motion->Gyro[0]+=ex[0];
   p_motion->Gyro[1]+=ex[1];
	 p_motion->Gyro[2]+=ex[2];
}

void  update_q(pMotion p_motion)
{
//	int i=0;
	float t_q[4];
//  t_q[0]=p_motion->Quater.q[0];
//	t_q[1]=p_motion->Quater.q[1];
//  t_q[2]=p_motion->Quater.q[2];
//	t_q[3]=p_motion->Quater.q[3];
	memcpy(t_q,p_motion->Quater.q,4*sizeof(float));
	p_motion->Quater.q[0]=t_q[0]+p_motion->Dt/2*(-p_motion->Gyro[0]*p_motion->Quater.q[1]-p_motion->Gyro[1]*p_motion->Quater.q[2]-p_motion->Gyro[2]*p_motion->Quater.q[3]);
	p_motion->Quater.q[1]=t_q[1]+p_motion->Dt/2*(p_motion->Gyro[0]*p_motion->Quater.q[0]+p_motion->Gyro[2]*p_motion->Quater.q[2]-p_motion->Gyro[1]*p_motion->Quater.q[3]);
	p_motion->Quater.q[2]=t_q[2]+p_motion->Dt/2*(p_motion->Gyro[1]*p_motion->Quater.q[0]-p_motion->Gyro[2]*p_motion->Quater.q[1]+p_motion->Gyro[0]*p_motion->Quater.q[3]);
	p_motion->Quater.q[3]=t_q[3]+p_motion->Dt/2*(p_motion->Gyro[2]*p_motion->Quater.q[1]+p_motion->Gyro[1]*p_motion->Quater.q[1]-p_motion->Gyro[2]*p_motion->Quater.q[2]);
  p_motion->Quater.q0q0=p_motion->Quater.q[0]*p_motion->Quater.q[0];
	p_motion->Quater.q0q1=p_motion->Quater.q[0]*p_motion->Quater.q[1];
	p_motion->Quater.q0q2=p_motion->Quater.q[0]*p_motion->Quater.q[2];
	p_motion->Quater.q0q3=p_motion->Quater.q[0]*p_motion->Quater.q[3];
	p_motion->Quater.q1q1=p_motion->Quater.q[1]*p_motion->Quater.q[1];
	p_motion->Quater.q1q2=p_motion->Quater.q[1]*p_motion->Quater.q[2];
	p_motion->Quater.q1q3=p_motion->Quater.q[1]*p_motion->Quater.q[3];
	p_motion->Quater.q2q2=p_motion->Quater.q[2]*p_motion->Quater.q[2];
	p_motion->Quater.q2q3=p_motion->Quater.q[2]*p_motion->Quater.q[3];
  p_motion->Quater.q3q3=p_motion->Quater.q[3]*p_motion->Quater.q[3];
}

void  update_mat(pMotion p_motion)
{
	float mat_0_0=1-2*(p_motion->Quater.q2q2+p_motion->Quater.q3q3),
		    mat_0_1=2*(p_motion->Quater.q1q2-p_motion->Quater.q0q3),
	      mat_0_2=2*(p_motion->Quater.q1q3+p_motion->Quater.q0q2),
	      mat_1_0=2*(p_motion->Quater.q1q2+p_motion->Quater.q0q3),
	      mat_1_1=1-2*(p_motion->Quater.q1q1+p_motion->Quater.q3q3),
	      mat_1_2=2*(p_motion->Quater.q2q3+p_motion->Quater.q0q1),
	      mat_2_0=2*(p_motion->Quater.q1q3-p_motion->Quater.q0q2),
	      mat_2_1=2*(p_motion->Quater.q2q3+p_motion->Quater.q0q1),
	      mat_2_2=1-2*(p_motion->Quater.q1q1+p_motion->Quater.q2q2);
  float n_B2N_Mat[9]={mat_0_0,mat_0_1,mat_0_2,
	                    mat_1_0,mat_1_1,mat_1_2,
	                    mat_2_0,mat_2_1,mat_2_2};

  float n_N2B_Mat[9]={mat_0_0,mat_1_0,mat_2_0,
	                    mat_0_1,mat_1_1,mat_2_1,
	                    mat_0_2,mat_1_2,mat_2_2};   
	memcpy(p_motion->B2N_Mat,n_B2N_Mat,9*sizeof(float));
  memcpy(p_motion->N2B_Mat,n_N2B_Mat,9*sizeof(float));
}
