#include "mpu.h"

void mpu6050_Init(void){
	seyByte(MPU6050_Addr,PWR_MGMT_1,0x00);
	seyByte(MPU6050_Addr,SMPLRT_DIV,0x07);
	seyByte(MPU6050_Addr,CONFIG,0x06);
	seyByte(MPU6050_Addr,GYRO_CONFIG,0x18);
	seyByte(MPU6050_Addr,ACCEL_CONFIG,0x01);
}

void mpu6050_Read(u8 *ACCEL,u8 *GYRO){
	*(ACCEL + 0) = hearByte(MPU6050_Addr,ACCEL_XOUT_L);
	*(ACCEL + 1) = hearByte(MPU6050_Addr,ACCEL_XOUT_H);
	*(ACCEL + 2) = hearByte(MPU6050_Addr,ACCEL_YOUT_L);
	*(ACCEL + 3) = hearByte(MPU6050_Addr,ACCEL_YOUT_H);
	*(ACCEL + 4) = hearByte(MPU6050_Addr,ACCEL_ZOUT_L);
	*(ACCEL + 5) = hearByte(MPU6050_Addr,ACCEL_ZOUT_H);
	
	*(GYRO + 0)  = hearByte(MPU6050_Addr,GYRO_XOUT_L);
	*(GYRO + 1)  = hearByte(MPU6050_Addr,GYRO_XOUT_H);
	*(GYRO + 2)  = hearByte(MPU6050_Addr,GYRO_YOUT_L);
	*(GYRO + 3)  = hearByte(MPU6050_Addr,GYRO_YOUT_H);
	*(GYRO + 4)  = hearByte(MPU6050_Addr,GYRO_ZOUT_L);
	*(GYRO + 5)  = hearByte(MPU6050_Addr,GYRO_ZOUT_H);
}

void mpu6050_RawData(u8 *ACCEL,u8 *GYRO)
{
  




}