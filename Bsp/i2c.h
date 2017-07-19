#ifndef __i2c_h
#define __i2c_h

#include "stm32f4xx.h"
#include "os.h"
#define SDA_H GPIO_WriteBit(GPIOB,GPIO_Pin_7,1)
#define SDA_L GPIO_WriteBit(GPIOB,GPIO_Pin_7,0)
#define SDA_READ GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7)
#define SCL_H GPIO_WriteBit(GPIOB,GPIO_Pin_6,1)
#define SCL_L GPIO_WriteBit(GPIOB,GPIO_Pin_6,0)

void iic_Init(void);	//初始化iic_GPIO

void iic_Test(void);

void Delay(void);			//iic延迟函数
u8 iic_Start();		//开始标志
u8 iic_Stop();	//结束标志
void iic_Ack(void);		//发送应答
void iic_NoAck(void);	//发送无应答
u8 iic_WaitAck(void);	//等待应答
void iic_WriteByte(u8 data);	//写入一个字节
u8 iic_ReadByte();	//读取一个字节
u8 setByte(u8 deviceAddr,u8 registerAddr,u8 data);	//写入一个数据
u8 hearByte(u8 deviceAddr,u8 registerAddr);		//读取一个数据
u8 getTest(void);  //获取读取标志位
void resetTest(void);		//复位读取标志位
void iic_burst_read(u8* array,u8 length,u8 deviceAddr,u8 regAddr);
#endif
