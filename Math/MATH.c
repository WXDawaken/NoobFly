#include "mMATH.h"
 float e_i=0.0f,e_p=20.0f;
void  Euler2Q(euler e,quaternion *qu)
{
	 float r_roll=e.Roll/180*PI,r_pitch=e.Pitch/180*PI,r_yaw=e.Yaw/180*PI;
	 float cos_roll_h=COS(r_roll/2),sin_roll_h=SIN(r_roll/2),
	           cos_pitch_h=COS(r_pitch/2),sin_pitch_h=SIN(r_pitch/2),
	           cos_yaw_h=COS(r_yaw/2),sin_yaw_h=SIN(r_yaw/2);
   qu->q[0]=cos_roll_h*cos_pitch_h*cos_yaw_h
	      +sin_roll_h*sin_pitch_h*sin_yaw_h;
	 qu->q[1]=sin_roll_h*cos_pitch_h*cos_yaw_h
	      -cos_roll_h*sin_pitch_h*sin_yaw_h;
	 qu->q[2]=cos_roll_h*sin_pitch_h*cos_yaw_h
	      +sin_roll_h*cos_pitch_h*sin_yaw_h;
	 qu->q[3]=cos_roll_h*cos_pitch_h*sin_yaw_h
	      +sin_roll_h*sin_pitch_h*cos_yaw_h;
	 	qu->q0q1=qu->q[0]*qu->q[1];
	qu->q0q2=qu->q[0]*qu->q[2];
	qu->q0q3=qu->q[0]*qu->q[3];
	qu->q1q1=qu->q[1]*qu->q[1];
	qu->q1q2=qu->q[1]*qu->q[2];
	qu->q1q3=qu->q[1]*qu->q[3];
	qu->q2q2=qu->q[2]*qu->q[2];
	qu->q2q3=qu->q[2]*qu->q[3];
  qu->q3q3=qu->q[3]*qu->q[3];
	return;
}

void Q2Euler(quaternion qu,euler *eul)
{
	float r_roll,r_pitch,r_yaw;
  r_yaw=atan2f(2*(qu.q1q2+qu.q0q3),(1-2*(qu.q2q2+qu.q3q3)));
  r_pitch=-asin(2*(qu.q1q3-qu.q0q2));
	r_roll=atan2f(2*(qu.q2q3+qu.q0q1),(1-2*(qu.q1q1+qu.q2q2)));
	eul->Yaw=(r_yaw<0?2*PI+r_yaw:r_yaw)/PI*180;
	eul->Pitch=r_pitch/PI*180;
	eul->Roll=r_roll/PI*180;
}

void  R2Quater(float DCMgb[3][3],quaternion* qu)
{
	float		trace;
	float		s;
	int     	i,j,k;
	float		q[4];
	int 	next[ 3 ] = { 1, 2, 0 };

	trace = DCMgb[0][0] +DCMgb[1][1] + DCMgb[2][2];

	if ( trace > 0.0f ) 
	{ s = sqrtf(trace + 1.0);
		qu->q[0]= s / 2.0f;
		if(s!=0)
			 s = 0.5f / s;
		qu->q[1]= ( DCMgb[ 2 ][ 1 ] - DCMgb[ 1 ][ 2 ] ) * s;
		qu->q[2] = ( DCMgb[ 0 ][ 2 ] - DCMgb[ 2 ][ 0 ] ) * s;
		qu->q[3] = ( DCMgb[ 1 ][ 0 ] - DCMgb[ 0 ][ 1 ] ) * s;

	} 
	else 
	{
		i = 0;
		if ( DCMgb[ 1 ][ 1 ] > DCMgb[ 0 ][ 0 ] ) {
			i = 1;
		}
		if ( DCMgb[ 2 ][ 2 ] > DCMgb[ i ][ i ] ) {
			i = 2;
		}
		j = next[ i ];
		k = next[ j ];
		
		s = sqrt((DCMgb[i][i] - (DCMgb[j][j] + DCMgb[k][k])) + 1.0f);
		q[i] = s * 0.5f;
    if( s!= 0.0f) 
			s = 0.5f / s;
    q[3] = (-DCMgb[j][k] - DCMgb[k][j]) * s;
    q[j] = (DCMgb[i][j] + DCMgb[j][i]) * s;
    q[k] = (DCMgb[i][k] + DCMgb[k][i]) * s;
    qu->q[1] = q[0];
    qu->q[2]	= q[1];
    qu->q[3]	= q[2];
    qu->q[0] = q[3];
	}

	qu->q0q0=qu->q[0]*qu->q[0];
	qu->q0q1=qu->q[0]*qu->q[1];
	qu->q0q2=qu->q[0]*qu->q[2];
	qu->q0q3=qu->q[0]*qu->q[3];
	qu->q1q1=qu->q[1]*qu->q[1];
	qu->q1q2=qu->q[1]*qu->q[2];
	qu->q1q3=qu->q[1]*qu->q[3];
	qu->q2q2=qu->q[2]*qu->q[2];
	qu->q2q3=qu->q[2]*qu->q[3];
  qu->q3q3=qu->q[3]*qu->q[3];
}	
void Vector_Rotation(float *mat, float *v_source,float *v_result)
{
  arm_matrix_instance_f32 matN,vm,result;
  arm_mat_init_f32(&matN,3,3,mat);
  arm_mat_init_f32(&vm,3,1,v_source);
	arm_mat_init_f32(&result,3,1,v_result);								 
  arm_mat_mult_f32(&matN,&vm,&result);
}

void  Cross_Product3(float *m,float *n,float *c)
{
  c[0]=m[1]*n[2]-m[2]*n[1];
	c[1]=m[2]*n[0]-m[0]*n[2];
  c[2]=m[0]*n[1]-m[1]*n[0];  
}

void  Normalize(float *q,const int length)	
{
	 int i;
	 float sum=0,q_s[length],rsqrt;
	 arm_power_f32(q,length,q_s);
	 for(i=0;i<length;i++)
	    sum+=q_s[i];
	 rsqrt=Q_rsqrt(sum);
   for(i=0;i<length;i++)
	    q[i]=q[i]*rsqrt;
}
	
float Q_rsqrt(float number)       
{
    long i;
    float x2, y;
    const float threehalfs = 1.5F;
    x2 = number * 0.5F;
    y = number;
    i = *(long *)&y;                       
    i = 0x5f375a86 - (i >> 1);               
    y = *(float *)&i;
    y = y * (threehalfs - (x2 * y * y));   
    y = y * (threehalfs - (x2 * y * y));   
    return y;
}	

float low_pass(float sample[],float cutoff_freq)
{
  float result;
	float dt=1/cutoff_freq;
  result=(1-dt)*sample[1]+dt*sample[0];
  return result;
}
