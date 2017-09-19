#include "motion.h"
#include "stm32f4xx.h"
#include "bsp.h"
#include "mpu.h"
float g[3]={0.0f,0.0f,1.0};
float ex_a=0;
void Motion_Init(pMotion p_motion,unsigned long freq)
{
	 unsigned char i;
	 float row_0[3],row_1[3];
	 float Mat[3][3];
//	 p_motion->Eul=init_e;
	 p_motion->isOK=0;
   Euler2Q(p_motion->Eul,&p_motion->Quater);
	 update_mat(p_motion);
	 p_motion->Dt=1.0f/freq;
	 mpu9150_Init();
	 mpu9150_Adjust_Val(p_motion->Accel_Adjust,p_motion->Gyro_Adjust,p_motion->Mag_Adjust);
	 Motion_Update_Sensor(p_motion);
	 Normalize(p_motion->Accel,3);
	 Normalize(p_motion->Mag,3);
	 Cross_Product3(p_motion->Accel,p_motion->Mag,row_1);
	 Cross_Product3(row_1,p_motion->Accel,row_0);
	 memcpy(Mat[0],row_0,3*sizeof(float));
	 memcpy(Mat[1],row_1,3*sizeof(float));
	 memcpy(Mat[2],p_motion->Accel,3*sizeof(float));
	 R2Quater(Mat,&p_motion->Quater);
	 Q2Euler(p_motion->Quater,&p_motion->Eul);
	 p_motion->Eul.Yaw=360-p_motion->Eul.Yaw;
	 
	#ifdef __DEBUG
    // printf("%f\n",p_motion->Eul.Pitch);
	#endif
}
void Motion_Update_Sensor(pMotion p_motion)
{
	 double start=Milsec();
	 int i;
   //float pressure;
	 mpu9150_RawData(p_motion->Accel,p_motion->Gyro,p_motion->Mag);
   p_motion->Altitude=(1013.25-Pressure/100.0)*9;
   for(i=0;i<3;i++)
	  {
		  p_motion->Accel[i]=p_motion->Accel[i]+p_motion->Accel_Adjust[i];
		  p_motion->Gyro[i]=p_motion->Gyro[i]+p_motion->Gyro_Adjust[i];
			p_motion->Mag[i]=p_motion->Mag[i]*p_motion->Mag_Adjust[i];	
		} 

	 #ifdef __DEBUG
//	  printf("\n------------Adjusted Value-------------\n");
//	  printf("ac:");
//	  for(i=0;i<3;i++)
//      printf("%f\t", p_motion->Accel[i]);
//	  printf("\ngy:");
//	  for(i=0;i<3;i++)
//		  printf("%f\t", p_motion->Gyro[i]);
//    printf("\nmag:");
//	  for(i=0;i<3;i++)
//		  printf("%f\t",p_motion->Mag[i]);
//    printf("\n");
	//	printf("%f\n",p_motion->Altitude);
	 #endif 
}

void Motion_Detect(pMotion p_motion)
{
   double start;
	 int i;
	 float vg[3],h[3],b[3],w[3];
	 float aXm[3],aXrow_3[3],row_2Xam[3];
	 float axvg[3],mxw[3];
	 Motion_Update_Sensor(p_motion);
   Normalize(p_motion->Accel,3);
	 Normalize(p_motion->Mag,3);
	 Vector_Rotation(p_motion->N2B_Mat,g,vg);
	 Vector_Rotation(p_motion->B2N_Mat,p_motion->Mag,h);
	 get_b(h,b);
	 Vector_Rotation(p_motion->N2B_Mat,b,w);
	 Cross_Product3(p_motion->Accel,vg,axvg);
	 Cross_Product3(p_motion->Mag,w,mxw);
	 get_Erro(p_motion->Erro,axvg,mxw);
//	 Cross_Product3(p_motion->Accel,p_motion->Mag,aXm);
//	 Cross_Product3(p_motion->N2B_Mat+6,p_motion->Accel,aXrow_3);
//	 Cross_Product3(p_motion->N2B_Mat+3,aXm,row_2Xam);
//	 get_Erro(p_motion->Erro,aXrow_3,row_2Xam);
	 w_adjust(p_motion);
	 update_q(p_motion);
	 Normalize(p_motion->Quater.q,4);
	 Q2Euler(p_motion->Quater,&p_motion->Eul);
	 p_motion->Eul.Yaw=(360-p_motion->Eul.Yaw);
	 p_motion->Eul.Pitch=-p_motion->Eul.Pitch;
	// p_motion->Eul.Roll=(p_motion->Eul.Roll<0)?-180-p_motion->Eul.Roll:-p_motion->Eul.Roll+180;
	 update_mat(p_motion);
	 	#ifdef __DEBUG
    //printf("%lf",Milsec()-start);
		printf("d:%f\n", p_motion->Eul.Pitch);
	 #endif 
}

