#ifndef __i2c_h
#define __i2c_h

#include "stm32f4xx.h"



void iic_Init(void);	//初始化iic_GPIO

void iic_Test(void);

void Delay(void);			//iic延迟函数
u8 iic_Start(I2C_TypeDef *I2Cx);		//开始标志
u8 iic_Stop(I2C_TypeDef *I2Cx);	//结束标志
void iic_Ack(void);		//发送应答
void iic_NoAck(void);	//发送无应答
u8 iic_WaitAck(void);	//等待应答
void iic_WriteByte(I2C_TypeDef *I2Cx,u8 data);	//写入一个字节
u8 iic_ReadByte(I2C_TypeDef *I2Cx);	//读取一个字节
u8 seyByte(I2C_TypeDef *I2Cx,u8 deviceAddr,u8 registerAddr,u8 data);	//写入一个数据
u8 hearByte(I2C_TypeDef *I2Cx,u8 deviceAddr,u8 registerAddr);		//读取一个数据
u8 getTest(void);  //获取读取标志位
void resetTest(void);		//复位读取标志位
void iic_burst_read(I2C_TypeDef *I2Cx,u8* array,u8 length,u8 deviceAddr,u8 regAddr);
#endif
