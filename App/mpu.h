#ifndef __mpu_h
#define __mpu_h

#include "stm32f4xx.h"
#include "i2c.h"
#include <arm_math.h>

// ����MPU6050�ڲ���ַ
//****************************************
#define        SMPLRT_DIV          0x19        //�����ǲ����ʣ�����ֵ��0x07(125Hz)
#define        CONFIG              0x1A        //��ͨ�˲�Ƶ�ʣ�����ֵ��0x06(5Hz)
#define        GYRO_CONFIG         0x1B        //�������Լ켰������Χ������ֵ��0x18(���Լ죬2000deg/s)
#define        ACCEL_CONFIG        0x1C        //���ټ��Լ졢������Χ����ͨ�˲�Ƶ�ʣ�����ֵ��0x01(���Լ죬2G��5Hz)

#define        ACCEL_XOUT_H        0x3B
#define        ACCEL_XOUT_L        0x3C
#define        ACCEL_YOUT_H        0x3D
#define        ACCEL_YOUT_L        0x3E
#define        ACCEL_ZOUT_H        0x3F
#define        ACCEL_ZOUT_L        0x40

#define        TEMP_OUT_H          0x41
#define        TEMP_OUT_L          0x42

#define        GYRO_XOUT_H         0x43
#define        GYRO_XOUT_L         0x44        
#define        GYRO_YOUT_H         0x45
#define        GYRO_YOUT_L         0x46
#define        GYRO_ZOUT_H         0x47
#define        GYRO_ZOUT_L         0x48

#define        PWR_MGMT_1          0x6B        //��Դ��������ֵ��0x00(��������)
#define        WHO_AM_I            0x75        //IIC��ַ�Ĵ���(Ĭ����ֵ0x68��ֻ��)


//****************************

#define        MPU6050_Addr        0xD0          //����������IIC�����еĴӵ�ַ,����ALT  ADDRESS��ַ���Ų�ͬ�޸�

void mpu6050_Read(u8 *ACCEL,u8 *GYRO);
void mpu6050_Init(void);
void mpu6050_RawData(u8 *ACCEL,u8 *GYRO);
#endif
