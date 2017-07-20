#ifndef __MATHTYPE_H_
#define __MATHTYPE_H_
typedef struct Quaternion
{
  float q[4];
	float q0q0, q0q1, q0q2, q0q3;
  float q1q1, q1q2, q1q3;
  float q2q2, q2q3;
  float q3q3;
}quaternion;

typedef struct Euler
{
  float Yaw,
        Pitch,
	      Roll;
}euler;
#endif