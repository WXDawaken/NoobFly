#include "mpu.h"

void mpu6050_Init(void){
	seyByte(I2C1,MPU6050_Addr,PWR_MGMT_1,0x00);
	seyByte(I2C1,MPU6050_Addr,SMPLRT_DIV,0x07);
	seyByte(I2C1,MPU6050_Addr,CONFIG,0x06);
	seyByte(I2C1,MPU6050_Addr,GYRO_CONFIG,0x18);
	seyByte(I2C1,MPU6050_Addr,ACCEL_CONFIG,0x01);
}

void mpu6050_Read(u8 *ACCEL,u8 *GYRO){
	*(ACCEL + 0) = hearByte(I2C1,MPU6050_Addr,ACCEL_XOUT_H);
	*(ACCEL + 1) = hearByte(I2C1,MPU6050_Addr,ACCEL_XOUT_L);
	*(ACCEL + 2) = hearByte(I2C1,MPU6050_Addr,ACCEL_YOUT_H);
	*(ACCEL + 3) = hearByte(I2C1,MPU6050_Addr,ACCEL_YOUT_L);
	*(ACCEL + 4) = hearByte(I2C1,MPU6050_Addr,ACCEL_ZOUT_H);
	*(ACCEL + 5) = hearByte(I2C1,MPU6050_Addr,ACCEL_ZOUT_L);
	
	*(GYRO + 0)  = hearByte(I2C1,MPU6050_Addr,GYRO_XOUT_H);
	*(GYRO + 1)  = hearByte(I2C1,MPU6050_Addr,GYRO_XOUT_L);
	*(GYRO + 2)  = hearByte(I2C1,MPU6050_Addr,GYRO_YOUT_H);
	*(GYRO + 3)  = hearByte(I2C1,MPU6050_Addr,GYRO_YOUT_L);
	*(GYRO + 4)  = hearByte(I2C1,MPU6050_Addr,GYRO_ZOUT_H);
	*(GYRO + 5)  = hearByte(I2C1,MPU6050_Addr,GYRO_ZOUT_L);
}

void mpu6050_RawData(u8 *ACCEL,u8 *GYRO)
{
  mpu6050_Read(ACCEL,GYRO);
}