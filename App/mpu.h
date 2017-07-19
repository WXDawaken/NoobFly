#ifndef __mpu_h
#define __mpu_h

#include "stm32f4xx.h"
#include "i2c.h"
#include <arm_math.h>
#include "USART.h"
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
#define        INT_PIN_CFG         0x37
#define        I2C_MASTER_CFG      0x6A

#define        I2C_BYPASS_SET      0x02
#define        I2C_MASTER_DIS      0x00
//AKB8975 REG
#define        AK_STATUS_1        0x02
#define        AK_HXL             0x03
#define        AK_HXH             0x04
#define        AK_HYL             0x05
#define        AK_HYH             0x06
#define        AK_HZL             0x07
#define        AK_HZH             0x08
#define        AK_STATUS_2        0x09
#define        AK_CNTL            0x0A
#define        AK_ASAX            0x10 //only accessable in FUSE ROM MODE
#define        AK_ASAY            0x11
#define        AK_ASAZ            0x12

#define        AK_DATA_READY      0x01
#define 	     AK_DATA_ERROR      0x04
#define        AK_SENSOR_OVERFLOW 0x08
#define        AK_MODE_SINGLE_MEASURE  0x01
#define        AK_MODE_SELF_TEST  0x08
#define        AK_MODE_FUSE_ROM   0x0F
//****************************

#define        MPU6050_Addr        0xD0          //����������IIC�����еĴӵ�ַ,����ALT  ADDRESS��ַ���Ų�ͬ�޸�
#define        AK8975_Addr        0x18
void mpu9150_Read(u8 *ACCEL,u8 *GYRO,u8 *MAG);
void mpu9150_Init(void);
void mpu9150_RawData(float *ACCEL,float *GYRO,float *MAG);
void mpu9150_Adjust_Val(float *ac_adjust,float *gy_adjust,float *mag_adjust);
void AK8975_Adjust_Val(u8 *adjust);

#endif
