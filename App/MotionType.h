#ifndef __MOTIONTYPE_H_
#define __MOTIONTYPE_H_
#include "MathType.h"



//              b2n[9]={1-2*(q2q2+q3q3),2*(q1q2-q0q3),2*(q1q3+q0q2),
//	                   2*(q1q2+q0q3),1-2*(q1q1+q3q3),2*(q2q3+q0q1),
//	                   2*(q1q3-q0q2),2*(q2q3+q0q1),1-2*(q1q1+q2q2)};

//	           n2b[9]={1-2*(q2q2+q3q3),2*(q1q2+q0q3),2*(q1q3-q0q2),
//		                2*(q1q2-q0q3),1-2*(q1q1+q3q3),2*(q1q3+q0q2),
//           		      2*(q2q3+q0q1),2*(q2q3+q0q1),1-2*(q1q1+q2q2)};
typedef struct Motion
{
  float Accel[3];
  float Gyro[3];
  float Mag[3];
	float Accel_Adjust[3];
	float Gyro_Adjust[3];
	float Mag_Adjust[3];
  quaternion Quater;
	euler Eul;
	float Erro[3];
	float Kp,Ki;
	float Dt;
	float N2B_Mat[9];                                
	float B2N_Mat[9];
}Motion,*pMotion;
#endif