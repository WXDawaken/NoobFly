#ifndef __i2c_h
#define __i2c_h

#include "stm32f4xx.h"
#include "os.h"
#define SDA_H GPIOB->ODR|=GPIO_Pin_7
#define SDA_L GPIOB->ODR&=~GPIO_Pin_7
#define SDA_READ GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7)
#define SCL_H GPIOB->ODR|=GPIO_Pin_6
#define SCL_L GPIOB->ODR&=~GPIO_Pin_6

void iic_Init(void);	//初始化iic_GPIO

void iic_Test(void);

void Delay(void);			//iic延迟函数
uchar iic_Start(void);		//开始标志
uchar iic_Stop(void);	//结束标志
void iic_Ack(void);		//发送应答
void iic_NoAck(void);	//发送无应答
uchar iic_WaitAck(void);	//等待应答
void iic_WriteByte(uchar data);	//写入一个字节
uchar iic_ReadByte(void);	//读取一个字节
uchar setByte(uchar deviceAddr,uchar registerAddr,uchar data);	//写入一个数据
uchar hearByte(uchar deviceAddr,uchar registerAddr);		//读取一个数据
uchar getTest(void);  //获取读取标志位
void resetTest(void);		//复位读取标志位
void iic_burst_read(uchar* array,uchar length,uchar deviceAddr,uchar regAddr);
#endif