//void Motion_Ctrl(pMotion p_motion)
//{
//  float correct[3],yaw_d,pitch_d,roll_d;
//	yaw_d=p_motion->Expect_Eul.Yaw-p_motion->Eul.Yaw;
//	pitch_d=p_motion->Expect_Eul.Pitch-p_motion->Eul.Pitch;
//	roll_d=p_motion->Expect_Eul.Roll-p_motion->Eul.Roll;
//	yaw_d=(fabs(yaw_d)>180?(yaw_d>0?-360+yaw_d:360+yaw_d):((fabs(yaw_d)<3.0)?0:yaw_d));
//  pitch_d=fabs(pitch_d)>60?(pitch_d>0?60:-60):(fabs(pitch_d)<1.2?0:pitch_d);
//	roll_d=fabs(roll_d)>60?(roll_d>0?60:-60):(fabs(roll_d)<1.2?0:roll_d);
//	p_motion->Yaw_Correct=PID_Ctrl(&p_motion->Eul_PI[0],yaw_d);
//  p_motion->Pitch_Correct=PID_Ctrl(&p_motion->Eul_PI[1],pitch_d);
//  p_motion->Roll_Correct=PID_Ctrl(&p_motion->Eul_PI[2],roll_d);
//	p_motion->Yaw_Rate_Correct=PID_Ctrl(&p_motion->Rate_PI[0],p_motion->Yaw_Correct-p_motion->Gyro[0]/PI*180);
//	p_motion->Pitch_Rate_Correct=PID_Ctrl(&p_motion->Rate_PI[1],p_motion->Pitch_Correct-p_motion->Gyro[1]/PI*180);
//	p_motion->Roll_Rate_Correct=PID_Ctrl(&p_motion->Rate_PI[2],p_motion->Roll_Correct-p_motion->Gyro[2]/PI*180);
//	p_motion->Yaw_Rate_Correct=fabs(p_motion->Yaw_Rate_Correct)>2.5?2.5:p_motion->Yaw_Rate_Correct;
//  p_motion->Pitch_Rate_Correct=fabs(p_motion->Pitch_Rate_Correct)>2.5?2.5:p_motion->Pitch_Rate_Correct;
//	p_motion->Roll_Rate_Correct=fabs(p_motion->Roll_Rate_Correct)>2.5?2.5:p_motion->Roll_Rate_Correct;
//	#ifdef __DEBUG
//	  //printf("%f\n", pitch_d);
//	#endif
//}
void Motion_Ctrl(pMotion p_motion)
{
   float correct[3],yaw_d,pitch_d,roll_d;
	yaw_d=p_motion->Expect_Eul.Yaw-p_motion->Eul.Yaw;
	pitch_d=p_motion->Expect_Eul.Pitch-p_motion->Eul.Pitch;
	roll_d=p_motion->Expect_Eul.Roll-p_motion->Eul.Roll;
	yaw_d=(fabs(yaw_d)>180?(yaw_d>0?-360+yaw_d:360+yaw_d):((fabs(yaw_d)<3.0)?0:yaw_d));
  pitch_d=fabs(pitch_d)>60?(pitch_d>0?60:-60):(fabs(pitch_d)<1?0:pitch_d);
	roll_d=fabs(roll_d)>60?(roll_d>0?60:-60):(fabs(roll_d)<1?0:roll_d);
	p_motion->Yaw_Correct=PID_Ctrl(&p_motion->Eul_PI[0],yaw_d);
  p_motion->Pitch_Correct=PID_Ctrl(&p_motion->Eul_PI[1],pitch_d);
  p_motion->Roll_Correct=PID_Ctrl(&p_motion->Eul_PI[2],roll_d);
	p_motion->Yaw_Correct=fabs(p_motion->Yaw_Correct)>2.5?2.5:p_motion->Yaw_Correct;
  p_motion->Pitch_Correct=fabs(p_motion->Pitch_Correct)>2.5?2.5:p_motion->Pitch_Correct;
	p_motion->Roll_Correct=fabs(p_motion->Roll_Correct)>2.5?2.5:p_motion->Roll_Correct;

	#ifdef __DEBUG
	  //printf("%f\n", pitch_d);
	#endif
}
//void get_Erro(float *e,float *a,float *v,float *m,float *w)
//{
//	float aXv[3],mXw[3];
//  Cross_Product3(a,v,aXv);
//	Cross_Product3(m,w,mXw);
//	e[0]=aXv[0]+mXw[0];
//	e[1]=aXv[1]+mXw[1];	
//	e[2]=aXv[2]+mXw[2];
//}
void get_Erro(float *e,float *v1,float *v2)
{
  	e[0]=v1[0]+v2[0];
	  e[1]=v1[1]+v2[1];	
		e[2]=v1[2]+v2[2];
//	  printf("%f\t",e[0]);
//	  printf("%f\t",e[1]);
//  	printf("%f\n",e[2]);
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
	 ex[0]=PID_Ctrl(&p_motion->Error_PI[0], p_motion->Erro[0]);
	 ex[1]=PID_Ctrl(&p_motion->Error_PI[1], p_motion->Erro[1]);
	 ex[2]=PID_Ctrl(&p_motion->Error_PI[2], p_motion->Erro[2]);
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
